#include <ctype.h>

/* Converts a sentence to all uppercase.  The actual value of the string is modified */
void convertSentence(char *sentence)
{
	int sentence_index = 0;
	while (sentence[sentence_index] != '\0')
	{
		sentence[sentence_index] = toupper(sentence[sentence_index]);
		sentence_index++;
	}
}
 /* Converts every sentence in a sentence array to all uppercase.  The actual values of the strings are modified */
void convertAll(char **sentenceList, int numofSentences)
{
	int sentences_index = 0;
	for (sentences_index = 0; sentences_index < numofSentences; sentences_index++)
	{
		convertSentence(sentenceList[sentences_index]);
	}
}

/* Returns total amount of nonwhitespace characters in a given string */
int nonwhitespace_char_couunt(char *sentence)
{
	int count = 0;
	int sentence_index = 0;
	char check_car = sentence[sentence_index];
	while (check_car != '\n')
	{
		if ( (check_car != (' ') ) && (check_car != ('\t')) &&
			(check_car != ('\v')) && (check_car != ('\f')) )
		{
			count++;
		}
		check_car = sentence[++sentence_index];
	}
	return count;
}

/* Returns total amount of whitespace characters in a given string */
int whitespace_char_count(char *sentence)
{
	int count = 0;
	int sentence_index = 0;
	char check_car = sentence[sentence_index];
	while (check_car != '\n')
	{
		if ((check_car == (' ')) || (check_car == ('\t')) ||
			(check_car == ('\v')) || (check_car == ('\f')))
		{
			count++;
		}
		check_car = sentence[++sentence_index];
	}
	return count;
}
