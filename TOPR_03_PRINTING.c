#include "TOPR_03_PRINTING.h"
void fprintfWrapper(const char* format, va_list args)
{
	
	{	
		vfprintf(stdout, format, args);		/* vytiskne zformtovany text do stdout bufferu */
		fflush(stdout);				/* vyprzdní stdout buffer */
	}
}

void fprintfWrapper_newline(const char* format, ...)
{
	if (debugLevel >= MainAndFuncbounds)
	{
		va_list args;				/* načte argumenty do listu*/		
		va_start(args, format);			/* vloží argumenty do textu s %d %f %s... */
		fprintfWrapper(format, args);
		fprintf(stdout, "\n");
		va_end(args);				/* pročistí array s argumenty */
	}
}

void fprintfWrapper_newline_tail(const char* format, ...)
{
	if (debugLevel >= MainAndFuncbounds)
	{
		va_list args;
		va_start(args, format);	
		fprintf(stdout, "\t");
		fprintfWrapper(format, args);
		fprintf(stdout, "\n");
		va_end(args);	
	}
}

void fprintfWrapper_newline_mid_upper(const char* format, ...)
{
	if (debugLevel >= MainAndFuncboundsAndUpper)
	{
		va_list args;
		va_start(args, format);	
		fprintf(stdout, "\t\t");
		fprintfWrapper(format, args);
		fprintf(stdout, "\n");
		va_end(args);
	}
}

void fprintfWrapper_newline_mid_lower(const char* format, ...)
{
	if (debugLevel >= MainAndFuncboundsAndUpperAndLower)
	{
		va_list args;
		va_start(args, format);	
		fprintf(stdout, "\t\t\t");
		fprintfWrapper(format, args);
		fprintf(stdout, "\n");
		va_end(args);
	}
}

void fprintfWrapper_newline_head(const char* format, ...)
{
	if (debugLevel >= MainAndFuncbounds)
	{
		va_list args;
		va_start(args, format);	
		fprintf(stdout, "\n");
		fprintfWrapper(format, args);
		fprintf(stdout, "\n");
		va_end(args);	
	}
}

void fprintfWrapper_newline_MAIN(const char* format, ...)
{
	if (debugLevel >= MAIN)
	{
		va_list args;				/* načte argumenty do listu*/		
		va_start(args, format);			/* vloží argumenty do textu s %d %f %s... */
		fprintf(stdout, "MAIN:\t");
		fprintfWrapper(format, args);
		fprintf(stdout, "\n");
		va_end(args);				/* pročistí array s argumenty */
	}
}

void fprintfWrapper_comma(const char* format, ...)
{
	if (debugLevel >= MainAndFuncboundsAndUpper)
	{
		va_list args;
		va_start(args, format);	
		fprintf(stdout, "\t");
		fprintfWrapper(format, args);
		fprintf(stdout, ", ");
		va_end(args);	
	}
}

void printArrayNative1D_string(char **arrayIpt, int length)
{
	if (debugLevel>= MainAndFuncboundsAndUpper)
	{
		fprintfWrapper_newline_head("printArrayNative1D_string: start!");
		unsigned int index;
		printf("[");
		for (index = 0; index < length; index++)
		{
			printf("%s, ", arrayIpt[index]);
		}
		printf("]\n");
		fflush(stdout);
		fprintfWrapper_newline_tail("printArrayNative1D_string: success!");
	}
}

void printArrayNative1D_double(double *arrayIpt, int length)
{
	if (debugLevel >= MainAndFuncboundsAndUpper)
	{
		fprintfWrapper_newline_head("printArrayNative1D_double: start!");
		unsigned int index;
		printf("[");
		for (index = 0; index < length; index++)
		{
			printf("%lf, ", arrayIpt[index]);
		}
		printf("]\n");
		fflush(stdout);
		fprintfWrapper_newline_tail("printArrayNative1D_double: success!");
	}
}