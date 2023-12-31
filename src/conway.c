/** 
 * conway.c 
 * 
 * Conway's game of life implementation logic
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "conway.h"

/**
 * print_field 
 * 
 * Prints the field of life to the wonderful user
 * 
 * @param field Field to print
*/
void print_field(char field[][DIM_Y]) { 
	int x, y;
	for (y = 0; y < DIM_Y; y++) { 
		for (x = 0; x < DIM_X; x++) { 
			putc(field[x][y], stdout);
		}
		putc('\n', stdout);
	}
	putc('\n', stdout);
}

/**
 * update
 * 
 * Update the field using the rules from Conway's game of life
 * 
 * @param curr_gen Current generation 
 * @param next_gen Next generation to update with conway's rules 
 * 
 * @return Number of alive cells 
*/
int update(char curr_gen[][DIM_Y]) { 
	char next_gen[DIM_X][DIM_Y]; // initialize the next generation 
	int population = 0;          // Keeps count of how many cells are alive
	int nalive; // Keeps count of alive cells around a current cell

	// Loop were the game of life will update to the following each tick
	int x, y;
	for (y = 0; y < DIM_Y; y++) { 
		for (x = 0; x < DIM_X; x++) { 
			nalive = get_survivors(curr_gen, x, y); // Get number of cells alive current cell
			
			if (curr_gen[x][y] == ALIVE) { 
				if (nalive < 2 || nalive > 3) { 
					// 1. Any live cell with fewer than two live neighbors dies (if by underpopulation)
					// 2. Any live cell with more than three live neighbors dies (if by overpopulation)
					next_gen[x][y] = DEAD;
					continue;
				}
			} else if (nalive == 3) { 
				// 3. Any dead cell with exactly three live neighbors comes to life
				next_gen[x][y] = ALIVE;
				continue;
			}
			next_gen[x][y] = curr_gen[x][y];
		}
	}
	// Copy the next generation into the current generation
	for (y = 0; y < DIM_Y; y++)  {
		for (x = 0; x < DIM_X; x++) { 
			if(next_gen[x][y] == ALIVE)
				population++;
			curr_gen[x][y] = next_gen[x][y];
		}
	}

	return population;
}

/**
 * get_survivors
 * 
 * Returns number of alive cells around a target cell
 * 
 * @param field Field to search for alive cells
 * @param target Target cell to search for alive cells around
 * 
 * @return Number of alive cells
*/
int get_survivors(char field[][DIM_Y], int c_x, int c_y) { 
	int count = 0;

	// Check all 8 neighbors to see if they are alive and return count
	int y, x;
	for (y = c_y-1; y <= c_y+1; y++) { 
		for (x = c_x-1; x <= c_x+1; x++) { 
			if (x == c_x && y == c_y)
				continue; // Base case : Do not count the target cells state && Non existent neighbor 
			if (x < 0 || y < 0 || x >= DIM_X || y >= DIM_Y) { // Wrap to find states if we are on an x or y edge
				    int wrap_x = ((x % DIM_X) + DIM_X) % DIM_X;
    				int wrap_y = ((y % DIM_Y) + DIM_Y) % DIM_Y;
					//printf("%d %d Checking instead %d %d\n", x, y, wrap_x, wrap_y);
					if(field[wrap_x][wrap_y] == ALIVE)
						count++;
			} else if (field[x][y] == ALIVE) {
				count++;
			}
		}
	}

	return count;
}

/**
 * init_life
 * 
 * Initializes the simulation from a file or a random set of starting values
 * 
 * @param field 2D array that stores the state of the field
 * @param initfn Pointer to initialization file if one exists, if not random starting values will be used
 * @param random Boolean to set random starting cell values if true
 * 
 * @return 1 if initialization was successful, 0 otherwise
*/
int init_life(char field[][DIM_Y], char* initfn, int random) { 
	// Check if file exists
	FILE* f; 
	char ch;
 
	// Initialize the field array with empty spaces
	int x, y = 0;
	for (y = 0; y < DIM_Y; y++) { 
		for (x = 0; x < DIM_X; x++) { 
			field[x][y] = DEAD;
		}
	}

	if (random) {
		// Random generate a file with coordinates
		/// Generate a random name for the .cells file
		char cellname[80];
		strcpy(cellname, CELLS_DIR);
		strcat(cellname, "testfile");
		strcat(cellname, ".cells");
		initfn = cellname;

		printf("writing to %s\n", initfn);

		/// open it in write mode and randomly generate 
		f = fopen(initfn, "w");
		if(f == NULL) { 
			printf("Unable to generate cells file\n");
			return 0;
		}

		srand(time(0)); // Seed random number generator

		/// begin filling in the cells 		
		for (y = 0; y < DIM_Y; y++) { 
			for (x = 0; x < DIM_X; x++) { 
				if((rand() % 9) < CELL_SPAWN_CHANCE-1) { 
					field[x][y] = ALIVE;
					fputc(ALIVE, f);
				} else
					fputc(DEAD, f);
			}
			fputc('\n', f);
		}
		fputc('\n', f);
		fclose(f);
	}  

	printf("successfully generated\n");
	f = fopen(initfn, "r");
	if (f == NULL) { 
		printf("File does not exist!\n");
		return 0;
	}

	printf("Reading %s\n", initfn);
	// Otherwise, initialize the field with the contents of the file
	x = 0; 
	y = 0;
	do { 
		printf("yippe %c %d %d\n", ch, x ,y );
		ch = fgetc(f);
		if (ch == ALIVE || ch == DEAD) { 
			field[x][y] = ch;
		} else { 
			x = 0;
			y++;
		}
		x++;
	} while(ch != EOF && x <= DIM_X+1 && y < DIM_Y);
	
	fclose(f);
	return 1;
}
