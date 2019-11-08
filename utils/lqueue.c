
/* 
 * queue.h -- public interface to the queue module
 */
#include <stdint.h>
#include <stdbool.h>
#include <queue.h>
#include <pthread.h>
#include <lqueue.h>

pthread_mutex_t m;

/* the queue representation is hidden from users of the module */


/* create an empty queue */
lqueue_t* lqopen(void){
	pthread_mutex_init(&m, NULL);
	// maybe need to lock here?
	lqueue_t *myLockedQueue = (lqueue_t *)qopen();
	return myLockedQueue;
}

/* deallocate a queue, frees everything in it */
void lqclose(lqueue_t *qp){
	queue_t *q = (queue_t *)qp;
	qclose(q);
	pthread_mutex_destroy(&m);
}

/* put element at the end of the queue
 * returns 0 is successful; nonzero otherwise 
 */
int32_t lqput(lqueue_t *qp, void *elementp){
	pthread_mutex_lock(&m);
	queue_t *q = (queue_t *)qp;
	int32_t happiness = qput(q, elementp);
	pthread_mutex_unlock(&m);
	return happiness;
}

/* get the first first element from queue, removing it from the queue */
void* lqget(lqueue_t *qp){
	pthread_mutex_lock(&m);
	queue_t *q = (queue_t *)qp;
	void *love = qget(q);
	pthread_mutex_unlock(&m);
	return love;
}

/* apply a function to every element of the queue */
void lqapply(lqueue_t *qp, void (*fn)(void* elementp)){
	pthread_mutex_lock(&m);
	queue_t *q = (queue_t *)qp;
	qapply(q, fn);
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
void* lqsearch(lqueue_t *qp, 
							bool (*searchfn)(void* elementp,const void* keyp),
							 const void* skeyp){
	pthread_mutex_lock(&m);
	queue_t *q = (queue_t *)qp;
	void *lust = qsearch(q, searchfn, skeyp);
	pthread_mutex_unlock(&m);
	return lust;
}





