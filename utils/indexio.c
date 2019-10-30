/* indexio.c --- 
 * 
 * 
 * Author: Sudharsan Balasubramani
 * Created: Sat Oct 26 21:53:32 2019 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */

#include <stdio.h>
#include <hash.h>
#include <queue.h>
#include <webpage.h>
#include <pageio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

FILE *indexname;


typedef struct hashIndex {
	char *word; // word
	queue_t *pages; //queue pointer of webpages that have this word
} hashIndex_t;

typedef struct wordPage {
	int id; //webpage id
	int frequency; //word frequency
} wordPage_t;


static void savesq(void *wordPage) {
	wordPage_t *mywp = (wordPage_t *)wordPage;
	fprintf(indexname, " %d %d", mywp->id, mywp->frequency);
}

static void savesht(void *hashIndex) {
	hashIndex_t *myhi = (hashIndex_t *)hashIndex;
	fprintf(indexname, "%s", myhi->word);
	qapply(myhi->pages, savesq);
	fprintf(indexname,"\n");
}

static wordPage_t *makeWordPage(int id) {
	wordPage_t *page;
	if (!(page = (wordPage_t *)malloc(sizeof(wordPage_t)))) {
		return NULL;
	}
	page->id = id;
	page-> frequency = 1;

	return page;
}

static hashIndex_t *makeIndex(char *word, int id) {
	hashIndex_t *Hi;
	wordPage_t *page;
	if (!(Hi = (hashIndex_t *)malloc(sizeof(hashIndex_t)))) {
		return NULL;
	}
	Hi->pages = qopen();
	page = makeWordPage(id);
	qput(Hi->pages, (void *)page);

	Hi->word = word;
	return Hi;
}

static void deleteWordPage(void *wordPage){                                                                                             
  free(wordPage);                                                                                                                       
} 
                                                                                                                                      
static void deleteIndex(void *hashIndex){                                                                                               
  hashIndex_t *hi = (hashIndex_t *)hashIndex;                                                                                           
  queue_t *myQueue = hi->pages;                                                                                                         
  qapply(myQueue, deleteWordPage);                                                                                                      
  qclose(myQueue);                                                                                                                      
  free(hi->word);                                                                                                                       
  free(hashIndex);                                                                                                                      
}         


static bool searchfn(void *elementp, const void *searchkeyp) {
	hashIndex_t *Hi = (hashIndex_t *)elementp;
	if(strcmp(Hi->word, searchkeyp) == 0) {
		return true;
	}
	return false;
}

static bool qsearchfn(void *elementp, const void *searchkeyp) {
	wordPage_t *myWordPage = (wordPage_t *)elementp;
	if (strcmp((const char *)(&(myWordPage->id)), searchkeyp) == 0) {
		return true;
	}
	return false;
}

static void NormalizeWord(char *word) {
	if(word != NULL) {
		for (char *c = word; *c != '\0'; c++) {
			*c = (char)tolower(*c);
		}
	}
}


/* Saves an index */
int indexsave(hashtable_t *index, char *filename) {
	indexname = fopen(filename, "w");
	if (indexname != NULL) {
		happly(index, savesht);
		fclose(indexname);
		return 0;
	}
	return 1;
}

/* Loads an index from an index file*/
hashtable_t *indexload(char *indexnm) {
	FILE *file = fopen(indexnm, "r");
	if (file == NULL){
		return NULL;
	}
	hashtable_t *index = hopen(50);
 
	// Fscanf to the first word
	// add to the index with key of word a new
	// then keep f scanning the integers, until we hit a new line
	// THen scan to the next word and repeat
	
}
