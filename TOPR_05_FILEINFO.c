#include "TOPR_05_FILEINFO.h"
int countLinesFile(char *filePath)
{
	fprintfWrapper_newline_head("countLinesFile: start -> from %s!", filePath);
	FILE *file = fopen(filePath, "r");
	if (file != NULL)
	{
		fprintfWrapper_newline_mid_upper("countLinesFile: reading from %s", filePath);
		char buf[BUFFER_SIZE];
		int counter = 0;
		for(;;)
		{
				size_t res = fread(buf, 1, BUFFER_SIZE, file);
				if (ferror(file))
				{ 
					fprintfWrapper_newline_tail("countLinesFile: failure -> ferror(%s)", filePath) ;
					exit(1);
				}

				int i;
				for(i = 0; i < res; i++)
				{
					if (buf[i] == '\n') {counter++ ;}
				}
				if (feof(file)) {break; }
		}
		fprintfWrapper_newline_mid_upper("countLinesFile: closing %s", filePath);
		fclose(file);

		fprintfWrapper_newline_tail("countLinesFile: success -> lines in %s: %d", filePath, counter);
		return counter;
	}
	fprintfWrapper_newline_tail("countLinesFile: failure -> not exist: %s", filePath);
	exit(1);
}

int countLinesFile_noHeader(char *filePath)
{
	fprintfWrapper_newline_head("countLinesFile_noHeader: start -> from %s without a header!", filePath);
	int result = countLinesFile(filePath) - 1;
	fprintfWrapper_newline_tail("countLinesFile_noHeader: success -> %d lines!", result);
	return result;
}

int countColumnsFile(char *filePath)
{
	fprintfWrapper_newline_head("countColumnsFile: start -> from %s!", filePath);
	FILE *file = fopen(filePath, "r");
	if (file != NULL)
	{
		fprintfWrapper_newline_mid_upper("countColumnsFile: reading %s", filePath);
		char buf[BUFFER_SIZE];
		int counter = 0;
		for(;;)
		{
				size_t res = fread(buf, 1, BUFFER_SIZE, file);
				if (ferror(file)) { return -1; }

				int i;
				for(i = 0; i < res; i++)
			{
						if (buf[i] == '\n') {break ;}
						if (buf[i] == ',') {counter ++ ;}
			}

				if (feof(file)) {break; }
		}

		fclose(file);

		if (counter > 1) {counter++;}
		fprintfWrapper_newline_tail("countColumnsFile: success -> columns in %s: %d", filePath, counter);
		return counter;
	}
	fprintfWrapper_newline_tail("countColumnsFile: failure -> does not exist: %s", filePath);
	exit(1);
}
