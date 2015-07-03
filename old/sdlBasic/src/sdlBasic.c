/*
    Name:       sdlBasic.c
    Purpose:    Core (wxBasic) version for sdlBasic
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
#define RELEASE     "20050625"
#define DATEOF      "giugno , 2005"
#define COPYRIGHT   "(c) 2001 David Cuny and __vroby__"
#define LICENSE     "Lesser GNU Public License"
#define SDLBASIC    1

#ifdef AMIGA
#define Node ExecNode
#include <exec/nodes.h>
#undef Node
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <limits.h>
#include <float.h>
#ifdef __amigaos4__
#include <sys/stat.h>
#endif

/* This piece of code allows sdlBasic to compile on FreeBSD 5.3 without <malloc.h> include problem */
/* Contributed by Tails92 */
#if !defined(FBSD) && !defined(__amigaos4__) && !defined(__APPLE__)
#include <malloc.h>
#else
#include <stdlib.h>
#endif

#if defined(WIN32)
#include <windows.h>
#endif

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
#include "dir.h"			/* dir support							*/
#include "datafunction.h"   /* data support							*/
#include "membank.h"	    /* membank support						*/
#include "builtin.h"        /* builtin routines                     */

#include "const.h"          /* builtin constants                    */
#include "eval.h"           /* parse tree evaluator                 */
#include "keyword.h"        /* keywords (defined in grammar file)   */
#include "lexer.h"          /* lexer                                */
#include "class.h"          /* class wrappers                       */

#include "unzip.h"	    /* Unzip Rutines */
#include "SDLengine.h"	    /* SDL rutines */
//#include "GLengine.h"	    /* GL rutines */


#include "trace.c"          /* tracing code                         */
#include "error.c"          /* error messages and 'safe' routines   */
#include "stack.c"          /* integer stack                        */
#include "data.c"           /* data types                           */
#include "symbol.c"         /* symbol table                         */
#include "var.c"            /* variables                            */
#include "array.c"          /* array structures                     */
#include "node.c"           /* parse tree data structure            */
#include "file.c"           /* file support                         */
#include "dir.c"	    /* dir support			    */
#include "datafunction.c"   /* data support			    */
#include "membank.c"	    /* membank support			    */
#include "builtin.c"        /* builtin routines                     */


#include "const.c"          /* builtin constants                    */
#include "eval.c"           /* parse tree evaluator                 */
#include "y_tab.c"          /* bison parser                         */
#include "keyword.c"        /* keywords (defined in grammar file)   */
#include "lexer.c"          /* lexer                                */
#include "class.c"          /* class wrappers                       */

//____________________________________________________________________________________________________________________

int interpret( int argc, char **argv )
{

    /* initialize */
    argCountStack = newStack(32);	/* count of parameters */
    blockStack = newStack(32);		/* control block stack */
    lineStack = newStack(32);		/* current line number */
    sharedStack = newStack(128);	/* list of shared variables */
    currentScope = NULL;            	/* routine being defined */
    oldScopeP=0;			/* intial oldScope=0 */
    oldScope[oldScopeP]=currentScope;	/*oldScope0 =NULL */
    currentType = NULL;             	/* no type being defined */
    initKeywords();                 	/* builtin keywords */
    initBuiltins();                 	/* builtin functions */
    initConsts();                   	/* builtin constants */


        /* require file name */
        if (argc == 1) {
            ePrintf( Init, "expected a file name\n");
		return -1;
        }

        /* load the file */
        pushIncludeFile( argv[1], 0 );

    /* parse the file */
    yyparse();

    /* deferred? */
    if (deferredCode != NULL) {
        eval( deferredCode );
        freeNode( deferredCode );
    }
    return 0;
}

//____________________________________________________________________________________________________________________

int main(int argc, char **argv)
{
    int r,r1;
    char *curdir;
    int audio,socket,debug;

    if (argc<=1) { return 0; }
    /* default settings */
    audio=1;
    socket=1;
    debug=0;

    remove("sdlBasic.err");

    /* the command line param */
    r1=0;
    for (r=0;r<argc;r++){
	cmdArgv[r1]=eCopyString(argv[r]);
#ifdef UNIX
	if (strcasecmp(cmdArgv[r1],"--nosound")==0)
	    audio=0;
	if (strcasecmp(cmdArgv[r1],"--nosocket")==0)
	    socket=0;
	if (strcasecmp(cmdArgv[r1],"--debug")==0)
	    debug=1;
	if (strcasecmp(cmdArgv[r1],"--version")==0)
	    ePrintf( Init, "sdlBasic release  %s\n", RELEASE);
	if (strcasecmp(cmdArgv[r1],"--license")==0)
	    ePrintf( Init, "%s\n", LICENSE);
	if (strcasecmp(cmdArgv[r1],"--copyright")==0)
	    ePrintf( Init, "%s\n", COPYRIGHT);
#else
	if (stricmp(cmdArgv[r1],"--nosound")==0)
	    audio=0;
	if (stricmp(cmdArgv[r1],"--nosocket")==0)
	    socket=0;
	if (stricmp(cmdArgv[r1],"--debug")==0)
	    debug=1;
	if (stricmp(cmdArgv[r1],"--version")==0)
	    ePrintf( Init, "sdlBasic release  %s\n", RELEASE);
	if (stricmp(cmdArgv[r1],"--license")==0)
	    ePrintf( Init, "%s\n", LICENSE);
	if (stricmp(cmdArgv[r1],"--copyright")==0)
	    ePrintf( Init, "%s\n", COPYRIGHT);
#endif
	if (cmdArgv[r1][0]!='-')
	    r1++;
	else
	    eFree(cmdArgv[r1]);
    }
    cmdArgc=r1;

    /* if name <> sdlBasic =autoboot */
    r=strlen(cmdArgv[0]);
#ifdef UNIX
    if (strncasecmp(cmdArgv[0]+(r-8),"sdlbasic",8)!=0 ){
	cmdArgv[cmdArgc]=(char *)malloc(strlen(cmdArgv[0])+16);
	strcpy(cmdArgv[cmdArgc],cmdArgv[0]);
	strcat(cmdArgv[cmdArgc],".sdlbas\0");
	//ePrintf(Init,"argc=%d\n arg0=%s\n arg1=%s\n",cmdArgc,cmdArgv[0],cmdArgv[1]);
    	cmdArgc++;
    }
#else
    if (strnicmp(cmdArgv[0]+(r-12),"sdlbasic.exe",12)!=0 && strnicmp(cmdArgv[0]+(r-8),"sdlbasic",8)!=0 ){
	cmdArgv[cmdArgc]=(char *)malloc(strlen(cmdArgv[0])+16);
	strncpy(cmdArgv[cmdArgc],cmdArgv[0],strlen(cmdArgv[0])-4 );
	strcat(cmdArgv[cmdArgc],".sdlbas\0");
	//ePrintf(Init,"argc=%d\n arg0=%s\n arg1=%s\n",cmdArgc,cmdArgv[0],cmdArgv[1]);
    	cmdArgc++;
    }
#endif

    /*initializing SDLengine module */
    r=initialize(audio,socket);
    if (r==1){
	ePrintf( Init,"DejaVu fonts not found. Please reinstall sdlBasic.\n");
	return -1;
    }
    /*initialize basic data function buffer */
    init_data();

    /* initialize lvldir */
    lvldir=0;

    /* initialize sistem memory bank */
    init_membank();

    /* autochange directory */
#ifdef UNIX
    if(cmdArgv[1][0]=='/'){
	r=strlen(cmdArgv[1]);
	curdir=(char *)malloc(r+1);
	while(cmdArgv[1][r]!='/')r--;
	strncpy(curdir,cmdArgv[1],r);
	chdir(curdir);
    }
#elif AMIGA
    {
	r=strlen(cmdArgv[1]);
	curdir=(char *)malloc(r+1);

	if (curdir)
	{
	    STRPTR path;
	    strcpy(curdir,cmdArgv[1]);
	    path = FilePart(curdir);
	    *path = 0;
	    chdir(curdir);
	}
    }
#else
    if(cmdArgv[1][2]=='\\'){
	r=strlen(cmdArgv[1]);
	curdir=(char *)malloc(r+1);
	while(cmdArgv[1][r]!='\\')r--;
	strncpy(curdir,cmdArgv[1],r);
	chdir(curdir);
    }
#endif


    /* starting interpreter */
    r= interpret(cmdArgc, cmdArgv);

    /* terminate SDLengine */
    terminate();
    return r;
}
