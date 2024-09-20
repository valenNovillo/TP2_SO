#ifndef ELIMINATOR_H
#define ELIMINATOR_H

#include "../../Library/include/stdio1.h"
#include "../../Library/include/unistd1.h"
#include "../../Library/include/string.h"
#include "music.h"


#define GAME_SIZE 2
#define LEVEL1 4000000
#define LEVEL2 3000000
#define LEVEL3 2000000
#define HEIGHT 768
#define WIDTH  1024
#define SQUARELENGTH 16
#define MAPHEIGHT HEIGHT / SQUARELENGTH
#define MAPWIDTH WIDTH / SQUARELENGTH
#define START ' '
#define ENTER '\n'
#define PLAYING 1
#define QUIT 0

typedef enum {
    LEFT, RIGHT, UP, DOWN
} Direction;

typedef struct Position{
    int x;
    int y
} Position;

//Estas constantes definen valores simbólicos para las teclas de flecha
#define LEFT_ARROW_1 'a'
#define RIGHT_ARROW_1 'd'
#define UP_ARROW_1 'w'
#define DOWN_ARROW_1 's'

#define LEFT_ARROW_2 'j'
#define RIGHT_ARROW_2 'l'
#define UP_ARROW_2 'i'
#define DOWN_ARROW_2 'k'

#define STARTING_1_X MAPWIDTH / 2
#define STARTING_1_Y 4

#define STARTING_2_X MAPWIDTH / 2
#define STARTING_2_Y MAPHEIGHT - 4


#endif //ELIMINATOR_H