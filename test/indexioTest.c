/* indexioTest.c --- 
 * 
 * 
 * Author: James D. Fleming
 * Created: Wed Oct 30 17:29:30 2019 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */
#include <hash.h>
#include <indexio.h>
#include <stdlib.h>
#include <stdio.h>
#include <queue.h>

typedef struct hashIndex{
	char *word;
	
	// frequency is outdated
	//int frequency;
	// queue pointer of webpages that have this word
	queue_t *pages;
	
	
} hashIndex_t;


// MAKE A STRUCT THAT HOLDS A WEBPAGE and frequency of a word asssociated with that webpage

typedef struct wordPage{
	int id; // webpage id
	int frequency; // word frequency
} wordPage_t;


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


int main(void) {
	hashtable_t *index = indexload("../indexer/valTest.txt");
	indexsave(index, "testIO.txt");
	happly(index, deleteIndex); 
	hclose(index);
	return 0;
}
