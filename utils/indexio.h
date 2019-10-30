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



/* Saves an index! Happy Halloween! */
int indexsave(hashtable_t *index, char *filename);

/* Loads an index! Load! Load! Load! */
hashtable_t *indexload(char *indexnm);
