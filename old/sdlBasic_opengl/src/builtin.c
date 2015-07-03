/*
    Name:       builtin.c
    Purpose:    builtin routines for wxBasic modify for sdlBasic
    Author:     David Cuny    modify __vroby__
    Copyright:  (c) 2001 David Cuny <dcuny@lanset.com>
    Licence:    LGPL
*/

//_______________________________________________________________________________________________________________________
void notYet( char *name )
{
    ePrintf( Runtime, "%s is not implemented yet", name );
}
//_______________________________________________________________________________________________________________________

/* basPrintStack: debugging wrapper of printStack */
void basPrintStack()
{
    printStack();
    pushNumber( 0 );
}

/* basConcat: concatenate two strings */
void basConcat()
{
    int     len1, len2;
    char    *string1, *string2, *buffer;

    string2 = popString();
    string1 = popString();

    len1 = strlen( string1 ),
    len2 = strlen( string2 );

    buffer = (char *)eMalloc( len1 + len2 + 1 );

    memcpy( buffer, string1, len1 );
    memcpy( buffer+len1, string2, len2 );
    buffer[len1+len2] = '\0';
    pushString( buffer );
}
//_______________________________________________________________________________________________________________________

/* basConvBas: returns representation from base to base */
void basConvBase()
{
    notYet("ConvBase");
    pushNumber(0);
}
//_______________________________________________________________________________________________________________________

void basIndexes()
{
    int     indexes;

    /* array is on stack */
    indexes = getArrayIndexes();
    popNumber();

    pushNumber(indexes);
}
//_______________________________________________________________________________________________________________________



//_________________________________________________________________________________________________________________________
//
// Run time
//_________________________________________________________________________________________________________________________

/* basArgc: return number of args in caller */
void basArgc()
{
    /* arg count is at localstart */
    //copyStackItem( localstart );
    pushNumber(cmdArgc);
}
//_______________________________________________________________________________________________________________________

/* basArgv: copy arg onto stack */
void basArgv()
{
    int     index;
    index = (int)popNumber();
    pushStringCopy( cmdArgv[index]);
}
//_______________________________________________________________________________________________________________________

/* basCommand: returns requested arg from command line */
void basCommand()
{
    int     argNum;

    /* was an arg passed? */
    if (argCount == 0) {
        /* return count of args */
        pushNumber( cmdArgc );

    } else {

        /* get arg number */
        argNum = popNumber();
        if (argNum < 0 || argNum > cmdArgc) {
            /* out of range */
            pushStringCopy( "" );

        } else {
            /* return copy of arg */
            pushStringCopy( cmdArgv[ argNum-1 ] );
        }
    }
}
//_______________________________________________________________________________________________________________________

/* basRun: run command on new thread */
void basRun()
{
    notYet("Run");
    pushNumber(0);
}
//_______________________________________________________________________________________________________________________

/* basShell: execute command, wait until finished */
void basShell()
{
    char    *string;
    string = popString();
    system(string);
    eFree(string);
    pushNumber(0);
}
//_______________________________________________________________________________________________________________________
//
// Array
//_________________________________________________________________________________________________________________________

/* basLBound: returns lower bound of array */
void basLBound()
{
    int     index, range;

    index = popNumber();
    /* leave array pointer on stack */

    range = getArrayBound( index, 0 );

    /* pop array pointer */
    popNumber();

    pushNumber(range);
}
//_______________________________________________________________________________________________________________________

/* basQuickSort: sort an array */
void basQuickSort()
{
    notYet("QuickSort");
    pushNumber(0);
}
//_______________________________________________________________________________________________________________________

/* basUBound: upperbound of array */
void basUBound()
{
    int     index, range;

    index = popNumber();
    /* leave array pointer on stack */

    range = getArrayBound( index, 1 );

    /* pop array pointer */
    popNumber();

    pushNumber(range);
}
//_______________________________________________________________________________________________________________________
//
//Strings
//_________________________________________________________________________________________________________________________

/* basAsc: returns ascii value of first char in string */
void basAsc()
{
    char    *string;
    string = popString();
    pushNumber( string[0] );
    eFree( string );
}
//_______________________________________________________________________________________________________________________

/* basChr: return string corresponding to ascii key value */
void basChr()
{
    char    *buffer = (char *)eMalloc(sizeof(char)*2);
    buffer[0] = (char)popNumber();
    buffer[1] = '\0';
    pushString( buffer );
}
//_______________________________________________________________________________________________________________________

/* basFormat: returns formatted string with embedded args */
void basFormat()
{
    notYet("Format");
    pushNumber(0);
}
//_______________________________________________________________________________________________________________________

/* basInsert: insert source string to target at index */
void basInsert()
{
    char    *source, *target,*dest;
    int     i;

    i = (int)popNumber();
    target = popString();
    source = popString();

	dest = (char *) eMalloc(strlen(source)+strlen(target)+1);

	//strcpy(dest,"");
	strncpy(dest,source,i);
	strcpy(&dest[i],target);
	strcat(dest,source+i);
	pushString(dest);

	eFree(dest);
    eFree(source);
    eFree(target);
}
//_______________________________________________________________________________________________________________________

/* basInstr: returns position in source that target was found */
void basInstr()
{
    int     start, searchLen, findLen, i;
    char    *find, *search;

    find = popString();
    search = popString();
    if (argCount==3) {
        start = (int)popNumber()-1;
        if (start < 0) {
            start = 0;
        }

    } else {
        start = 0;
    }

    searchLen = strlen( search );
    findLen = strlen( find );

    /* searching for empty string? */
    if (searchLen == 0 || findLen == 0) {
        pushNumber( 0 );
        return;
    }

    start = (int)floor( start );
    for ( i = start; i < searchLen; i++ ) {
        if (strncmp( search+i, find, findLen ) == 0) {
            pushNumber( i+1 );
            return;
        }
    }

    /* not found */
    pushNumber( 0 );
}
//_______________________________________________________________________________________________________________________

/* basLCase: convert string to lower case */
void basLCase()
{
    int     len, i;
    char    *string;

    string = popString();
    len = strlen( string );
    for ( i = 0; i<len; i++){
        string[i] = (char)tolower( (int)string[i] );
    }
    pushString( string );
}
//_______________________________________________________________________________________________________________________

/* basLeft: returns leftmost chars in string */
void basLeft()
{
    int     pos, len, newLen;
    char    *string, *left;

    pos = (int)popNumber();
    string = popString();
    len = strlen(string);

    if (pos > len-1) {
        pushString( string );
        return;
    }

    if (pos < 1) {
        eFree( string );
        pushString( "" );
        return;
    }

    newLen = (pos);
    left = (char *) eMalloc(newLen+1);
    for (pos=newLen;pos>0;pos--)left[pos]='\0';
    //memcpy(left,string,newLen);
    strncpy(left,string,newLen);
    left[newLen] = '\0';
    eFree( string );
    pushString( left );
    eFree(left);
}
//_______________________________________________________________________________________________________________________

/* basLen: return length of string */
void basLen()
{
    char    *string;
    string = popString();
    pushNumber( strlen( string ) );
    eFree( string );
}
//_______________________________________________________________________________________________________________________

/* basLTrim: returns string with left whitespace removed */
void basLTrim()
{
    char    *string, *trimmed;

    string = popString();
    trimmed = string;
    while ( trimmed[0] != '\0' ) {
        if (!isspace(trimmed[0])) {
            break;
        }
        trimmed++;
    }
    pushStringCopy( trimmed );
    eFree( string );
}
//_______________________________________________________________________________________________________________________

/* basMid: returns string with chars 1..n from source */
void basMid()
{
    int     i, j, n, len;
    char    *string, *buffer;

    /* get args */
	if (argCount==3)
		n = (int)popNumber();

    i = (int)popNumber();
    string = popString();

	if (argCount==2)
		n=strlen(string)-i;

    /* early outs */
    len = strlen( string ) - i;
    if ( i < 1 || len < 0 ) {
        pushStringCopy("");
        return;
    }

    buffer = (char *)eMalloc(len+2);
    for ( j = 0; j < n; j++ ) {
        if (string[i+j-1] == '\0' ) {
            break;
        }
        buffer[j] = string[i+j-1];
    }
    buffer[j] = '\0';
    pushStringCopy( buffer );
    eFree( string );
}
//_______________________________________________________________________________________________________________________

/* basReplace: replace string from source with replace starting at index */
void basReplace()
{
    int        i;
    char    *src, *rep;

    rep = popString();
    src = popString();
    if (argCount == 3){
            i = (int)popNumber();
    } else {
            i = 0;
    }
	strcat(rep,src+i+strlen(rep));
	strncpy(src+i,rep,strlen(rep));
	pushString(src);

    //eFree( rep );
}
//_______________________________________________________________________________________________________________________

/* basReplaceSubStr: replace substring in source with withstring */
void basReplaceSubStr()
{
    unsigned int i;
    char    *dest, *source, *replace, *with;

    with = popString();
    replace = popString();
    source = popString();

    i=0;
    while (i<strlen(source)){

	if (strncmp(source+i,replace,strlen(replace))==0){
		dest = (char *) eMalloc(strlen(source)+strlen(with)+1);
		strcpy(dest,"");
		strncpy(dest,source,i);
		 dest[i] = '\0';
		strcat(dest,with);
		strcat(dest,source+i+strlen(replace));
		eFree(source);
		source = (char *) eMalloc(strlen(dest));
		strcpy(source,dest);
		eFree(dest);
		i+=strlen(with);
	}
	i++;
     }

    eFree( with );
    eFree( replace );

    pushString(source);
}
//_______________________________________________________________________________________________________________________

/* basReverse: reverse a string */
void basReverse()
{
    int     len, i, j;
    char    *string, *buffer;

    string = popString();
    len = strlen( string );

    buffer = (char *)eMalloc(len+1);
    j = len-1;
    for( i = 0; i < len; i++ ) {
        buffer[j--] = string[i];
    }
    buffer[len] = '\0';

    pushString(buffer);
    eFree( string );
}
//_______________________________________________________________________________________________________________________

/* basRight: returns rightmost chars in string */
void basRight()
{
    int     pos, len, newLen;
    char    *string, *right;

    pos = (int)popNumber();
    string = popString();
    len = strlen(string);

    if (pos >= len) {
        pushString( string );
        return;
    }

    if (pos < 1) {
        eFree( string );
        pushString( "" );
        return;
    }

    newLen = (pos)+1;
    right = (char *)eMalloc((len-newLen)+1);
    memcpy(right,string+(len-pos),newLen);
    pushString( right );
    eFree( string );
}
//_______________________________________________________________________________________________________________________

/* basRInstr: reverse Instr function, search from end to start */
void basRInstr()
{
    int     start, searchLen, findLen, i;
    char    *find, *search;

    find = popString();
    search = popString();

    findLen = strlen( find );
    searchLen = strlen( search );

    /* optional starting position */
    if (argCount==3) {
        start = (int)popNumber()-1;
        if (start > searchLen ) {
            start = searchLen-1;
        }
    } else {
        start = searchLen-1;
    }

    /* searching for empty string? */
    if (searchLen == 0 || findLen == 0) {
        pushNumber( 0 );
        return;
    }


    /* search */
    for ( i = start; i >= 0; i-- ) {
        if (strncmp( search+i, find, findLen ) == 0) {
            pushNumber( i+1 );
            return;
        }
    }

    /* not found */
    pushNumber( 0 );
}
//_______________________________________________________________________________________________________________________

/* basRTrim: right trim string */
void basRTrim()
{
    int     len, i;
    char    *string;
    string = popString();
    len = strlen(string);
    for ( i = len-1; i > -1; i-- ) {
        if (!isspace(string[i])) {
            string[i+1] = '\0';
            pushString( string );
            return;
        }
    }
    string[0] = '\0';
    pushString( string );
}
//_______________________________________________________________________________________________________________________

/* basSpace: return string with n spaces in it */
void basSpace()
{
    char    *string;
    int     n;
    n = (int)popNumber();
    if (n<0) {
        n = 0;
    }
    string = (char *)eMalloc((unsigned int)n+1);
    memset(string,' ',n);
    string[n] = '\0';
    pushString(string);
}
//_______________________________________________________________________________________________________________________

/* basStr: return string representation of numeric expression */
void basStr()
{
    char    *string;
    string = popString();
    pushString( string );
}
//_______________________________________________________________________________________________________________________

/* basStrF: converts a floating point or number value to a string */
void basStrF()
{
    char    *string;
    string = popString();
    pushString( string );
}
//_______________________________________________________________________________________________________________________

/* basString: returns string m chars wide with n in it */
void basString()
{
    int     m, n;
    char    *string, *buffer;

    if (stack[tos].datatype == DATA_STRING) {
        string = popString();
        m = (int)popNumber();
        n = string[0];
        eFree( string );
    } else {
        n = (char)popNumber();
        m = (int)popNumber();
    }
    buffer = (char *)eMalloc(m+1);
    memset(buffer, n, m);
    buffer[m] = '\0';
    pushString( buffer );
}
//_______________________________________________________________________________________________________________________

/* basTrim: returns string with left and right whitespace removed */
void basTrim()
{
      int     len, i;
      char    *string, *trimmed;

    string = popString();
    len = strlen(string);
    for ( i = len-1; i > -1; i-- ) {
        if (!isspace(string[i])) {
            string[i+1] = '\0';
            //pushString( string );
            break;
        }
    }

    if (i==0)
	    string[0] = '\0';

    trimmed=string;
    while ( trimmed[0] != '\0' ) {
        if (!isspace(trimmed[0])) {
            break;
        }
        trimmed++;
    }

    pushStringCopy( trimmed );
    eFree( string );
}
//_______________________________________________________________________________________________________________________

/* basTally:  returns number of occurances of matchstring */
void basTally()
{
    int		i,start,ret;
    char    *src, *sub;

    /* get the args */
    sub = popString();
    src = popString();

    /* optional start */
    if (argCount==3) {
        start = popNumber();
	}
    else {
	start = 0;
    }
    ret=0;
    for(i=start;i<strlen(src);i++)
        if (strncmp(src+i,sub,strlen(sub))==0)
            ret++;

	eFree(src);
	eFree(sub);
    pushNumber(ret);

}
//_______________________________________________________________________________________________________________________

/* basTypeOf:  returns string with datatype */
void basTypeOf()
{
    int         datatype;
    Variant     *v;
    v = getStackPointer( tos );
    datatype = v->datatype;
    dropTos();
    switch(datatype) {
    case DATA_NUMBER:
        pushStringCopy( "number" );
        break;

    case DATA_STRING:
        pushStringCopy( "string" );
        break;

    case DATA_OBJECT:
        pushStringCopy( "object" );
        break;

    default:
        pushStringCopy( "unknown" );
        break;
    }
}
//_______________________________________________________________________________________________________________________

/* basUCase: convert string to upper case */
void basUCase()
{
    int     len, i;
    char    *string;

    string = popString();
    len = strlen( string );
    for ( i = 0; i<len; i++){
        string[i] = toupper( string[i] );
    }
    pushString( string );
}
//_______________________________________________________________________________________________________________________

/* basVal: returns closest numeric representation of number */
void basVal()
{
    char    *string;
    string = popString();
    pushNumber( atof(string) );
    eFree( string );
}
//_______________________________________________________________________________________________________________________
//
//Maths
//_________________________________________________________________________________________________________________________

/* basAbs: returns absolute value of number */
void basAbs()
{
    Number  n;
    n = popNumber();
    if (n < 0) {
        pushNumber( -n );
    } else {
        pushNumber( n );
    }
}
//_______________________________________________________________________________________________________________________

/* basACos: returns arccos of number */
void basACos()
{
    pushNumber( acos( popNumber() ) );
}
//_______________________________________________________________________________________________________________________

/* basASin: returns arcsin of number */
void basASin()
{
    pushNumber( asin( popNumber() ) );
}
//_______________________________________________________________________________________________________________________

/* basATan: returns arctan of number */
void basATan()
{
    pushNumber( atan( popNumber() ) );
}
//_______________________________________________________________________________________________________________________


/* basCos: returns cos of number */
void basCos()
{
    pushNumber( cos( popNumber() ) );
}
//_______________________________________________________________________________________________________________________

/* basExp: returns exponential function */
void basExp()
{
    pushNumber( exp( popNumber() ) );
}
//_______________________________________________________________________________________________________________________

/* basFix: truncate fractional number, rounding down towards zero */
void basFix()
{
    pushNumber( floor( popNumber() ) );
}
//_______________________________________________________________________________________________________________________

/* basFrac: return fractional portion of number */
void basFrac()
{
    Number  n;
    n = popNumber();
    n -= floor(n);
    pushNumber(n);
}
//_______________________________________________________________________________________________________________________

/* basHex: returns hexidecimal representation of number */
void basHex()
{
    int i;
    char    *buffer = (char *)eMalloc(16);
    i = (int)popNumber();
    sprintf( buffer, "%x", i);
    pushString( buffer );
}
//_______________________________________________________________________________________________________________________

/* basInt: convert to 32 bit integer, truncating decimals */
void basInt()
{
    pushNumber( (int)popNumber() );
}
//_______________________________________________________________________________________________________________________

/* basLog: returns natural log of expression */
void basLog()
{
    pushNumber( log( popNumber() ) );
}
//_______________________________________________________________________________________________________________________

/* basRandomize: reseed the random number generator */
void basRandomize()
{
    int seed;
    if (argCount==1) {
        seed = (int)popNumber();
    } else {
        seed = time(NULL);
    }
    srand( seed );
    pushNumber(0);
}
//_______________________________________________________________________________________________________________________

/* basRnd: return an random number */
void basRnd()
{
    int upper, result;

    result = rand();
    if (argCount==1) {
        upper = (int)popNumber();
        result = (result % (upper-1))+1;
    }
    pushNumber( (Number)result );
}
//_______________________________________________________________________________________________________________________

/* basRound: round to nearest integer */
void basRound()
{
    Number n1, n2;
    n1 = popNumber();
    n2 = floor(n1);
    if (n2 == 0) {
        /* leave it alone */

    } else if (n2 > 0) {
        if ((n1 - n2) > 0.5) {
            n2++;
        }
    } else {
        if (-(n2 - n1) > 0.5) {
            n2++;
        }
    }
    pushNumber( n2 );
}
//_______________________________________________________________________________________________________________________

/* basSgn: returns sign of numeric expression */
void basSgn()
{
    Number  n;
    n = popNumber();
    if (n < 0) {
        pushNumber(-1);
    } else if (n > 0) {
        pushNumber(1);
    } else {
        pushNumber(0);
    }
}
//_______________________________________________________________________________________________________________________

/* basSin: returns sine of given angle in radians */
void basSin()
{
    pushNumber( sin( popNumber() ) );
}
//_______________________________________________________________________________________________________________________

/* basSqr: return square root - make sure it's non-negative */
void basSqr()
{
    pushNumber( sqrt( popNumber() ) );
}
//_______________________________________________________________________________________________________________________

/* basTan: return tanget of given angle in radians */
void basTan()
{
    pushNumber( tan( popNumber() ) );
}
//_______________________________________________________________________________________________________________________
//
//Files
//_________________________________________________________________________________________________________________________

/* basEof: returns nonzero if Eof */
void basEof()
{
    pushNumber( (Number)fileEof( (int)popNumber() ) );
}
//_______________________________________________________________________________________________________________________

/* basFileExists: returns true if the file exists */
void basFileExists()
{
    char    *fname;

    fname = popString();
    pushNumber(FileExists(fname));
}
//_______________________________________________________________________________________________________________________

/* basFileCopy : copy a file */
void basFileCopy()
{
char *source;
char *dest;

dest=popString();
source=popString();
pushNumber(filecopy(source,dest));


}
//_______________________________________________________________________________________________________________________

/* basFileMove:move a file */
void basFileMove()
{
char *source;
char *dest;

dest=popString();
source=popString();
pushNumber(filemove(source,dest));
}
//_______________________________________________________________________________________________________________________

/* basFileRename: move a file */
void basFileRename()
{
char *source;
char *dest;

dest=popString();
source=popString();
pushNumber(filerename(source,dest));
}
//_______________________________________________________________________________________________________________________

/* basFreeFile: return the handle of the next free file */
void basFreeFile()
{
    pushNumber(fileFree());
}
//_______________________________________________________________________________________________________________________

/* basKill: delete filename */
void basKill()
{
    char    *name = popString();
    remove( name );
    eFree(name);
    pushNumber(0);
}
//_______________________________________________________________________________________________________________________

/* basLoc: returns position in file */
void basLoc()
{
    pushNumber( filePosition( popNumber() ) );
}
//_______________________________________________________________________________________________________________________

/* basLof: returns length of file */
void basLof()
{
    pushNumber( fileSize( popNumber() ) );
}
//_______________________________________________________________________________________________________________________

/* basReadByte: return a byte from the file */
void basReadByte()
{
    pushNumber( (Number)fileReadByte( (char)popNumber() ) );
}
//_______________________________________________________________________________________________________________________

/* basRename: rename file */
void basRename()
{
    char    *oldName, *newName;
    newName = popString();
    oldName = popString();
    rename( oldName, newName );
    eFree( oldName );
    eFree( newName );
    pushNumber(0);
}
//_______________________________________________________________________________________________________________________

/* basSeek: seek file position/return current file position */
void basSeek()
{
    int     file, pos;

    /* which version? */
    if (argCount==2) {
        /* seek to position */
        pos = popNumber();
        file = popNumber();
        pushNumber( fileSeek( file, pos ));

    } else {
        /* return position in file */
        file = popNumber();
        pushNumber( filePosition( file ) );
    }
}
//_______________________________________________________________________________________________________________________


/* basWriteByte: return a byte from the file */
void basWriteByte()
{
    char    c;
    int     fileNum;

    c = popNumber();
    fileNum = popNumber();
    fileWriteByte( fileNum, c );
    pushNumber( 0 );
}

//_______________________________________________________________________________________________________________________
//
//Dirs
//_________________________________________________________________________________________________________________________

/* basChDir: change directory */
void basChDir()
{
    char    *name;
    name = popString();
	pushNumber(chdir(name));
}
//_______________________________________________________________________________________________________________________

/* basDir: returns current path */
void basDir()
{
	pushString(dir());
}
//_______________________________________________________________________________________________________________________

/* basDirExists: returns true if directory exists */
void basDirExists()
{
	char *path;
	path=popString();
	pushNumber(direxists(path));
}
//_______________________________________________________________________________________________________________________

/* basDirFirst: returns first entry in path dir */
void basDirFirst()
{
	char *path;
	path = popString();
	pushString(dirfirst(path));
}
//_______________________________________________________________________________________________________________________

/* basDirNext: returns next entry in  path dir */
void basDirNext()
{
	pushString(dirnext());
}
//_______________________________________________________________________________________________________________________

/* basMkDir: create a new directory */
void basMkDir()
{
    char    *name;
    name = popString();
	pushNumber(mkdir(name));
}
//_______________________________________________________________________________________________________________________

/* basRmDir: remove directory */
void basRmDir()
{
    char    *name;
    name = popString();
	pushNumber(rmdir(name));
}
//_______________________________________________________________________________________________________________________
//Date and time
//_________________________________________________________________________________________________________________________
/* basDate: returns date in MM-DD-YYYY format */
void basDate()
{
    time_t      t;
    struct tm   *tmp;
    char        *buffer = (char *)eMalloc(11);

    /* get time */
    t = time(NULL);
    tmp = localtime(&t);

    sprintf( buffer, "%02d-%02d-%04d",
        tmp->tm_mon+1, tmp->tm_mday, tmp->tm_year+1900 );

    pushString( buffer );
}
//_______________________________________________________________________________________________________________________

/* basTicks: returns current timer value */
void basTicks()
{
#if unix
    //fixed using mandel demo
    pushNumber( clock()/826 );
#else
    pushNumber( clock() );
#endif
}
//_______________________________________________________________________________________________________________________

/* basTime: return time in HH:MM:SS format */
void basTime()
{
    time_t      t;
    struct tm   *tmp;
    char        *buffer = (char *)eMalloc(9);

    /* get time */
    t = time(NULL);
    tmp = localtime(&t);

    sprintf( buffer, "%02d:%02d:%02d",
        tmp->tm_hour, tmp->tm_min, tmp->tm_sec );

    pushString( buffer );
}
//_______________________________________________________________________________________________________________________




//_______________________________________________________________________________________________________________________
//
//------- SDLengine-link ------------------------------------------------------------------------------------------
//_______________________________________________________________________________________________________________________

//_________________________________________________________________________________________________________________________
//
//Screen
//_________________________________________________________________________________________________________________________

//setdisplay(w,h,bpp,m): open the screen/window in double buffer. 0=screen|1=window|2=resizable
void basSetDisplay()
{
    int width,height,bpp,mode;
    int ret;

    mode = popNumber();
    bpp = popNumber();
    height = popNumber();
    width = popNumber();

    ret=setdisplay( width,height,bpp,mode);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);

}
//_______________________________________________________________________________________________________________________

//displaywidth: give the display width
void basDisplayWidth()
{
    pushNumber( displaywidth() );
}
//_______________________________________________________________________________________________________________________

//displayheight: give the display height
void basDisplayHeight()
{
    pushNumber( displayheight() );
}
//_______________________________________________________________________________________________________________________

//displaybpp: give the display color depth
void basDisplayBpp()
{
    pushNumber( displaybpp() );
}
//_______________________________________________________________________________________________________________________

//screen(n): attivated the logic screen n
void basScreen()
{
    int n;
    int ret;
    if (argCount!=0)
	    n = popNumber();
    else
	    n=-1;
    ret=screen(n);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);
}
//_______________________________________________________________________________________________________________________

//directscreen: direct drawing on display like a screen(more fast but sprite and offset does not works)
void basDirectScreen()
{
    int ret;
    ret=directscreen();
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);
}
//_______________________________________________________________________________________________________________________

//screenopen(n,w,h,dx,dy,dw,dh,flag): open the logic screen n of dimension w,h in display coordinates dx,dy,dw,dh
void basScreenOpen()
{
    int n,w,h,dx,dy,dw,dh,flag;
    int ret;

    flag = popNumber();
    dh = popNumber();
    dw = popNumber();
    dy = popNumber();
    dx = popNumber();
     h = popNumber();
     w = popNumber();
     n = popNumber();

    ret=screenopen(n,w,h,dx,dy,dw,dh,flag);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);
}
//_______________________________________________________________________________________________________________________

//screenclose(n): close the logic screen n
void basScreenClose()
{
    int n;
    int ret;

    n = popNumber();
    ret=screenclose(n);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);

}
//_______________________________________________________________________________________________________________________

//screenclone(n,s,x,y,w,h,flag): create a  new viewport in logic screen s
void basScreenClone()
{
    int n,s,x,y,w,h,flag;

    flag = popNumber();
    h = popNumber();
    w = popNumber();
    y = popNumber();
    x = popNumber();
    s = popNumber();
    n = popNumber();

    pushNumber(screenclone(n,s,x,y,w,h,flag));
}
//_______________________________________________________________________________________________________________________

//screencopy(n,x,y,w,h,nd,xd,yd): copy a portion of screen n in a screen nd
void basScreenCopy()
{
    int n,x,y,w,h,nd,xd,yd;
    int ret;

    yd = popNumber();
    xd = popNumber();
    nd = popNumber();
    h = popNumber();
    w = popNumber();
    y = popNumber();
    x = popNumber();
    n = popNumber();

    ret=screencopy(n,x,y,w,h,nd,xd,yd);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);

}
//_______________________________________________________________________________________________________________________

//screenfade(n,t): fade the screen n in t time
void basScreenFade()
{
    int n,t;
    int ret;

    t = popNumber();
    n = popNumber();

    ret=screenfade(n,t);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);

}
//_______________________________________________________________________________________________________________________

//screenfadein(n,i,t): fade the screen n to image i in t time
void basScreenFadein()
{
    int n,i,t;
    int ret;

    t = popNumber();
    i = popNumber();
    n = popNumber();

    ret=screenfadein(n,i,t);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);

}
//_______________________________________________________________________________________________________________________

//screencrossfade(n,i,t): fade the screen n from current screen  to image i in t time
void basScreenCrossFade()
{
    int n,i,t;
    int ret;

    t = popNumber();
    i = popNumber();
    n = popNumber();

    ret=screencrossfade(n,i,t);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);

}
//_______________________________________________________________________________________________________________________

//screenalpha(n,a): set alpha(trasparency) of screen n
void basScreenAlpha()
{
	int ret;
	int n,a;
    a = popNumber();
    n = popNumber();
    ret=screenalpha(n,a);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);
}
//_______________________________________________________________________________________________________________________

//screenlock(n): lock the  screen n for direct graphics access
void basScreenLock()
{
	int ret;
	int n;
     n = popNumber();
    ret=screenlock(n);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);
}
//_______________________________________________________________________________________________________________________

//screenunlock(n): unlock the  screen n for direct graphics access
void basScreenUnlock()
{
	int ret;
	int n;
     n = popNumber();
    ret=screenunlock(n);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);
}
//_______________________________________________________________________________________________________________________

//screenrect(x,y,w,h,flag): change the display output coordinates of the current screen
void basScreenRect()
{
    int x,y,w,h,flag;

    flag = popNumber();
    h = popNumber();
    w = popNumber();
    y = popNumber();
    x = popNumber();

    pushNumber(screenrect(x,y,w,h,flag));
}
//_______________________________________________________________________________________________________________________

//xscreenrect: give the x coordinate of current screen viewport
void basXScreenRect()
{
    pushNumber( xscreenrect() );
}
//_______________________________________________________________________________________________________________________

//yscreenrect: give the y coordinate of current screen viewport
void basYScreenRect()
{
    pushNumber( yscreenrect() );
}
//_______________________________________________________________________________________________________________________

//wscreenrect: give the w value of current screen viewport
void basWScreenRect()
{
    pushNumber( wscreenrect() );
}
//_______________________________________________________________________________________________________________________

//hscreenrect: give the h value of current screen viewport
void basHScreenRect()
{
    pushNumber( hscreenrect() );
}
//_______________________________________________________________________________________________________________________

//flagscreenrect: give the flag value of current screen viewport
void basFlagScreenRect()
{
    pushNumber( flagscreenrect() );
}
//_______________________________________________________________________________________________________________________

//screenwidth: give the current screen width
void basScreenWidth()
{
    pushNumber( screenwidth() );
}
//_______________________________________________________________________________________________________________________

//screenheight: give the current screen height
void basScreenHeight()
{
    pushNumber( screenheight() );
}
//_______________________________________________________________________________________________________________________

//offset(x,y): set the logical current screen position
void basOffset()
{
    int xoff,yoff;

    yoff = popNumber();
    xoff = popNumber();
    offset(xoff,yoff);

    pushNumber( 0 );
}
//_______________________________________________________________________________________________________________________

//xoffset: give the x coordinate offset in current screen
void basXOffset()
{
    pushNumber( xoffset() );
}
//_______________________________________________________________________________________________________________________

//yoffset: give the y coordinate offset in current screen
void basYOffset()
{
    pushNumber( yoffset() );
}
//_______________________________________________________________________________________________________________________

//cls: clear the current logic screen
void basCls()
{
    pushNumber( cls() );
}
//_______________________________________________________________________________________________________________________

//screenswap: update display,bobsystem,spritesystem
void basScreenSwap()
{
    int ret;
    ret= screenswap();
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);

}
//_______________________________________________________________________________________________________________________

//autoback setautoback(m): enable / disable automatic screenswap  m=0 disable m>0 wait m milliseconds and perform screenswap m<0 perform the invocate in code screenswap after m milliseconds
void basAutoBack()
{
    int m;
    if (argCount==1)
	    m=popNumber();
    else
	    m=-1;
    pushNumber(autoback(m));
}
//_______________________________________________________________________________________________________________________

//dualplayfield(m): set/unset automatic update of a screen upper sprite
void basDualPlayfield()
{
    int m;
	if (argCount==1)
		m=popNumber();
	else
		m=-1;

    pushNumber(dualplayfield(m));
}
//_______________________________________________________________________________________________________________________

//waitvbl: wait automatic screenswap
void basWaitVbl()
{
    pushNumber(waitvbl());
}
//_______________________________________________________________________________________________________________________

//fps(n): set/unset or give the current frame rate counter
void basFps()
{
    int n;
	if (argCount==1)
		n=popNumber();
	else
		n=-1;

    pushNumber(fps(n));
}
//_______________________________________________________________________________________________________________________

//_________________________________________________________________________________________________________________________
//
//Graphics
//_________________________________________________________________________________________________________________________

//rgb(r,g,b): give current color in Uint32 format
void basRgb()
{
	int r,g,b;
    b = popNumber();
    g = popNumber();
    r = popNumber();

    pushNumber( rgb(r,g,b) );
}
//_______________________________________________________________________________________________________________________

//color (c,optional v): set  palette color c with value v se missing give the current color c
void basColor()
{
	int c,v;
	int ret;
    if (argCount==2)
	v=popNumber();
    else
	v=-1;
    c=popNumber();

    ret=color(c,v);

    if (ret==-1) ePrintf( Runtime, "%s color Error\n", c );
    pushNumber(ret );
}
//_______________________________________________________________________________________________________________________

//palette(optional 0-255 param): set all palettes color
void basPalette()
{
	int i,v;
	int ret;

    for (i=0;i<argCount;i++){
	v=popNumber();
	ret=color(i,v);
	if (ret==-1)break;
    }
    pushNumber(ret );
}
//_______________________________________________________________________________________________________________________

//ink(c): select the current color in Uint32 format
void basInk()
{
	int c;
    c=popNumber();
    pushNumber( ink(c) );
}
//_______________________________________________________________________________________________________________________

//point(x,y)				: give the color of x,y point
void basPoint()
{
	int x,y;
    y = popNumber();
    x = popNumber();

    pushNumber( point(x,y) );
}
//_______________________________________________________________________________________________________________________

//dot(x,y): write x,y point with a current color
void basDot()
{
	int x,y;
    y = popNumber();
    x = popNumber();

    pushNumber( dot(x,y) );
}
//_______________________________________________________________________________________________________________________

//plot(x,y,c): write x,y point with a c color
void basPlot()
{
	int x,y,c;
    c = popNumber();
    y = popNumber();
    x = popNumber();

    pushNumber( dot_ex(x,y,c) );
}
//_______________________________________________________________________________________________________________________

//line(x,y,x1,y1): write a line
void basLine()
{
	int x,y,x1,y1;
    y1 = popNumber();
    x1 = popNumber();
    y = popNumber();
    x = popNumber();

    pushNumber( line(x,y,x1,y1) );
}
//_______________________________________________________________________________________________________________________

//box(x,y,x1,y1): write a empty rettangle
void basBox()
{
	int x,y,x1,y1;
    y1 = popNumber();
    x1 = popNumber();
    y = popNumber();
    x = popNumber();

    pushNumber( box(x,y,x1,y1) );
}
//_______________________________________________________________________________________________________________________

//bar(x,y,x1,y1): write a fill rettangle
void basBar()
{
	int x,y,x1,y1;
    y1 = popNumber();
    x1 = popNumber();
    y = popNumber();
    x = popNumber();

    pushNumber( bar(x,y,x1,y1) );
}
//_______________________________________________________________________________________________________________________

//circle(x,y,r): write a circle
void basCircle()
{
	int x,y,r;
    r = popNumber();
    y = popNumber();
    x = popNumber();

    pushNumber( circle(x,y,r) );
}
//_______________________________________________________________________________________________________________________

//fillcircle(x,y,r): write a fill circle
void basFillCircle()
{
	int x,y,r;
    r = popNumber();
    y = popNumber();
    x = popNumber();

    pushNumber( fillcircle(x,y,r) );
}
//_______________________________________________________________________________________________________________________

//ellipse(x,y,rx,ry): write a ellipse
void basEllipse()
{
	int x,y,rx,ry;
    ry = popNumber();
    rx= popNumber();
    y = popNumber();
    x = popNumber();

    pushNumber( ellipse(x,y,rx,ry) );
}
//_______________________________________________________________________________________________________________________

//fillellipse(x,y,rx,ry): write a fill ellipse
void basFillEllipse()
{
	int x,y,rx,ry;
    ry = popNumber();
    rx = popNumber();
    y = popNumber();
    x = popNumber();

    pushNumber( fillellipse(x,y,rx,ry) );
}
//_______________________________________________________________________________________________________________________

//paint(x,y): fill a close area
void basPaint()
{
	int x,y;
    y = popNumber();
    x = popNumber();

    pushNumber(paint(x,y) );
}

//_______________________________________________________________________________________________________________________
//
//graphics input output
//_________________________________________________________________________________________________________________________

//loadimage(filename,n): load a graphics file in a slot n
void basLoadImage()
{
	char	*filename;
	int n;

	int ret;

    n = popNumber();
    filename=popString();
    ret =loadimage(filename,n);
	eFree(filename);
    if (ret==-1) ePrintf( Runtime, "%s not found Error\n", filename );
    pushNumber(ret);

}
//_______________________________________________________________________________________________________________________

//saveimage(filename,n): save slot n in a graphics file(only bmp)
void basSaveImage()
{
	char	*filename;
	int n;

	int ret;

    n = popNumber();
    filename=popString();

    ret= saveimage(filename,n);
	eFree(filename);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);

}
//_______________________________________________________________________________________________________________________

//loadsound(filename,n): load a wave file in a sound slot n
void basLoadSound()
{
	char	*filename;
	int n;

	int ret;

    n = popNumber();
    filename=popString();

    ret=loadsound(filename,n);
	eFree(filename);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);

}
//_______________________________________________________________________________________________________________________

//soundexists(n): give 1 if the sound buffer n exist 0 if empty
void basSoundExists()
{
	int n;
    n = popNumber();
    pushNumber( soundexists(n) );

}
//_______________________________________________________________________________________________________________________

//loadmusic(filename): load a music module (xm,mod,ogg and only for linux mp3)
void basLoadMusic()
{
	char	*filename;

	int ret;
    filename=popString();

    ret=loadmusic(filename);
	eFree(filename);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);


}
//_______________________________________________________________________________________________________________________

//musicexists(): give 1 if the music is load 0 if empty
void basMusicExists()
{
    pushNumber( musicexists() );
}
//_______________________________________________________________________________________________________________________
//
// Image manipulation
//_________________________________________________________________________________________________________________________

//hotspot(n,x,y): select the point of coordinates in a imageslot (n,0,0=up left(default) | n,1,1=center | n,2,2 =down right)n= image
void basHotSpot()
{
	int n,x,y;
	int ret;

    y=popNumber();
    x=popNumber();
    n=popNumber();


    ret=hotspot(n,x,y);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);

}
//_______________________________________________________________________________________________________________________

//imageexists(n): give 1 if the image buffer n exist 0 if empty
void basImageExists()
{
	int n;
    n = popNumber();
    pushNumber( imageexists(n) );

}
//_______________________________________________________________________________________________________________________

//imagewidth(n): give the image n width or error if image n if empty
void basImageWidth()
{
	int n;
    n = popNumber();
    pushNumber( imagewidth(n) );

}
//_______________________________________________________________________________________________________________________

//imageheight(n): give the image n height or error if image n if empty
void basImageHeight()
{
	int n;
    n = popNumber();
    pushNumber( imageheight(n) );

}
//_______________________________________________________________________________________________________________________

//deleteimage(n): erase the image n of the memory
void basDeleteImage()
{
	int ret;
	int n;
    n = popNumber();
    ret=deleteimage(n);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);
}
//_______________________________________________________________________________________________________________________

//copyimage(s,d): copy image s to image d
void basCopyImage()
{
	int ret;
	int s,d;
    d = popNumber();
    s = popNumber();
    ret=copyimage(s,d);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);
}
//_______________________________________________________________________________________________________________________

//setalpha(n,a): set trasparency
void basSetAlpha()
{
	int ret;
	int n,a;
    a = popNumber();
    n = popNumber();
    ret=setalpha(n,a);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);
}
//_______________________________________________________________________________________________________________________

//zoomimage(n,zoomx,zoomy): zoom image
void basZoomImage()
{
	int ret;
	int n;
	double zoomx,zoomy;
    zoomy = popNumber();
    zoomx = popNumber();
    n = popNumber();
    ret=zoomimage(n,zoomx,zoomy);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);
}
//_______________________________________________________________________________________________________________________

//rotateimage(n,angle): rotate image
void basRotateImage()
{
	int ret;
	int n;
	double angle;

    angle = popNumber();
    n = popNumber();
    ret=rotateimage(n,angle);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);
}
//_______________________________________________________________________________________________________________________

//rotozoomimage(n,angle,zoom): rotate and zoom image
void basRotozoomImage()
{
	int ret;
	int n;
	double angle,zoom;

    zoom = popNumber();
    angle = popNumber();
    n = popNumber();
    ret=rotozoomimage(n,angle,zoom);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);
}
//_______________________________________________________________________________________________________________________
//
// Blitting
//_________________________________________________________________________________________________________________________

//blt(n,sx,sy,sw,sh,dx,dy): copy a part of graphics slot in screen
void basBlt()
{
    int n,sx,sy,sw,sh,dx,dy;
    int ret;

    dy = popNumber();
    dx = popNumber();
    sh = popNumber();
    sw = popNumber();
    sy = popNumber();
    sx = popNumber();
    n = popNumber();

    ret=blt(n,sx,sy,sw,sh,dx,dy);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);

}
//_______________________________________________________________________________________________________________________

//pastebob(x,y,n): copy on screen image n at x,y performing clip
void basPasteBob()
{
	int x,y,n;
        int ret;

    n = popNumber();
    y = popNumber();
    x = popNumber();

    ret=pastebob(x,y,n);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);

}
//_______________________________________________________________________________________________________________________

//pasteicon(x,y,n): copy on screen image n at x,y without clipping
void basPasteIcon()
{
	int x,y,n;
	int ret;

    n = popNumber();
    y = popNumber();
    x = popNumber();

    ret= pasteicon(x,y,n);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);

}
//_______________________________________________________________________________________________________________________

//grab(n,x,y,w,h): grab a a selectarea and copy it in slot n
void basGrab()
{
	int n,x,y,w,h;
	int ret;

    h = popNumber();
    w = popNumber();
    y = popNumber();
    x = popNumber();
    n = popNumber();

    ret=grab(n,x,y,w,h);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);

}
//_______________________________________________________________________________________________________________________
//
// Sprite system
//_______________________________________________________________________________________________________________________

//spriteclip(x,y,w,h): set the visibilty area of sprites
void basSpriteClip()
{
	int x,y,w,h;

    h = popNumber();
    w = popNumber();
    y = popNumber();
    x = popNumber();

    pushNumber( spriteclip(x,y,w,h) );
}
//_______________________________________________________________________________________________________________________

//sprite(n,x,y,fr): set or move the sprite n at x,y with image fr
void basSprite()
{
	int n,x,y,fr;
	int ret;

    fr = popNumber();
    y = popNumber();
    x = popNumber();
    n = popNumber();

    ret=sprite(n,x,y,fr);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);

}
//_______________________________________________________________________________________________________________________

//deletesprite(n): unset sprite n
void basDeleteSprite()
{
	int n;

    n = popNumber();

    pushNumber( deletesprite(n) );
}
//_______________________________________________________________________________________________________________________

//xsprite(n): give the x of sprite n
void basXSprite()
{
	int n;

    n = popNumber();

    pushNumber( xsprite(n) );
}
//_______________________________________________________________________________________________________________________

//ysprite(n): give the y of sprite n
void basYSprite()
{
	int n;

    n = popNumber();

    pushNumber( ysprite(n) );
}
//_______________________________________________________________________________________________________________________

//spritewidth(n): give the width of sprite n
void basSpriteWidth()
{
	int n;

    n = popNumber();

    pushNumber( spritewidth(n) );
}
//_______________________________________________________________________________________________________________________

//spriteheight(n): give the height of sprite n
void basSpriteHeight()
{
	int n;

    n = popNumber();

    pushNumber( spriteheight(n) );
}
//_______________________________________________________________________________________________________________________

//frsprite(n): give the frame of sprite n
void basFrSprite()
{
	int n;

    n = popNumber();

    pushNumber( frsprite(n) );
}
//_______________________________________________________________________________________________________________________

//livesprite(n): give 1 if sprite n is "live"
void basLiveSprite()
{
	int n;

    n = popNumber();

    pushNumber( livesprite(n) );
}
//_______________________________________________________________________________________________________________________

//spritehit(n,optional x): give 1 if sprite n have a collission with sprite x if x=-1 with any
void basSpriteHit()
{
	int n,x;
	int ret;

	if (argCount==1)
		x=-1;
	else
		x = popNumber();
    n = popNumber();

	ret=spritehit(n,x);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret  );
}
//_______________________________________________________________________________________________________________________

//autoupdatesprite(m): set/ unset automatic sprites update at screenswap
void basAutoUpdateSprite()
{
	int m;

    m = popNumber();

    pushNumber( autoupdatesprite(m) );
}
//_______________________________________________________________________________________________________________________

//updatesprite				: manual sprites updates at next screenswap
void basUpdateSprite()
{

    pushNumber( updatesprite() );
}
//_______________________________________________________________________________________________________________________
//
// Bob system
//_________________________________________________________________________________________________________________________

//setbob(n,scr): set bob n at logic screen scr
void basSetBob()
{
	int n,scr;

    scr = popNumber();
    n = popNumber();

    pushNumber( setbob(n,scr) );
}
//_______________________________________________________________________________________________________________________

//bob(n,x,y,fr): set or move bob n at x,y with frame fr
void basBob()
{
	int n,x,y,fr;
	int ret;

    fr = popNumber();
    y = popNumber();
    x = popNumber();
    n = popNumber();

    ret=bob(n,x,y,fr);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);

}
//_______________________________________________________________________________________________________________________

//deletebob(n): unset bob n
void basDeleteBob()
{
	int n;

    n = popNumber();

    pushNumber( deletebob(n) );
}
//_______________________________________________________________________________________________________________________

//xbob(n): give x of bob n
void basXBob()
{
	int n;

    n = popNumber();

    pushNumber( xbob(n) );
}
//_______________________________________________________________________________________________________________________

//ybob(n): give y of bob n
void basYBob()
{
	int n;

    n = popNumber();

    pushNumber( ybob(n) );
}
//_______________________________________________________________________________________________________________________

//bobwidth(n): give width of bob n
void basBobWidth()
{
	int n;

    n = popNumber();

    pushNumber( bobwidth(n) );
}
//_______________________________________________________________________________________________________________________

//bobheight(n): give height of bob n
void basBobHeight()
{
	int n;

    n = popNumber();

    pushNumber( bobheight(n) );
}
//_______________________________________________________________________________________________________________________

//frbob(n): give the frame of bob n
void basFrBob()
{
	int n;

    n = popNumber();

    pushNumber( frbob(n) );
}
//_______________________________________________________________________________________________________________________

//livebob(n): give 1 if bob n is "live"
void basLiveBob()
{
	int n;

    n = popNumber();

    pushNumber( livebob(n) );
}
//_______________________________________________________________________________________________________________________

//bobhit(n,optional x): give 1 if bob n have a collision with bob x if x=-1 with any
void basBobHit()
{
	int n,x;
	int ret;

	if (argCount==1)
		x=-1;
	else
		x = popNumber();
    n = popNumber();

	ret=bobhit(n,x);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret  );

}
//_______________________________________________________________________________________________________________________

//autoupdatebob(m): set/ unset automatic bobs update at screenswap
void basAutoUpdateBob()
{
	int m;

    m = popNumber();

    pushNumber( autoupdatebob(m) );
}
//_______________________________________________________________________________________________________________________

//updatebob: manual bobs updates at next screenswap
void basUpdateBob()
{

    pushNumber( updatebob() );
}

//_______________________________________________________________________________________________________________________
//
// Text draw
//_________________________________________________________________________________________________________________________

//text(x,y,s,testo): print the text testo  on current screen with s size
void basText()
{
	char	*txt;
	int x,y,s;
	int ret;

    txt=popString();
    s = popNumber();
    y = popNumber();
    x = popNumber();

    ret=text(x,y,s,txt);
	eFree(txt);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);

}
//_______________________________________________________________________________________________________________________

//setfont(path): select the font
void basSetFont()
{
	char	*path;
	int ret;

    path=popString();

    ret=setfont(path);
	//eFree(path);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);

}
//_______________________________________________________________________________________________________________________

//textrender(testo,s,n): make an image slot n with the text  write with a current font and size s
void basTextRender()
{
	char	*txt;
	int s,n;

	int ret;

    n = popNumber();
    s = popNumber();
    txt=popString();

    ret=textrender(txt,s,n);
	eFree(txt);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);

}
//_______________________________________________________________________________________________________________________
//
// Print screen
//_________________________________________________________________________________________________________________________

//pen(c)					: set prints color (without parameter give the current color)
void basPen()
{

	int c;
	int ret;

    if (argCount==1)
		c = popNumber();
	else c=-1;

    ret=pen(c);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);

}
//_______________________________________________________________________________________________________________________

//paper(c): set caractere back color (without parameter give the current color)
void basPaper()
{

	int c;
	int ret;

    if (argCount==1)
		c = popNumber();
	else c=-1;

    ret=paper(c);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);

}
//_______________________________________________________________________________________________________________________

//fprints(testo): print a text monospace without cariage return
void basfPrints()
{
	int ret;
	char *txt;

    txt = popString();
	ret=fprints(txt);
	eFree(txt);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);

}
//_______________________________________________________________________________________________________________________

//prints(testo): print a text monospace
void basPrints()
{

	int ret;
	char *txt;

    txt = popString();
	ret=prints(txt);
	eFree(txt);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);

}
//_______________________________________________________________________________________________________________________

//locate(x,y): move the cursor at x y
void basLocate()
{

	int x,y;

    y = popNumber();
    x = popNumber();

    pushNumber(locate(x,y) );

}
//_______________________________________________________________________________________________________________________

//atx: give x of cursor
void basAtx()
{
    pushNumber(atx());
}
//_______________________________________________________________________________________________________________________

//aty: give y of cursor
void basAty()
{
    pushNumber(aty());
}
//_______________________________________________________________________________________________________________________

//curson: show the text cursor on screen at(atx,aty)
void basCursOn()
{
    pushNumber(curson());
}
//_______________________________________________________________________________________________________________________

//cursoff: hide the text cursor
void basCursOff()
{
    pushNumber(cursoff());
}
//_______________________________________________________________________________________________________________________

//inputs(prompt,defs): give the string insert to keyboard(default is default text)
void basInputs()
{
	char *txt;
	char *request;
	char defs[255];
	txt=popString();
	strcpy(defs,txt);
	request=popString();
	inputs(request,defs);
	pushString(defs);
}
//_______________________________________________________________________________________________________________________

//zoneinputs(x,y,l,default): give the string insert to keyboard in  x,y coordinates with l lenght
void basZoneInputs()
{
	char *txt;
	int l,y,x;

	char defs[255];
	txt=popString();
	strcpy(defs,txt);
	l=popNumber();
	y=popNumber();
	x=popNumber();
	zoneinputs(x,y,l,defs);
	pushString(defs);
}
//_______________________________________________________________________________________________________________________
//
// Sounds and music
//_________________________________________________________________________________________________________________________

//playsound(n,c,optional l): play the sound n in  channel c l loops
void basPlaySound()
{
	int n,c,l;
	int ret;
    if (argCount==3)
	l=popNumber();
    else
	l=0;
    c = popNumber();
    n = popNumber();

    ret=playsound(n,c,l);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);

}
//_______________________________________________________________________________________________________________________

//playfreqsound(n,c,rate,optional l): play the sound n in  channel c at freq rate l loops
void basPlayFreqSound()
{
	int n,c,rate,l;
	int ret;
    if (argCount==4)
	l=popNumber();
    else
	l=0;

    rate = popNumber();
    c = popNumber();
    n = popNumber();

    ret=playfreqsound(n,c,rate,l);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);

}
//_______________________________________________________________________________________________________________________

//volumesound(c,optional v): change the volume of channel c (-1  all) at value v (0-128) without v or -1 give the current volume
void basVolumeSound()
{
	int ret;
	int c,v;

    if (argCount==2)
	    v = popNumber();
    else
	    v=-1;

    c = popNumber();

    ret=volumesound(c,v);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);

}
//_______________________________________________________________________________________________________________________

//stopsound(optional c): stop the  wave play in channel c (-1 or none =all)
void basStopSound()
{
    int c;
    if (argCount==1)
	    c=popNumber();
    else
	    c=-1;
    pushNumber( stopsound(c));
}
//_______________________________________________________________________________________________________________________

//pausesound(optional c): paused channel c (-1 or none =all)
void basPauseSound()
{
    int c;
    if (argCount==1)
	    c=popNumber();
    else
	    c=-1;
    pushNumber( pausesound(c));
}
//_______________________________________________________________________________________________________________________

//resumesound(optional c): unpaused channel c (-1 or none =all)
void basResumeSound()
{
    int c;
    if (argCount==1)
	    c=popNumber();
    else
	    c=-1;
    pushNumber( resumesound(c));
}
//_______________________________________________________________________________________________________________________

//vumetersound(optional c): give the current state of sound channel (-1 or none=all)
void basVuMeterSound()
{
    int c;
    if (argCount==1)
	    c=popNumber();
    else
	    c=-1;
    pushNumber( vumetersound(c));
}
//_______________________________________________________________________________________________________________________

//positionsound(c,angle,dist): position sound in 3d space
void basPositionSound()
{
    int c,angle,dist;

    dist=popNumber();
    angle=popNumber();
    c=popNumber();

    pushNumber( positionsound(c,angle,dist));
}
//_______________________________________________________________________________________________________________________

//soundchannels(n): dinamically change the number of channells managed by sdlBasic
void basSoundChannels()
{
    int n;

    n=popNumber();
    pushNumber( soundchannels(n));
}
//_______________________________________________________________________________________________________________________

//playmusic(n): play track xm,mod,ogg,mp3 n=number of ripetition(-1 always)
void basPlayMusic()
{
	int n;
	int ret;

    n = popNumber();

    ret=playmusic(n);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);

}
//_______________________________________________________________________________________________________________________

//positionmusic(p): move the execution at p second
void basPositionMusic()
{
	int p;
	int ret;

    p = popNumber();

    ret=positionmusic(p);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);

}
//_______________________________________________________________________________________________________________________

//stopmusic: terminate the music play
void basStopMusic()
{
    pushNumber( stopmusic() );
}
//_______________________________________________________________________________________________________________________

//pausemusic: set pause of the current music
void basPauseMusic(void)
{
    pushNumber( pausemusic() );
}
//_______________________________________________________________________________________________________________________

//resumemusic: unset pause of musica
void basResumeMusic(void)
{
    pushNumber( resumemusic() );
}
//_______________________________________________________________________________________________________________________

//rewindmusic: report at start the music
void basRewindMusic(void)
{
    pushNumber( rewindmusic() );
}
//_______________________________________________________________________________________________________________________

//fademusic(t): music fade
void basFadeMusic()
{
	int t;

    t = popNumber();

    pushNumber( fademusic(t));
}
//_______________________________________________________________________________________________________________________

//volumemusic(optional v): change the volume of music (0-128) -1 or none give the current volume
void basVolumeMusic()
{
	int v;
    if (argCount==1)
	    v = popNumber();
    else
	    v=-1;

    pushNumber( volumemusic(v));

}
//_______________________________________________________________________________________________________________________

//speedmusic(v): change the speed of music
void basSpeedMusic()
{
	int v;
	    v = popNumber();
    pushNumber( speedmusic(v));

}
//_______________________________________________________________________________________________________________________
//
// Keyboard
//_________________________________________________________________________________________________________________________

//key(keycode): give 1 if is press the key keycode
void basKey()
{
	int keycode;

    keycode = popNumber();

    pushNumber( key(keycode) );

}
//_______________________________________________________________________________________________________________________

//inkey: give ascii code of key press
void basInkey()
{
    pushNumber(inkey());
}
//_______________________________________________________________________________________________________________________

//waitkey(optional keycode): wait a key pression (0 =any key)
void basWaitKey()
{
    int keycode;

    if (argCount==1)
        keycode = popNumber();
    else
	keycode=0;
    pushNumber(waitkey(keycode));
}
//_______________________________________________________________________________________________________________________
//
// Mouse
//_________________________________________________________________________________________________________________________

//xmouse: give mouse x coordinate on display
void basXMouse()
{
    pushNumber( xmouse() );
}
//_______________________________________________________________________________________________________________________

//ymouse: give mouse y coordinate on display
void basYMouse()
{
    pushNumber( ymouse() );
}
//_______________________________________________________________________________________________________________________

//xmousescreen(n): give mouse x coordinate on screen
void basXMouseScreen()
{
    int n;
    n=popNumber();
    pushNumber( xmousescreen(n) );
}
//_______________________________________________________________________________________________________________________

//ymousescreen(n): give mouse y coordinate on screen
void basYMouseScreen()
{
    int n;
    n=popNumber();
    pushNumber( ymousescreen(n) );
}
//_______________________________________________________________________________________________________________________

//bmouse: give the buttonclick on the mouse
void basBMouse()
{
    pushNumber( bmouse() );
}
//_______________________________________________________________________________________________________________________

//mouseshow: show the mouse cursor
void basMouseShow()
{
    pushNumber( mouseshow() );
}
//_______________________________________________________________________________________________________________________

//mousehide: hide the mouse cursor
void basMouseHide()
{
    pushNumber( mousehide() );
}
//_______________________________________________________________________________________________________________________

//mousezone(x,y,w,h): give 1 if the mouse pointer is in rectangle(xy with size wh)
void basMouseZone()
{
    int x,y,w,h;

    h = popNumber();
    w = popNumber();
    y = popNumber();
    x = popNumber();

    pushNumber( mousezone(x,y,w,h) );

}
//_______________________________________________________________________________________________________________________
//
// Joystick
//_________________________________________________________________________________________________________________________

//xjoy: give joystick x coordinate
void basXJoy()
{
	int ret;
    ret=xjoy();
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);

}
//_______________________________________________________________________________________________________________________

//yjoy: give joystick y coordinate
void basYJoy()
{
	int ret;
    ret= yjoy();
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);

}
//_______________________________________________________________________________________________________________________

//bjoy					: give joystick button pressed
void basBJoy()
{
    pushNumber( bjoy() );
}
//_______________________________________________________________________________________________________________________
//
// SDLtime
//_________________________________________________________________________________________________________________________

//wait(t): wait t milliseconds
void basWait()
{
	int t;
	t= popNumber();
	pushNumber( wait(t));
}
//_______________________________________________________________________________________________________________________

//timer: give the current tick
void basTimer()
{
	pushNumber( timer());
}
//_______________________________________________________________________________________________________________________



//_________________________________________________________________________________________________________________________
//
//---- GLengine --------------------------------------------------------------------------------------------------
//_________________________________________________________________________________________________________________________

//_________________________________________________________________________________________________________________________
// GLscreen
//_________________________________________________________________________________________________________________________

//GLsetdisplay(w,h,bpp,mode)	: open the sdldisplay in opengl blit mode (0 full screen | 1 windows 2 resizable
void basGLsetdisplay()
{
    int w,h,bpp,mode;
    int ret;

    mode= popNumber();
    bpp= popNumber();
    h= popNumber();
    w= popNumber();

    ret=GLsetdisplay(w,h,bpp,mode);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);

}
//_______________________________________________________________________________________________________________________

//GLscreenswap()			: swap the buffers of display
void basGLscreenswap()
{
    int ret;
    ret=GLscreenswap();
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);
}
//_______________________________________________________________________________________________________________________

//GLscreen2D(m)			: enable(1) disable(0) the updating of 2d sdl screen in GLdisplay
void basGLscreen2D()
{
    int m;
    int ret;
    if (argCount==1)
	m= popNumber();
    else
	m=-1;
    ret=GLscreen2D(m);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);
}
//_________________________________________________________________________________________________________________________

//GLscreenrect (x,y,w,h)		: define the glviewport
void basGLscreenrect(void)
{
    int x,y,w,h;
    int ret;

    h= popNumber();
    w= popNumber();
    y= popNumber();
    x= popNumber();

    ret=GLscreenrect(x,y,w,h);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);
}
//_________________________________________________________________________________________________________________________

//_________________________________________________________________________________________________________________________
// GLcamera
//_________________________________________________________________________________________________________________________

void basGLcameraposition()
{
    double x,y,z;

    z=popNumber();
    y=popNumber();
    x=popNumber();

    GLcameraposition( x, y, z);
    pushNumber(0);
}
//_________________________________________________________________________________________________________________________

void basGLcameraangle()
{
    double x,y,z;

    z=popNumber();
    y=popNumber();
    x=popNumber();

    GLcameraangle( x, y, z);
    pushNumber(0);
}
//_________________________________________________________________________________________________________________________

void basGLcameralook()
{
    double x,y,z;

    z=popNumber();
    y=popNumber();
    x=popNumber();

    GLcameralook( x, y, z);
    pushNumber(0);
}
//_________________________________________________________________________________________________________________________
//_______________________________________________________________________________________________________________________
//GLlight
//_______________________________________________________________________________________________________________________

//_______________________________________________________________________________________________________________________
// GLgraphics
//_______________________________________________________________________________________________________________________

// GLcolor(r,g,b)
void basGLcolor(void)
{
	double r,g,b;

	b=popNumber();
	g=popNumber();
	r=popNumber();

	glColor3f(r,g,b);
	pushNumber(0);
}
//_______________________________________________________________________________________________________________________

//_______________________________________________________________________________________________________________________
// GLtextures
//_______________________________________________________________________________________________________________________

void basGLloadtexture( void )
{
    int n;
    char *file;
    int ret;

    n=popNumber();
    file=popString();

    ret=GLloadtexture(file,n);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);
}
//_______________________________________________________________________________________________________________________

void basGLmaketexture( void )
{
    int n,m;
    int ret;

    m=popNumber();
    n=popNumber();

    ret=GLmaketexture(n,m);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);
}
//_______________________________________________________________________________________________________________________

void basGLtexture( void )
{
    int n;
    int ret;

    n=popNumber();
    ret=GLtexture(n);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);
}


//_______________________________________________________________________________________________________________________
// GLobject
//_______________________________________________________________________________________________________________________

void basGLmakecube( void )
{
    double x,y,z,l;
    int ret;

    l=popNumber();
    z=popNumber();
    y=popNumber();
    x=popNumber();

    ret=GLmakecubef(x,y,z,l);

    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);
}
//_______________________________________________________________________________________________________________________

void basGLmakesphere()
{
    double x,y,z,l;
    int ret;

    l=popNumber();
    z=popNumber();
    y=popNumber();
    x=popNumber();

    ret=GLmakesphere(x,y,z,l);

    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);

}
//_______________________________________________________________________________________________________________________

void basGLmakesurface( void )
{
    int v1,v2,v3,v4;
    int ret;

    v1=popNumber();
    v2=popNumber();
    v3=popNumber();
    v4=popNumber();

    ret=GLmakesurfacefv(v1,v2,v3,v4);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);
}
//_______________________________________________________________________________________________________________________
//_______________________________________________________________________________________________________________________
// GLmatrix
//_______________________________________________________________________________________________________________________

void basGLbegin(void)
{
    int m;
    int ret;

    m=popNumber();

    glBegin(m);
    ret=0;
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);
}
//_______________________________________________________________________________________________________________________

void basGLend(void)
{
    int ret;

    glEnd();
    ret=0;
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);

}
//_______________________________________________________________________________________________________________________

void basGLnormal(void)
{
    double x,y,z;
    int ret;

    z=popNumber();
    y=popNumber();
    x=popNumber();

    glNormal3f(x,y,z);
    ret=0;

    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);
}
//_______________________________________________________________________________________________________________________

void basGLtexcoord(void)
{
    double x,y;
    int ret;

    y=popNumber();
    x=popNumber();

    glTexCoord2f(x,y);
    ret=0;

    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);
}
//_______________________________________________________________________________________________________________________

void basGLsetvertex(void)
{
    int v;
    double x,y,z;
    int ret;


    if (argCount==1){
	v=popNumber();
	ret=GLsetvertexfv(v);
    }
    else{
	z=popNumber();
	y=popNumber();
	x=popNumber();
	glVertex3f(x,y,z);
	ret=0;
    }
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);
}
//_______________________________________________________________________________________________________________________

void basGLpushmatrix(void)
{
   glPushMatrix();
    pushNumber(0);
}
//_______________________________________________________________________________________________________________________

void basGLpopmatrix(void)
{
   glPopMatrix();
    pushNumber(0);
}
//_______________________________________________________________________________________________________________________

void basGLflush(void)
{
   glFlush();
    pushNumber(0);
}

//_______________________________________________________________________________________________________________________

//_______________________________________________________________________________________________________________________
// GLlowlevel
//_______________________________________________________________________________________________________________________

void basGLvertex( )
{
    int n;
    double x,y,z;
    int ret;

    z=popNumber();
    y=popNumber();
    x=popNumber();
    n=popNumber();

    ret=GLvertex( n, x, y, z);
    if (ret==-1) ePrintf( Runtime, "SDLengine Error \n");
    pushNumber(ret);

}
//_______________________________________________________________________________________________________________________

void basGLtranslate()
{
    double x,y,z;

    z=popNumber();
    y=popNumber();
    x=popNumber();

    glTranslatef( x, y, z);
    pushNumber(0);
}
//_________________________________________________________________________________________________________________________

void basGLrotate()
{

    double v,x,y,z;

    z=popNumber();
    y=popNumber();
    x=popNumber();
    v=popNumber();

    glRotatef( v, x, y, z );
    pushNumber(0);
}
//_______________________________________________________________________________________________________________________

void basGLenable(void)
{
    int p;

    p=popNumber();
    glEnable(p);
    pushNumber(0);

}
//_______________________________________________________________________________________________________________________

/*--- end ---------------------------------------------------------------*/




/* addBuiltin: add a builtin to the dictionary */
void addBuiltin( char *name, void (*routine)(void), int args, int optargs )
{
    Symbol  *s;
    s = findSymbol( name, NULL );
    if (s==NULL) {
        s = addSymbol( name, NULL, SYM_BUILTIN );
        s->builtin = routine;
        s->args = args;
        s->optargs = optargs;

    } else {
        ePrintf( Runtime, "builtin %s is already declared as a %s",
            s->name, symbolName[s->symboltype] );
    }

}

/* initBuiltins: install all the builtins */
void initBuiltins()
{
    int i;

    for (i = 0; builtin[i].name != NULL; i++) {
        addBuiltin( builtin[i].name,
                    builtin[i].routine,
                    builtin[i].args,
                    builtin[i].optargs );
    }
}

