/* 
 * hash.c -- implements a generic hash table as an indexed set of queues.
 *
 */
#include <stdint.h>
#include "queue.h"
#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
/* 
 * SuperFastHash() -- produces a number between 0 and the tablesize-1.
 * 
 * The following (rather complicated) code, has been taken from Paul
 * Hsieh's website under the terms of the BSD license. It's a hash
 * function used all over the place nowadays, including Google Sparse
 * Hash.
 */
#define get16bits(d) (*((const uint16_t *) (d)))

static uint32_t SuperFastHash (const char *data,int len,uint32_t tablesize) {
  uint32_t hash = len, tmp;
  int rem;
  
  if (len <= 0 || data == NULL)
		return 0;
  rem = len & 3;
  len >>= 2;
  /* Main loop */
  for (;len > 0; len--) {
    hash  += get16bits (data);
    tmp    = (get16bits (data+2) << 11) ^ hash;
    hash   = (hash << 16) ^ tmp;
    data  += 2*sizeof (uint16_t);
    hash  += hash >> 11;
  }
  /* Handle end cases */
  switch (rem) {
  case 3: hash += get16bits (data);
    hash ^= hash << 16;
    hash ^= data[sizeof (uint16_t)] << 18;
    hash += hash >> 11;
    break;
  case 2: hash += get16bits (data);
    hash ^= hash << 11;
    hash += hash >> 17;
    break;
  case 1: hash += *data;
    hash ^= hash << 10;
    hash += hash >> 1;
  }
  /* Force "avalanching" of final 127 bits */
  hash ^= hash << 3;
  hash += hash >> 5;
  hash ^= hash << 4;
  hash += hash >> 17;
  hash ^= hash << 25;
  hash += hash >> 6;
  return hash % tablesize;
}



typedef struct myHash_t{
  uint32_t size;
  queue_t **table;

  
} myHash_t;


static myHash_t *makeHash(uint32_t hsize){
  myHash_t *ht;
  if(!(ht = (myHash_t *)malloc(sizeof(myHash_t)))){
    return NULL;
  }

  ht->size = hsize;
	// make a table that holds hsize # of queue_t pointers!
  queue_t **table = malloc(hsize*sizeof(queue_t *));
 
	for(int i = 0; i < hsize; i++){
		// go through the loop hsize # of times
		// go to each memory address and set it equal to a new queue
		// pointer is a pointer to a queue_t pointer
		queue_t **pointer = table + i;
		// set what pointer points to to be a pointer returned by qopen
		*pointer = qopen();
  }

  ht->table = table;
  return ht;
}

hashtable_t *hopen(uint32_t hsize){
	// just make a new hash table
  myHash_t *ht = makeHash(hsize);
  if(ht != NULL){
    return (hashtable_t *)ht;
  }
  else{
    return NULL;
  }
}

void hclose(hashtable_t *htp){
	// coerce htp to a myHash and call it ht
  myHash_t *ht = (myHash_t *)htp;

	// loop through the hash table closing the queues
	for(int i = 0; i < ht->size; i++){
		// notice that ht->table + i is a pointer to a pointer so we must dereferene,ce it
		qclose(*((ht->table)+i));
  }
	// free the hashtable itself
  free(ht->table);
	free(ht);
}


int32_t hput(hashtable_t *htp, void *ep, const char *key, int keylen){
  //coerce the hash table
  myHash_t *ht = (myHash_t *)htp;
  // get an index
  uint32_t index = SuperFastHash(key, keylen, ht->size);
  // qput returns a 0 or 1 so we can just return whatever it returns
  // (ht->table) gives us the address of the first queue in the array
  // then add index + sizeof(queue_t) to go to the address of the indexed queue
  return qput(*((ht->table)+index), ep);
}

// applies a function to every entry in the hash table
void happly(hashtable_t *htp, void(*fn)(void* ep)){
  // coerce the hash table as always
  myHash_t *ht = (myHash_t *)htp;
  
  // go through all the queues in the hash table and then just call qapply!
  for(int i = 0; i < ht->size; i++){
    qapply(*((ht->table)+i), fn);
  }
}

/* hsearch -- searchs for an entry under a designated key using a
 * designated search fn -- returns a pointer to the entry or NULL if
 * not found
 */
void *hsearch(hashtable_t *htp,
              bool (*searchfn)(void* elementp, const void* searchkeyp),
              const char *key,
              int32_t keylen){
  //c coerce the hash table to our own
  myHash_t *ht = (myHash_t *)htp;
  uint32_t index = SuperFastHash(key, keylen, ht->size);
  
  return qsearch(*((ht->table)+index), searchfn, key);
}


/* hremove -- removes and returns an entry under a designated key
 * using a designated search fn -- returns a pointer to the entry or
 * NULL if not found
 */

void *hremove(hashtable_t *htp,
              bool (*searchfn)(void* elementp, const void* searchkeyp),
              const char *key,
              int32_t keylen){
  myHash_t *ht = (myHash_t *)htp;
  uint32_t index = SuperFastHash(key, keylen, ht->size);
  
  return qremove(*((ht->table)+index), searchfn, key);
}



