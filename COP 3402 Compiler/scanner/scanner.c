#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "scanner.h"

#define IDENT_LIMIT 11
#define NUM_LIMIT 5
#define __INPUT_FILE__ "./input.txt"
#define __CLEAN_INPUT__ "./cleaninput.txt"
#define __LEX_TABLE__ "./lexemetable.txt"
#define __TOKEN_LIST__ "./tokenlist.txt"

// Prototypes
void clean_input();
void scan();
void get_digits();
void get_variable();

int main()
{
	clean_input();
	return 0;
}

void clean_input()
{
	FILE * fp, * ofp;
	fp = (FILE *) fopen(__INPUT_FILE__, "r");
	ofp = (FILE *)fopen(__CLEAN_INPUT__, "w");

	if (fp == NULL || ofp == NULL)
	{
		fprintf("Could not open: %s", __INPUT_FILE__);
		exit(0);
	}

	char chr, nextchr;
	//fscanf(fp, "%s", &file)
	bool ignore = false;
	while ((chr = fgetc(fp)) != EOF)
	{
		if (chr == '/' && (nextchr = fgetc(fp)) == '*') { ignore = true; }
		else if (chr == '*' && (nextchr = fgetc(fp)) == '/') { ignore = false; continue; }
		if (!ignore) { fputc(chr, ofp); }
	}
	fclose(ofp);
	fclose(fp);
}
void scan()
{

}