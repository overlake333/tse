/* indexioTest.c --- 
 * 
 * 
 * Author: James D. Fleming
 * Created: Wed Oct 30 17:29:30 2019 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */
#include <hash.h>
#include <indexio.h>
#include <stdlib.h>
#include <stdio.h>


int main(void){
	hashtable_t *index = indexload("../indexer/valTest.txt");
	indexsave(index, "testIO.txt");
	
}
