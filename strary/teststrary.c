


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "dstrary.h"


char **dstrary(char *, STRING_DELM *, int);

int main(int argc, char **argv)
{


	int x;

	//                       head     :  head  :    tail   :  tail  : flags
	//			delimiter : length : delimiter : length
	STRING_DELM delms[] = { {  ",",     1,         ",",       1,      IGNORE_DELMS               },
	         		        { "\"",     1,        "\"",       1,      KEEP_DELMS                 },
							{ "{" ,     1,         "}",       1,      IGNORE_DELMS               },
							{  "'",     1,         "'",       1,      KEEP_DELMS                 },
							{ "<<",     2,        ">>",       2,      KEEP_DELMS                 },
							{ "/*",     2,        "*/",       2,      IGNORE_STRING | KEEP_DELMS },
							{ "[" ,     1,         "]",       1,      KEEP_DELMS                 },
							{ "//",     2,        NULL,       0,      IGNORE_STRING              },
							{ NULL,     0,        NULL,       0,      END_DELM_ARRAY             } };

	char *astr;
	char *bstr;

	if (argc > 1) {
		astr = argv[1];	
	} else {
		astr = "This is a \" test of the emergency\" and 'now for this' string before // broadcasting service";
	}

	if ( argc > 2) 
		bstr = argv[2];
	else
		bstr = "This <<is a shorter>> /* This should not be printed out */ string";

	char **array = NULL;
	array = dstrary(astr, &delms[0], strlen(astr));
	if (array != NULL) {
		x = 0;
        
		while(array[x] != NULL)
		{
			printf("1: \"%s\"\n", array[x]);
			x++;
		}
        
		x = 0;
		free(array);
	}	

	array = dstrary(bstr, &delms[0], strlen(bstr));
	if (array != NULL) {
		x = 0;
		while(array[x] != NULL)
		{
			printf("2: \"%s\"\n", array[x]);
			x++;
		}
		x = 0;
		free(array);
	}

	dstrary(NULL, NULL, 0);

	return(0);
}
