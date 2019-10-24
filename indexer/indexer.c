/* indexer.c --- indexes for tse
 * 
 * 
 * Author: Sudharsan Balasubramani
 * Created: Thu Oct 24 10:19:21 2019 (-0400)
 * Version: 
 * 
 * Description: Reads the html assocaiated with eavery webpage fetched by crawler and 
 * creates in memory an index data structure.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <webpage.h>
#include <pageio.h>
#include <ctype.h>

/* INTERNAL FUNCTION PROTOTYPES */
void NormalizeWord(char *word);


// Main
int main(void) {

	// Loads a webpage with pageio module
	webpage_t *page = pageload(1, "pages");
	int pos = 0;
	char *word;
	FILE *output = fopen("output.txt", "w"); // output file to write html to
 
	// Uses webpage_getNextWord to print all words in HTML. SKIPS TAGS! 		
	while ((pos = webpage_getNextWord(page, pos, &word)) > 0) {
		//printf("%s\n", word);
		NormalizeWord(word);
		fprintf(output, "%s\n", word); // prints the words of the html to a file
		free(word);
	}
		
	exit(EXIT_SUCCESS);
}


/* INTERNAL FUNCTIONS */


/* NormalizeWord - 
 * Converts a word to lowercase
 *
 * Inputs: word - a string
 */
void NormalizeWord(char *word) {

	if (word != NULL) {
		for (char *c = word; *c != '\0'; c++) {
			*c = (char)tolower(*c);
		}
	}

}
