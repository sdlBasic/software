/*
    Name:       file.c
    Purpose:    file support for wxBasic
    Author:     David Cuny
    Copyright:  (c) 2001 David Cuny <dcuny@lanset.com>
    Licence:    LGPL
*/

/* fileCheckHandle: ensure handle is valid */
void fileCheckHandle( int handle, int mustBeOpen )
{
    if (handle < 1 || handle > FILE_MAX_HANDLE) {
        ePrintf( Runtime, "bad file number #%d", handle);
    }

    if (mustBeOpen && fileHandles[handle-1] == NULL) {
        ePrintf( Runtime, "file #%d is not open", handle );
    }

}

/* fileFree: return index of next free file */
int fileFree()
{
    int     i;
    for( i = 1; i <= FILE_MAX_HANDLE; i++ ) {
        if (fileHandles[i-1] == NULL) {
            return i;
        }
    }
    return 0;
}


/* fileOpen: close an open file */
void fileOpen( char *name, int mode, int handle )
{
    fileCheckHandle( handle, 0 );

    if (fileHandles[handle-1] != NULL) {
        ePrintf( Runtime, "file #%d is already open", handle );
    }

    fileMode[handle-1] = mode;
    switch (mode) {
    case FILE_MODE_INPUT:
        fileHandles[handle-1] = fopen( name, "rb" );
        break;
    case FILE_MODE_OUTPUT:
        fileHandles[handle-1] = fopen( name, "wb" );
        break;
    case FILE_MODE_APPEND:
        fileHandles[handle-1] = fopen( name, "ab" );
        break;
    default:
        /* should not happen */
        ePrintf( Runtime, "fileOpen: unknown file mode" );
    }

    if ((int)(fileHandles[handle-1] ) == EOF) {
        ePrintf( Runtime, "fileOpen: Error opening file #%d", handle );
    }
}


/* fileClose: close an open file */
void fileClose( int handle )
{
    fileCheckHandle( handle, 0 );
    if (fileHandles[handle-1] != NULL) {
        if (fclose( fileHandles[handle-1] ) == EOF) {
            ePrintf( Runtime, "fileClose: Error closing file #%d", handle );
        }
        fileHandles[handle-1] = NULL;
    }
}

/* fileCloseAll: close all open files */
void fileCloseAll()
{
    int     i;

    for( i = 1; i <= FILE_MAX_HANDLE; i++ ) {
        fileClose( i );
    }
}

/* fileLineInput: return a string from the file */
char *fileLineInput( int handle )
{
    int len;
    char buffer[FILE_BUFFER_MAX];

    fileCheckHandle( handle, 1 );

    if (fileMode[handle-1] != FILE_MODE_INPUT) {
        ePrintf( Runtime, "File #d is not open for Input", handle );
    }

    buffer[0] = '\0';
    fgets( buffer, FILE_BUFFER_MAX-1, fileHandles[handle-1] );

    /* remove eol */
    len = strlen( buffer );
    if (len) {
        if (buffer[len-1] == '\n' ) {
            buffer[len-1] = '\0';
        }
    }

    return eCopyString( buffer );
}

void filePrintString( int handle, char *text )
{
    fileCheckHandle( handle, 1 );
    switch ( fileMode[handle-1] ) {
    case FILE_MODE_INPUT:
        ePrintf( Runtime, "Bad file mode: file #%d is Input, not Output or Append", handle );
        break;
    case FILE_MODE_OUTPUT:
    case FILE_MODE_APPEND:
        if (fputs(text, fileHandles[handle-1]) == EOF) {
            ePrintf( Runtime, "filePrintString: error writing to file #d", handle );
        }
        break;
    default:
        /* should not happen */
        ePrintf( Runtime, "filePrintString: unknown file mode" );
    }
}

/* return true if end of file */
int fileEof( int handle )
{
    char    byte;
    FILE    *fp;

    fileCheckHandle( handle, 1 );
    fp = fileHandles[handle-1];

    /* feof isn't very good in DOS */
    if (fread( &byte, 1, 1, fp ) == 0) {
        return 1;
    }
    /* move back one byte */
    fseek( fp, -1, 1 );
    return 0;
}

/* return a byte from the file */
int fileReadByte( int handle )
{
    FILE    *fp;

    fileCheckHandle( handle, 1 );
    fp = fileHandles[handle-1];

    if (fileMode[handle-1] != FILE_MODE_INPUT) {
        ePrintf( Runtime, "Bad file mode" );
    }
    return fgetc( fp );
}

/* write a byte to the file */
void fileWriteByte( int handle, char c )
{
    FILE    *fp;

    fileCheckHandle( handle, 1 );
    fp = fileHandles[handle-1];

    if (fileMode[handle-1] == FILE_MODE_INPUT) {
        ePrintf( Runtime, "Bad file mode" );
    }
    fputc( c, fp );
}


/* seek to a file position */
long fileSeek( int handle, long pos )
{
    FILE    *fp;

    fileCheckHandle( handle, 1 );
    fp = fileHandles[handle-1];

    if (fileMode[handle-1] != FILE_MODE_INPUT) {
        ePrintf( Runtime, "Bad file mode" );
    }

    return fseek( fp, pos-1, 0 );
}

/* return size of file in bytes */
long fileSize( int handle )
{
    long    currentPos, endPos;
    FILE    *fp;

    fileCheckHandle( handle, 1 );
    fp = fileHandles[handle-1];

    if (fileMode[handle-1] != FILE_MODE_INPUT) {
        ePrintf( Runtime, "Bad file mode" );
    }

    /* save current position */
    currentPos = ftell( fp );

    /* seek to end */
    fseek( fp, 0, 2 );
    endPos = ftell( fp );

    /* restore prior position */
    fseek( fp, currentPos, 0 );

    return endPos;
}


/* return current file position */
long filePosition( int handle )
{
    long    pos;
    FILE    *fp;

    fileCheckHandle( handle, 1 );
    fp = fileHandles[handle-1];

    pos = ftell( fp );
    if (pos != -1) {
        /* offset starts at 1, not zero */
        pos++;
    }
    return pos;
}

/* see if a file exists */
int FileExists( char *name )
{
    FILE    *handle;

    /* try to open the file */
    handle = fopen( name, "rb" );
    if (handle == NULL) return 0;
    fclose( handle );
#ifdef unix    
    if(direxists(name))return 0;
#endif
    return 1;
}

