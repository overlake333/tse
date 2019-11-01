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

typedef struct queryPage{
	int rank;
	int id;
	webpage_t *page;
} queryPage_t;

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


static bool searchfn(void *elementp, const void* searchkeyp){
  hashIndex_t *Hi = (hashIndex_t *)elementp;
  if (strcmp(Hi->word, searchkeyp) == 0){
    return true;
  }
  return false;
}


static bool qsearchfn(void *elementp, const void *searchkeyp) {
  wordPage_t *myWordPage = (wordPage_t *)elementp;
  if(strcmp((const char *)(&(myWordPage->id)), searchkeyp) == 0) {
    return true;
  }
  return false;
}





static int readline(char **string){
  *string = (char *)malloc(100);
	char *p = *string;
	char ch;
	
	if (scanf("%c", &ch)==EOF){
		return -1;
		// end of file return -1
	}
	if (ch == '\n')
		{
			// just a newline return 0 
			return 0;
		}
	while(ch != '\n'){
		if (isalpha(ch) || isspace(ch)){
			*p++ = tolower(ch);
			scanf("%c", &ch);
		}else{
			while (getchar() != '\n');
			return 2;
		}
	}
	*p = '\0';
	// successful creation of string return 1

	return 1;
}


int main(void){
	
	printf("> ");
	char *line;
	char array[50][100];
	int value;
	hashtable_t *index = indexload("../indexer/querytest.txt");
	
	
	while ((value = readline(&line)) != -1){
		int i = 0;
		if (value == 1){
			char *token = strtok(line, " ");
			while (token != NULL) {
				strcpy(array[i], token);
				token = strtok(NULL, " ");
				i++;
		 	}
			int j = 0;
			int min;
			while (j < i) {
				if (strcmp(array[j], "and") != 0 && strcmp(array[j], "or") != 0) {
					hashIndex_t *hi = (hashIndex_t *)(hsearch(index, searchfn, (const char *)array[j], strlen(array[j])));			
					if (hi != NULL) {
						int id = 1;
						wordPage_t *mywp = (wordPage_t *)(qsearch(hi->pages, qsearchfn, (const char *)&id));
						if (mywp != NULL) {
							int frequency = mywp->frequency;
							if (j == 0) {
								min = frequency;
							} else if (frequency < min) {
								min = frequency;
							}
							printf("%s:%d ", array[j], frequency);
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
			}
			printf("-- %d\n", min); 
		}
		if (value == 2){
			printf("[invalid query]\n");
		}
		value = 0;
		printf("> ");
		free(line);
	}
	free(line);		
	happly(index, deleteIndex);
	hclose(index);	
 	return 0;
}
