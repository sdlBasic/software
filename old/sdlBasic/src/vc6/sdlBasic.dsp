# Microsoft Developer Studio Project File - Name="sdlBasic" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=sdlBasic - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "sdlBasic.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "sdlBasic.mak" CFG="sdlBasic - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "sdlBasic - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "sdlBasic - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 1
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "sdlBasic - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /w /W0 /GX /Zd /O2 /I "C:\devel\c-cpp\visual cpp\SDL\INCLUDE" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# SUBTRACT MTL /Oicf
# ADD BASE RSC /l 0x410 /d "NDEBUG"
# ADD RSC /l 0x410
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:yes /machine:I386 /out:"../sdlBasic.exe"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "sdlBasic - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MD /w /W0 /GX /Zd /Od /I "c:\devel\c-cpp\visual cpp\SDL\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Fp"Debug/testalpha.pch" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x410 /d "_DEBUG"
# ADD RSC /l 0x410 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../sdlBasic.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "sdlBasic - Win32 Release"
# Name "sdlBasic - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\inflate.c
# End Source File
# Begin Source File

SOURCE=..\sdlBasic.c
# End Source File
# Begin Source File

SOURCE=..\SDLengine.c
# End Source File
# Begin Source File

SOURCE=..\unzip.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE="..\..\..\..\c-cpp\visual cpp\SDL\INCLUDE\begin_code.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\c-cpp\visual cpp\SDL\INCLUDE\close_code.h"
# End Source File
# Begin Source File

SOURCE="c:\devel\c-cpp\visual cpp\SDL\INCLUDE\SDL.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\c-cpp\visual cpp\SDL\INCLUDE\SDL_active.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\c-cpp\visual cpp\SDL\INCLUDE\SDL_audio.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\c-cpp\visual cpp\SDL\INCLUDE\SDL_byteorder.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\c-cpp\visual cpp\SDL\INCLUDE\SDL_cdrom.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\c-cpp\visual cpp\SDL\INCLUDE\SDL_copying.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\c-cpp\visual cpp\SDL\INCLUDE\SDL_endian.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\c-cpp\visual cpp\SDL\INCLUDE\SDL_error.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\c-cpp\visual cpp\SDL\INCLUDE\SDL_events.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\c-cpp\visual cpp\SDL\INCLUDE\SDL_getenv.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\c-cpp\visual cpp\SDL\INCLUDE\SDL_image.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\c-cpp\visual cpp\SDL\INCLUDE\SDL_joystick.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\c-cpp\visual cpp\SDL\INCLUDE\SDL_keyboard.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\c-cpp\visual cpp\SDL\INCLUDE\SDL_keysym.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\c-cpp\visual cpp\SDL\INCLUDE\SDL_main.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\c-cpp\visual cpp\SDL\INCLUDE\SDL_mixer.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\c-cpp\visual cpp\SDL\INCLUDE\SDL_mouse.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\c-cpp\visual cpp\SDL\INCLUDE\SDL_mutex.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\c-cpp\visual cpp\SDL\INCLUDE\SDL_name.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\c-cpp\visual cpp\SDL\INCLUDE\SDL_opengl.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\c-cpp\visual cpp\SDL\INCLUDE\SDL_quit.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\c-cpp\visual cpp\SDL\INCLUDE\SDL_rwops.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\c-cpp\visual cpp\SDL\INCLUDE\SDL_syswm.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\c-cpp\visual cpp\SDL\INCLUDE\SDL_thread.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\c-cpp\visual cpp\SDL\INCLUDE\SDL_timer.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\c-cpp\visual cpp\SDL\INCLUDE\SDL_ttf.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\c-cpp\visual cpp\SDL\INCLUDE\SDL_types.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\c-cpp\visual cpp\SDL\INCLUDE\SDL_version.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\c-cpp\visual cpp\SDL\INCLUDE\SDL_video.h"
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\doc_default.ico
# End Source File
# Begin Source File

SOURCE=..\doc_sdlbas.ico
# End Source File
# Begin Source File

SOURCE=..\doc_sdlblib.ico
# End Source File
# Begin Source File

SOURCE=..\icon2.ico
# End Source File
# Begin Source File

SOURCE=..\Script1.rc
# End Source File
# Begin Source File

SOURCE=..\sdlBasic.ico
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\..\Programmi\Microsoft Visual Studio\VC98\Lib\glut32.lib"
# End Source File
# End Group
# Begin Source File

SOURCE="..\..\..\..\..\c-cpp\visual cpp\SDL\LIB\SDL.lib"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\c-cpp\visual cpp\SDL\LIB\SDL_image.lib"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\c-cpp\visual cpp\SDL\LIB\SDL_mixer.lib"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\c-cpp\visual cpp\SDL\LIB\SDL_ttf.lib"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\c-cpp\visual cpp\SDL\LIB\SDLmain.lib"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\c-cpp\visual cpp\SDL\LIB\SDL_net.lib"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\c-cpp\visual cpp\SDL\LIB\smpeg.lib"
# End Source File
# End Target
# End Project
