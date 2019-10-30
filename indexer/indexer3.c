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
	//	queue_t *pages;
	
	
} hashIndex_t;

static hashIndex_t *makeIndex(char *word){
	hashIndex_t *Hi;
	//wordPage_t *page;
	if (!(Hi = (hashIndex_t *)malloc(sizeof(hashIndex_t)))){
		return NULL;
	}
	//Hi->pages = qopen();
	//page = makeWordPage(id);
	//qput(Hi->pages, (void *)page);
	
	Hi->word = word;
	Hi->frequency = 1;
	return Hi;
}
/*
static void deleteWordPage(void *wordPage){
	free(wordPage);
}
*/
static void deleteIndex(void *hashIndex){
	hashIndex_t *hi = (hashIndex_t *)hashIndex;
	free(hi->word);
	free(hi);
}



static bool searchfn(void *elementp, const void* searchkeyp){
	hashIndex_t *Hi = (hashIndex_t *)elementp;
	if (strcmp(Hi->word, searchkeyp) == 0){
		return true;
	}
	return false;
}
/*
static bool qsearchfn(void *elementp, const void *searchkeyp) {
	wordPage_t *myWordPage = (wordPage_t *)elementp;
	if(strcmp((const char *)(&(myWordPage->id)), searchkeyp) == 0) {
		return true;
	}
	return false;
}
*/

static int wordCount = 0;
/*
static void addToCount2(void *ep) {
	wordPage_t *wp = (wordPage_t *)ep;
	wordCount = wordCount + (wp->frequency);
}
*/
static void addToCount(void *ep){
	hashIndex_t *hi = (hashIndex_t *)ep;
	//qapply(hi->pages, addToCount2); 
	wordCount = wordCount + (hi->frequency);
}

static int sumwords(hashtable_t *ht){
	happly(ht, addToCount);
	return wordCount;
	
}

// Main
int main(void) {
	
	
	// Loads a webpage with pageio module
	int id = 1;	
	
	//NEEDS TO CHECK IF DIR EXISTS
	 hashtable_t *index = hopen(1);
	
	 webpage_t *page = pageload(id, "pages");
	 int pos = 0;
		
	 char out[50];
	 sprintf(out, "out%d.txt", id);
	 FILE *output = fopen(out, "w"); // output file to write html to
		
	 // Uses webpage_getNextWord to print all words in HTML. SKIPS TAGS! 		
	 
	 char *word;
	 while ((pos = webpage_getNextWord(page, pos, &word)) > 0) {
			//printf("%s\n", word);
		 NormalizeWord(word);
		 if (strlen(word) >= 3){
			 fprintf(output, "%s\n", word); // prints the words of the html to a file
			 // if word is not already in hash table
				hashIndex_t *hw = (hashIndex_t *)(hsearch(index, searchfn, (const char *)word, strlen(word)));
				if (hw == NULL){
					// create a new hashIndex_t
					hashIndex_t *ht = makeIndex(word);
					// add it to hashtable
					hput(index, ht, (const char *)word, strlen(word));
				} else {

					// update the frequency of the hashindex_t by 1
					free(word);					
					
					hw->frequency = hw->frequency + 1;
					
					
				}
				

		 } else {
			 free(word);
		 }
 

	 }
	 webpage_delete(page);
	 fclose(output);
	 printf("%d\n", sumwords(index));
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

