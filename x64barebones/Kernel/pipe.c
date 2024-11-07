#include "../include/pipe.h"
#include "../include/semaphore.h"
#include "../include/processes.h"
#include "../include/memoryManager.h"
#include "../include/lib.h"
#include "../Drivers/include/fileDescriptors.h"
#include "../include/scheduler.h"
#include "../Drivers/include/videoDriver.h"

#define BUFF_SIZE 128
#define MAX_PIPES 2048
#define WRITER 'w'
#define READER 'r'
#define SEM_READ 0
#define SEM_WRITE 1
#define SEM_MUTEX 2


typedef struct PipeCDT {
    int16_t id; //cada pipe se identifica por su posición en el array de pipes
    int16_t pipe_idx; 
    int16_t fd_read;
    int16_t fd_write; 
    unsigned char buff[BUFF_SIZE];
    unsigned char read_idx;
    unsigned char write_idx;
    unsigned char used;
    semaphore* read;
    semaphore* write;
    //semaphore* characters; 
    semaphore* mutex;
    int16_t writer_pid; 
    int16_t reader_pid; 
} PipeCDT;

typedef PipeCDT *Pipe;
static Pipe pipes[MAX_PIPES] = {0};
int16_t count_pipes = 0;
int16_t next_pipe_idx = 0; 


/*static int find_free_pipe_idx() {
    for (int i = 0; i < MAX_PIPES; i++) {
        if (pipes[i] == NULL) {
            return i;
        }
    }
    return -1;
}*/

static Pipe find_by_id(int16_t id){

    if(id < 0 || id >= MAX_PIPES){
        return NULL;
    }

    for(int i = 0; i < MAX_PIPES; i++){
        if(pipes[id] != NULL){
            return pipes[id];
        }
    }

    return NULL;
}

static int16_t get_next_index(){
    if(count_pipes >= MAX_PIPES){
        return -1;
    }

    while(pipes[next_pipe_idx] != NULL){
        next_pipe_idx = (int16_t)((next_pipe_idx + 1) % MAX_PIPES);
    }

    return next_pipe_idx;
}

static Pipe init_pipe(){
    int16_t new_pipe_idx = get_next_idx();
    if(new_pipe_idx == -1){
        return NULL;
    }

    Pipe new_pipe = my_malloc(sizeof(Pipe));
    if(new_pipe == NULL){
        return NULL;
    }

    pipes[new_pipe_idx] = new_pipe;
    new_pipe->pipe_idx = new_pipe_idx;
    memset(new_pipe->buff, 0, BUFF_SIZE);
    new_pipe->read_idx = 0;
    new_pipe->write_idx = 0;
    new_pipe->writer_pid = -1;
    new_pipe->reader_pid = -1;

    my_sem_create(SEM_MUTEX, 1);
    my_sem_create(SEM_READ, 0);
    my_sem_create(SEM_WRITE, BUFF_SIZE);

    return new_pipe;
} 

int16_t create_pipe(int16_t id){
    if(count_pipes >= MAX_PIPES || id >= MAX_PIPES){
        return -1;
    }

    Pipe pipe = find_by_id(id);
    if(pipe == NULL){
        pipe = init_pipe();
        if(pipe == NULL){
            return -1;
        }
        pipe->id = id;
        return pipe;
    }

    return -1;
}

int16_t open_pipe_for_pid(int16_t id, int16_t pid, char mode){
    Pipe pipe = find_by_id(id);
    if(id == NULL){
        int16_t idx;
        if((idx = create_pipe(id)) == -1){
            return -1;
        }

        id = find_by_id(idx);
    }

    if((pipe->writer_pid != -1 && mode==WRITER) || (pipe->reader_pid != -1 && mode==READER)){
        return -1;
    }

    if(mode == WRITER){
        pipe->writer_pid = pid;
    }else if(mode == READER){
        pipe->reader_pid = pid;
    }else{
        return -1;
    }

    return pipe->pipe_idx;
}

static void init_fds(){
    my_sem_create(SEM_MUTEX, 1);
    open_pipe_for_pid(STDIN, 0, 'w');
    open_pipe_for_pid(STDOUT, 0, 'r');
    open_pipe_for_pid(STDERR, 0, 'r');
}

static void clear_stdin(){
    Pipe stdin = pipes[0];
    stdin->read_idx = stdin->write_idx;
    for(int i = 0; i < stdin->used; i++){
        my_sem_wait(stdin->read);
    }
    stdin->used = 0;
}

static void change_fd_writer(int16_t id, int16_t new_pid){
    Pipe obj_id = find_by_id(id);
    if(obj_id == NULL){
        return;
    }
    obj_id->writer_pid = new_pid;
}


static free_pipe(Pipe pipe){
    my_sem_destroy(pipe->mutex);
    my_sem_destroy(pipe->read);
    my_sem_destroy(pipe->write);
    pipes[pipe->pipe_idx] = NULL;
    count_pipes--;
    my_free(pipe);
}

void close_pipe_for_pid(int16_t id, int16_t pid){
    Pipe pipe = find_by_id(id);
    if(pipe == NULL){
        return;
    }

    if(pipe->writer_pid == pid){
        pipe->reader_pid = -1;
    }

    if(pipe->reader_pid == -1 && pipe->writer_pid == -1){
        free_pipe(pipe);
    }
}

int write_on_file(int16_t id, unsigned char *buff, unsigned long len){
    Pipe pipe = find_by_id(id);
    if(id == NULL || pipe->reader_pid != get_pid()){
        return -1;
    }
    switch(pipe->pipe_idx){
        case STDOUT:
            my_sem_wait(pipe->mutex);
            putString(STDOUT,(char*) buff, len);
            my_sem_post(pipe->mutex);
            break;
        case STDERR:
            my_sem_wait(pipe->mutex);
            //FALTA TERMINARRRR
            //Luego lo tengo que meter como syscall y usar en la shell
    }
}

int read_on_file(int16_t fd_idx, unsigned char *target, unsigned long len){
    //FALTA TERMINARRRR
    //Luego lo tengo que meter como syscall
}




