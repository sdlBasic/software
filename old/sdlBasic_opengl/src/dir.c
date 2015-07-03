/*
    Name:       dir.c
    Purpose:    dir support for wxBasic/sdlBasic
    Author:     Viola Roberto __vroby__
    Copyright:  (c) 2003 Viola Roberto <__vroby__@libero.it>
    Licence:    LGPL
*/

#ifdef unix
	#include <unistd.h>
	#include <dirent.h>
	DIR *hDir;
	struct dirent *s_file;
#else
	#include <direct.h>
	#include <io.h>
	struct _finddata_t c_file;
	long n_file;
#endif

char path[255];



/*______________________________________________________________________________________________________________________*/

/* mkdir: make new directory */
/*int mkdir(char *namedir)
{
char cmd[256];
#ifdef unix
	strcpy(cmd,"mkdir ");
	strcat(cmd,namedir);
	//printf(cmd);
#else
	strcpy(cmd,"md ");
	strcat(cmd,namedir);
	//printf(cmd);
#endif

	system(cmd);
	return 0;
}
/*______________________________________________________________________________________________________________________*/

char *dir()
{
getcwd(path,254);
return path;
}
/*______________________________________________________________________________________________________________________*/


/* dir: listed in directory path the first entry */
char *dirfirst(char *path)
{
#ifdef unix
	hDir = opendir(path);
	if (hDir == NULL )
		return "";
	s_file=readdir(hDir);
	if (s_file==NULL){
		closedir(hDir);
		return "";
	}
	return s_file->d_name;
#else
	strcat(path,"*");

	n_file=_findfirst( path, &c_file );
	if (n_file==-1L)return "";
	return c_file.name;
#endif
}
/*______________________________________________________________________________________________________________________*/

/* dir: listed in directory path the next entry or none if it's at the end list */
char *dirnext()
{
#ifdef unix
	s_file=readdir(hDir);

	if (s_file==NULL){
		closedir(hDir);
		return "";
	}
	return s_file->d_name;
#else

	if (_findnext(n_file, &c_file )!=0){
		_findclose(n_file);
		return "";
	}
	return c_file.name;
#endif
}
/*______________________________________________________________________________________________________________________*/

/* direxist: check if exist dir path */
int direxists(char *path)
{
	
	
#ifdef unix
	DIR *hDirEx;

	hDirEx = opendir(path);
	if (hDirEx == NULL ){
		closedir(hDirEx);
		return 0;
	}
	else{
		closedir(hDirEx);
		return 1;
	}
#else
	FILE    *handle;
	long n_fileEx;


	n_fileEx=_findfirst( path, &c_file );
	if (n_fileEx==-1L){
		_findclose(n_fileEx);
		return 0;
	}
	else{
		_findclose(n_fileEx);

		handle = fopen( path, "rb" );
		if (handle == NULL)return 1;
		
		fclose( handle );			
		return 0;
	}
#endif
}
/*______________________________________________________________________________________________________________________*/

/* isdir: return 1 if path is a directory*/
int isdir(char *path)
{
	return direxists(path);
}
/*______________________________________________________________________________________________________________________*/

int filecopy(char *source,char *dest)
{
char cmd[256];
#ifdef unix
	strcpy(cmd,"cp -r ");
	strcat(cmd,source);
	strcat(cmd," ");
	strcat(cmd,dest);
#else
	strcpy(cmd,"copy ");
	strcat(cmd,source);
	strcat(cmd," ");
	strcat(cmd,dest);
#endif
		system(cmd);
}
/*______________________________________________________________________________________________________________________*/

int filemove(char *source,char *dest)
{
char cmd[256];
#ifdef unix
	strcpy(cmd,"mv -r ");
	strcat(cmd,source);
	strcat(cmd," ");
	strcat(cmd,dest);
#else
	strcpy(cmd,"move ");
	strcat(cmd,source);
	strcat(cmd," ");
	strcat(cmd,dest);
#endif
	system(cmd);
}
/*______________________________________________________________________________________________________________________*/

int filerename(char *source,char *dest)
{
	return filemove(source,dest);
}
/*______________________________________________________________________________________________________________________*/
