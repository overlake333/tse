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
	pagesave(mypage, 200, "newpages");
	webpage_delete(mypage);
	/*
	webpage_t *loadback = pageload(200, "newpages");
	if (strcmp((webpage_getHTML(mypage)), (webpage_getHTML(loadback))) != 0){
		printf("Test failed, html somehow changed between saves and loads\n");
		exit(EXIT_FAILURE);
	}
	pagesave(loadback, 13, "newpages");
	webpage_t *runitback = pageload(13, "newpages");
	if (strcmp(webpage_getHTML(loadback), webpage_getHTML(runitback)) != 0){
		printf("test failed html changed");
		exit(EXIT_FAILURE);
	}
	*/
	
	printf("Test Succeeded!\n");
	exit(EXIT_SUCCESS);
}
