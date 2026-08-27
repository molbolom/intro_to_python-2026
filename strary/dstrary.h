

/*
 * 	flags
 *
 * 	SINGLE_DELM  
 * 			","  SINGLE_DELM implies that every element before and after the 
 * 					comma is treated as two array items.
 * 	REPEAT_DELM     "\"" Quotation is a repeat delm.  This is unnecessary as 
 * 					head = " and tail = " would suffice as well.
 * 	THROW_AWAY_DELM      Everything after head, tail, or between is ignored.
 * 	IGNORE_DELM	     Rename of THROW_AWAY_DELM
 *
 *
 * 	END_DELM_ARRAY       This is just to make the if statements more legible. 
 *
 */

#define IGNORE_DELMS       0
#define KEEP_DELMS         1
#define IGNORE_STRING      4
#define END_DELM_ARRAY   128

#define IS_END_DELMS(delm)  (delm.flags && END_DELM_ARRAY)

/*
 *  STRING_DELM   String delimeter data.
 *
 *  	head   The first part
 *  	tail   The last part
 *  	flags  
 *
 *
 * 	head     tail      Flag             Output
 *                                          
 * 	NULL     any       any              End read of delimiters.
 * 	any     NULL       IGNORE_STRING    Everything after "any" is not written to array.
 * 	any     NULL       IGNORE_DELMS     Everything from after "any" to end of string is 
 * 						written as one array item. 
 * 	any     NULL       KEEP_DELMS  	    Everything from "any" to end of string is written
 * 	  					as one array item.
 * 	any     NULL       IGNORE_STRING    Kind of a useless thing, but the
 * 	  					delimiter is kept while the string is discarded.
 * 	any     NULL       IGNORE_STRING | IGNORE_DELMS  Same as above, but does not keep the 
 * 						delimiter.
 * 	  A        B       IGNORE_STRING    Everything between A and B is not added to array. 
 * 	  A        B       IGNORE_STRING | IGNORE_DELMS   Everything between and including A
 * 	  A        B       
 *
 * 	  		   IGNORE_DELMS   is the default.
 * 	  					and B is not added to array.
*/


typedef struct STRING_DELM_S 
{
	char *head;
	int hlen;
	char *tail;
	int tlen;
	int flags;

} STRING_DELM;
