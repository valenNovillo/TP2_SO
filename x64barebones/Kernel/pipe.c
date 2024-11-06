#include "../include/pipe.h"
#include "../include/semaphore.h"
#include "../include/processes.h"
#include "../include/memoryManager.h"
#define BUFF_SIZE 128

typedef struct PipeCDT {
    int16_t id;
    int16_t pipe_idx; 
    int16_t fd_read;
    int16_t fd_write; 
    unsigned char buff[BUFF_SIZE];
    unsigned char readIdx;
    unsigned char writeIdx;
    unsigned char used;
    //semaphore* read;
    //semaphore* write;
    semaphore* characters; 
    semaphore* mutex;
    int16_t writer_pid; 
    int16_t reader_pid; 
} PipeCDT;

typedef PipeCDT *Pipe;
static Pipe pipes[MAX_PIPES];


/*static int find_free_pipe_idx() {
    for (int i = 0; i < MAX_PIPES; i++) {
        if (pipes[i] == NULL) {
            return i;
        }
    }
    return -1;
}*/

static int find_indx(int16_t id){
    return pipes[id] == NULL;
}


int pipe(int pipe_fd[DOS], int16_t id, char mode) {

    if(id >= MAX_PIPES)
    {return -1;}

    if(!find_inx(id))
    {
        
    }

    int pipe_idx = find_free_pipe_idx();
    if (pipe_idx == -1) {
        return -1; // No hay pipes disponibles
    }

    // Asignamos memoria para el nuevo pipe
    pipes[pipe_idx] = (Pipe) my_malloc(sizeof(PipeCDT));
    if (pipes[pipe_idx] == NULL) {
        return -1; // Error al asignar memoria
    }

    Pipe p = pipes[pipe_idx];
    p->pipe_idx = pipe_idx;

    // Asignación de los descriptores de archivo de lectura y escritura
    pipe_fd[0] = pipe_idx * 3;      // Descriptor de lectura
    pipe_fd[1] = pipe_idx * 3 + 1;  // Descriptor de escritura

    p->fd_read = pipe_fd[0];
    p->fd_write = pipe_fd[1];

    p->readIdx = 0;
    p->writeIdx = 0;
    p->used = 0;

    // Inicializamos semáforos para sincronización
    p->read = my_sem_create(pipe_fd[0], 0); // Inicialmente vacío
    p->write = my_sem_create(pipe_fd[1], BUFF_SIZE); // Espacio libre en el buffer
    p->mutex = my_sem_create(pipe_idx * 3 + DOS, 1); // Mutex para acceso exclusivo

    return 0;
}

int write_pipe(int fd, const void *buffer, size_t count) {
    int pipe_idx = fd / 3;
    if (pipe_idx < 0 || pipe_idx >= MAX_PIPES || pipes[pipe_idx] == NULL) {
        return -1; // Pipe inválido
    }

    Pipe p = pipes[pipe_idx];
    const unsigned char *data = (const unsigned char *)buffer;

    for (size_t i = 0; i < count; i++) {
        my_sem_wait(p->write);  // Espera si el buffer está lleno
        my_sem_wait(p->mutex);  // Acceso exclusivo al buffer

        // Escribimos el byte en el buffer
        p->buff[p->writeIdx] = data[i];
        p->writeIdx = (p->writeIdx + 1) % BUFF_SIZE;

        my_sem_post(p->mutex);  // Liberamos el acceso exclusivo
        my_sem_post(p->read);   // Notificamos que hay datos disponibles
    }

    return (int)count;
}

int read_pipe(int fd, void *buffer, size_t count) {
    int pipe_idx = fd / DOS;
    if (pipe_idx < 0 || pipe_idx >= MAX_PIPES || pipes[pipe_idx] == NULL) {
        return -1; // Pipe inválido
    }

    Pipe p = pipes[pipe_idx];
    unsigned char *data = (unsigned char *)buffer;

    for (size_t i = 0; i < count; i++) {
        my_sem_wait(p->read);   // Espera si no hay datos para leer
        my_sem_wait(p->mutex);  // Acceso exclusivo al buffer

        // Leemos el byte del buffer
        data[i] = p->buff[p->readIdx];
        p->readIdx = (p->readIdx + 1) % BUFF_SIZE;

        my_sem_post(p->mutex);  // Liberamos el acceso exclusivo
        my_sem_post(p->write);  // Notificamos que hay espacio libre en el buffer
    }

    return (int)count;
}

int close_pipe(int pipe_idx) {
    if (pipe_idx < 0 || pipe_idx >= MAX_PIPES || pipes[pipe_idx] == NULL) {
        return -1; // Pipe inválido
    }

    Pipe p = pipes[pipe_idx];

    // Destruimos los semáforos asociados al pipe
    my_sem_destroy(p->read);
    my_sem_destroy(p->write);
    my_sem_destroy(p->mutex);

    // Liberamos la memoria del pipe
    my_free(p);
    pipes[pipe_idx] = NULL;

    return 0;
}




