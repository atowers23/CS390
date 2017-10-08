#include "program3.h"
#include "program3_utilities.h"
#include <stdio.h>
#include <stdlib.h>

/* 
Andrew Towers
Program #3
	Insert block comment
*/


int main(void)
{
	int number_of_sentences;
	char **dynamic_sentence_array;
	int index;
	char bufferValue = 0; /*Used to clear buffer*/
	int num_whitespace_chars = 0;
	int num_nonwhitespace_chars = 0;
	int total_characters;

	printf("How many sentences do you wish to enter: ");
	scanf("%d", &number_of_sentences);
	while (bufferValue != '\n') {
		bufferValue = getchar();
	}

	dynamic_sentence_array = (char **)malloc(sizeof(char *) * number_of_sentences);

	for (index = 0; index < number_of_sentences; index++)
	{
		dynamic_sentence_array[index] = (char *)malloc(sizeof(char) * 100);
		printf("Enter sentence #%d: ", index + 1);
		fgets(dynamic_sentence_array[index], sizeof(dynamic_sentence_array[index]), stdin);
		remove_newline_char(dynamic_sentence_array[index]);
	}

	convertAll(dynamic_sentence_array, number_of_sentences);
	printf("Your sentences converted to uppercase are:\n");
	for (index = 0; index < number_of_sentences; index++)
	{
		num_whitespace_chars += whitespace_char_count(dynamic_sentence_array[index]);
		num_nonwhitespace_chars += nonwhitespace_char_couunt(dynamic_sentence_array[index]);
		puts(dynamic_sentence_array[index]);
	}
	
	total_characters = num_nonwhitespace_chars + num_whitespace_chars;
	printf("There are %d characters, or %d, not including whitespace", total_characters, num_nonwhitespace_chars);

	getchar();
}