/** 
 * conway.h 
 * 
 * Conway's game of life function declarations
 * 
 */

/** Dimensions of the game of life field */
#define DIM_X 200
#define DIM_Y 100

/** States on the board */
#define ALIVE 'O'
#define DEAD  '.'

/** File resource defines */
#define RAND_FILE "random-words.txt"
#define LINE_COUNT 2642 // Number of lines in the random word file
#define CELLS_DIR "cells/" // Directory where cells are stored

#define CELL_SPAWN_CHANCE 4 // Chance of randomly generating an alive cell 

void print_field(char field[][DIM_Y]);
int update(char curr_gen[][DIM_Y]);
int get_survivors(char field[][DIM_Y], int c_x, int c_y);
int init_life(char field[][DIM_Y], char* initfn, int random);
