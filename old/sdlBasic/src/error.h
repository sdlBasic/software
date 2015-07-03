/*
    Name:       error.h
    Purpose:    error support routines
    Author:     David Cuny
                Partially derived from Brian Kernighan and Rob Pike
                The Practice of Programming
*/


typedef struct MemLink MemLink;


struct MemLink {
    int     start;     /* start of allocated block */
    int     end;       /* end of allocated block */
    MemLink *next;      /* in list */
};

MemLink *lastMemLink = NULL;

enum {
    Init,       /* initialization error */
    Syntax,     /* syntax error */
    Runtime     /* runtime error */
};

//static char *eprogname = NULL;          /* program name for messages    */

void eShutdown( int errorlevel );
void ePrintf( int errType, char *fmt, ... );
void eConsole( char *fmt, ... );
void eAddMemLinkDebug( void *p, int n );
void eRemoveMemLinkDebug( void *address );
MemLink *lookupMemLink( void *p );
void eMemTestDebug( char *message, void *address );
void freeMemLinksDebug( void );
char *eCopyString(char *s);
void expected( char *expect );
void *eMalloc(size_t size);
void *eRealloc( void *old, size_t size);
void eFreeDebug( void *address );
void screendebug();
