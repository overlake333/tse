/* crawler.c - prints hello
 *
 * Author: Sudharsan B, Taylor O, James F
 * Date: 10/15/209 17:52
 *
 * Version 1.0
 *
 *
 * Desc. Print hello
 */

#include <stdio.h>
#include "webpage.h"
#include "queue.h"
#include "hash.h"
#include <stdbool.h>

void qprint(queue_t *q);


int main(void){
  
  webpage_t *page = webpage_new("https://thayer.github.io/engs50/", 0, NULL);

  if(webpage_fetch(page)){
    
    int pos = 0;
    char *result;
    queue_t *webpages = qopen();
    webpage_t *url;
    
    while((pos = webpage_getNextURL(page, pos, &result)) > 0){
      if(IsInternalURL(result)){
	url = webpage_new(result, 1, NULL);
	qput(webpages, (void *)url);
	printf("%s\n", webpage_getURL(url));
      }
      free(result);
    }

    webpage_delete(url); // valgrind error ye

    

    qclose(webpages);
    webpage_delete(page);

  } else {
    exit(EXIT_FAILURE);
  }
 
  
  exit(EXIT_SUCCESS);
}


void qprint(queue_t *q){

  char *page;
  
  while((page = webpage_getURL((webpage_t *)qget(q))) != NULL){
    printf("%s\n", page);
   
  }
  free(page);
  

}

