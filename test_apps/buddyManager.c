#include "memoryManagerBu.h"

#define MIN_BLOCK_SIZE 1024
#define MAX_EXP 19
#define BLOCKSIZE(i) ((unsigned long long)(1 << (i)) * MIN_BLOCK_SIZE) //Esta macro calcula el tamaño de un bloque en función de un índice i. (1 << (i)): Esto realiza un desplazamiento de bits, desplazando 1 a la izquierda i posiciones, lo que equivale a calcular 2^i. Es la forma eficiente de multiplicar por una potencia de 2.Multiplicación: El tamaño final del bloque es 2^i x 1024, lo que significa que el tamaño del bloque es una potencia de 2 multiplicada por el tamaño mínimo de 1024 bytes. 
#define GET_BUDDY(b, i) ((((unsigned long long )(b)) ^ (BLOCKSIZE(i)))) //Esta macro calcula la dirección del "buddy" (compañero) de un bloque. El concepto de "buddy" es fundamental en el Buddy System: cada bloque tiene un "buddy" con el cual puede fusionarse para formar un bloque más grande. b: Representa la dirección (o desplazamiento desde la base) del bloque actual. BLOCKSIZE(i): Calcula el tamaño del bloque en el nivel i. b ^ BLOCKSIZE(i): Aplica un XOR entre la dirección del bloque b y el tamaño del bloque. El operador XOR cambia ciertos bits en la dirección, esencialmente apuntando a la dirección del "buddy". Como ambos bloques (b y su "buddy") tienen el mismo tamaño y están alineados de cierta forma, el XOR te da la dirección del bloque adyacente (el "buddy") 

typedef struct Block {
    unsigned long long size;
    struct Block *next; 
} Block;

static Block *free_lists[MAX_EXP + 1];
static void * firstAddress;
MemoryInfo mem_info;

//Esta función busca determinar el índice mínimo i tal que el tamaño del bloque 2^i x 1024 sea lo suficientemente grande como para acomodar el tamaño de memoria solicitado (size)./
int obtainIndex(unsigned long size) {
    int i = 0;
    while (BLOCKSIZE(i) < size) {
        i++;
    }
    return i;
}

//La función memInit inicializa el sistema de memoria, preparando las estructuras necesarias para gestionar la memoria y dividiéndola en bloques según el Buddy System./
void memInit(void *start_ptr, unsigned long size_bytes) {
    if(size_bytes < MIN_BLOCK_SIZE )
        return ;

    for (int i = 0; i < MAX_EXP + 1; i++)
        free_lists[i] = NULL; //free_lists es un arreglo de punteros a listas enlazadas. Cada lista representa bloques de un tamaño específico, donde el índice i corresponde a bloques de tamaño 2^𝑖 × 1024. Inicialmente, todas las listas están vacías (NULL).

    int i = obtainIndex(size_bytes);

    if(BLOCKSIZE(i) > size_bytes ){
        i--;
    }

    if (i > MAX_EXP) {
        i = MAX_EXP;
    }

    Block *aligned_start_ptr = (Block *)(((unsigned long long)start_ptr + 7) & ~7);
    firstAddress = aligned_start_ptr;
    free_lists[i] = aligned_start_ptr;
    free_lists[i]->next = NULL;
    free_lists[i]->size = BLOCKSIZE(i);
    mem_info.total = free_lists[i]->size;
    mem_info.free = free_lists[i]->size;
    mem_info.reserved =0;
}

Block *memAllocRec(unsigned long bytes) {
    // Determina el índice i necesario para el tamaño de bloque que acomode bytes.
    int i = obtainIndex(bytes);

    // Si el índice calculado excede el máximo permitido (MAX_EXP), no hay bloques disponibles.
    if (i > MAX_EXP) {
        return NULL;
    }

    // Verifica si hay un bloque libre en la lista correspondiente al tamaño 2^i * 1024.
    if (free_lists[i] != NULL) {
        Block *block = free_lists[i];       // Toma el primer bloque disponible de la lista de tamaño i.
        free_lists[i] = free_lists[i]->next; // Remueve el bloque de la lista enlazada.
        block->size = BLOCKSIZE(i);          // Define el tamaño del bloque como el tamaño correspondiente al nivel i.
        return block;                        // Retorna el bloque asignado.
    }

    // Si no hay bloques libres en el nivel i, se llama recursivamente para un tamaño mayor, i + 1.
    Block *block = memAllocRec(BLOCKSIZE(i + 1));
    
    // Si se obtuvo un bloque de tamaño mayor (2^(i+1) * 1024), se lo divide en dos "buddies".
    if (block != NULL) {
        // Calcula la dirección de block relativa a firstAddress.
        unsigned long long addressDiffFromBase = (void *)block - firstAddress;

        // Calcula la dirección del "buddy" de tamaño i utilizando XOR.
        Block *buddy = (Block *)(GET_BUDDY(addressDiffFromBase, i) + (unsigned long long)firstAddress);

        // Ajusta el tamaño y enlaza el "buddy" recién creado en la lista de bloques libres de tamaño i.
        buddy->size = BLOCKSIZE(i);
        buddy->next = free_lists[i];
        free_lists[i] = buddy;

        // Ajusta el tamaño de block y lo retorna como bloque asignado.
        block->size = BLOCKSIZE(i);
    }
    return block; // Devuelve el bloque para el tamaño solicitado o NULL si no fue posible.
}


void * memAlloc(unsigned long bytes) {
    if(bytes==0){
        return NULL;
    }
    Block* ptr = memAllocRec(bytes + sizeof(Block));
    if(ptr){
        return (void*)(ptr + 1);
    }
    return NULL;
}

void memFreeRec(void* ptr) {
    unsigned long size = ((Block*)ptr)->size;
    int i = obtainIndex(size);

    unsigned long long addressDiffFromBase = ptr - firstAddress;
    Block* buddy = (Block *) (GET_BUDDY(addressDiffFromBase, i) + (unsigned long long ) firstAddress);
    Block** current = &free_lists[i];

    while (*current != NULL && *current != buddy) {
        current = &((*current)->next);
    }

    if (*current != buddy) {
        ((Block *)ptr)->next = free_lists[i];
        free_lists[i] = ptr;
    } else {
        *current = buddy->next;

        if (ptr > (void *)buddy) {
            buddy->size = BLOCKSIZE(i + 1);
            memFreeRec(buddy);
        } else {
            ((Block *)ptr)->size = BLOCKSIZE(i + 1);
            memFreeRec(ptr);
        }
    }
}

void memFree(void *ptr) {
    memFreeRec(((Block *)ptr) - 1);
}

MemoryInfo * getMemInfo(){
    uint64_t free = 0;
    for (int i = 0; i < MAX_EXP ; i++){
        Block * current = free_lists[i];
        while(current != NULL){
            free += current->size + sizeof (Block);
            current = current->next;
        }
    }
    mem_info.reserved = mem_info.total - free;
    mem_info.free = mem_info.free;

    return &mem_info;
}






