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

FILE *qOutput;
//queueApply function to be applied onto the queue associated to a word
// this queue is full of wordPages so this function should take in a wordpage
static void applyfn(void *queryPage){
	queryPage_t *myqp = (queryPage_t *)queryPage;
	fprintf(qOutput,"rank: %d: doc: %d: %s\n", myqp->rank, myqp->id, webpage_getURL((const webpage_t *)(myqp->page)));	
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





static int readline(char **string, FILE* qInput){
	// given a pointer to a pointer of a string
	// allocate memory for the data pointed to by string
	*string = (char *)malloc(100);
	char *p = *string;
	char ch;
	int i = 0;
	// scans the first character to make sure we are not reading the end of the file
	if (fscanf(qInput,"%c", &ch)==EOF){
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
			i++;
			if (i > 95) {
				return 2;
			}
			// scan the next character
			fscanf(qInput,"%c", &ch);
		}else{
			// if we got a number or punctiation return 2 but make sure we get to the end of the query entry first
			while (fscanf(qInput, "%c", &ch)) {
				if (ch == '\n') {
					return 2;
				}
			}
		}
	}
	// Always add a terminating character to the string~
	*p = '\0';
	
	// successful creation of string return 1

	return 1;
}

static bool querysearchfn(void *elementp, const void *keyp){
	queryPage_t *myQueryPage = (queryPage_t *)elementp;
	if (myQueryPage->id == *((int *)keyp)) {
		return true;
	}
	return false;
}


hashtable_t *index;
queue_t *queryList;

static void parseQuery(char *line, char *pagedir){
	printf("running parseQuery on %s\n", line);
	char array[50][100];
	
	int i = 0;
	char *token = strtok(line, " ");
	while (token != NULL) {
		//printf("%s ", token);
		strcpy(array[i], token);
		token = strtok(NULL, " ");
		i++;
	}
	//printf("\n");
	// j is used to loop through i number of strings in a query
	// check to see if queried word is in index
	int id = 1;
	webpage_t *myPage;
	
	
	//myPage = pageload(id, "pages");
	//if (myPage == NULL){
	//printf("did not get the page!");
	//}
	
	while((myPage = pageload(id, pagedir)) !=NULL){
		int j = 0;
		int min = 0;
		
		while (j < i) {
			// make sure we don't include reserved words in query
			if (strcmp(array[j], "and") != 0) {
				
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
		//ranksum += min;
		if(min != 0){
			queryPage_t *myQueryPage = (queryPage_t *)qsearch(queryList, querysearchfn, (const void *)&id);
			if (myQueryPage == NULL) {
				myQueryPage = makeQueryPage(id, min, myPage);
				qput(queryList, (void *)myQueryPage);
			} else {
				myQueryPage->rank += min;
				webpage_delete(myPage);
			}
		}else{
			webpage_delete(myPage);
		}
		id++;
	}
	
	
}



int main(int argc, char *argv[]){

	// usage statement
	char *usage = "usage: query <pageDirectory> <indexFile> [-q]";

	char *pagedir;
	char *indexnm;
	FILE *qInput;
	
	if (argc == 3 || argc == 6) {
		pagedir = argv[1];
		indexnm = argv[2];
		if (argc == 6) {
			if (strcmp(argv[3], "-q") == 0) {
				qInput = fopen(argv[4], "r");
				qOutput = fopen(argv[5], "w");
			} else {
				printf("%s\n", usage);
				exit(EXIT_FAILURE);
			}
		} else if (argc == 3) {
			qInput = stdin;
			qOutput = stdout;
		} 
	} else {
		printf("%s\n", usage);
		exit(EXIT_FAILURE);
	}

	

	// print the first carrot
	printf("> ");
	char *line;
	// holds an array of strings. This is the query given by the user split word by word
	// Return value of readline
	int value;

	char indexFile[50];
	sprintf(indexFile, "../indexer/%s", indexnm);
	index = indexload(indexFile);
	
	// read until we get to end o fo a 
	while ((value = readline(&line, qInput)) != -1){
		// reset i each loop, keeps track of number of strings in a query
	
		// if we got a successful query
		if (value == 1){
			queryList = qopen();
 
			int len = 0;
			//Split by " or "
			char *token = strtok(line, " ");
			char tstring[100];
			char array[20][100];
			bool didBreak = false;
			bool prevSpecial = false;
			// Replace or with a comma and check validity of queue
			while (token != NULL) {
				if (len == 0 || prevSpecial == true){
					if (strcmp(token, "or") == 0 || strcmp(token, "and") == 0){
						value = 2;
						didBreak = true;
						break;
					}
				}
				if (strcmp(token, "or") != 0) {
					if (strcmp(token, "and") ==0){
						prevSpecial = true;
					}else{
						prevSpecial = false;
					}
					if (len != 0) {
						sprintf(tstring, "%s %s", tstring, token);
						len+= strlen(token)+1;
					} else {
						sprintf(tstring, "%s", token);
						len = strlen(token);
					}
				} else {
					
					sprintf(tstring, "%s .", tstring);
					prevSpecial = true;
				}
				
				token = strtok(NULL, " ");	
				if (token ==NULL){
					if(prevSpecial == true){
						value = 2;
						didBreak = true;
						break;
					}
				}
			}
			int i = 0;
			
			char *tkn = strtok(tstring, ".");
			if (didBreak == false){
				while (tkn != NULL){
					
					
					strcpy(array[i], tkn);  
					
					tkn = strtok(NULL, ".");
					
					i++;
				}
				int j = 0;
				while (j< i){
					
					printf("about to run parseQuery on: %s\n", array[j]);
					parseQuery(array[j], pagedir);
					j++;
				}
				qapply(queryList, applyfn);
				qapply(queryList, deleteQueryPage);
				qclose(queryList);
				
			}
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
	fclose(qInput);
	fclose(qOutput);
 	return 0;
}


