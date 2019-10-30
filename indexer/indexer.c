/* indexer.c --- indexes for tse
 * 
 * 
 * Author: Sudharsan Balasubramani
 * Created: Thu Oct 24 10:19:21 2019 (-0400)
 * Version: 
 * 
 * Description: Reads the html assocaiated with eavery webpage fetched by crawler and 
 * creates in memory an index data structure.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <webpage.h>
#include <pageio.h>
#include <ctype.h>
#include <stdbool.h>
#include <hash.h>
#include <queue.h>
#include <indexio.h>

/* INTERNAL FUNCTION PROTOTYPES */




int main(int argc, char *argv[]) {
	
	char *usage = "usage: indexer <pagedir> <indexnm>";
	// Loads a webpage with pageio module
	// int toId = atoi(argv[1]);
	if (argc != 3) {
		printf("%s\n", usage);
		exit(EXIT_FAILURE);
	}
	char *pagedir = argv[1];
	char *indexnm = argv[2];
	hashtable_t *index = hopen(50);

	
	
	//NEEDS TO CHECK IF DIR EXISTS
	indexload(index, pagedir);
	indexsave(index, indexnm);
 
	hclose(index);
	exit(EXIT_SUCCESS);
}



