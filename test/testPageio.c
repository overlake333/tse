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

int main(void){
 	webpage_t *mypage = pageload(1, "pages");
	pagesave(mypage, 11, "newpages");
	//webpage_t *loadback = pageload(6, "newpages");
}
