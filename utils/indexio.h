/* indexio.h --- Saves and loads an index to a named file indexnm
 * 
 * 
 * Author: Sudharsan Balasubramani
 * Created: Sat Oct 26 21:53:14 2019 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */
#include <stdio.h>
#include <hash.h>
#include <queue.h>
#include <webpage.h>
#include <pageio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>


/* Structs in ht */
typedef struct hashIndex hashIndex_t;
typedef struct wordPage wordPage_t;

/* Saves an index! Happy Halloween! */
int indexsave(hashtable_t *index, char *filename);

/* Loads an index! Load! Load! Load! */
int indexload(hashtable_t *index, char *dirname);
