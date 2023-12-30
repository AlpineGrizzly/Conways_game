/** 
 * Conway's Game of life in C
 * 
 * Author R3v
 * December 19th, 2023 in a Chicago Airport
*/

// TODO would like to have the simulation run in ncurses eventually 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define RAND_FILE "random-words.txt"
#define LINE_COUNT 2642 // Number of lines in the random word file
#define CELLS_DIR "cells/" // Directory where cells are stored
#define CELL_SPAWN_CHANCE 4

#define MILLISECOND(x) ((x % 1000 ) * 1000)

/** Dimensions of the game of life field */
#define DIM_X 200
#define DIM_Y 50

/** States on the board */
#define ALIVE 'O'
#define DEAD  '.'

struct Cell { 
	int x;
	int y;
};

int is_valid_coord(struct Cell coord) { 
	if (coord.x < 0 || coord.y < 0 || coord.x >= DIM_X || coord.y >= DIM_Y)
		return 0;
	return 1;
}

/**
 * init_life
 * 
 * Initializes the field with a set of coordinates to start as alive
 * 
 * @param field Game of life field
 * @param coords Array of coordinates to mark alive on the field
 * 
 * @return 1 if coordinates were successfully set, 0 otherwise
*/
int initcoords_life(char field[][DIM_Y], struct Cell coords[], int ncoords) { 
	// Initialize the field array with empty spaces
	for (int y = 0; y < DIM_Y; y++) { 
		for (int x = 0; x < DIM_X; x++) { 
			field[x][y] = DEAD;
		}
	}

	// Validate and plot coordinates supplied by user
	for (int c = 0; c < ncoords; c++) { 
		if (is_valid_coord(coords[c]))
			field[coords[c].x][coords[c].y] = ALIVE;
		else
			return 0; // Disapointment
	}

	return 1; // cool beans 
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

		/// begin filling in the cells 		
		srand(time(0)); // Seed random number generator
		for (y = 0; y < DIM_Y/2 ; y++) { 
			for (x = 0; x < DIM_X/2; x++) { 
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

	// Otherwise, initialize the field with the contents of the file
	x, y = 0;
	do { 
		ch = fgetc(f);
		if (ch == ALIVE || ch == DEAD)
			field[x][y] = ch;
		else if (ch == '\n') { 
			x = 0;
			y++;
		}
		x++;
	} while(ch != EOF);

	fclose(f);
	return 1;
}

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
			if ((x == c_x && y == c_y) || x < 0 || y < 0 || x >= DIM_X || y >= DIM_Y)
				continue; // Base case : Do not count the target cells state && Non existent neighbor 
			if (field[x][y] == ALIVE) {
				count++;
			}
		}
	}

	return count;
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

void usage() { 
	char* usage_string = "Usage: life\n" 
						 "Given an initialization file or generating its own starting values, will conduct a Conway's Game of Life simulation.\n\n" 
						 "-h      Display this help message\n"  
						 "-r      Will generate random starting cells and save off as a file to be used"
						 "-f      Starting values to be used for simulation (If none are supplied, will be randomly generated)\n";
	printf("%s", usage_string);
	exit(0);
}

int main(int argc, char* argv[]) { 	
	int time = 0;                // How many seconds have passed 
	int population = 0;          // Number of alive cells in the simulation 
	char curr_gen[DIM_X][DIM_Y]; // Initialize the generation fields
	int delay = 100;
	int opt; 
	int random_on = 0;           // Boolean switch for enable randomized starting cell values
	char* init_fn = NULL;

	// Parse arguments 
	while ((opt = getopt(argc, argv, "hrf:")) != -1) {
        switch (opt) {
        	case 'h': usage(); break;
			case 'r': random_on = 1; break;
        	case 'f': init_fn = optarg; break;
        default:
            usage();
        }
    }

	// Generate the game board with list provided by user 
	if(!init_life(curr_gen, init_fn, random_on)) { 
		printf("Unable to generate simulation!\n");
		return 0;
	}
	
	// Simulation while loop 
	while(1) { 
		system("clear");    
		printf("T %d Population: %d\n", time, population);
		print_field(curr_gen);         // Print the field
		population = update(curr_gen); // Update per Conway's rules
		usleep(MILLISECOND(delay));           
		time += 1;
	}

	return 0;
}
