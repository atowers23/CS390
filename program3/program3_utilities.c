#include <ctype.h>
void convertSentence(char *sentence)
{
	int sentence_index = 0;
	while (sentence[sentence_index] != '\0')
	{
		sentence[sentence_index] = toupper(sentence[sentence_index]);
		sentence_index++;
	}
}

void convertAll(char **sentenceList, int numofSentences)
{
	int sentences_index = 0;
	for (sentences_index = 0; sentences_index < numofSentences; sentences_index++)
	{
		convertSentence(sentenceList[sentences_index]);
	}
}

int nonwhitespace_char_couunt(char *sentence)
{
	int count = 0;
	int sentence_index = 0;
	char check_car = sentence[sentence_index];
	while (check_car != '\0')
	{
		if ( (check_car != (' ') ) && (check_car != ('\t')) && (check_car != ('\n')) &&
			(check_car != ('\v')) && (check_car != ('\f')) )
		{
			count++;
		}
		check_car = sentence[++sentence_index];
	}
	return count;
}

int whitespace_char_count(char *sentence)
{
	int count = 0;
	int sentence_index = 0;
	char check_car = sentence[sentence_index];
	while (check_car != '\0')
	{
		if ((check_car == (' ')) || (check_car == ('\t')) || (check_car == ('\n')) ||
			(check_car == ('\v')) || (check_car == ('\f')))
		{
			count++;
		}
		check_car = sentence[++sentence_index];
	}
	return count;
}

void remove_newline_char(char *sentence)
{
	int index = 0;
	while (sentence[index] != '\0')
	{
		if (sentence[index] == '\n')
		{
			sentence[index] = '\0';
		}
		index++;
	}
}