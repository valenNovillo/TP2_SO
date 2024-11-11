#include "include/videoDriver.h"

#include "include/fileDescriptors.h"
#include "../include/lib.h"
#include "include/fonts.h"

// ===================================PROTOTIPOS DE FUNCIONES AUXILIARES==================================================

//función que imprime un pixel en pantalla del color y en la posición (x,y) que le pasan por parametro
static void putPixel(Color hexColor, uint64_t x, uint64_t y);

// función que imprime en pantalla un caracter a partir de la posicion (x,y) en los colores que le pasan por parametro
static void putLetter(unsigned char letter, int posX, int posY, Color fontColor, Color fontBackColor);

// función que imprime en pantalla un caracter (incluyendo el espacio, tab, delete) en los colores que le pasan por parametro
static void putChar(char letter, Color fontColor, Color fontBackColor);

// función que "mueve" (reimprime) hacia arriba todas las lineas k veces. De esta forma da una impresion de "scroll"
static void scrollPixel(int k);

// función que imprime un cuadrado en pantalla en el tamaño actualmente definido, del color y a partir de la posicion (x,y) que le pasan por parametro
static void scalePixel(Color hexColor, int x, int y);

// función que "mueve" (reimprime) hacia arriba todas las lineas 1 vez. De esta forma da una impresión de "nueva linea"
static void addLine();

// función que "elimina" el ultimo caracter escrito. Lo hace moviendo los cursores una posición hacia atras e imprime un espacio 
static void deleteChar();

// función que controla la posición en x de la pantalla. Si se llega al final de linea se posiciona x una linea mas abajo y al comienzo de la nueva linea
static void updateX();

// ========================================================================================================================

struct vbe_mode_info_structure{
	uint16_t attributes;		// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;			// deprecated
	uint8_t window_b;			// deprecated
	uint16_t granularity;		// deprecated; used while calculating bank numbers
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr;		// deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;			// number of bytes per horizontal line
	uint16_t width;			// width in pixels
	uint16_t height;			// height in pixels
	uint8_t w_char;			// unused...
	uint8_t y_char;			// ...
	uint8_t planes;
	uint8_t bpp;			// bits per pixel in this mode
	uint8_t banks;			// deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size;		// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
	uint8_t image_pages;
	uint8_t reserved0;
 
	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;
 
	uint32_t framebuffer;		// physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
} __attribute__ ((packed));

typedef struct vbe_mode_info_structure * VBEInfoPtr;

VBEInfoPtr VBE_mode_info = (VBEInfoPtr) 0x0000000000005C00;

int size = INITIAL_SIZE; // tamaño de la fuente inicializado en INITIAL_SIZE

int currentX = 0;
int currentY = 0;

int previousX = 0;
int previousY = 0;

Color colorBackUp[HEIGHT_PIXELS][WIDTH_PIXELS];

Color backgroundColor = {0, 0, 0};
Color fontColor = {255, 255, 255};


void putPixel(Color hexColor, uint64_t x, uint64_t y){
    Color * framebuffer = (Color *)(uint64_t) VBE_mode_info->framebuffer;
    uint64_t offset = x + y * VBE_mode_info->width;
    framebuffer[offset] = hexColor;
}


void putChar(char letter, Color fontColor, Color fontBackColor)
{
	switch(letter)
		{
			case '\t':
			{
				for(int i=0; i<4; i++) // un tab son 4 espacios
				{
					putLetter(' ', currentX, currentY, fontColor, fontBackColor);
					updateX();
				}
				break;
			}
			case '\b':
			{
				deleteChar();
				break;
			}
			case '\n':
			{
				addLine();
				break;
			}
			default: // si no estoy en ninguno de estos casos, entonces es un caracter cuya representacion puedo encontrar en el bitmap
			{
				putLetter(letter, currentX, currentY, fontColor, fontBackColor);
				updateX();
				break;
			}
		}
}

void putLetter(unsigned char letter, int posX, int posY, Color fontColor, Color fontBackColor){
	char * letterBitmap = font8x8_basic[letter];
	char mask[BITMAP_SIZE] = {1,2,4,8,16,32,64,128}; 
	for(int row=0; row<BITMAP_SIZE; row++){
		for(int col=0; col<BITMAP_SIZE; col++){ // recorro los bits de cada hexa para hacer masks distintas
			char current = (letterBitmap[row] & mask[col]); // aplico la mascara para cada bit
				if(current != 0){
					scalePixel(fontColor, posX + col * size,  posY + row * size);
				}
				else{
					scalePixel(fontBackColor, posX + col * size, posY + row * size);
				}
		}
	}
}

int putString(uint64_t fd, char * string, int length){

	Color aux = fontColor;

	if(fd == STDERR) {
		Color red = {0,0,255};
		setColor(red);
	}else if (fd != STDOUT) {
		return 0;
	}

	int i;
	for(i=0; i<length && string[i] != 0; i++) {
		putChar(string[i], fontColor, backgroundColor);
	}
	
	fontColor = aux;
	return i;
}

void addLine(){
	currentX = 0;		//reseteo la pos en x ---> vuelvo al comienzo de linea
	if(currentY + size*BITMAP_SIZE*2 >= VBE_mode_info->height)	//si al agregar una linea me estoy pasando del largo de la pantalla entonces "scrolleo" una linea
		scrollPixel(size*BITMAP_SIZE);
	else
		currentY += (size*BITMAP_SIZE);	//me desplazo en el eje y una linea
}

void deleteChar(){
	if(currentX != 0) {
		currentX -= size*BITMAP_SIZE;
	} else { 	//currentX==0 --> estoy al comienzo de la linea
		if(currentY!=0) {	//solo puedo borrar si no estoy en la primera linea
			currentX = ((VBE_mode_info->width / (size * BITMAP_SIZE)) * (size * BITMAP_SIZE)) - (size*BITMAP_SIZE);
			currentY -= size*BITMAP_SIZE;
		}
	}
	putLetter(' ', currentX, currentY, fontColor, backgroundColor);
}

void updateX()
{
	currentX += (size*BITMAP_SIZE);
	if(currentX + (size*BITMAP_SIZE) > VBE_mode_info->width)
		addLine();
} 

void setColor(Color c){
	fontColor = c;
}

void printRegs(REGISTERS r){
	char buff[BUFFERSIZE];
	int length;

	length = decimalToHexadecimal(r.rax, buff, BUFFERSIZE);
	putString(1,"RAX: ", 5);
	putString(1, buff, length);
    putString(1,"\n", 1);

    length = decimalToHexadecimal(r.rbx, buff, BUFFERSIZE);
    putString(1,"RBX: ", 5);
    putString(1,buff, length);
    putString(1,"\n", 1);

    length = decimalToHexadecimal(r.rcx, buff, BUFFERSIZE);
    putString(1,"RCX: ", 5);
    putString(1,buff, length);
    putString(1,"\n", 1);

    length = decimalToHexadecimal(r.rdx, buff, BUFFERSIZE);
    putString(1,"RDX: ", 5);
    putString(1,buff, length);
    putString(1,"\n", 1);

    length = decimalToHexadecimal(r.rbp, buff, BUFFERSIZE);
    putString(1,"RBP: ", 5);
    putString(1,buff, length);
    putString(1,"\n", 1);

    length = decimalToHexadecimal(r.rdi, buff, BUFFERSIZE);
    putString(1,"RDI: ", 5);
    putString(1,buff, length);
    putString(1,"\n", 1);

    length = decimalToHexadecimal(r.rsi, buff, BUFFERSIZE);
    putString(1,"RSI: ", 5);
    putString(1,buff, length);
    putString(1,"\n", 1);

    length = decimalToHexadecimal(r.r8, buff, BUFFERSIZE);
    putString(1,"R8: ", 5);
    putString(1,buff, length);
    putString(1,"\n", 1);

    length = decimalToHexadecimal(r.r9, buff, BUFFERSIZE);
    putString(1,"R9: ", 5);
    putString(1,buff, length);
    putString(1,"\n", 1);

    length = decimalToHexadecimal(r.r10, buff, BUFFERSIZE);
    putString(1,"R10: ", 5);
    putString(1,buff, length);
    putString(1,"\n", 1);

    length = decimalToHexadecimal(r.r11, buff, BUFFERSIZE);
    putString(1,"R11: ", 5);
    putString(1,buff, length);
    putString(1,"\n", 1);

    length = decimalToHexadecimal(r.r12, buff, BUFFERSIZE);
    putString(1,"R12: ", 5);
    putString(1,buff, length);
    putString(1,"\n", 1);

    length = decimalToHexadecimal(r.r13, buff, BUFFERSIZE);
    putString(1,"R13: ", 5);
    putString(1,buff, length);
    putString(1,"\n", 1);

    length = decimalToHexadecimal(r.r14, buff, BUFFERSIZE);
    putString(1,"R14: ", 5);
    putString(1,buff, length);
    putString(1,"\n", 1);

    length = decimalToHexadecimal(r.r15, buff, BUFFERSIZE);
    putString(1,"R15: ", 5);
    putString(1,buff, length);
    putString(1,"\n", 1);

	length = decimalToHexadecimal(r.rip, buff, BUFFERSIZE);
    putString(1,"RIP: ", 5);
    putString(1,buff, length);
    putString(1,"\n", 1);

	length = decimalToHexadecimal(r.cs, buff, BUFFERSIZE);
    putString(1,"CS: ", 4);
    putString(1,buff, length);
    putString(1,"\n", 1);

	length = decimalToHexadecimal(r.rflags, buff, BUFFERSIZE);
    putString(1,"RFLAGS: ", 8);
    putString(1,buff, length);
    putString(1,"\n", 1);

	length = decimalToHexadecimal(r.rsp, buff, BUFFERSIZE);
    putString(1,"RSP: ", 5);
    putString(1,buff, length);
    putString(1,"\n", 1);
}

void scalePixel(Color hexColor, int x, int y){
	printSquare(hexColor, x, y, size);
}

void printSquare(Color hexColor, int x, int y, int length){
	for(int i=0; i<length; i++)
	{
		for(int j=0; j<length; j++)
		{
			putPixel(hexColor, x + i, y + j);
		}
	}
}

void scrollPixel(int k){
    Color * framebuffer = (Color *)((uint64_t) VBE_mode_info->framebuffer);
    uint64_t offset = 0;
    for (int i = 0 ; i < VBE_mode_info->height - k ; i++) {
        for (int j = 0 ; j < VBE_mode_info->width ; j++){
            framebuffer[offset+j] = framebuffer[offset + j + VBE_mode_info->width*k];
        }
        offset += VBE_mode_info->width;
    }

	for(int n = k ; n > 0; n--) {
        for (int i = 0 ; i < VBE_mode_info->width ; i++){
            framebuffer[offset+i] = backgroundColor;
        }
        offset += VBE_mode_info->width;
    }
}

void cleanScreen(){
	scrollPixel(VBE_mode_info->height);
	currentX = 0;
	currentY = 0;
}

void setSize(int newSize){
	size = newSize;
	cleanScreen();
}

void sizeInc(){
	setSize(size+UPDATE_SIZE);
}

void sizeDec(){
	int aux = size - UPDATE_SIZE;
	if(aux >= 1)
		setSize(aux);
}

void save(){
	memcpy((void *)colorBackUp, (const void *) ((uint64_t)VBE_mode_info->framebuffer), (uint64_t) sizeof(Color) * VBE_mode_info->height * VBE_mode_info->width);
    previousX = currentX;
    previousY = currentY;
}

void load(){
    memcpy((void *) ((uint64_t)VBE_mode_info->framebuffer),(const void *) colorBackUp, (uint64_t)sizeof(Color) * VBE_mode_info->height * VBE_mode_info->width);
    currentX = previousX;
    currentY = previousY;
}
