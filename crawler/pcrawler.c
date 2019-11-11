/* crawler.c - prints hello
 *
 * Author: Sudharsan B, Taylor O., James F.
 * Date: 10/15/209 17:52
 *
 * Version 1.0
 *
 *
 * Desc. 
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
#include <pageio.h>
#include <pthread.h>
#include <lhash.h>
#include <lqueue.h>


// SHOULD CAUSE PROBLEMS...MAYBE (Check with Tay and James) jk figured it out :p :)
int id = 0;
int waitingThreads = 0;
static pthread_mutex_t m;
static pthread_mutex_t waitLock;


typedef struct lockedResources{
	lhashtable_t *lht;
	lqueue_t *lq;
	int maxDepth;
	int numthreads;
	char dir[50];
} lResource_t;



void crawler(char *seedURL, char*dirname, int maxDepth, int numthreads);

void *crawl(void *ep);

// Makes our locked resource
lResource_t *makeLockedResource(lhashtable_t *lht, lqueue_t *lq, int maxDepth, int numthreads, char dir[]) {
	lResource_t *lhq = (lResource_t *)malloc(sizeof(lResource_t));
	lhq->lht = lht;
	lhq->lq = lq;
	lhq->maxDepth = maxDepth;
	lhq->numthreads = numthreads;
	strcpy(lhq->dir, dir);

	return lhq;
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
	char *usage = "usage: crawler <seedurl> <pagedir> <maxdepth> <numthreads>";

  // collect arguments 
  char *seedurl;
  char *pagedir;
  int maxDepth;
	int numthreads;
	
	// check for proper number of arguments
	if(argc != 5){
		printf("Wrong argument #: %s\n", usage);
		exit(EXIT_FAILURE);
	} else {
		seedurl = argv[1];
		pagedir = argv[2];
		maxDepth = atoi(argv[3]);
		numthreads = atoi(argv[4]);
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

	crawler(seedurl, pagedir, maxDepth, numthreads);
 	exit(EXIT_SUCCESS);
}

void crawler(char *seedURL, char *dirname, int maxDepth, int numthreads) {
	
	// Create threads
	pthread_t *threads = (pthread_t *)calloc(sizeof(pthread_t), numthreads);

	// used for storing the webpages
	lqueue_t *que = lqopen();
	lhashtable_t *htp = lhopen(20);

	// initially with null html and 0 depth and story the webpage
	webpage_t *wp = webpage_new(seedURL, 0, NULL);
		
	
	lqput(que, wp);
	lhput(htp, wp, (const char *)webpage_getURL(wp), strlen((const char *)webpage_getURL(wp)));
	
	lResource_t *resource = makeLockedResource(htp, que, maxDepth, numthreads, dirname);
	for (int i = 0; i < numthreads; i++) {
		pthread_create(&threads[i], NULL, crawl, (void *)resource);
	}

	for (int i = 0; i < numthreads; i++) {
		pthread_join(threads[i], NULL);
	}

	pthread_mutex_destroy(&m);
	pthread_mutex_destroy(&waitLock);
	free(resource);
	free(threads);
	lqclose(que);
	lhapply(htp, webpage_delete);
	lhclose(htp);
}

void *crawl(void *ep) {
	
	// Resource Collection
	lResource_t *lhq = (lResource_t *)ep;
	lqueue_t *lq = lhq->lq;
	lhashtable_t *lht = lhq->lht;
	int numthreads = lhq->numthreads;
	int maxDepth = lhq->maxDepth;
	char dirname[50];
	strcpy(dirname, lhq->dir);

	bool didWait = false;
	webpage_t *current = lqget(lq);

	// If current is null, it could be because there is nothing in the queue, so maybe wait a bit. gosh dude threads are so impatient >:(
	pthread_mutex_lock(&waitLock);
	while (waitingThreads != numthreads) {
		printf("waiting: %d num: %d\n", waitingThreads, numthreads);
		pthread_mutex_unlock(&waitLock);
		while (current == NULL) {
			if (didWait == false){
				pthread_mutex_lock(&waitLock);
				waitingThreads++;
				pthread_mutex_unlock(&waitLock);
				didWait = true;
			}
			if (waitingThreads == numthreads) {
				return NULL;
			}
			sleep(1);
			current = lqget(lq);	
		}
		// Once we are out with a webpage, our thread isn't waiting anymore
		if (didWait) {
			pthread_mutex_lock(&waitLock);
			waitingThreads--;
			pthread_mutex_unlock(&waitLock);
		}
		// Saves the the page to the directory like done before.
		if (webpage_fetch(current)) {
			pthread_mutex_lock(&m);
			id++;
			if ((pagesave(current, id, dirname)) == 1) {
				printf("error: page not saved");			
			}
			pthread_mutex_unlock(&m);
		}
		
		if (webpage_getDepth(current) < maxDepth) {
			int pos = 0;
			char *result;
			while ((pos = webpage_getNextURL(current, pos, &result)) > 0) {
				// ignore page if it isn't internal
				if (IsInternalURL(result)) {
					// check if it is in the locked hashtable already
					if (lhsearch(lht, compareByURL, (const char *)result, strlen((const char *)result)) == NULL) {
						// If not, add it to ht, and the queue to be visited
						webpage_t *next = webpage_new(result, webpage_getDepth(current) + 1, NULL);
						lhput(lht, next, (const char *)result, strlen((const char *)result));
						lqput(lq, (void *)next);
					}
				}
				free(result);
			}
		}
		webpage_t *next = lqget(lq);
		current = next;
		pthread_mutex_lock(&waitLock);
		didWait = false;
	}

	pthread_mutex_unlock(&waitLock);
	return NULL;	
}
