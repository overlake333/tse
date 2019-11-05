/* orTest.c --- 
 * 
 * 
 * Author: Sudharsan Balasubramani
 * Created: Mon Nov  4 18:48:45 2019 (-0500)
 * Version: 
 * 
 * Description: 
 * 
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>



void orTest(char *line) {
	
	for (int i = 0; i < strlen(line)-3; i++) {
	  char temp[5];
		sprintf(temp, "%c%c%c%c", line[i], line[i+1], line[i+2], line[i+3]);
		printf("%s\n", temp);
		if (strcmp(temp, " or ") == 0) {
			printf("works");
		}

	}
	

}


int main(void) {

	orTest("dartmouth or cs");




}
