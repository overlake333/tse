/*
 * 
 * 
 * Author: James D. Fleming
 * Created: Thu Oct 31 16:55:14 2019 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

static int readline(char **string){
  *string = (char *)malloc(100);
	char *p = *string;
	char ch;
	
	if (scanf("%c", &ch)==EOF){
		return -1;
		// end of file return -1
	}
	if (ch == '\n')
		{
			// just a newline return 0 
			return 0;
		}
	while(ch != '\n'){
		if (isalpha(ch) || isspace(ch)){
			*p++ = tolower(ch);
			scanf("%c", &ch);
		}else{
			while (getchar() != '\n');
			return 2;
		}
	}
	*p = '\0';
	// successful creation of string return 1
	//	fflush(stdin);
	return 1;
}


int main(void){
	
	printf("> ");
	char *line;
	char array[50][100];
	int value;
	while ((value = readline(&line)) != -1){
		int i = 0;
		if (value == 1){
			//printf("%s\n", line);
			char *token = strtok(line, " ");
			
			while (token != NULL) {
				strcpy(array[i], token);
				token = strtok(NULL, " ");
				i++;
		 	}
			int j = 0;
			while (j < i) {
				printf("%s ", array[j]);
				j++;
			}
			printf("\n");
			

		}
		if (value == 2){
			
			printf("[invalid query]\n");
		}
		fflush(stdin);
		value = 0;
		printf("> ");
		free(line);
	}

	


	
	
	free(line);	
	return 0;
}
