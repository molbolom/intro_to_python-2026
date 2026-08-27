

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dstrary.h"



struct TEMP_STRARY_S
{
	char *str;
	int len;
	struct TEMP_STRARY_S *next;
};


struct TEMP_STRARY_S *tempstrary = NULL;

void free_tempstrary();


char **dstrary(char *str, STRING_DELM *d, int len)
{

	if (str == NULL) {
		free_tempstrary();
		return(NULL);
	}

	if (tempstrary == NULL) {
		tempstrary = malloc(sizeof(struct TEMP_STRARY_S));
		tempstrary->str = NULL;
		tempstrary->len = 0;
		tempstrary->next = NULL;
	}

	struct TEMP_STRARY_S *t = tempstrary;

	// if bstr == str, then read a normal string.
	char *bstr = str;		// Backup string.
	
	char **array = NULL;
	
	int x = 0;


	while( *str != 0)
	{
		x = 0;
	//	Remove extra white spaces before string.
		while(*str < 0x21) str++;
	// Search string for the head.
	// bstr == str on entrant to next while loop.

		bstr = str;
	
		while(d[x].head != NULL)
		{

			if (strncmp(str, d[x].head, d[x].hlen) == 0) {

				str+=d[x].hlen;
		// If tail is NULL, read til the end of the string, and exit.
				if(d[x].tail == NULL) {
					while(*str != 0) str++;
					break;
				}

				while(*str != 0)
				{
					if (strncmp(str, d[x].tail, d[x].tlen) == 0) {
						break;
					}
					str++;

				}
		// Tail wasn't found, read like this was a normal string.
				if( *str == 0 ) {
				      	str = bstr;	
					break;
				}

		// Add the delimiters if the flag is set.
				if (d[x].flags & KEEP_DELMS) {
					str = str + d[x].tlen;
				} else {
					bstr = bstr + d[x].hlen;
				}
		// We are done.
				break;


			}

			x++;
		}

	// Read a normal string.
		if (bstr == str) {
			while(*str > 0x20) str++;
		} 


	// Add the string to the temporary string list 
	// if flags not set.
		if ( (d[x].flags & IGNORE_STRING) != 4) {

		// If next list item is NULL, create a new list item.
			if (t->next == NULL) {
				t->next = malloc(sizeof(struct TEMP_STRARY_S));
				t->next->next = NULL;
				t->next->str = NULL;
				t->next->len = 0;
		// Else empty the string of the next item.
			} else {
				t->next->str = NULL;
			}

			t->str = bstr;
			t->len = str-bstr+1;   // Add \0 character.
		
			t = t->next;
		}




	// Error checking
		if ( *str == 0) break;
		str++;
	}

	// Get length of string needed.
	
	t = tempstrary;

	x = 0;
	int c = 1;
	while(t != NULL)
	{
		if (t->str == NULL) break;
		x = x + t->len;
		c++;
		t = t->next;
	}

	t = tempstrary;
	array = calloc(sizeof(char), x + sizeof(char *)*(c+1));
	bstr = (char *)array+sizeof(char *)*(c+1);



	for(x=0;x<c-1;x++) {
			
		array[x] = bstr;
		strncpy(bstr, t->str, t->len-1);
		bstr = bstr + t->len-1;
		*bstr = 0;
		bstr++;

		t = t->next;

		if(t == NULL) break;
		if(t->str == NULL) break;

	}	

	return(array);
}






void free_tempstrary(void)
{
	struct TEMP_STRARY_S *t = tempstrary;

	while(tempstrary != NULL) {
	
		t = t->next;
		free(tempstrary);
		tempstrary = t;
	}


}
