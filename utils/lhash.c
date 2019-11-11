#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <lhash.h>
#include <hash.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct myLht{
	pthread_mutex_t m;
	hashtable_t *h;
} myLht_t;


static myLht_t *makeLht(uint32_t hsize){
	myLht_t *lht;
	if (!(lht = (myLht_t*)malloc(sizeof(myLht_t)))){
		printf("error creating lht");
		return NULL;
	}
	pthread_mutex_init(&(lht->m), NULL);
	lht->h = hopen(hsize);
	return lht;
}

/* the queue representation is hidden from users of the module */


/* create an empty queue */
lhashtable_t *lhopen(uint32_t hsize){
	// maybe need to lock here?
	lhashtable_t *lht = (lhashtable_t *)makeLht(hsize);
	return lht;
}

/* deallocate a queue, frees everything in it */
void lhclose(lhashtable_t *ht){
	myLht_t *lht = (myLht_t *)ht;
	hclose(lht->h);
	pthread_mutex_destroy(&(lht->m));
	free(lht);
}

/* put element at the end of the queue
 * returns 0 is successful; nonzero otherwise 
 */
int32_t lhput(lhashtable_t *htp, void *ep, const char *key, int keylen){
	myLht_t *lht = (myLht_t *)htp;
	pthread_mutex_lock(&(lht->m));
	uint32_t happiness = hput(lht->h, ep, key, keylen);
	pthread_mutex_unlock(&(lht->m));
	return happiness;
}


/* apply a function to every element of the queue */
void lhapply(lhashtable_t *htp, void (*fn)(void* elementp)){
	myLht_t *lht = (myLht_t *)htp;
	pthread_mutex_lock(&(lht->m));
	happly(lht->h, fn);
	pthread_mutex_unlock(&(lht->m));
}

/* search a queue using a supplied boolean function
 * skeyp -- a key to search for
 * searchfn -- a function applied to every element of the queue
 *          -- elementp - a pointer to an element
 *          -- keyp - the key being searched for (i.e. will be 
 *             set to skey at each step of the search
 *          -- returns TRUE or FALSE as defined in bool.h
 * returns a pointer to an element, or NULL if not found
 */
void* lhsearch(lhashtable_t *htp, 
							bool (*searchfn)(void* elementp,const void* searchkeyp),
							 const char* key, int32_t keylen){
	myLht_t *lht = (myLht_t *)htp;
	pthread_mutex_lock(&(lht->m));
	void *love = hsearch(lht->h, searchfn, key, keylen);
	pthread_mutex_unlock(&(lht->m));
	return love;
}





