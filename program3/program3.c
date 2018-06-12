#include "program3_utilities.h"
#include <stdio.h>
#include <stdlib.h>

/* 
Andrew Towers
Program #3
This program receives up to 10 sentences from the user. The sentences are stored in a dynamically allocated 2-D character array.
After sentences have been entered, the sentences are printed in all uppercase and the character count and the non-whitespace character count
are printed.
*/


int main(void)
{
	int number_of_sentences = -1;
	char **dynamic_sentence_array;
	int index;
	char bufferValue = 0; /*Used to clear buffer*/
	int num_whitespace_chars = 0;
	int num_nonwhitespace_chars = 0;
	int total_characters;

	printf("How many sentences (maximum of 10) do you wish to enter: ");
	scanf("%d", &number_of_sentences);

	while (number_of_sentences < 0 || number_of_sentences > 10)
	{
		printf("Invalid number. Please select valid number (1 - 10)\n");
		scanf("%d", &number_of_sentences);
	}

	while (bufferValue != '\n') /* Flush buffer */
	{
		bufferValue = getchar();
	}

	dynamic_sentence_array = (char **)malloc(sizeof(char *) * number_of_sentences); /* Dynamically allocate sentence array */

	printf("Sentences can be up to 100 characters\n");

	for (index = 0; index < number_of_sentences; index++) /* For every sentence */
	{
		dynamic_sentence_array[index] = (char *)malloc(sizeof(char) * 100); /*Dynamically allocate 100 characters for each sentence*/
		printf("Enter sentence #%d: ", index + 1); /* Prompt user to enter sentence */
		fgets(dynamic_sentence_array[index], sizeof(char) * 100, stdin); /* Store sentence entered from keyboard into sentence array */
	}

	convertAll(dynamic_sentence_array, number_of_sentences); /* Convert each sentence to uppercase */

	printf("\nYour sentences converted to uppercase are:\n\n");
	for (index = 0; index < number_of_sentences; index++)
	{
		num_whitespace_chars += whitespace_char_count(dynamic_sentence_array[index]); /* Count whitespace characters, add to total */
		num_nonwhitespace_chars += nonwhitespace_char_couunt(dynamic_sentence_array[index]); /* Count non-whitespace characters, add to total */
		fputs(dynamic_sentence_array[index], stdout); /* Print uppercase sentence to screen */
	}
	
	total_characters = num_nonwhitespace_chars + num_whitespace_chars;
	printf("\nThere are %d characters, or %d, not including whitespace\n", total_characters, num_nonwhitespace_chars);

	for(index = 0; index < number_of_sentences; index++)
	{
		free((void *) dynamic_sentence_array[index]); /* Deallocate each sentence */
	}

	free((void *) dynamic_sentence_array); /* Deallocate sentence array */

	printf("Press any key to continue\n");
	getchar();
}