/*
    Name:       error.c
    Purpose:    error support routines
    Author:     David Cuny and modify for sdlBasic  __vroby__
                Partially derived from Brian Kernighan and Rob Pike
                The Practice of Programming
    Copyright:  (c) 2001 David Cuny <dcuny@lanset.com>
    Licence:    LGPL
		
*/


/* which version to use? */
#ifdef __DEBUG_MEMORY__
    #define eAddMemLink(a,n)        eAddMemLinkDebug(a,n)
    #define eRemoveMemLink(a)       eRemoveMemLinkDebug(a)
    #define eMemTest(s,a)           eMemTestDebug(s,a)
    #define eFree(a)                eFreeDebug(a)
#else
    #define eAddMemLink(a,n)
    #define eRemoveMemLink(a)
    #define eMemTest(s,a)
    #define eFree(a)                free(a)
#endif


/* eShutdown: close the application gracefully */
void eShutdown( int errorlevel )
{
    /* close any open files here... */
    exit(errorlevel);
}

/* ePrintf: print an error message and exit */
void ePrintf( int errType, char *fmt, ... )
{
    char        buffer[512], message[512];
    va_list     args;
    FILE        *errFile;
    SourceCode  *source = NULL;

#ifndef __WXWIN__
    fflush(stdout);
    fprintf(stderr, "\n" );
#endif

    switch (errType) {
    case Init:
        sprintf( message, "Initialization error\n" );
        break;

    case Syntax:
        source = eFindSource( parseLineId );
        if (source == NULL) {
            sprintf( message, "Syntax error, source code lookup corrupted\n" );
        } else {
            sprintf( message, "Syntax error on line number %d of %s\n", source->lineNum, includeList[source->fileNum] );
        }
        break;

    case Runtime:
        source = eFindSource( runLineId );
        if (source == NULL) {
            sprintf( message, "Syntax error, source code lookup corrupted\n" );
        } else {
            sprintf( message, "Runtime error on line number %d of %s\n", source->lineNum, includeList[source->fileNum] );
        }
        break;

    default:
        sprintf( message, "Unknown error type\n" );

    }

    va_start(args, fmt);
    vsprintf( buffer, fmt, args );
    va_end(args);
    strcat( message, buffer );

    if (fmt[0] != '\0' && fmt[strlen(fmt)-1] == ':') {
        sprintf( message, "\n%s", strerror(errno));
        strcat( buffer, message );
    }
    strcat( buffer, "\n");

    /* show the line */
    if (errType != Init) {
        if (source == NULL) {
            strcat( message, "\n<<Can't Find Source Line>>" );

        } else {
            sprintf( buffer, "\n%s", source->text);
            strcat( message, buffer );
        }
    }

    fprintf( stderr, message );
    //printf( message );

    /* open the error file */
    errFile = fopen( "sdlBasic.err", "w" );
    if (errFile != NULL) {
        fprintf( errFile, message );
        fclose( errFile );
    }


    eShutdown(2); /* conventional for failed execution */
}

/* eConsole: cross-platform version of printf */
void eConsole( char *fmt, ... )
{
    char    buffer[512];
    int     i, linesOnConsole, removeTo;
    va_list args;

    va_start(args, fmt);
    vsprintf( buffer, fmt, args );
    va_end(args);


    fprintf( stdout, buffer );
}




/* eAddMemLinkDebug: add memory chunk to tracking list */
void eAddMemLinkDebug( void *p, int n )
{
    MemLink     *link;

    /* malloc the link */
    link = (MemLink *)malloc(sizeof(MemLink));
    if (link == NULL) {
        ePrintf( Runtime, "eMalloc: malloc of MemLink failed:" );
    }

    /* set values */
    link->start = (int)p;
    link->end = link->start+(n-1);
    link->next = lastMemLink;
    lastMemLink = link;

}

/* eRemoveMemLink: remove memory chunk from tracking list */
void eRemoveMemLinkDebug( void *address )
{
    MemLink     *link;
    MemLink     *prev;

    if (address == NULL) {
        return;
    }

    prev = NULL;
    for ( link = lastMemLink; link != NULL; link = link->next ) {
        if (link->start == (int)address) {

            if (prev == NULL) {
                lastMemLink = link->next;
            } else {
                prev->next = link->next;
            }
            return;
        }
        prev = link;
    }
    ePrintf( Runtime, "eRemoveMemLink: memory address %p not in list", address );
}

/* lookup: sequential search through MemLink list */
MemLink *lookupMemLink( void *p )
{
    MemLink *link = lastMemLink;

    for ( ; link != NULL; link = link->next ) {
        if ((int)p >= link->start && (int)p <= link->end) {
            return link;
        }
    }
    return NULL; /* no match */
}

/* eMemTest: ensure that the memory was allocated */
void eMemTestDebug( char *message, void *address )
{
    if (address != NULL) {
        if (lookupMemLink(address) == NULL) {
            ePrintf( Runtime, "eMemTest: %s: address %p not allocated",
                message, address );
        }
    }
}

/* freeAll: free all the elements in list */
void freeMemLinksDebug()
{
    MemLink *link = lastMemLink;
    MemLink *next;

    for ( ; link != NULL; link = next) {
        next = link->next;
        free( (void*)(link->start) );     /* the chunk */
        free( link );                   /* the link */
    }
}

/* eCopyString: copy a string, report if error */
char *eCopyString(char *s)
{
    char    *t;

    t = (char *)eMalloc(strlen(s)+1);
    strcpy(t, s);
    return t;
}

/* expected: print error and die */
void expected( char *expect )
{
    ePrintf( Syntax, "Parse error: expected %s, but got %s", expect, currtok );
}

/* eMalloc: malloc and report if error */
void *eMalloc(size_t size)
{
    void        *p;

    /* malloc the chunk */
    p = (void *)malloc(size);
    if (p == NULL) {
        ePrintf( Runtime, "eMalloc: malloc of %u bytes failed:", size );
    }
    eAddMemLink( p, size );
    return p;

}

/* eRealloc: realloc and report if error */
void *eRealloc( void *theOld, size_t size)
{
    void        *theNew;

    /* realloc the chunk */
    theNew = (void *)realloc(theOld,size);
    if (theNew == (void *)NULL) {
        ePrintf( Runtime, "eRealloc: realloc of %u bytes failed:", size );
    }

#ifdef __DEBUG_MEMORY__
    if (theNew != theOld) {
        eRemoveMemLink(theOld);
        eAddMemLink(theNew,size);
    }
#endif

    return theNew;

}

/* eFreeDebug: free chunk of memory */
void eFreeDebug( void *address )
{
    if (address != NULL) {
        eRemoveMemLink( address );
        free( address );
    }
}

