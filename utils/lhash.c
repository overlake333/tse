
/* 
 * queue.h -- public interface to the queue module
 */
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <lhash.h>
#include <hash.h>

pthread_mutex_t m;

/* the queue representation is hidden from users of the module */


/* create an empty queue */
lhashtable_t *lhopen(uint32_t hsize){
	pthread_mutex_init(&m, NULL);
	// maybe need to lock here?
	lhashtable_t *h = hopen(hsize);
	return h;
}

/* deallocate a queue, frees everything in it */
void lhclose(lhashtable_t *ht){
	hashtable_t *h = (hashtable_t *)ht;
	hclose(h);
	pthread_mutex_destroy(&m);
}

/* put element at the end of the queue
 * returns 0 is successful; nonzero otherwise 
 */
int32_t lhput(lhashtable_t *htp, void *ep, const char *key, int keylen){
	pthread_mutex_lock(&m);
	hashtable_t *ht = (hashtable_t *)htp;
	uint32_t happiness = hput(ht, ep, key, keylen);
	pthread_mutex_unlock(&m);
	return happiness;
}


/* apply a function to every element of the queue */
void lhapply(lhashtable_t *htp, void (*fn)(void* elementp)){
	pthread_mutex_lock(&m);
	hashtable_t *ht = (hashtable_t *)htp;
	happly(ht, fn);
	pthread_mutex_unlock(&m);
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
void* lqsearch(lhashtable_t *htp, 
							bool (*searchfn)(void* elementp,const void* searchkeyp),
							 const char* key, int32_t keylen){
	pthread_mutex_lock(&m);
	hashtable_t *ht = (hashtable_t *)htp;
	void *love = hsearch(ht, searchfn, key, keylen);
	pthread_mutex_unlock(&m);
	return love;
}





