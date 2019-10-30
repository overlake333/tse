
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
typedef struct hashIndex{
	char *word;
	queue_t *pages;
} hashIndex_t;                                                                                                             

typedef struct wordPage{                                                                                                   
  int id; // webpage id                                                                                                    
  int frequency; // word frequency                                                                                         
} wordPage_t;        



hashIndex_t *makeIndex(char *word){
  hashIndex_t *Hi;
  if (!(Hi = (hashIndex_t *)malloc(sizeof(hashIndex_t)))){
    return NULL;
  }
  Hi->pages = qopen();
  Hi->word = word;
	
	return Hi;
}

wordPage_t *makeWordPage(int id, int count) {
  wordPage_t *page;
  if (!(page = (wordPage_t *)malloc(sizeof(wordPage_t)))){
    return NULL;
  }

  page->id = id;
  page->frequency = count;

  return page;
}


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
	char word[50];
	while(fscanf(file, "%s", word) != EOF){
		char *wordd = (char *)malloc(strlen(word)*sizeof(char));
		strcpy(wordd, word);
		hashIndex_t *ht = makeIndex(wordd);
		printf("%s\n", ht->word);
		int id;
		int count;
		while(fscanf(file, "%d %d", &id, &count) > 0){
			wordPage_t *wordPage = makeWordPage(id, count);
			qput((ht->pages), wordPage);		
		}
		hput(index, ht, (const char *)word, strlen(word));
	}
	return index;
}
