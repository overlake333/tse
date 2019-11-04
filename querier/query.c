/*
 * 
 * 
 * Author: James D. Fleming
 * Created: Thu Oct 31 16:55:14 2019 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <indexio.h>
#include <hash.h>
#include <webpage.h>
#include <queue.h>


// Struct that is put into queue based on query
typedef struct queryPage{
	int rank;
	int id;
	webpage_t *page;
} queryPage_t;


// Word page and hash Index Structs
typedef struct wordPage{
  int id; // webpage id
  int frequency; // word frequency
} wordPage_t;

typedef struct hashIndex{
  char *word;
  // frequency is outdated
  //int frequency;
  // queue pointer of webpages that have this word
  queue_t *pages;
} hashIndex_t;


//create a queryPage_t
static queryPage_t *makeQueryPage(int id, int rank, webpage_t *page){
	queryPage_t *myQueryPage = (queryPage_t *)malloc(sizeof(queryPage_t));
	myQueryPage->id = id;
	myQueryPage->rank= rank;
	myQueryPage->page = page;
	return myQueryPage;
}

//queueApply function to be applied onto the queue associated to a word
// this queue is full of wordPages so this function should take in a wordpage
static void applyfn(void *queryPage){
	queryPage_t *myqp = (queryPage_t *)queryPage;
	printf("rank: %d: doc: %d: %s\n", myqp->rank, myqp->id, webpage_getURL((const webpage_t *)(myqp->page)));	
}
// FUnctions to help with memory leaks


void deleteWordPage(void *wordPage){
	free(wordPage);
}

void deleteIndex(void *hashIndex){
	hashIndex_t *hi = (hashIndex_t *)hashIndex;
	queue_t *myQueue = hi->pages;
	qapply(myQueue, deleteWordPage);
	qclose(myQueue);
	free(hi->word);
	free(hashIndex);
}

void deleteQueryPage(void *queryPage){
	queryPage_t *qp = (queryPage_t *)queryPage;
	webpage_delete(qp->page);
	free(qp);
}
// Search functions for our hash tables and queues

// this one asks if the hash table has an entry with word given by searchkeyp
static bool searchfn(void *elementp, const void* searchkeyp){
  hashIndex_t *Hi = (hashIndex_t *)elementp;
  if (strcmp(Hi->word, searchkeyp) == 0){
    return true;
  }
  return false;
}

// searches the queue inside a hashtable storing wordPages
// to see if a word has a frequency for a given id
static bool qsearchfn(void *elementp, const void *searchkeyp) {
  wordPage_t *myWordPage = (wordPage_t *)elementp;
  if(strcmp((const char *)(&(myWordPage->id)), searchkeyp) == 0) {
    return true;
  }
  return false;
}





static int readline(char **string){
	// given a pointer to a pointer of a string
	// allocate memory for the data pointed to by string
	*string = (char *)malloc(100);
	char *p = *string;
	char ch;

	// scans the first character to make sure we are not reading the end of the file
	if (scanf("%c", &ch)==EOF){
		return -1;
		// end of file return -1
	}
	// handle a carriage return
	if (ch == '\n')
		{
			// just a newline return 0 
			return 0;
		}
	// until we hit the end of the line
	while(ch != '\n'){
		// make sure the characters we are reading are either a letter or a space
		if (isalpha(ch) || isspace(ch)){
			// add the lowercase letter to the string
			*p++ = tolower(ch);
			// scan the next character
			scanf("%c", &ch);
		}else{
			// if we got a number or punctiation return 2 but make sure we get to the end of the query entry first
			while (getchar() != '\n');
			return 2;
		}
	}
	// Always add a terminating character to the string~
	*p = '\0';
	// successful creation of string return 1

	return 1;
}


int main(void){

	
	
	
	// print the first carrot
	printf("> ");
	char *line;
	// holds an array of strings. This is the query given by the user split word by word
	char array[50][100];

	// Return value of readline
	int value;
	
	hashtable_t *index = indexload("../indexer/valTest.txt");
	
	// read until we get to end o fo a 
	while ((value = readline(&line)) != -1){
		// reset i each loop, keeps track of number of strings in a query
		int i = 0;
		// if we got a successful query
		if (value == 1){
			// splt the query word by word with strtok
			char *token = strtok(line, " ");
			while (token != NULL) {
				printf("%s ", token);
				strcpy(array[i], token);
				token = strtok(NULL, " ");
				i++;
		 	}
			printf("\n");
			// j is used to loop through i number of strings in a query
								// check to see if queried word is in index
			int id = 1;
			webpage_t *myPage;
			
		 
		  //myPage = pageload(id, "pages");
			//if (myPage == NULL){
			//printf("did not get the page!");
			//}
			queue_t *queryList;
			queryList = qopen();
			while((myPage = pageload(id, "pages")) !=NULL){
			 	
				int j = 0;
				int min = 0;
				
				while (j < i) {
					// make sure we don't include reserved words in query
					if (strcmp(array[j], "and") != 0 && strcmp(array[j], "or") != 0) {
						
						hashIndex_t *hi = (hashIndex_t *)(hsearch(index, searchfn, (const char *)array[j], strlen(array[j])));			
						if (hi != NULL) {
							
							queue_t *documents = hi->pages;
							// check to see if word has a frequency for given page id
							
							wordPage_t *mywp = (wordPage_t *)(qsearch(documents, qsearchfn, (const char *)&id));
							if (mywp != NULL) {
								// if it does, grab that frequency
								int frequency = mywp->frequency;
								
								// determine what word in the query has the minimum rank and keep track of it
								if (j == 0) {
									min = frequency;
								} else if (frequency < min) {
									min = frequency;
								}
								// prints the output: word and frequency and then rank at end for query
								
							} else {
								// In case query has a word not in the page we are looking at
								min = 0;
							}
						} else {
							// In case a word in the query is not in the index
							min = 0;
						}
					}
					j++;
					
					if (min == 0){
						break;
					}
					
				}
				if(min != 0){
					queryPage_t *myQueryPage = makeQueryPage(id, min, myPage);
					qput(queryList, myQueryPage);
					
				}else{
					webpage_delete(myPage);
				}
				id++;
			}
			printf("\n");
			qapply(queryList, applyfn);
			qapply(queryList, deleteQueryPage);
			qclose(queryList);

			
		}
	
		if (value == 2){
			printf("[invalid query]\n");
		}
		value = 0;
		printf("> ");
		free(line);
		
		
	}
	// memory grinding ;)
	free(line);		
	happly(index, deleteIndex);	
	hclose(index);	
 	return 0;
}
