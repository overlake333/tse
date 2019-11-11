
/* 
 * queue.h -- public interface to the queue module
 */
#include <stdint.h>
#include <stdbool.h>
#include <queue.h>
#include <pthread.h>
#include <lqueue.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct myLqueue{
	pthread_mutex_t m;
	queue_t *q;
} myLqueue_t;


static myLqueue_t *makeLq(){

	myLqueue_t *lq;
	if (!(lq = (myLqueue_t*)malloc(sizeof(myLqueue_t)))){
		printf("error creating lq");
		return NULL;
	}
	pthread_mutex_init(&(lq->m), NULL);
	lq->q = qopen();
	return lq;
}
/* the queue representation is hidden from users of the module */


/* create an empty queue */
lqueue_t* lqopen(void){
	
	lqueue_t *myQueue = (lqueue_t *)makeLq();
	return myQueue;
}

/* deallocate a queue, frees everything in it */
void lqclose(lqueue_t *qp){
	myLqueue_t *lq = (myLqueue_t *)qp;
	qclose(lq->q);
	pthread_mutex_destroy(&(lq->m));
	free(lq);
												
}

/* put element at the end of the queue
 * returns 0 is successful; nonzero otherwise 
 */
int32_t lqput(lqueue_t *qp, void *elementp){
	myLqueue_t *lq = (myLqueue_t *)qp;
	pthread_mutex_lock(&(lq->m));
	int32_t happiness = qput(lq->q, elementp);
	pthread_mutex_unlock(&(lq->m));
	return happiness;
}

/* get the first first element from queue, removing it from the queue */
void* lqget(lqueue_t *qp){
	myLqueue_t *lq = (myLqueue_t *)qp;
	pthread_mutex_lock(&(lq->m));
	void *love = qget(lq->q);
	pthread_mutex_unlock(&(lq->m));
	return love;
}

/* apply a function to every element of the queue */
void lqapply(lqueue_t *qp, void (*fn)(void* elementp)){

	myLqueue_t *lq = (myLqueue_t *)qp;
	pthread_mutex_lock(&(lq->m));
	qapply(lq->q, fn);
	pthread_mutex_unlock(&(lq->m));
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
	myLqueue_t *lq = (myLqueue_t *)qp;
	pthread_mutex_lock(&(lq->m));
	void *lust = qsearch(lq->q, searchfn, skeyp);
	pthread_mutex_unlock(&(lq->m));
	return lust;
}





