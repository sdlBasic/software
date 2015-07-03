/*
    Name:       file.h
    Purpose:    file support for wxBasic
    Author:     David Cuny
    Copyright:  (c) 2001 David Cuny <dcuny@lanset.com>
    Licence:    LGPL
*/

#define FILE_MODE_INPUT     0
#define FILE_MODE_OUTPUT    1
#define FILE_MODE_APPEND    2
#define FILE_MAX_HANDLE     16
#define FILE_BUFFER_MAX     1024

/* handles are 1 relative */
FILE    *fileHandles[FILE_MAX_HANDLE+1];
int     fileMode[FILE_MAX_HANDLE+1];

void fileCheckHandle( int handle, int mustBeOpen );
int fileFree(void);
void fileOpen( char *name, int mode, int handle );
void fileClose( int handle );
void fileCloseAll(void);
char *fileLineInput( int handle );
void filePrintString( int handle, char *text );
int fileEof( int handle );
int fileReadByte( int handle );
void fileWriteByte( int handle, char c );
long fileSeek( int handle, long pos );
long fileSize( int handle );
long filePosition( int handle );
int FileExists( char *name );

int filecopy(char *source,char *dest);
int filemove(char *source,char *dest);
int filerename(char *source,char *dest);
