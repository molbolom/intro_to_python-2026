


#define RETEMPLIST_ERROR_LENGTH  1
#define RETEMPLIST_ERROR_ALLOC   2
#define RETEMPLIST_OK            0

#define REDELIMLIST_ERROR_LENGTH 1
#define REDELIMLIST_ERROR_ALLOC  2
#define REDELIMLIST_OK           0

#define KEEP_DELIMITERS          0
#define DISCARD_DELIMITERS       1
#define DISCARD_STRING           2
#define DELIMITER_WITH_STRING    4

struct DELIMITERS {
	char *head;
	int   headlen;
	char *tail;
	int   taillen;
	int   flags;
};

struct STRING_DELIMITERS {
	int len;
	struct DELIMITERS *item;
};

struct STRING_LIST {
	char *head;
	char *tail;
};

struct TEMP_LIST {
	int c;
	int len;

	struct STRING_LIST *item;
};

