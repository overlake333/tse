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
	//	indexload(index, pagedir);


	indexsave(index, indexnm);
<<<<<<< HEAD
	/*
	for (int id = 1; id <= toId; id++) {
		webpage_t *page = pageload(id, "pages");
		int pos = 0;
		char *word;
		char out[50];
		sprintf(out, "out%d.txt", id);
		FILE *output = fopen(out, "w"); // output file to write html to
		
		// Uses webpage_getNextWord to print all words in HTML. SKIPS TAGS! 		
		
		
		while ((pos = webpage_getNextWord(page, pos, &word)) > 0) {
			//printf("%s\n", word);
			NormalizeWord(word);
			if (strlen(word) >= 3){
				fprintf(output, "%s\n", word); // prints the words of the html to a file
				// if word is not already in hash table
				hashIndex_t *hw = (hashIndex_t *)(hsearch(index, searchfn, (const char *)word, sizeof(word)));
				if (hw == NULL){
					// create a new hashIndex_t
					hashIndex_t *ht = makeIndex(word, id);
					// add it to hashtable
					hput(index, ht, (const char *)word, sizeof(word));
					
				} else {
					// update the frequency of the hashindex_t by 1
					//hw->frequency = hw->frequency +1;
					wordPage_t *wp = (wordPage_t *)(qsearch(hw->pages, qsearchfn, (const char *)&id));
					if (wp == NULL) {
						// create new wordPage - has a id and frequency!
						wordPage_t *newPage = makeWordPage(id);
						// add it to queue of the word
						qput(hw->pages, (void *)newPage);
					} else {
						// Update a wordPage's frequency by 1
						wp->frequency = wp->frequency +1;
					}
					
				}
				
				
			}
			//free(word);
		}
	}
	*/
	//	printf("%d\n", sumwords(index));
	happly(index, deleteIndex);
=======
 
>>>>>>> ddea77a245b96dcfd203abecc7784bbb0c5ad331
	hclose(index);
	exit(EXIT_SUCCESS);
}



