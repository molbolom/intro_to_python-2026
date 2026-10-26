#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "str2list.h"

struct TEMP_LIST tmplist = { 0, 0, NULL };
struct STRING_DELIMITERS delms = {0,NULL};


int resize_delimiters(int l)
{
	if (l <= 0)
		return(REDELIMLIST_ERROR_LENGTH);

	int i = delms.len;
	    l = delms.len + l;

	struct DELIMITERS *newdelms = NULL;

	newdelms = realloc(delms.item, sizeof(struct DELIMITERS)*l);

	if (newdelms == NULL)
		return(REDELIMLIST_ERROR_ALLOC);

	for (int x=i;x<l;x++) {
		newdelms[x].head = NULL;
		newdelms[x].headlen = 1;
		newdelms[x].tail = NULL;
		newdelms[x].taillen = 0;
		newdelms[x].flags   = 0;
	}

	delms.len = l;
	delms.item = newdelms;
	delms.item[l-1].headlen = 0;		// Set the end read.

	return(REDELIMLIST_OK);

}


int add_delimiter(char *head, char *tail, int flags)
{
	int x;


	for(x=0;delms.item[x].head != NULL;x++);

	if (delms.item[x].headlen == 0)			// End of list.
		resize_delimiters(5);

	int headlen = strlen(head);
	int taillen = strlen(tail);

	char *newstr = malloc(sizeof(char)*(headlen+taillen+2));

	if(newstr == NULL)
		return(1);

	strncpy(newstr, head, headlen);
	*(newstr+headlen) = 0;
	strncpy(newstr+headlen+1, tail, taillen);
	*(newstr+headlen+taillen+1) = 0;

	delms.item[x].head = newstr;
	delms.item[x].tail = newstr+headlen+1;
	delms.item[x].headlen = headlen;
	delms.item[x].taillen = taillen;

}

int resize_tmplist(int l)
{

	if (l <= 0) 
		return(RETEMPLIST_ERROR_LENGTH);

	int i = tmplist.len;			// Start index for initializing data.
	l     = tmplist.len + l;

	struct STRING_LIST *newlist = NULL;

	newlist = realloc(tmplist.item, sizeof(struct STRING_LIST)*l);

	if (newlist == NULL)
		return(RETEMPLIST_ERROR_ALLOC);

	for (int x=i;x<l;x++) {
		newlist[x].head = NULL;
		newlist[x].tail = NULL;
	}

	tmplist.len = l;
	tmplist.item = newlist;
	return(RETEMPLIST_OK);
}


char **str2list(char *str)
{

	if (*str == 0)
		return(NULL);

	if (tmplist.len == 0)
		return(NULL);

	tmplist.c = 0;

	char *head = NULL;
	char *tail = NULL;

	int x;

	while (*str != 0)
	{
		if (head == NULL)
		{
			if (*str == 0x20)
			{
				str++;
				continue;
			}
			head = str;				// Start reading the string.
		}

		for (x=0;delms.item[x].head != NULL;x++)
		{
			if (strncmp(str, delms.item[x].head, delms.item[x].headlen) == 0)
			{
				tail = strstr(str+delms.item[x].headlen, delms.item[x].tail);
				if (tail != NULL)
				{

					if (head + delms.item[x].headlen == tail) {
						str = tail + delms.item[x].taillen;
						tail = head;
						break;
					}
					if (delms.item[x].flags == DELIMITER_WITH_STRING) {
						if (head < str) {
							tail = tail + delms.item[x].taillen;
							str = tail;
						} else {
							tail = NULL;
							continue;
						}
						break;
					}

					if (head < str) {
						tail =str;
					} else if (delms.item[x].flags == DISCARD_DELIMITERS) {		// Don't keep delims.
						head = head + delms.item[x].headlen;
						str = tail + delms.item[x].taillen;

					} else if (delms.item[x].flags == DISCARD_STRING) {			// Comment string out.
						str = tail + delms.item[x].taillen;
						head = NULL;
						tail = NULL;
					} else {											// Keep delims. 
						tail = tail + delms.item[x].taillen;
						str = tail;
					}

					break;

				}
			}
		}

		if (tail == head) {
			head = NULL;
			tail = NULL;
			continue;
		}

		if (tail == NULL) {						// If a normal string is being read.
			str++;
			if ((*str == 0x20) || (*str == 0))
				tail = str;
		}

		

		if ((tail != NULL) && (tail != head))   	// If tail was found, write to string.
		{

			if (tmplist.c >= tmplist.len)
			{
				int resize = resize_tmplist(5);
				if (resize != 0)
					return(NULL);
			}

			tmplist.item[tmplist.c].head = head;
			tmplist.item[tmplist.c].tail = tail;
			tmplist.c++;
			head = NULL;
			tail = NULL;

		} 
	}


	int sl = 0;

	char **newlist = NULL;

	for (x=0;x<tmplist.c;x++) {
		sl = sl + tmplist.item[x].tail - tmplist.item[x].head+1;	
	}

	newlist = malloc(sizeof(char *)*(tmplist.c+1) + sizeof(char)*sl);
	str = (char *)(newlist+(tmplist.c+1));

	for(x=0;x<tmplist.c;x++) {
		sl = tmplist.item[x].tail-tmplist.item[x].head;		// Get the length of the string.
		newlist[x] = str;
		strncpy(str, tmplist.item[x].head, sl);					// Copy the string over.
		*(str+sl) = 0;
		str = str + sl + 1;
	}

	newlist[tmplist.c] = NULL;


	return(newlist);

}


int main(int argc, char **argv)
{

	resize_delimiters(5);

	add_delimiter("{", "}", DISCARD_DELIMITERS);
	add_delimiter("[", "]", DELIMITER_WITH_STRING);
	add_delimiter("[", "]", DISCARD_DELIMITERS);
	add_delimiter("\"", "\"", 0);
	add_delimiter("'", "'",   0);
	add_delimiter("/*", "*/", DISCARD_STRING);
	add_delimiter("<<", ">>", 0);
	add_delimiter("<|", "|>", 0);

	int x;

	for (x=0;x<delms.len;x++) {
		if (delms.item[x].head == NULL) {
			if (delms.item[x].headlen > 0) {
				printf("Delimiter[%d] will be skipped.\n", x);
			} else {
				printf("Delimiter[%d] is the last item.\n", x);
			}
		} else {
			printf("Delimiter[%d] = '%s':'%s'.\n", x, delms.item[x].head, delms.item[x].tail);
		}

	}

	return(0);

//	STRING_DELIMITERS d[] = { {"{", 1, "}", 1, DISCARD_DELIMITERS},
//							  {"[", 1, "]", 1, DELIMITER_WITH_STRING},
//							  {"[", 1, "]", 1, DISCARD_DELIMITERS},
//							  {"\"", 1, "\"", 1, KEEP_DELIMITERS},
//							  {"'", 1, "'", 1, KEEP_DELIMITERS},
//							  {"/*", 2, "*/", 2, DISCARD_STRING},
//							  {NULL, 0, NULL, 0, 0}};
//
//	delms = &d[0];
//
//	resize_delimiters(5);
//
//	return(0);
//
//
//	int resize = resize_tmplist(15);
//	char **list;
//	if (resize != 0 ) {
//		printf("Error in creating tmplist.\n");
//		return(1);
//	}
//
//	if (argc == 2) {
//		list = str2list(&d[0], argv[1]);
//	}
//
//	int x;
//
//	for (x=0;list[x]!=NULL;x++) {
//		printf("%s\n", list[x]);
//	}
//
//	free(tmplist.item);
//	free(list);
//	
//	return(0);

}
