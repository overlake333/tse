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
#include <sys/stat.h>

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

//static void releaseWebPage(void *wp){
//if (wp !=NULL){
//	webpage_delete((webpage_t *)wp);
//}
//}

int main(int argc, char *argv[]){
  // Create a new webpage
	char *usage = "usage: crawler <seedurl> <pagedir> <maxdepth>";

	if(argc != 4){
		printf("Wrong argument #: %s\n", usage);
		exit(EXIT_FAILURE);
	}
	
	char *seedurl = argv[1];
	char *pagedir = argv[2];
	int maxDepth = atoi(argv[3]);

	if((strcmp(seedurl, "") == 0) || (strcmp(pagedir, "") == 0) || (maxDepth < 0)){
		printf("Arguments don't work: %s\n", usage);
		exit(EXIT_FAILURE);
	}

	char folder[20];
	sprintf(folder, "../%s", pagedir);
	struct stat sb;

	if(!(stat(folder, &sb) == 0 && S_ISDIR(sb.st_mode))){
		printf("Directory Doesn't Exist!\n%s\n", usage);
		exit(EXIT_FAILURE);
	}
	

	
  webpage_t *page = webpage_new(seedurl, 0, NULL);
  // Grab the webpage's html
  hashtable_t *visited = hopen(20);

	queue_t *webpages = qopen();
	queue_t *webpagesToDelete = qopen();
	// add first webpage to queue
	qput(webpages, (void *) page);
	qput(webpagesToDelete, (void *)page);
	int id = 1;
	// Loop through queue while it has webpages and they are above max depth
	webpage_t *myPage = (webpage_t *)qget(webpages);
	// put seed in visited
	const char *key = (const char *)(webpage_getURL(myPage));
	hput(visited, (void *)myPage, key, sizeof(key));
 	
	while ((myPage!= NULL) && (webpage_getDepth(myPage) <= maxDepth)){
		if(webpage_fetch(myPage)){
			// grab the depth of this webpage
			int depth = webpage_getDepth(myPage);
			// save this page
			// and print it to showed it has been save
			pagesave(myPage, id, pagedir);
			id = id + 1;
			printf("saved: %s\n", webpage_getURL(myPage));
			int pos = 0;
			char *result;
			webpage_t *url;
			//Grab the next url from the page starting at pos 0 but only do this if we can go a depth down. 
			while(((pos = webpage_getNextURL(myPage, pos, &result)) > 0) && (depth+1 <= maxDepth)){
				// make sure it is an internal URL
				if(IsInternalURL(result)){
					// Make sure it hasn't already been visited
					if (hsearch(visited, compareByURL, (const char *)result, sizeof((const char *)result)) == NULL){
						// Create a new webpage from the result and increase its depth by 1
						url = webpage_new(result, depth+1, NULL);
						// put it in the visited hashtable
						hput(visited, url, (const char *)result, sizeof((const char *)result));
						// put it in the queue
						qput(webpages, (void *)url);
						qput(webpagesToDelete, (void *)url);
					}
				}
				free(result);
			}
			
			// re-assign myPage pointer;
			myPage = (webpage_t *)qget(webpages);
		}
	}
	


	// Now that all the webpages are in the queue lets run through it printing the webpages and freeing them as we go.
	webpage_t *w = (webpage_t *)qget(webpagesToDelete);
	while (w != NULL){
		printf("%s\n", webpage_getURL(w));
		webpage_delete(w);
		w =(webpage_t *)qget(webpagesToDelete);
	}
  // free all the webpages in the hash table at the end.
	//happly(visited, releaseWebPage); 
	
	
	qclose(webpages);
	qclose(webpagesToDelete);
	hclose(visited);
	exit(EXIT_SUCCESS);
}
