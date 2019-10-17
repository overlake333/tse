/* queue.c --- 
 * 
 * 
 * Author: James D. Fleming, Sudharsan B, Taylor O.
 * Created: Fri Oct  4 13:47:51 2019 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */

#include "queue.h"
#include <stdint.h>                                                                             
#include <stdbool.h>                                                                            
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct qElement_t {
	// every element holds a value which can be anything (double, car, person, cat) and a next Pointer
	void *value;
	void *next;

} qElement_t;


typedef struct myQueue_t{
	//Since we are hiding what a queue object is we need another
	//structure to hold the queue front and back pointers Everytime we
	//get a queue_t we need to convert it to a myQueue_t and every time
	//we send it back we need to do this as well
	qElement_t *front;
	qElement_t *back;
	
} myQueue_t;

static myQueue_t *makeQueue(){
	myQueue_t *qt;
	if (!(qt =(myQueue_t*)malloc(sizeof(myQueue_t)))){
		printf("error creating queue");
		return NULL;
	}
	qt->front =NULL;
	qt->back = NULL;
	return qt;
}
// method for creating a qElement given some sort of value elementp
static qElement_t *makeElement(void *elementp){
	qElement_t *e;
	if (!(e=(qElement_t*)malloc(sizeof(qElement_t)))){
	  printf("error creating element of queue");
		return NULL;
	}
	e->next = NULL;
	e->value = elementp;
	return e;
}

	
/* create an empty queue */
// Should be constant time
queue_t* qopen(void){
	queue_t *qt = (void*)makeQueue();
	return qt;
}
                                                                                                

/* deallocate a queue, frees everything in it */
void qclose(queue_t *qp){
	 // Loop through all the elements of the queue freeing them up
	 myQueue_t *mqp = (myQueue_t*)qp; //Switching queue_t to myQueue_t here
	 qElement_t *elementPointer = mqp->front;
	 qElement_t *nextElement;
	 // Loop through all the pointers until we reach the end. Keep a
	 // temporary nextElement so that we can delete the current element
	 // and still have reference to that element's next pointer
	 while ( elementPointer !=NULL){
	 nextElement = elementPointer->next;
	 free(elementPointer);
	 elementPointer = nextElement;
	 }
	 // Finally free up the queue itself
 	 free(qp);

} 
                                                                                                
/* put element at the end of the queue */
/*  * returns 0 is successful; nonzero otherwise */
/*  *\/ */
int32_t qput(queue_t *qp, void *elementp){
  // how can i test if it can correctly convert the queue
	 myQueue_t *mqp = (myQueue_t*)qp;
	 qElement_t *e;
	 // make sure we can successfully make an qElement
	 if((e=makeElement(elementp)) == NULL){
		 return 1;
	 }

	 // CASE if queue is currently empty
	 if (mqp->front == NULL){
		 // set both front and back pointers to e;
		 mqp->front = e;
		 mqp->back = e;
	 }else{
		 // if queue is not empty update next of current back pointer and
		 // switch backpointer to new element
		 (mqp->back)->next = e;
		 mqp->back = e;
	 }
	 return 0;
 }

/* get the first first element from queue, removing it from the queue*/

void* qget(queue_t *qp){
	// coerce the queue object back into a myQueue
	myQueue_t *mqp = (myQueue_t*)qp;
	//create a pointer to the first element
	if (mqp->front == NULL){
		return NULL;
	}
	qElement_t *first = mqp->front;
	// Set the front pointer to the second element
	mqp->front = first->next;
	// Grab the value of first, because what the user actually wants is the value
	void *val = first->value;
	// free up first
	free(first);
	// return the pointer to the value
	return(val);
	
} 
                                                                                                
/*     apply a function to every element of the queue *\/ */
void qapply(queue_t *qp, void (*fn)(void* elementp)){
	// as always, coerce the queue into our queue data structure!
	myQueue_t *mqp = (myQueue_t*)qp;
	// loop through the queue 
	qElement_t *p;
	for (p = mqp->front ; p != NULL ; p = p->next){
		void *v = (void*)(p->value);
		fn(v);
	}
} 
                                                                                                
/* /\* search a queue using a supplied boolean function */
/*  * skeyp -- a key to search for */
/*  * searchfn -- a function applied to every element of the queue */
/*  *          -- elementp - a pointer to an element */
/*  *          -- keyp - the key being searched for (i.e. will be */
/*  *             set to skey at each step of the search */
/*  *          -- returns TRUE or FALSE as defined in bool.h */
/*  * returns a pointer to an element, or NULL if not found */

void* qsearch(queue_t *qp, bool (*searchfn)(void* elementp,const void* keyp), const void* skeyp){ 
	myQueue_t *mqp = (myQueue_t*)qp;

	// go through each item
	qElement_t *p;
	for (p = mqp->front ; p != NULL ; p = p->next){
		void *v = (void*)(p->value);
		if (searchfn(v, skeyp) == true){
			// return pointer to element value if found
			return(v);
		}
		
	}
	// if we exit the loop with no success, then just return NULL!
	return NULL;

	
}
                                                                                                
/* /\* search a queue using a supplied boolean function (as in qsearch), */
/*  * removes the element from the queue and returns a pointer to it or */
/*  * NULL if not found */

void* qremove(queue_t *qp, bool (*searchfn)(void* elementp,const void* keyp), const void* skeyp){ 
	myQueue_t *mqp = (myQueue_t*)qp;

	// go through each item
	qElement_t *p;
	qElement_t *previousp;
	for (p = mqp->front ; p != NULL ; p = p->next){
		void *v = (void*)(p->value);
		if (searchfn(v, skeyp) == true){
			// If element is FRONT
			if (p == mqp->front){
				mqp->front = p->next;
				p->next = NULL;
			}else if (p ==mqp-> back){
				mqp->back = previousp;
				previousp->next = NULL;
			}else{
				previousp->next=p->next;
				p->next = NULL;
			}
			free(p);
			return v;
		}
		previousp = p;
	}
	

	// if we exit the loop with no success, then just return NULL!
	return NULL;

 } 
                                                                                                
/* concatenatenates elements of q2 into q1 */
/* q2 is dealocated, closed, and unusable upon completion */

void qconcat(queue_t *q1p, queue_t *q2p){
  myQueue_t *mqp1 = (myQueue_t*)q1p;
  myQueue_t *mqp2 = (myQueue_t*)q2p;
  // next pointer of back of q1 to be front of q2

  
  if(mqp1->front == NULL){
    mqp1->front = mqp2->front;
    mqp1->back = mqp2->back;
  } else if (mqp2->front != NULL){
    qElement_t *temp = mqp1->back;
    temp->next = mqp2->front;
    mqp1->back = mqp2->back;
  }

  q1p = mqp1;
  free(q2p);
} 
                                                                                                
                                                                                                
                                                                
