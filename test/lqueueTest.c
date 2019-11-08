/* lqueueTest.c --- 
 * 
 * 
 * Author: James D. Fleming
 * Created: Thu Nov  7 17:47:49 2019 (-0500)
 * Version: 
 * 
 * Description: 
 * 
 */
 
#include <lqueue.c>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
static int x = 5;
static int y = 27;

pthread_mutex_t m;


void *putinQueue( void *q){
	lqueue_t *lq = (lqueue_t *)q;
	lqput(lq, (void *)&x);
	return NULL;
}
void *putinQueue2( void *q){
	lqueue_t *lq = (lqueue_t *)q;
	lqput(lq, (void *)&y);
	return NULL;
}

void *printit(void *print){
	sleep(1);
	pthread_mutex_lock(&m);
	
	printf("this should print second");
	print = NULL;
	pthread_mutex_unlock(&m);
	return print;
}
void *printafter(void *print){

	pthread_mutex_lock(&m);
	sleep(10);
	printf("THis should print first");

	print = NULL;
	pthread_mutex_unlock(&m);
	return print;
}
void applyfn(void *element){
	printf("%d\n", *(int *)element);
}

int main(void){
	pthread_t pt1, pt2, pt3, pt4;
	lqueue_t *myLq = lqopen();
	
	
	if(pthread_create(&pt1,NULL, putinQueue, (void *)myLq)!= 0){
		printf("DID NOT OPEN");
		exit(EXIT_FAILURE);
		
	}
	if(pthread_create(&pt2, NULL, putinQueue2, (void*) myLq)!=0){
		printf("DID not open 2nd");
		exit(EXIT_FAILURE);
	}
	if(pthread_create(&pt3,NULL, printit, (void *)myLq)!= 0){
		printf("DID NOT OPEN");
		exit(EXIT_FAILURE);
		
	}
	if(pthread_create(&pt4, NULL, printafter, (void*) myLq)!=0){
		printf("DID not open 2nd");
		exit(EXIT_FAILURE);
	}
	if(pthread_join(pt1, NULL)!=0){
		printf("DID NOT COME BACK");
		exit(EXIT_FAILURE);
	}
	if(pthread_join(pt2, NULL)!=0){
		printf("2 didnot comeback");
		exit(EXIT_FAILURE);
	}
	if(pthread_join(pt3, NULL)!=0){
		printf("thread 3DID NOT COME BACK");
		exit(EXIT_FAILURE);
	}
	if(pthread_join(pt4, NULL)!=0){
		printf("2 didnot comeback");
		exit(EXIT_FAILURE);
	}
	printf("ALL GOOD\n");
	lqapply(myLq, applyfn);

	exit(EXIT_SUCCESS);
	lqclose(myLq);

	
}
