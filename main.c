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

/** Dimensions of the game of life field */
#define DIM_X 50
#define DIM_Y 20

/** States on the board */
#define ALIVE '*'
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
int init_life(char field[][DIM_Y], struct Cell coords[], int ncoords) { 
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
 * print_field 
 * 
 * Prints the field of life to the wonderful user
 * 
 * @param field Field to print
*/
void print_field(char field[][DIM_Y]) { 
	for (int y = 0; y < DIM_Y; y++) { 
		for (int x = 0; x < DIM_X; x++) { 
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
				printf("(%d, %d) == Alive: %d\n", x, y, nalive);
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

int main(int argc, char* argv[]) { 	
	int time = 0;                  // How many seconds have passed 
	int population = 0;            // Number of alive cells in the simulation 
	char curr_gen[DIM_X][DIM_Y]; // Initialize the generation fields
	
	// User will supply a list of starting cells to initialize to alive
	struct Cell coords[] = {
		// Static box {1,1}, {1,2}, {2,2}, {2,1}
		{5,5}, {5,7}, {6,6}, {6,7}, {7,6} // Glider
		}; // hardcoded coordinates
	int len = 5; // number of coordinatates

	// Generate the game board with list provided by user 
	if(!init_life(curr_gen, coords, len)) { 
		printf("Invalid Coordinates!\n");
		return 0;
	}
	
	while(1) { 
		printf("Time: %d Population: %d\n", time, population);
		print_field(curr_gen);         // Print the field
		population = update(curr_gen); // Update per Conway's rules
		sleep(1);           
		time += 1;
		system("clear");    
	}

	return 0;
}
