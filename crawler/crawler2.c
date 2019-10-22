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
#include <stdbool.h> 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <webpage.h>
#include <queue.h>
#include <hash.h>
#include <sys/stat.h>

bool webpage_save(webpage_t *pagep, int id, char *dirname);
void crawler(char *seedURL, char*dirname, int maxDepth);

bool webpage_save(webpage_t *pagep, int id, char *dirname) {
	if (pagep == NULL || dirname == NULL || id < 0) return false;

	FILE *save;
	char filename[20];

	sprintf(filename, "../%s/%i", dirname, id);
	if ((save = fopen(filename, "w")) == NULL) {
		return false;
	}

	// save with a unique id
	fprintf(save, "%s\n", webpage_getURL(pagep));
	fprintf(save, "%d\n", webpage_getDepth(pagep));
	fprintf(save, "%d\n", webpage_getHTMLlen(pagep));
	fprintf(save, "%s\n", webpage_getHTML(pagep));

	fclose(save);
	
  return true;
}

bool compareByURL(void* elementp, const void* searchkeyp) {
	// this function needs to see if the url searchkeyp is the same as the url from the element
	if (elementp != NULL && searchkeyp != NULL) {
		if (strcmp(webpage_getURL((webpage_t *)elementp), (const char*)searchkeyp) == 0) {
			return true;
		}
	}
	return false;
}
	

int main(int argc, char *argv[]){
  // Create a new webpage
	char *usage = "usage: crawler <seedurl> <pagedir> <maxdepth>";

  // collect arguments 
  char *seedurl;
  char *pagedir;
  int maxDepth;
	
	// check for proper number of arguments
	if(argc != 4){
		printf("Wrong argument #: %s\n", usage);
		exit(EXIT_FAILURE);
	} else {
		seedurl = argv[1];
		pagedir = argv[2];
		maxDepth = atoi(argv[3]);
	}

	// check for a valid url
	if (IsInternalURL(seedurl) == false) {
		printf("Seed url is not valid: %s\n", usage);
		exit(EXIT_FAILURE);
	}

	char folder[20];
  sprintf(folder, "../%s", pagedir);
  struct stat sb;
	
  if(!(stat(folder, &sb) == 0 && S_ISDIR(sb.st_mode))){
    printf("Directory Doesn't Exist!\n%s\n", usage);
    exit(EXIT_FAILURE);
  }     

	// check for valid max depth
	if(maxDepth < 0){
		printf("Max depth must be a positive integer: %s\n", usage);
		exit(EXIT_FAILURE);
	}

	crawler(seedurl, pagedir, maxDepth);
 	exit(EXIT_SUCCESS);
}

void crawler(char *seedURL, char *dirname, int maxDepth) {
	// used for storing the webpages
	queue_t *que = qopen();
	hashtable_t *htp = hopen(20);

	// initially with null html and 0 depth and story the webpage
	webpage_t *wp = webpage_new(seedURL, 0, NULL);

	qput(que, wp);
	char *currUrl = webpage_getURL(wp);
	hput(htp, wp, (const char*)currUrl, sizeof((const char*)currUrl)); // add the url to seen list

	webpage_t *current = qget(que); // extracts a page to crawl
	int id = 1;
	while (current != NULL) { // while there are still pages to be crawled
		if (webpage_fetch(current)) { // gets html of current page 			
			if ((webpage_save(current, id, dirname)) == false) {
					printf("error: page not saved\n");
				} else {
					id++; // save it into the directory with a unique id 
				}
		} 
		
		if (webpage_getDepth(current) < maxDepth) {
			int pos = 0;
			char *result;
			while ((pos = webpage_getNextURL(current, pos, &result)) > 0) {
				// ignore page if it isn't internal
				if (IsInternalURL(result)) {
					if(hsearch(htp, compareByURL, (const char *)result, sizeof((const char *)result)) == NULL) {
						webpage_t *next = webpage_new(result, webpage_getDepth(current)+1, NULL);
						hput(htp, next, (const char *)result, sizeof((const char*)result));
						qput(que, (void *)next);
					 }
				}
				free(result);
			}
		}
		webpage_t *next = qget(que);
		current = next;
	}

	
	webpage_delete(current);
	qclose(que);
	happly(htp, webpage_delete);
	hclose(htp);
}
