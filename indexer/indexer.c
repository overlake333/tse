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

/* INTERNAL FUNCTION PROTOTYPES */
void NormalizeWord(char *word);


typedef struct hashIndex{
	char *word;

	// frequency is outdated
	int frequency;
	// queue pointer of webpages that have this word
	
} hashIndex_t;


// MAKE A STRUCT THAT HOLDS A WEBPAGE and frequency of a word asssociated with that webpage

// make a queue of these types 


static hashIndex_t *makeIndex(char *word){
	hashIndex_t *Hi;
	if (!(Hi = (hashIndex_t *)malloc(sizeof(hashIndex_t)))){
		return NULL;
	}
	Hi->frequency = 1;
	Hi->word = word;
	return Hi;
}
static void deleteIndex(void *hashIndex){
	free(hashIndex);
}
static bool searchfn(void *elementp, const void* searchkeyp){
	hashIndex_t *Hi = (hashIndex_t *)elementp;
	if (strcmp(Hi->word, searchkeyp) == 0){
		return true;
	}
	return false;
}

static int wordCount = 0;
static void addToCount(void *ep){
	hashIndex_t *hi = (hashIndex_t *)ep;
	wordCount = wordCount + (hi->frequency);
}
static int sumwords(hashtable_t *ht){
	happly(ht, addToCount);
	return wordCount;
	
}

// Main
int main(void) {

	// Loads a webpage with pageio module
	webpage_t *page = pageload(2, "pages");
	int pos = 0;
	char *word;
	FILE *output = fopen("output.txt", "w"); // output file to write html to
	hashtable_t *index = hopen(50);
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
				hashIndex_t *ht = makeIndex(word);
				// add it to hashtable
				hput(index, ht, (const char *)word, sizeof(word));
				
			}else{
				// update the frequency of the hashindex_t by 1
				hw->frequency = hw->frequency +1;
			}

						
		}
		//free(word);
	}
	printf("%d", sumwords(index));
	happly(index, deleteIndex);
	hclose(index);
	exit(EXIT_SUCCESS);
}


/* INTERNAL FUNCTIONS */


/* NormalizeWord - 
 * Converts a word to lowercase
 *
 * Inputs: word - a string
 */
void NormalizeWord(char *word) {

	if (word != NULL) {
		for (char *c = word; *c != '\0'; c++) {
			*c = (char)tolower(*c);
		}
	}

}
