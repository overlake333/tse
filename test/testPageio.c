/* testPageio.c --- 
 * 
 * 
 * Author: James D. Fleming
 * Created: Tue Oct 22 18:18:25 2019 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */
#include <pageio.h>
#include <webpage.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main(void){
 	webpage_t *mypage = pageload(2, "pages");
	pagesave(mypage, 12, "newpages");
	webpage_t *loadback = pageload(12, "newpages");
	if (strcmp((webpage_getHTML(mypage)), (webpage_getHTML(loadback))) != 0){
		printf("Test failed, html somehow changed between saves and loads\n");
		exit(EXIT_FAILURE);
	}
	printf("Test Succeeded!\n");
	exit(EXIT_SUCCESS);
}
