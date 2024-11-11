#include "include/eliminator.h"

//===================================================================================== PROTOTIPOS ===================================================================================================================

//función que para darle una introduccion al juego eliminator - suena una musica y se imprime un mensaje
void intro();

//función para registrar el modo de juego (cantidad de jugadores) 
void choosePlayerMode();

//función para registrar el nivel de dificultad del juego 
void setLevel();

//función que limpia el mapa del juego
static void fillMatrix();

//función que indica los controles para comenzar o salir del juego
void initialIndication();

//función que implementa la modalidad del juego para 1 jugador
void player1Mode();

//función que implementa la modalidad del juego para 2 jugadores
void player2Mode();

//función que actualiza la dirección en la que se mueve el jugador (en modo 1 jugador)
void updateCurrent1(Direction * current, char key);

//función que actualiza la dirección en la que se mueven los jugadores (en modo 2 jugadores)
void updateCurrent2(Direction * current, Direction * current2, char key);

//función que actualiza la posición de un jugador en el mapa de juego 
void updatePosition(Direction * current, Position * playerPos, char * colision, unsigned char color[]);

//función que actualiza el mapa con el color que le pasan en la posición de coordenadas (x,y)
void printShape(unsigned char color[3], int x, int y);

//función que imprime un borde en el mapa de juego
void printBorder();

//función que controla el flag de playing, controla si se esta llevando a cabo una partida del juego
void setPlaying();

//función para darle un cierre a cada partida del juego - suena una musica y se imprime un mensaje
void gameOver();

//función para controlar las velocidades en el juego
void speedLoop(int speed);

//========================================================================================================================================================================================================================

int players;
int level;
static int playing = PLAYING;
static int score1 = 0;
static int score2 = 0;

char gameMap[MAPHEIGHT][MAPWIDTH];

int speed[3] = {LEVEL1,LEVEL2, LEVEL3};

unsigned char red[] = {0, 0, 255};
unsigned char black[] = {0, 0, 0};
unsigned char green[] = {0, 255, 0};
unsigned char blue[] = {255, 0, 0};

void eliminator(){
    playing = PLAYING;
    score1 = 0;
    score2 = 0;
    cleanScreen();
    setSize(GAME_SIZE);
    intro();
    reading(1);
    choosePlayerMode();
    setLevel();
    reading(0);
    fillMatrix();
    cleanScreen();
    initialIndication();
    cleanScreen();
    while(playing){
        if(players == 1)
            player1Mode();
        else
            player2Mode();
    }
    cleanScreen();
}

void intro(){
    printf("\n\n\n\n\n\n\n\n\n");
    printf("                     WELCOME TO ELIMINATOR!\n");
    musicDispatcher(MARIO);
    cleanScreen();
}

void choosePlayerMode(){
    printf("\nSelect the player mode:\n\n");
    printf("\nPress 1 to enter single player mode\n\n");
    printf("\nPress 2 to enter two player mode\n\n");
    scanf1("%d", &players);
    while(players != 1 && players != 2){ 
        printf("\n\nPlease press 1 (one player) or 2 (two players)\n\n");
        scanf1("%d", &players);
    }
    printf("\n\n%d player/s mode selected\n\n", players);
}

void setLevel(){
    printf("\nChoose level 1, 2 or 3 (3 is the most difficult level):\n\n");
    scanf1("%d", &level);
    while(level != 1 && level != 2 && level != 3) { 
        printf("\n\nPlease press a valid level (1, 2, 3)\n\n");
        scanf1("%d", &level);
    }
    printf("\n\nlevel %d chosen\n\n", level); 
}

static void fillMatrix(){
    for(int i = 0; i < MAPHEIGHT; i++)
    {
        for(int j = 0; j < MAPWIDTH; j++)
        {
            gameMap[i][j] = 0;
        }
    }

    return;
}

void initialIndication(){
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf(" \t\t\t\t\tPRESS 'SPACE' TO START\n");
    printf(" \t\t\t\t\tOR PRESS 'ENTER' TO QUIT");
    
    setPlaying();
    return;
};

void player1Mode(){
    playing = 1;
    printBorder();
    Direction current1 = DOWN;
    printShape(green, STARTING_1_X, STARTING_1_Y);
    Position playerPos = {STARTING_1_X, STARTING_1_Y};
    char colision = 0;
    reading(1);
    while (!colision){
        speedLoop(speed[level-1]);
        char key;
        read(0, &key, 1);
        updateCurrent1(&current1, key);
        updatePosition(&current1, &playerPos, &colision, green);
    }
    reading(0);
    gameOver();
}

void player2Mode(){
    playing = 1;
    printBorder();
    Direction current1 = DOWN;
    Direction current2 = UP;
    printShape(green, STARTING_1_X, STARTING_1_Y);
    printShape(blue, STARTING_2_X, STARTING_2_Y);
    Position playerPos1 = {STARTING_1_X, STARTING_1_Y};
    Position playerPos2 = {STARTING_2_X, STARTING_2_Y};
    char colision1 = 0;
    char colision2 = 0;
    reading(1);
    while (!colision1 && !colision2){
        speedLoop(speed[level-1]);
        char key;

        read(0, &key, 1);
        
        updateCurrent2(&current1, &current2, key);

        updatePosition(&current2, &playerPos2, &colision2, blue);
        updatePosition(&current1, &playerPos1, &colision1, green);
        
    }
    reading(0);
    score1 += colision2;
    score2 += colision1;
    gameOver();
}

void updateCurrent1(Direction * current, char key){
    switch (key){
            case LEFT_ARROW_1: {
                if (*current != RIGHT){
                    *current = LEFT;
                }
                break;
            }
            case RIGHT_ARROW_1: {
                if (*current != LEFT) {
                    *current = RIGHT;
                }
                break;
            }
            case UP_ARROW_1: {
                if (*current != DOWN) {
                    *current = UP;
                }    
                break;
            }
            case DOWN_ARROW_1: {
                if (*current != UP) {
                    *current = DOWN;
                }    
                break;
            } 
            default:
                break;
        }
}

void updateCurrent2(Direction * current, Direction * current2, char key){
    switch (key) {
            case LEFT_ARROW_1: {
                if (*current != RIGHT) {
                    *current = LEFT;
                }
                break;
            }
            case RIGHT_ARROW_1: {
                if (*current != LEFT) {
                    *current = RIGHT;
                }
                break;
            }
            case UP_ARROW_1: {
                if (*current != DOWN) {
                    *current = UP;
                }    
                break;
            }
            case DOWN_ARROW_1: {
                if (*current != UP) {
                    *current = DOWN;
                }    
                break;
            }
            case LEFT_ARROW_2: {
                if (*current2 != RIGHT) {
                    *current2 = LEFT;
                }
                break;
            }
            case RIGHT_ARROW_2: {
                if (*current2 != LEFT) {
                    *current2 = RIGHT;
                }
                break;
            }
            case UP_ARROW_2: {
                if (*current2 != DOWN) {
                    *current2 = UP;
                }    
                break;
            }
            case DOWN_ARROW_2: {
                if (*current2 != UP) {
                    *current2 = DOWN;
                }    
                break;
            } 
            default:
                break;
        }
}

void updatePosition(Direction * current, Position * playerPos, char * colision, unsigned char color[]){
    switch (*current){
            case RIGHT: {
                *colision = gameMap[playerPos->y][playerPos->x +1];
                playerPos->x += 1;
                break;
            }
            case LEFT: {
                *colision = gameMap[playerPos->y][playerPos->x -1];
                playerPos->x -= 1;
        
                break;
            }
            case UP: {
                *colision = gameMap[playerPos->y-1][playerPos->x];
                playerPos->y -= 1;
                break;
            }
            case DOWN: {
                *colision = gameMap[playerPos->y+1][playerPos->x];
                playerPos->y += 1;
                break;
            }
        }
    printShape(color, playerPos->x, playerPos->y);
}


void printShape(unsigned char color[3], int x, int y){
    gameMap[y][x] = 1;
    printSquare(color, x * SQUARELENGTH, y * SQUARELENGTH, SQUARELENGTH);
}


void printBorder(){
    for(int i=0; i < MAPWIDTH; i++){
        speedLoop(speed[level-1]);
        printShape(red, i, 0);
        printShape(red, i, MAPHEIGHT-1);
    }
    for(int i=0; i<MAPHEIGHT; i++){
        speedLoop(speed[level-1]);
        printShape(red, 0, i);
        printShape(red, MAPWIDTH-1, i);
    }
}

void setPlaying(){
    reading(1);
    int c = getChar(); 
    while(c != START && c != ENTER){
            c = getChar(); 
        }
     
     if(c == START){
            cleanScreen();
            fillMatrix();
            playing = 1;
            return;
        }
        else if(c == ENTER){
            playing = 0;
            return;
        }
        reading(0);

        //Al salir del while me fijo si fue por un ENTER o un START y seteo al flag playing
        playing = (c == START)?  PLAYING : QUIT; 

        return;
}

void gameOver(){
    printf("\n\n\n\n\n\n\n\n\n\n");
    printf("\t\t\t\t\t\t\tGAME OVER\n");
    if(players == 2){
        printf("\t\t\t\t\t\t\tSCORE PLAYER 1: %d\n", score1);
        printf("\t\t\t\t\t\t\tSCORE PLAYER 2: %d\n", score2);
    }
    printf("\t\t\t\t\tPRESS SPACE TO PLAY AGAIN\n\t\t\t\t\tPRESS ENTER TO EXIT\n");
    musicDispatcher(STARTUPMUSIC);
    setPlaying();
    return;
}

void speedLoop(int speed){
    int j = 0;
    while (j < speed){
            j++;
        }
}

