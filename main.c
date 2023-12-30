/** 
 * Conway's Game of life in C
 * 
 * Author R3v
 * December 19th, 2023 in a Chicago Airport
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "conway.h"

#define MILLISECOND(x) ((x % 1000 ) * 1000)

/**
 * usage
 * 
 * Prints the usage of the program
*/
void usage() { 
	char* usage_string = "Usage: life\n" 
						 "Given an initialization file or generating its own starting values, will conduct a Conway's Game of Life simulation.\n\n" 
						 "-h      Display this help message\n"  
						 "-r      Will generate random starting cells and save off as a file to be used\n"
						 "-f      Starting values to be used for simulation (If none are supplied, will be randomly generated)\n";
	printf("%s", usage_string);
	exit(0);
}

int main(int argc, char* argv[]) { 	
	int time = 0;                // How many seconds have passed 
	int population = 0;          // Number of alive cells in the simulation 
	char curr_gen[DIM_X][DIM_Y]; // Initialize the generation fields
	int delay = 100;             // Delay of each life update in milliseconds
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

	// If a file wasn't provided + randomization isn't turned on print usage
	if (init_fn == NULL && !random_on) { usage(); }

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
