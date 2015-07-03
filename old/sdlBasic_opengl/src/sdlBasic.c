/*
    Name:       sdlBasic.c
    Purpose:    Core (wxBasic) version of sdlBasic
    Author:     David Cuny
    modify __vroby__ <__vroby__@libero.it>
    Copyright:  (c) 2001 David Cuny <dcuny@lanset.com>
    Licence:    LGPL
*/

/*
#define __DEBUG_MEMORY__
#define __DEBUG_TRACE__
*/


#define PROGNAME    "sdlBasic"
#define RELEASE     "beta 0.50 "
#define DATEOF      "Gennaio , 2004"
#define COPYRIGHT   "(c) 2001 David Cuny modify __vroby__"
#define LICENSE     "Lesser GNU Public License"
#define SDLBASIC    1

#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <malloc.h>
#include <limits.h>
#include <float.h>

#ifndef UNIX
	#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

extern int yylex();
extern void yyerror( char* );

#include "shared.h"         /* shared routines                      */
#include "trace.h"          /* tracing code                         */
#include "error.h"          /* error messages and 'safe' routines   */
#include "stack.h"          /* integer stack                        */
#include "data.h"           /* data types                           */
#include "symbol.h"         /* symbol table                         */
#include "var.h"            /* variables                            */
#include "array.h"          /* array structures                     */
#include "node.h"           /* parse tree data structure            */
#include "file.h"           /* file support                         */
#include "dir.h"	    /* dir support		*/
#include "builtin.h"        /* builtin routines                     */

#include "const.h"          /* builtin constants                    */
#include "eval.h"           /* parse tree evaluator                 */
#include "keyword.h"        /* keywords (defined in grammar file)   */
#include "lexer.h"          /* lexer                                */
#include "class.h"          /* class wrappers                       */


#include "trace.c"          /* tracing code                         */
#include "error.c"          /* error messages and 'safe' routines   */
#include "stack.c"          /* integer stack                        */
#include "data.c"           /* data types                           */
#include "symbol.c"         /* symbol table                         */
#include "var.c"            /* variables                            */
#include "array.c"          /* array structures                     */
#include "node.c"           /* parse tree data structure            */
#include "file.c"           /* file support                         */
#include "dir.c"	    /* dir support		*/
#include "builtin.c"        /* builtin routines                     */


#include "const.c"          /* builtin constants                    */
#include "eval.c"           /* parse tree evaluator                 */
#include "y_tab.c"          /* bison parser                         */
#include "keyword.c"        /* keywords (defined in grammar file)   */
#include "lexer.c"          /* lexer                                */
#include "class.c"          /* class wrappers                       */

#include "SDLengine.h"	    /* SDL rutines */
//#include "GLengine.h"	    /* GL rutines */


/* setCwd: set current working directory based on command line */
void setCwd( char *text )
{
    /* probably not portable to the Mac... */

    int     at, i;
    char    delim;

    /* copy the string */
    cwd = eCopyString( text );

    /* what delimiter to use? */
    if (strchr(cwd, '\\') != NULL) {
        delim = '\\';

    } else if (strchr(cwd, '/') != NULL) {
        delim = '/';

    } else {
        cwd[0] = '\0';
        return;
    }

    /* search */
    at = 0;
    for ( i = 0; cwd[i] != '\0'; i++ ) {
        if (cwd[i] == delim) {
            at = i;
        }
    }
    cwd[at+1] = '\0';


}


int interpret( int argc, char **argv )
{
    FILE    *file;
    char    tag[32];
    int     i;
    long    offset;


    /* initialize */
    argCountStack = newStack(32);   /* count of parameters */
    blockStack = newStack(32);      /* control block stack */
    lineStack = newStack(32);       /* current line number */
    sharedStack = newStack(128);    /* list of shared variables */
    currentScope = NULL;            /* routine being defined */
    currentType = NULL;             /* no type being defined */
    initKeywords();                 /* builtin keywords */
    initBuiltins();                 /* builtin functions */
    initConsts();                   /* builtin constants */


        /* require file name */
        if (argc == 1) {
            ePrintf( Init, "expected a file name\n");
		return -1;
        }

        /* load the file */
        pushIncludeFile( argv[1], 0 );

    //}

    /* parse the file */
    yyparse();

    /* deferred? */
    if (deferredCode != NULL) {
        eval( deferredCode );
        freeNode( deferredCode );
    }
    return 0;
}


int main(int argc, char **argv)
{
    int r;
    for (r=0;r<argc;r++)
	cmdArgv[r]=argv[r];
    cmdArgc=argc;

    initialize();
    GLinit();

    r= interpret(argc, argv);
    GLterminate();
    terminate();
    return r;
}
