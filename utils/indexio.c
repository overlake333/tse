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

/* Loads an index */
int indexload(hashtable_t *index, char *dirname) {
	int id = 1;
	//char crawlerFile[50];
	//sprintf(crawlerFile, "../%s/%d", dirname, id);
	webpage_t *page;
	
	while((page = pageload(id, dirname)) != NULL) {
		int pos = 0;
		char *word;

		while ((pos = webpage_getNextWord(page, pos, &word)) >0) {

			NormalizeWord(word);
			if (strlen(word) >= 3) {
				hashIndex_t *hw = (hashIndex_t *)(hsearch(index, searchfn, (const char *)word, sizeof(word)));
				if (hw == NULL) {
					// create new hashIndex_t
					hashIndex_t *ht = makeIndex(word,id);
					// add it to table
					hput(index, ht, (const char *)word, sizeof(word));

				} else {
					//update frequency of hashIndex_t by 1
					wordPage_t *wp = (wordPage_t *)(qsearch(hw->pages, qsearchfn, (const char *)&id));
					if(wp == NULL) {
						// create new wordPage - id and frequency
						wordPage_t *newPage = makeWordPage(id);
						// add it to queue of the word
						qput(hw->pages, (void *)newPage);
					} else {
						//update frequency by 1
						wp ->frequency = wp->frequency + 1;
					}
				}
			}
		}
		id++;
	}
	return 0;			
}
