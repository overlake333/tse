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
 
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <lhash.c>
#include <string.h>
//static char *x = "James";
//static char *y = "Pizza";

static double x = 2.0;
static double y = 5.0;

pthread_mutex_t m;


void *putinHash(void *lht){
	lhashtable_t *ht = (lhashtable_t *)lht;
	lhput(ht, (void *)&x, (const char *)&x, strlen((const char *)&x));
	return NULL;
}

void *putinHash2(void *lht){
	lhashtable_t *ht = (lhashtable_t *)lht;
	lhput(ht, (void *)&y, (const char *)&y, strlen((const char *)&y));
	return NULL;
}
/*
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
*/

void applyfn(void *element){
	printf("%lf\n", *(double *)element);
}

int main(void){
	pthread_t pt1, pt2;// pt3, pt4;
	lhashtable_t *lht = lhopen(10);

	pthread_t array[3];
	for (int i = 0; i < 3; i++) {
		if(pthread_create(&array[i],NULL, putinHash, (void *)lht)!= 0){
			printf("DID NOT OPEN");
			exit(EXIT_FAILURE);			
		}
	}

	for (int i = 0; i < 3; i++) {
		if(pthread_join(array[i], NULL)!=0){
			printf("DID NOT COME BACK");
			exit(EXIT_FAILURE);
		}
	}
				
 
	printf("ALL GOOD\n");
	lhapply(lht, applyfn);

	exit(EXIT_SUCCESS);
	lhclose(lht);

	
}
