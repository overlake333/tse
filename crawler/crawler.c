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
#include <string.h>
#include <webpage.h>
#include <queue.h>
#include <hash.h>
#include <stdbool.h>


int32_t pagesave(webpage_t *pagep, int id, char *dirname){
  char filename[20];
  FILE *save;
  char *URL = webpage_getURL(pagep);
  int depth = webpage_getDepth(pagep);
  int htmllength = webpage_getHTMLlen(pagep);
  char *html = webpage_getHTML(pagep);
  
  // creates filename
  sprintf(filename, "../%s/%d", dirname, id);

  // opens file
  save = fopen(filename, "w");

  // prints to file
  fprintf(save, "%s\n", URL);
  fprintf(save, "%d\n", depth);
  fprintf(save, "%d\n", htmllength);
  fprintf(save, "%s\n", html);

  return fclose(save);
}


static bool compareByURL(void* elementp, const void* searchkeyp){
  // this function needs to see if the url searchkeyp is the same as the url from the element
  if (strcmp(webpage_getURL((webpage_t *)elementp), (const char *)searchkeyp) == 0){
    return true;
  }
  return false;
}

int main(int argc, char *argv[]){
  // Create a new webpage
	char *seedurl;
	int maxDepth;
	char *pagedir;
  webpage_t *page = webpage_new(seedurl, 0, NULL);
  // Grab the webpage's html
  hashtable_t *visited = hopen(20);
  queue_t *webpages = qopen();
	// add first webpage to queue
	qput(webpages, (void *) page);
	// Loop through queue while it has webpages and they are above max depth
	webpage_t *myPage = (webpage_t *)qget(webpages);
	while ((myPage!= NULL) && (webpage_getDepth(myPage) <= maxDepth)){
		if(webpage_fetch(myPage)){
			// grab the depth of this webpage
			int depth = webpage_getDepth(myPage);
			// save this page
			pagesave(myPage, depth, pagedir);
			// add this page to the list of visited webpages
			const char *key = (const char *)(webpage_getURL(myPage));
			
			hput(visited, (void *)myPage, key, sizeof(key));
			int pos = 0;
			char *result;
			
			webpage_t *url;
			//Grab the next url from the page starting at pos 0
			while((pos = webpage_getNextURL(myPage, pos, &result)) > 0){
				if(IsInternalURL(result)){
					if (hsearch(visited, compareByURL, (const char *)result, sizeof((const char *)result)) == NULL){
						url = webpage_new(result, depth+1, NULL);
						hput(visited, url, (const char *)result, sizeof((const char *)result));
						qput(webpages, (void *)url);
					}
				}
				free(result);
			}
			
		}else{
			// if we could not fetch webpage exit failure
			exit(EXIT_FAILURE);
		}
		myPage = (webpage_t *)qget(webpages);
	}



	// Now that all the webpages are in the queue lets run through it printing the webpages and freeing them as we go.
	webpage_t *w = (webpage_t *)qget(webpages);
	while (w != NULL){
		printf("%s\n", webpage_getURL(w));
		webpage_delete(w);
		w =(webpage_t *)qget(webpages);
	}
  
	qclose(webpages);
	webpage_delete(page);
	hclose(visited);
	

  
	exit(EXIT_SUCCESS);
}
