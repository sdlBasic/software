// SciTE - Scintilla based Text Editor
/** @file SciTEWin.cxx
 ** Main code for the Windows version of the editor.
 **/
// Copyright 1998-2003 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#include <time.h>

#include "SciTEWin.h"

#ifndef NO_EXTENSIONS
#include "MultiplexExtension.h"

#ifndef NO_FILER
#include "DirectorExtension.h"
#endif

#ifndef NO_LUA
#include "SingleThreadExtension.h"
#include "LuaExtension.h"
#endif

#endif

#ifdef STATIC_BUILD
const char appName[] = "Sc1";
#else
const char appName[] = "SciTE";
#endif

long SciTEKeys::ParseKeyCode(const char *mnemonic) {
	int modsInKey = 0;
	int keyval = -1;

	if (mnemonic && *mnemonic) {
		SString sKey = mnemonic;

		if (sKey.contains("Ctrl+")) {
			modsInKey |= SCMOD_CTRL;
			sKey.remove("Ctrl+");
		}
		if (sKey.contains("Shift+")) {
			modsInKey |= SCMOD_SHIFT;
			sKey.remove("Shift+");
		}
		if (sKey.contains("Alt+")) {
			modsInKey |= SCMOD_ALT;
			sKey.remove("Alt+");
		}

		if (sKey.length() == 1) {
			keyval = VkKeyScan(sKey[0]) & 0xFF;
		} else if (sKey.length() > 1) {
			if ((sKey[0] == 'F') && (isdigit(sKey[1]))) {
				sKey.remove("F");
				int fkeyNum = sKey.value();
				if (fkeyNum >= 1 && fkeyNum <= 12)
					keyval = fkeyNum - 1 + VK_F1;
			} else if ((sKey[0] == 'V') && (isdigit(sKey[1]))) {
				sKey.remove("V");
				int vkey = sKey.value();
				if (vkey > 0 && vkey <= 0x7FFF)
					keyval = vkey;
			} else if (sKey.search("Keypad") == 0) {
				sKey.remove("Keypad");
				if (isdigit(sKey[0])) {
					int keyNum = sKey.value();
					if (keyNum >= 0 && keyNum <= 9)
						keyval = keyNum + VK_NUMPAD0;
				} else if (sKey == "Plus") {
					keyval = VK_ADD;
				} else if (sKey == "Minus") {
					keyval = VK_SUBTRACT;
				} else if (sKey == "Decimal") {
					keyval = VK_DECIMAL;
				} else if (sKey == "Divide") {
					keyval = VK_DIVIDE;
				} else if (sKey == "Multiply") {
					keyval = VK_MULTIPLY;
				}
			} else if (sKey == "Left") {
				keyval = VK_LEFT;
			} else if (sKey == "Right") {
				keyval = VK_RIGHT;
			} else if (sKey == "Up") {
				keyval = VK_UP;
			} else if (sKey == "Down") {
				keyval = VK_DOWN;
			} else if (sKey == "Insert") {
				keyval = VK_INSERT;
			} else if (sKey == "End") {
				keyval = VK_END;
			} else if (sKey == "Home") {
				keyval = VK_HOME;
			} else if (sKey == "Enter") {
				keyval = VK_RETURN;
			} else if (sKey == "Space") {
				keyval = VK_SPACE;
			} else if (sKey == "Escape") {
				keyval = VK_ESCAPE;
			} else if (sKey == "Delete") {
				keyval = VK_DELETE;
			} else if (sKey == "PageUp") {
				keyval = VK_PRIOR;
			} else if (sKey == "PageDown") {
				keyval = VK_NEXT;
			}
		}
	}

	return (keyval > 0) ? (keyval | (modsInKey<<16)) : 0;
}

bool SciTEKeys::MatchKeyCode(long parsedKeyCode, int keyval, int modifiers) {
	// TODO: are the 0x11 and 0x10 special cases needed, or are they
	// just short-circuits?  If not needed, this test could removed,
	// and perhaps the function moved to Platform.h as an inline.
	if (keyval == 0x11 || keyval == 0x10)
		return false;
	return parsedKeyCode && !(0xFFFF0000 & (keyval | modifiers)) && (parsedKeyCode == (keyval | (modifiers<<16)));
}

HINSTANCE SciTEWin::hInstance = 0;
char *SciTEWin::className = NULL;
char *SciTEWin::classNameInternal = NULL;
SciTEWin *SciTEWin::app = NULL;

SciTEWin::SciTEWin(Extension *ext) : SciTEBase(ext) {
	app = this;
	cmdShow = 0;
	heightBar = 7;
	fontTabs = 0;
	wFocus = 0;

	winPlace.length = 0;

	openWhat[0] = '\0';
	memset(&fr, 0, sizeof(fr));
	modalParameters = false;
	filterDefault = 1;
	menuSource = 0;
	hWriteSubProcess = NULL;

	// Read properties resource into propsEmbed
	// The embedded properties file is to allow distributions to be sure
	// that they have a sensible default configuration even if the properties
	// files are missing. Also allows a one file distribution of Sc1.EXE.
	propsEmbed.Clear();
	// System type properties are also stored in the embedded properties.
	propsEmbed.Set("PLAT_WIN", "1");
	OSVERSIONINFO osv = {sizeof(OSVERSIONINFO), 0, 0, 0, 0, ""};
	::GetVersionEx(&osv);
	if (osv.dwPlatformId == VER_PLATFORM_WIN32_NT)
		propsEmbed.Set("PLAT_WINNT", "1");
	else if (osv.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
		propsEmbed.Set("PLAT_WIN95", "1");
	HRSRC handProps = ::FindResource(hInstance, "Embedded", "Properties");
	if (handProps) {
		DWORD size = ::SizeofResource(hInstance, handProps);
		HGLOBAL hmem = ::LoadResource(hInstance, handProps);
		if (hmem) {
			const void *pv = ::LockResource(hmem);
			if (pv) {
				propsEmbed.ReadFromMemory(
				    reinterpret_cast<const char *>(pv), size, 0);
			}
		}
		::FreeResource(handProps);
	}

	ReadGlobalPropFile();
	/// Need to copy properties to variables before setting up window
	SetPropertiesInitial();
	ReadAbbrevPropFile();

	hDevMode = 0;
	hDevNames = 0;
	::ZeroMemory(&pagesetupMargin, sizeof(pagesetupMargin));

	hHH = 0;
	hMM = 0;
	uniqueInstance.Init(this);

	hAccTable = ::LoadAccelerators(hInstance, "ACCELS"); // md
}

SciTEWin::~SciTEWin() {
	if (hDevMode)
		::GlobalFree(hDevMode);
	if (hDevNames)
		::GlobalFree(hDevNames);
	if (hHH)
		::FreeLibrary(hHH);
	if (hMM)
		::FreeLibrary(hMM);
	if (fontTabs)
		::DeleteObject(fontTabs);
}

uptr_t SciTEWin::GetInstance() {
	return reinterpret_cast<uptr_t>(hInstance);
}

void SciTEWin::Register(HINSTANCE hInstance_) {
	const char resourceName[] = "SciTE";

	hInstance = hInstance_;

	WNDCLASS wndclass;

	// Register the frame window
	className = "SciTEWindow";
	wndclass.style = 0;
	wndclass.lpfnWndProc = SciTEWin::TWndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = sizeof(SciTEWin*);
	wndclass.hInstance = hInstance;
	wndclass.hIcon = ::LoadIcon(hInstance, resourceName);
	wndclass.hCursor = NULL;
	wndclass.hbrBackground = NULL;
	wndclass.lpszMenuName = resourceName;
	wndclass.lpszClassName = className;
	if (!::RegisterClass(&wndclass))
		exit(FALSE);

	// Register the window that holds the two Scintilla edit windows and the separator
	classNameInternal = "SciTEWindowContent";
	wndclass.lpfnWndProc = SciTEWin::IWndProc;
	wndclass.lpszMenuName = 0;
	wndclass.lpszClassName = classNameInternal;
	if (!::RegisterClass(&wndclass))
		exit(FALSE);
}

static void GetSciTEPath(char *path, unsigned int lenPath, char *home) {
	*path = '\0';
	if (home) {
		strncpy(path, home, lenPath);
	} else {
		::GetModuleFileName(0, path, lenPath);
		// Remove the SciTE.exe
		char *lastSlash = strrchr(path, pathSepChar);
		if (lastSlash)
			*lastSlash = '\0';
	}
	path[lenPath - 1] = '\0';
	ChopTerminalSlash(path);
}

void SciTEWin::GetDefaultDirectory(char *directory, size_t size) {
	char *home = getenv("SciTE_HOME");
	GetSciTEPath(directory, static_cast<unsigned int>(size), home);
}

bool SciTEWin::GetSciteDefaultHome(char *path, unsigned int lenPath) {
	*path = '\0';
	char *home = getenv("SciTE_HOME");
	GetSciTEPath(path, lenPath, home);
	return true;
}

bool SciTEWin::GetSciteUserHome(char *path, unsigned int lenPath) {
	*path = '\0';
	char *home = getenv("SciTE_HOME");
	if (!home)
		home = getenv("USERPROFILE");
	GetSciTEPath(path, lenPath, home);
	return true;
}

// Help command lines contain topic!path
void SciTEWin::ExecuteOtherHelp(const char *cmd) {
	char *topic = StringDup(cmd);
	if (topic) {
		char *path = strchr(topic, '!');
		if (path) {
			*path = '\0';
			path++;	// After the !
			::WinHelp(MainHWND(),
			          path,
			          HELP_KEY,
			          reinterpret_cast<unsigned long>(topic));
		}
	}
	delete []topic;
}

// HH_AKLINK not in mingw headers
struct XHH_AKLINK {
	long cbStruct;
	BOOL fReserved;
	const char *pszKeywords;
	char *pszUrl;
	char *pszMsgText;
	char *pszMsgTitle;
	char *pszWindow;
	BOOL fIndexOnFail;
};

// Help command lines contain topic!path
void SciTEWin::ExecuteHelp(const char *cmd) {
	if (!hHH)
		hHH = ::LoadLibrary("HHCTRL.OCX");

	if (hHH) {
		char *topic = StringDup(cmd);
		char *path = strchr(topic, '!');
		if (topic && path) {
			*path = '\0';
			path++;	// After the !
			typedef HWND (WINAPI *HelpFn) (HWND, const char *, UINT, DWORD);
			HelpFn fnHHA = (HelpFn)::GetProcAddress(hHH, "HtmlHelpA");
			if (fnHHA) {
				XHH_AKLINK ak;
				ak.cbStruct = sizeof(ak);
				ak.fReserved = FALSE;
				ak.pszKeywords = topic;
				ak.pszUrl = NULL;
				ak.pszMsgText = NULL;
				ak.pszMsgTitle = NULL;
				ak.pszWindow = NULL;
				ak.fIndexOnFail = TRUE;
				fnHHA(NULL,
				      path,
				      0x000d,          	// HH_KEYWORD_LOOKUP
				      reinterpret_cast<DWORD>(&ak)
				     );
			}
		}
		delete []topic;
	}
}

void SciTEWin::CopyAsRTF() {
	CharacterRange cr = GetSelection();
	char *fileNameTemp = tmpnam(0);
	if (fileNameTemp) {
		SaveToRTF(fileNameTemp, cr.cpMin, cr.cpMax);
		FILE *fp = fopen(fileNameTemp, fileRead);
		if (fp) {
			fseek(fp, 0, SEEK_END);
			int len = ftell(fp);
			fseek(fp, 0, SEEK_SET);
			HGLOBAL hand = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, len + 1);
			if (hand) {
				::OpenClipboard(MainHWND());
				::EmptyClipboard();
				char *ptr = static_cast<char *>(::GlobalLock(hand));
				fread(ptr, 1, len, fp);
				ptr[len] = '\0';
				::GlobalUnlock(hand);
				::SetClipboardData(::RegisterClipboardFormat(CF_RTF), hand);
				::CloseClipboard();
			}
			fclose(fp);
		}
		unlink(fileNameTemp);
	}
}

void SciTEWin::FullScreenToggle() {
	HWND wTaskBar = FindWindow("Shell_TrayWnd", "");
	fullScreen = !fullScreen;
	if (fullScreen) {
		::SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWorkArea, 0);
		::SystemParametersInfo(SPI_SETWORKAREA, 0, 0, SPIF_SENDCHANGE);
		::ShowWindow(wTaskBar, SW_HIDE);

		winPlace.length = sizeof(winPlace);
		::GetWindowPlacement(MainHWND(), &winPlace);
		int topStuff = ::GetSystemMetrics(SM_CYMENU) +
		               ::GetSystemMetrics(SM_CYEDGE);
		if (props.GetInt("full.screen.hides.menu"))
			topStuff += ::GetSystemMetrics(SM_CYCAPTION);
		::SetWindowPos(MainHWND(), HWND_TOP,
		               -::GetSystemMetrics(SM_CXSIZEFRAME) - 1,
		               -topStuff - 2,
		               ::GetSystemMetrics(SM_CXSCREEN) +
		               2 * ::GetSystemMetrics(SM_CXSIZEFRAME) + 2,
		               ::GetSystemMetrics(SM_CYSCREEN) + topStuff +
		               ::GetSystemMetrics(SM_CYSIZEFRAME) + 3,
		               0);
	} else {
		::ShowWindow(wTaskBar, SW_SHOW);
		if (winPlace.length) {
			::SystemParametersInfo(SPI_SETWORKAREA, 0, &rcWorkArea, 0);
			if (winPlace.showCmd == SW_SHOWMAXIMIZED) {
				::ShowWindow(MainHWND(), SW_RESTORE);
				::ShowWindow(MainHWND(), SW_SHOWMAXIMIZED);
			} else {
				::SetWindowPlacement(MainHWND(), &winPlace);
			}
		}
	}
	::SetForegroundWindow(MainHWND());
	CheckMenus();
}

HWND SciTEWin::MainHWND() {
	return reinterpret_cast<HWND>(wSciTE.GetID());
}

void SciTEWin::Command(WPARAM wParam, LPARAM lParam) {
	int cmdID = ControlIDOfCommand(wParam);
	if (wParam & 0x10000) {
		// From accelerator -> goes to focused pane.
		menuSource = 0;
	}

	switch (cmdID) {

	case IDM_SRCWIN:
	case IDM_RUNWIN:
		if (HIWORD(wParam) == SCEN_SETFOCUS) {
			wFocus = reinterpret_cast<HWND>(lParam);
			CheckMenus();
		}
		if (HIWORD(wParam) == SCEN_KILLFOCUS) {
			CheckMenus();
		}
		break;

	case IDM_ACTIVATE:
		Activate(lParam);
		break;

	case IDM_FINISHEDEXECUTE: {
			executing = false;
			CheckMenus();
			for (int icmd = 0; icmd < commandMax; icmd++) {
				jobQueue[icmd].Clear();
			}
			commandCurrent = 0;
			CheckReload();
		}
		break;

	case IDM_ONTOP:
		topMost = (topMost ? false : true);
		::SetWindowPos(MainHWND(), (topMost ? HWND_TOPMOST : HWND_NOTOPMOST ), 0, 0, 0, 0, SWP_NOMOVE + SWP_NOSIZE);
		CheckAMenuItem(IDM_ONTOP, topMost);
		break;

	case IDM_OPENFILESHERE:
		uniqueInstance.ToggleOpenFilesHere();
		break;

	case IDM_FULLSCREEN:
		FullScreenToggle();
		break;

	default:
		SciTEBase::MenuCommand(cmdID, menuSource);
	}
}

/**
 * Makes a long path from a given, possibly short path/file.
 *
 * The short path/file must exist, and if it is a file it must be fully specified
 * otherwise the function fails.
 *
 * sizeof @a longPath buffer must be a least _MAX_PATH
 * @returns true on success, and the long path in @a longPath buffer,
 * false on failure, and copies the @a shortPath arg to the @a longPath buffer.
 */
bool MakeLongPath(const char* shortPath, char* longPath) {
	// when we have pfnGetLong, we assume it never changes as kernel32 is always loaded
	static DWORD (STDAPICALLTYPE* pfnGetLong)(const char* lpszShortPath, char* lpszLongPath, DWORD cchBuffer) = NULL;
	static bool kernelTried = FALSE;
	bool ok = FALSE;

	if (!kernelTried) {
		HMODULE hModule;
		kernelTried = true;
		hModule = ::GetModuleHandleA("KERNEL32");
		//assert(hModule != NULL); // must not call FreeLibrary on such handle

		// attempt to get GetLongPathName (implemented in Win98/2000 only!)
		(FARPROC&)pfnGetLong = ::GetProcAddress(hModule, "GetLongPathNameA");
	}

	// the kernel GetLongPathName proc is faster and (hopefully) more reliable
	if (pfnGetLong != NULL) {
		// call kernel proc
		ok = (pfnGetLong)(shortPath, longPath, _MAX_PATH) != 0;
	} else {
		char short_path[_MAX_PATH];  // copy, so we can modify it
		char* tok;

		*longPath = '\0';

		lstrcpyn(short_path, shortPath, _MAX_PATH);

		for (;;) {
			tok = strtok(short_path, "\\");
			if (tok == NULL)
				break;

			if ((strlen(shortPath) > 3) &&
			        (shortPath[0] == '\\') && (shortPath[1] == '\\')) {
				// UNC, skip first seps
				strcat(longPath, "\\\\");
				strcat(longPath, tok);
				strcat(longPath, "\\");

				tok = strtok(NULL, "\\");
				if (tok == NULL)
					break;
			}
			strcat(longPath, tok);

			bool isDir = false;

			for (;;) {
				WIN32_FIND_DATA fd;
				HANDLE hfind;
				char* tokend;

				tok = strtok(NULL, "\\");
				if (tok == NULL)
					break;

				strcat(longPath, "\\");
				tokend = longPath + strlen(longPath);

				// temporary add short component
				strcpy(tokend, tok);

				hfind = ::FindFirstFile(longPath, &fd);
				if (hfind == INVALID_HANDLE_VALUE)
					break;

				isDir = (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

				// finally add long component we got
				strcpy(tokend, fd.cFileName);

				::FindClose(hfind);
			}
			ok = tok == NULL;

			if (ok && isDir)
				strcat(longPath, "\\");

			break;
		}
	}

	if (!ok) {
		lstrcpyn(longPath, shortPath, _MAX_PATH);
	}
	return ok;
}

void SciTEWin::FixFilePath() {
	char longPath[_MAX_PATH];
	// first try MakeLongPath which corrects the path and the case of filename too
	if (MakeLongPath(fullPath, longPath)) {
		strcpy(fullPath, longPath);
		char *cpDirEnd = strrchr(fullPath, pathSepChar);
		if (cpDirEnd) {
			strcpy(fileName, cpDirEnd + 1);
			strcpy(dirName, fullPath);
			dirName[cpDirEnd - fullPath] = '\0';
		}
	} else {
		// On Windows file comparison is done case insensitively so the user can
		// enter scite.cxx and still open this file, SciTE.cxx. To ensure that the file
		// is saved with correct capitalisation FindFirstFile is used to find out the
		// real name of the file.
		WIN32_FIND_DATA FindFileData;
		HANDLE hFind = ::FindFirstFile(fullPath, &FindFileData);
		if (hFind != INVALID_HANDLE_VALUE) {	// FindFirstFile found the file
			char *cpDirEnd = strrchr(fullPath, pathSepChar);
			if (cpDirEnd) {
				strcpy(fileName, FindFileData.cFileName);
				strcpy(dirName, fullPath);
				dirName[cpDirEnd - fullPath] = '\0';
				strcpy(fullPath, dirName);
				strcat(fullPath, pathSepString);
				strcat(fullPath, fileName);
			}
			::FindClose(hFind);
		}
	}
}

/**
 * Take a filename or relative path and put it at the end of the current path.
 * If the path is absolute, return the same path.
 */
void SciTEWin::AbsolutePath(char *absPath, const char *relativePath, int size) {
	// The runtime libraries for GCC and Visual C++ give different results for _fullpath
	// so use the OS.
	*absPath = '\0';
	LPTSTR fileBit = 0;
	::GetFullPathName(relativePath, size, absPath, &fileBit);
	//Platform::DebugPrintf("AbsolutePath: <%s> -> <%s>\n", relativePath, absPath);
}

/**
 * Run a command with redirected input and output streams
 * so the output can be put in a window.
 * It is based upon several usenet posts and a knowledge base article.
 */
DWORD SciTEWin::ExecuteOne(const Job &jobToRun, bool &seenOutput) {
	DWORD exitcode = 0;
	ElapsedTime commandTime;

	if (jobToRun.jobType == jobShell) {
		ShellExec(jobToRun.command, jobToRun.directory);
		return exitcode;
	}

	if (jobToRun.jobType == jobExtension) {
		if (extender) {
			// Problem: we are in the wrong thread!  That is the cause of the cursed PC.
			// It could also lead to other problems.

			if (jobToRun.flags & jobGroupUndo)
				SendEditor(SCI_BEGINUNDOACTION);
			
			extender->OnExecute(jobToRun.command.c_str());

			if (jobToRun.flags & jobGroupUndo)
				SendEditor(SCI_ENDUNDOACTION);

			Redraw();
			// A Redraw "might" be needed, since Lua and Director
			// provide enough low-level capabilities to corrupt the
			// display.
		}
		return exitcode;
	}

	if (jobToRun.jobType == jobHelp) {
		ExecuteHelp(jobToRun.command.c_str());
		return exitcode;
	}

	if (jobToRun.jobType == jobOtherHelp) {
		ExecuteOtherHelp(jobToRun.command.c_str());
		return exitcode;
	}

	OSVERSIONINFO osv = {sizeof(OSVERSIONINFO), 0, 0, 0, 0, ""};
	::GetVersionEx(&osv);
	bool windows95 = osv.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS;

	SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), 0, 0};
	char buffer[16384];
	//Platform::DebugPrintf("Execute <%s>\n", command);
	OutputAppendStringSynchronised(">");
	OutputAppendStringSynchronised(jobToRun.command.c_str());
	OutputAppendStringSynchronised("\n");

	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;

	SECURITY_DESCRIPTOR sd;
	// If NT make a real security thing to allow inheriting handles
	if (!windows95) {
		::InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
		::SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE);
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = &sd;
	}

	HANDLE hPipeWrite = NULL;
	HANDLE hPipeRead = NULL;
	// Create pipe for output redirection
	// read handle, write handle, security attributes,  number of bytes reserved for pipe - 0 default
	::CreatePipe(&hPipeRead, &hPipeWrite, &sa, 0);

	//Platform::DebugPrintf("2Execute <%s>\n");
	// Create pipe for input redirection. In this code, you do not
	// redirect the output of the child process, but you need a handle
	// to set the hStdInput field in the STARTUP_INFO struct. For safety,
	// you should not set the handles to an invalid handle.

	hWriteSubProcess = NULL;
	subProcessGroupId = 0;
	HANDLE hRead2 = NULL;
	// read handle, write handle, security attributes,  number of bytes reserved for pipe - 0 default
	::CreatePipe(&hRead2, &hWriteSubProcess, &sa, 0);

	::SetHandleInformation(hPipeRead, HANDLE_FLAG_INHERIT, 0);
	::SetHandleInformation(hWriteSubProcess, HANDLE_FLAG_INHERIT, 0);

	//Platform::DebugPrintf("3Execute <%s>\n");
	// Make child process use hPipeWrite as standard out, and make
	// sure it does not show on screen.
	STARTUPINFO si = {
			     sizeof(STARTUPINFO), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
			 };
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	if (jobToRun.jobType == jobCLI)
		si.wShowWindow = SW_HIDE;
	else
		si.wShowWindow = SW_SHOW;
	si.hStdInput = hRead2;
	si.hStdOutput = hPipeWrite;
	si.hStdError = hPipeWrite;

	char startDirectory[_MAX_PATH];
	startDirectory[0] = '\0';
	AbsolutePath(startDirectory, jobToRun.directory.c_str(), _MAX_PATH);

	PROCESS_INFORMATION pi = {0, 0, 0, 0};

	bool running = ::CreateProcess(
			  NULL,
			  const_cast<char *>(jobToRun.command.c_str()),
			  NULL, NULL,
			  TRUE, CREATE_NEW_PROCESS_GROUP,
			  NULL,
			  startDirectory[0] ? startDirectory : NULL,
			  &si, &pi);

	if (running) {
		subProcessGroupId = pi.dwProcessId;

		bool cancelled = false;

		SString repSelBuf;

		DWORD timeDetectedDeath = 0;

		size_t totalBytesToWrite = 0;
		if (jobToRun.flags & jobHasInput) {
			totalBytesToWrite = jobToRun.input.length();
		}

		if (totalBytesToWrite > 0 && !(jobToRun.flags & jobQuiet)) {
			SString input = jobToRun.input;
			input.substitute("\n", "\n>> ");

			OutputAppendStringSynchronised(">> ");
			OutputAppendStringSynchronised(input.c_str());
			OutputAppendStringSynchronised("\n");
		}

		unsigned writingPosition = 0;

		while (running) {
			if (writingPosition >= totalBytesToWrite) {
				::Sleep(100L);
			}

			DWORD bytesRead = 0;
			DWORD bytesAvail = 0;

			if (!::PeekNamedPipe(hPipeRead, buffer,
					     sizeof(buffer), &bytesRead, &bytesAvail, NULL)) {
				bytesAvail = 0;
			}

			if ((bytesAvail < 1000) && (hWriteSubProcess != INVALID_HANDLE_VALUE) && (writingPosition < totalBytesToWrite)) {
				// There is input to transmit to the process.  Do it in small blocks, interleaved
				// with reads, so that our hRead buffer will not be overrun with results.

				size_t bytesToWrite = jobToRun.input.search("\n", writingPosition) + 1 - writingPosition;
				if ((bytesToWrite <= 0) || (writingPosition + bytesToWrite >= totalBytesToWrite)) {
					bytesToWrite = totalBytesToWrite - writingPosition;
				}
				if (bytesToWrite > 250) {
					bytesToWrite = 250;
				}

				DWORD bytesWrote = 0;

				int bTest = ::WriteFile(hWriteSubProcess,
					    const_cast<char *>(jobToRun.input.c_str() + writingPosition),
					    bytesToWrite, &bytesWrote, NULL);

				if (bTest) {
					if ((writingPosition + bytesToWrite) / 1024 > writingPosition / 1024) {
						// sleep occasionally, even when writing
						::Sleep(100L);
					}

					writingPosition += bytesWrote;

					if (writingPosition >= totalBytesToWrite) {
						if (windows95) {
							// Write a Ctrl+Z to output to mark the end of the text
							char stop[] = "\032";
							::WriteFile(hWriteSubProcess,
								stop, static_cast<DWORD>(strlen(stop)), &bytesWrote, NULL);
						}
						::CloseHandle(hWriteSubProcess);
						hWriteSubProcess = INVALID_HANDLE_VALUE;
					}

				} else {
					// Is this the right thing to do when writing to the pipe fails?
					::CloseHandle(hWriteSubProcess);
					hWriteSubProcess = INVALID_HANDLE_VALUE;
					OutputAppendStringSynchronised("\n>Input pipe closed due to write failure.\n");
				}

			} else if (bytesAvail > 0) {
				int bTest = ::ReadFile(hPipeRead, buffer,
						       sizeof(buffer), &bytesRead, NULL);

				if (bTest && bytesRead) {

					if (jobToRun.flags & jobRepSelMask) {
						repSelBuf.append(buffer, bytesRead);
					}

					if (!(jobToRun.flags & jobQuiet)) {
						if (!seenOutput) {
							MakeOutputVisible();
							seenOutput = true;
						}
						// Display the data
						OutputAppendStringSynchronised(buffer, bytesRead);
					}

					::UpdateWindow(MainHWND());
				} else {
					running = false;
				}
			} else {
				if (::GetExitCodeProcess(pi.hProcess, &exitcode)) {
					if (STILL_ACTIVE != exitcode) {
						if (windows95) {
							// Process is dead, but wait a second in case there is some output in transit
							if (timeDetectedDeath == 0) {
								timeDetectedDeath = ::GetTickCount();
							} else {
								if ((::GetTickCount() - timeDetectedDeath) >
									static_cast<unsigned int>(props.GetInt("win95.death.delay", 500))) {
									running = false;    // It's a dead process
								}
							}
						} else {	// NT, so dead already
							running = false;
						}
					}
				}
			}

			if (::InterlockedExchange(&cancelFlag, 0)) {
				if (WAIT_OBJECT_0 != ::WaitForSingleObject(pi.hProcess, 500)) {
					// We should use it only if the GUI process is stuck and
					// don't answer to a normal termination command.
					// This function is dangerous: dependant DLLs don't know the process
					// is terminated, and memory isn't released.
					OutputAppendStringSynchronised("\n>Process failed to respond; forcing abrupt termination...\n");
					::TerminateProcess(pi.hProcess, 1);
				}
				running = false;
				cancelled = true;
			}
		}

		if (WAIT_OBJECT_0 != ::WaitForSingleObject(pi.hProcess, 1000)) {
			OutputAppendStringSynchronised("\n>Process failed to respond; forcing abrupt termination...");
			::TerminateProcess(pi.hProcess, 2);
		}
		::GetExitCodeProcess(pi.hProcess, &exitcode);
		SString sExitMessage(exitcode);
		sExitMessage.insert(0, ">Exit code: ");
		if (timeCommands) {
			sExitMessage += "    Time: ";
			sExitMessage += SString(commandTime.Duration(), 3);
		}
		sExitMessage += "\n";
		OutputAppendStringSynchronised(sExitMessage.c_str());

		::CloseHandle(pi.hProcess);
		::CloseHandle(pi.hThread);

		if (!cancelled) {
			bool doRepSel = false;
			if (jobToRun.flags & jobRepSelYes)
				doRepSel = true;
			else if (jobToRun.flags & jobRepSelAuto)
				doRepSel = (0 == exitcode);

			if (doRepSel) {
				SendEditor(SCI_REPLACESEL,0,(sptr_t)(repSelBuf.c_str()));
			}
		}

		WarnUser(warnExecuteOK);

	} else {
		DWORD nRet = ::GetLastError();
		LPTSTR lpMsgBuf = NULL;
		::FormatMessage(
		    FORMAT_MESSAGE_ALLOCATE_BUFFER |
		    FORMAT_MESSAGE_FROM_SYSTEM |
		    FORMAT_MESSAGE_IGNORE_INSERTS,
		    NULL,
		    nRet,
		    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),   // Default language
		    reinterpret_cast<LPTSTR>(&lpMsgBuf),
		    0,
		    NULL
		);
		OutputAppendStringSynchronised(">");
		OutputAppendStringSynchronised(lpMsgBuf);
		::LocalFree(lpMsgBuf);
		WarnUser(warnExecuteKO);
	}
	::CloseHandle(hPipeRead);
	::CloseHandle(hPipeWrite);
	::CloseHandle(hRead2);
	::CloseHandle(hWriteSubProcess);
	hWriteSubProcess = NULL;
	subProcessGroupId = 0;
	return exitcode;
}

/**
 * Run the commands in the job queue, stopping if one fails.
 */
void SciTEWin::ProcessExecute() {
	DWORD exitcode = 0;
	if (scrollOutput)
		SendOutput(SCI_GOTOPOS, SendOutput(SCI_GETTEXTLENGTH));
	int originalEnd = SendOutput(SCI_GETCURRENTPOS);
	bool seenOutput = false;

	for (int icmd = 0; icmd < commandCurrent && icmd < commandMax && exitcode == 0; icmd++) {
		exitcode = ExecuteOne(jobQueue[icmd], seenOutput);
		if (isBuilding) {
			// The build command is first command in a sequence so it is only built if
			// that command succeeds not if a second returns after document is modified.
			isBuilding = false;
			if (exitcode == 0)
				isBuilt = true;
		}
	}

	// Move selection back to beginning of this run so that F4 will go
	// to first error of this run.
	// scroll and return only if output.scroll equals
	// one in the properties file
	if (props.GetInt("output.scroll", 1) == 1 && returnOutputToCommand)
		SendOutputEx(SCI_GOTOPOS, originalEnd, 0, false);
	returnOutputToCommand = true;
	::SendMessage(MainHWND(), WM_COMMAND, IDM_FINISHEDEXECUTE, 0);
}

void ExecThread(void *ptw) {
	SciTEWin *tw = reinterpret_cast<SciTEWin *>(ptw);
	tw->ProcessExecute();
}

struct ShellErr {
	DWORD code;
	const char *descr;
};

void SciTEWin::ShellExec(const SString &cmd, const SString &dir) {
	char *mycmd;

	// guess if cmd is an executable, if this succeeds it can
	// contain spaces without enclosing it with "
	char *mycmdcopy = StringDup(cmd.c_str());
	strlwr(mycmdcopy);

	char *mycmd_end = NULL;
	char *myparams = NULL;

	char *s = strstr(mycmdcopy, ".exe");
	if (s == NULL)
		s = strstr(mycmdcopy, ".cmd");
	if (s == NULL)
		s = strstr(mycmdcopy, ".bat");
	if (s == NULL)
		s = strstr(mycmdcopy, ".com");
	if ((s != NULL) && ((*(s + 4) == '\0') || (*(s + 4) == ' '))) {
		int len_mycmd = s - mycmdcopy + 4;
		delete []mycmdcopy;
		mycmdcopy = StringDup(cmd.c_str());
		mycmd = mycmdcopy;
		mycmd_end = mycmdcopy + len_mycmd;
	} else {
		delete []mycmdcopy;
		mycmdcopy = StringDup(cmd.c_str());
		if (*mycmdcopy != '"') {
			// get next space to separate cmd and parameters
			mycmd_end = strchr(mycmdcopy, ' ');
			mycmd = mycmdcopy;
		} else {
			// the cmd is surrounded by ", so it can contain spaces, but we must
			// strip the " for ShellExec
			mycmd = mycmdcopy + 1;
			char *s = strchr(mycmdcopy + 1, '"');
			if (s != NULL) {
				*s = '\0';
				mycmd_end = s + 1;
			}
		}
	}

	if ((mycmd_end != NULL) && (*mycmd_end != '\0')) {
		*mycmd_end = '\0';
		// test for remaining params after cmd, they may be surrounded by " but
		// we give them as-is to ShellExec
		++mycmd_end;
		while (*mycmd_end == ' ')
			++mycmd_end;

		if (*mycmd_end != '\0')
			myparams = mycmd_end;
	}

	uptr_t rc = reinterpret_cast<uptr_t>(
	                ::ShellExecute(
	                    MainHWND(),          // parent wnd for msgboxes during app start
	                    NULL,           // cmd is open
	                    mycmd,          // file to open
	                    myparams,          // parameters
	                    dir.c_str(),          // launch directory
	                    SW_SHOWNORMAL)); //default show cmd

	if (rc > 32) {
		// it worked!
		delete []mycmdcopy;
		return;
	}

	const int numErrcodes = 15;
	static const ShellErr field[numErrcodes] = {
	            { 0, "The operating system is out of memory or resources." },
	            { ERROR_FILE_NOT_FOUND, "The specified file was not found." },
	            { ERROR_PATH_NOT_FOUND, "The specified path was not found." },
	            { ERROR_BAD_FORMAT, "The .exe file is invalid (non-Win32\256 .exe or error in .exe image)." },
	            { SE_ERR_ACCESSDENIED, "The operating system denied access to the specified file." },
	            { SE_ERR_ASSOCINCOMPLETE, "The file name association is incomplete or invalid." },
	            { SE_ERR_DDEBUSY, "The DDE transaction could not be completed because other DDE transactions were being processed." },
	            { SE_ERR_DDEFAIL, "The DDE transaction failed." },
	            { SE_ERR_DDETIMEOUT, "The DDE transaction could not be completed because the request timed out." },
	            { SE_ERR_DLLNOTFOUND, "The specified dynamic-link library was not found." },
	            { SE_ERR_FNF, "The specified file was not found." },
	            { SE_ERR_NOASSOC, "There is no application associated with the given file name extension." },
	            { SE_ERR_OOM, "There was not enough memory to complete the operation." },
	            { SE_ERR_PNF, "The specified path was not found." },
	            { SE_ERR_SHARE, "A sharing violation occurred." },
	        };

	int i;
	for (i = 0; i < numErrcodes; ++i) {
		if (field[i].code == rc)
			break;
	}

	SString errormsg("Error while launching:\n\"");
	errormsg += mycmdcopy;
	if (myparams != NULL) {
		errormsg += "\" with Params:\n\"";
		errormsg += myparams;
	}
	errormsg += "\"\n";
	if (i < numErrcodes) {
		errormsg += field[i].descr;
	} else {
		errormsg += "Unknown error code: ";
		errormsg += SString(rc);
	}
	WindowMessageBox(wSciTE, errormsg, MB_OK);

	delete []mycmdcopy;
}

void SciTEWin::Execute() {
	SciTEBase::Execute();

	_beginthread(ExecThread, 1024 * 1024, reinterpret_cast<void *>(this));
}

void SciTEWin::StopExecute() {
	if (hWriteSubProcess && (hWriteSubProcess != INVALID_HANDLE_VALUE)) {
		char stop[] = "\032";
		DWORD bytesWrote = 0;
		::WriteFile(hWriteSubProcess, stop, static_cast<DWORD>(strlen(stop)), &bytesWrote, NULL);
		Sleep(500L);
	}

#ifdef USE_CONSOLE_EVENT
	if (subProcessGroupId) {
		// this also doesn't work
		OutputAppendStringSynchronised("\n>Attempting to cancel process...");

		if (!GenerateConsoleCtrlEvent(CTRL_BREAK_EVENT, subProcessGroupId)) {
			LONG errCode = GetLastError();
			OutputAppendStringSynchronised("\n>BREAK Failed ");
			OutputAppendStringSynchronised(SString(errCode).c_str());
			OutputAppendStringSynchronised("\n");
		}
		Sleep(100L);
	}
#endif

	::InterlockedExchange(&cancelFlag, 1L);
}

void SciTEWin::AddCommand(const SString &cmd, const SString &dir, JobSubsystem jobType, const SString &input, int flags) {
	if (cmd.length()) {
		if ((jobType == jobShell) && ((flags & jobForceQueue) == 0)) {
			SString pCmd = cmd;
			parameterisedCommand = "";
			if (pCmd[0] == '*') {
				pCmd.remove(0);
				parameterisedCommand = pCmd;
				if (!ParametersDialog(true)) {
					return;
				}
			} else {
				ParamGrab();
			}
			pCmd = props.Expand(pCmd.c_str());
			ShellExec(pCmd, dir);
		} else {
			SciTEBase::AddCommand(cmd, dir, jobType, input, flags);
		}
	}
}

void SciTEWin::QuitProgram() {
	if (SaveIfUnsureAll() != IDCANCEL) {
		if (fullScreen)	// Ensure tray visible on exit
			FullScreenToggle();
		::PostQuitMessage(0);
		wSciTE.Destroy();
	}
}

void SciTEWin::CreateUI() {
	int left = props.GetInt("position.left", CW_USEDEFAULT);
	int top = props.GetInt("position.top", CW_USEDEFAULT);
	int width = props.GetInt("position.width", CW_USEDEFAULT);
	int height = props.GetInt("position.height", CW_USEDEFAULT);
	if (width == -1 || height == -1) {
		cmdShow = SW_MAXIMIZE;
		width = CW_USEDEFAULT;
		height = CW_USEDEFAULT;
	}
	if (props.GetInt("position.tile") && ::FindWindow("SciTEWindow", NULL) &&
	        (left != static_cast<int>(CW_USEDEFAULT))) {
		left += width;
	}
	// Pass 'this' pointer in lpParam of CreateWindow().
	wSciTE = ::CreateWindowEx(
	             0,
	             className,
	             windowName.c_str(),
	             WS_CAPTION | WS_SYSMENU | WS_THICKFRAME |
	             WS_MINIMIZEBOX | WS_MAXIMIZEBOX |
	             WS_CLIPCHILDREN,
	             left, top, width, height,
	             NULL,
	             NULL,
	             hInstance,
	             reinterpret_cast<LPSTR>(this));
	if (!wSciTE.Created())
		exit(FALSE);

	LocaliseMenus();
	LocaliseAccelerators();
	SString pageSetup = props.Get("print.margins");
	char val[32];
	char *ps = StringDup(pageSetup.c_str());
	const char *next = GetNextPropItem(ps, val, 32);
	pagesetupMargin.left = atol(val);
	next = GetNextPropItem(next, val, 32);
	pagesetupMargin.right = atol(val);
	next = GetNextPropItem(next, val, 32);
	pagesetupMargin.top = atol(val);
	GetNextPropItem(next, val, 32);
	pagesetupMargin.bottom = atol(val);
	delete []ps;

	UIAvailable();
}

static bool IsASpace(int ch) {
	return (ch == ' ') || (ch == '\t');
}

/**
 * Break up the command line into individual arguments and strip double quotes
 * from each argument.
 * @return A string with each argument separated by '\n'.
 */
SString SciTEWin::ProcessArgs(const char *cmdLine) {
	SString args;
	const char *startArg = cmdLine;
	while (*startArg) {
		while (IsASpace(*startArg)) {
			startArg++;
		}
		const char *endArg = startArg;
		if (*startArg == '"') {	// Opening double-quote
			startArg++;
			endArg = startArg;
			while (*endArg && *endArg != '\"') {
				endArg++;
			}
		} else {	// No double-quote, end of argument on first space
			while (*endArg && !IsASpace(*endArg)) {
				endArg++;
			}
		}
		SString arg(startArg, 0, endArg - startArg);
		args.appendwithseparator(arg.c_str(), '\n');
		startArg = endArg;	// On a space or a double-quote, or on the end of the command line
		if (*startArg) {
			startArg++;
		}
	}

	return args;
}

/**
 * Process the command line, check for other instance wanting to open files,
 * create the SciTE window, perform batch processing (print) or transmit command line
 * to other instance and exit or just show the window and open files.
 */
void SciTEWin::Run(const char *cmdLine) {
	// Break up the command line into individual arguments
	SString args = ProcessArgs(cmdLine);
	// Read the command line parameters:
	// In case the check.if.already.open property has been set or reset on the command line,
	// we still get a last chance to force checking or to open a separate instance;
	// Check if the user just want to print the file(s).
	// Don't process files yet.
	bool bBatchProcessing = ProcessCommandLine(args, 0);

	// No need to check for other instances when doing a batch job:
	// perform some tasks and exit immediately.
	if (!bBatchProcessing && props.GetInt("check.if.already.open") != 0) {
		uniqueInstance.CheckOtherInstance();
	}

	// We create the window, so it can be found by EnumWindows below,
	// and the Scintilla control is thus created, allowing to print the file(s).
	// We don't show it yet, so if it is destroyed (duplicate instance), it will
	// not flash on the taskbar or on the display.
	CreateUI();

	if (bBatchProcessing) {
		// Reprocess the command line and read the files
		ProcessCommandLine(args, 1);
		Print(false);	// Don't ask user for print parameters
		// Done, we exit the program
		::PostQuitMessage(0);
		wSciTE.Destroy();
		return;
	}

	if (props.GetInt("check.if.already.open") != 0 && uniqueInstance.FindOtherInstance()) {
		uniqueInstance.SendCommands(cmdLine);

		// Kill itself, leaving room to the previous instance
		::PostQuitMessage(0);
		wSciTE.Destroy();
		return;	// Don't do anything else
	}

	// OK, the instance will be displayed
	SizeSubWindows();
	wSciTE.Show();
	if (cmdShow) {	// assume SW_MAXIMIZE only
		::ShowWindow(MainHWND(), cmdShow);
	}

	// Open all files given on command line.
	// The filenames containing spaces must be enquoted.
	// In case of not using buffers they get closed immediately except
	// the last one, but they move to the MRU file list
	ProcessCommandLine(args, 1);
	Redraw();
}

/**
 * Draw the split bar.
 */
void SciTEWin::Paint(Surface *surfaceWindow, PRectangle) {
	PRectangle rcInternal = GetClientRectangle();
	//surfaceWindow->FillRectangle(rcInternal, Colour(0xff,0x80,0x80));

	int heightClient = rcInternal.Height();
	int widthClient = rcInternal.Width();

	int heightEditor = heightClient - heightOutput - heightBar;
	int yBorder = heightEditor;
	int xBorder = widthClient - heightOutput - heightBar;
	for (int i = 0; i < heightBar; i++) {
		if (i == 1)
			surfaceWindow->PenColour(GetSysColor(COLOR_3DHIGHLIGHT));
		else if (i == heightBar - 2)
			surfaceWindow->PenColour(GetSysColor(COLOR_3DSHADOW));
		else if (i == heightBar - 1)
			surfaceWindow->PenColour(GetSysColor(COLOR_3DDKSHADOW));
		else
			surfaceWindow->PenColour(GetSysColor(COLOR_3DFACE));
		if (splitVertical) {
			surfaceWindow->MoveTo(xBorder + i, 0);
			surfaceWindow->LineTo(xBorder + i, heightClient);
		} else {
			surfaceWindow->MoveTo(0, yBorder + i);
			surfaceWindow->LineTo(widthClient, yBorder + i);
		}
	}
}

void SciTEWin::AboutDialog() {
#ifdef STATIC_BUILD
	AboutDialogWithBuild(1);
#else
	AboutDialogWithBuild(0);
#endif
}

/**
 * Open files dropped on the SciTE window.
 */
void SciTEWin::DropFiles(HDROP hdrop) {
	// If drag'n'drop inside the SciTE window but outside
	// Scintilla, hdrop is null, and an exception is generated!
	if (hdrop) {
		int filesDropped = ::DragQueryFile(hdrop, 0xffffffff, NULL, 0);
		for (int i = 0; i < filesDropped; ++i) {
			char pathDropped[MAX_PATH];
			::DragQueryFile(hdrop, i, pathDropped, sizeof(pathDropped));
			if (!Open(pathDropped)) {
				break;
			}
		}
		::DragFinish(hdrop);
		// Put SciTE to forefront
		// May not work for Win2k, but OK for lower versions
		// Note: how to drop a file to an iconic window?
		// Actually, it is the Send To command that generates a drop.
		if (::IsIconic(MainHWND())) {
			::ShowWindow(MainHWND(), SW_RESTORE);
		}
		::SetForegroundWindow(MainHWND());
	}
}

/**
 * Handle simple wild-card file patterns and directory requests.
 */
bool SciTEWin::PreOpenCheck(const char *arg) {
	bool isHandled = false;
	HANDLE hFFile;
	WIN32_FIND_DATA ffile;
	DWORD fileattributes = ::GetFileAttributes(arg);
	char filename[MAX_PATH];
	int nbuffers = props.GetInt("buffers");

	if (fileattributes != (DWORD) -1) {	// arg is an existing directory or filename
		// if the command line argument is a directory, use OpenDialog()
		if (fileattributes & FILE_ATTRIBUTE_DIRECTORY) {
			strcpy(dirName, arg);
			OpenDialog();
			isHandled = true;
		}
	} else if (nbuffers > 1 && (hFFile = ::FindFirstFile(arg, &ffile)) != INVALID_HANDLE_VALUE) {
		// If several buffers is accepted and the arg is a filename pattern matching at least an existing file
		isHandled = true;
		strcpy(filename, arg);
		char *lastslash;
		if (NULL == (lastslash = strrchr(filename, '\\')))
			lastslash = filename;	// No path
		else
			lastslash++;
		// Open files matching the given pattern until no more files or all available buffers are exhausted
		do {
			if (!(ffile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {	// Skip directories
				strcpy(lastslash, ffile.cFileName);
				Open(filename);
				--nbuffers;
			}
		} while (nbuffers > 0 && ::FindNextFile(hFFile, &ffile));
		::FindClose(hFFile);
	} else {
		const char *lastslash = strrchr(arg, '\\');
		const char *lastdot = strrchr(arg, '.');

		// if the filename is only an extension, open the dialog box with it as the extension filter
		if (lastslash && lastdot && lastslash == lastdot - 1 || !lastslash && lastdot == arg) {
			isHandled = true;

			if (lastslash) { // the arg contains a path, so copy that part to dirName
				strncpy(dirName, arg, lastslash - arg + 1);
				dirName[lastslash - arg + 1] = '\0';
			} else {
				strcpy(dirName, ".\\");
			}

			strcpy(filename, "*");
			strcat(filename, lastdot);
			strcat(filename, "|");
			strcat(filename, "*");
			strcat(filename, lastdot);
			OpenDialog(filename);
		} else if (!lastdot || lastslash && lastdot < lastslash) {
			// if the filename has no extension, try to match a file with list of standard extensions
			SString extensions = props.GetExpanded("source.default.extensions");
			if (extensions.length()) {
				strcpy(filename, arg);
				char *endfilename = filename + strlen(filename);
				extensions.substitute('|', '\0');
				size_t start = 0;
				while (start < extensions.length()) {
					const char *filterName = extensions.c_str() + start;
					strcpy(endfilename, filterName);
					if (::GetFileAttributes(filename) != (DWORD) -1) {
						isHandled = true;
						Open(filename);
						break;	// Found!
					} else {
						// Next extension
						start += strlen(extensions.c_str() + start) + 1;
					}
				}
			}
		}
	}

	return isHandled;
}

void SciTEWin::MinimizeToTray() {
	char n[64] = "SciTE";
	NOTIFYICONDATA nid;
	memset(&nid, 0, sizeof(nid));
	nid.cbSize = sizeof(nid);
	nid.hWnd = MainHWND();
	nid.uID = 1;
	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	nid.uCallbackMessage = SCITE_TRAY;
	nid.hIcon = static_cast<HICON>(
	                ::LoadImage(hInstance, "SCITE", IMAGE_ICON, 16, 16, LR_DEFAULTSIZE));
	strcpy(nid.szTip, n);
	::ShowWindow(MainHWND(), SW_MINIMIZE);
	if (::Shell_NotifyIcon(NIM_ADD, &nid)) {
		::ShowWindow(MainHWND(), SW_HIDE);
	}
}

void SciTEWin::RestoreFromTray() {
	NOTIFYICONDATA nid;
	memset(&nid, 0, sizeof(nid));
	nid.cbSize = sizeof(nid);
	nid.hWnd = MainHWND();
	nid.uID = 1;
	::ShowWindow(MainHWND(), SW_SHOW);
	::Sleep(100);
	::Shell_NotifyIcon(NIM_DELETE, &nid);
}

#ifndef VK_OEM_2
static const int VK_OEM_2=0xbf;
static const int VK_OEM_3=0xc0;
static const int VK_OEM_4=0xdb;
static const int VK_OEM_5=0xdc;
static const int VK_OEM_6=0xdd;
#endif
#ifndef VK_OEM_PLUS
static const int VK_OEM_PLUS=0xbb;
#endif

inline bool KeyMatch(const SString &sKey, int keyval, int modifiers) {
	return SciTEKeys::MatchKeyCode(
		SciTEKeys::ParseKeyCode(sKey.c_str()), keyval, modifiers);
}

LRESULT SciTEWin::KeyDown(WPARAM wParam) {
	// Look through lexer menu
	int modifiers =
	    (Platform::IsKeyDown(VK_SHIFT) ? SCMOD_SHIFT : 0) |
	    (Platform::IsKeyDown(VK_CONTROL) ? SCMOD_CTRL : 0) |
	    (Platform::IsKeyDown(VK_MENU) ? SCMOD_ALT : 0);

	for (int j = 0; j < languageItems; j++) {
		if (KeyMatch(languageMenu[j].menuKey, wParam, modifiers)) {
			SciTEBase::MenuCommand(IDM_LANGUAGE + j);
			return 1l;
		}
	}

	// loop through the Tools menu's active commands.
	HMENU hMenu = ::GetMenu(MainHWND());
	HMENU hToolsMenu = ::GetSubMenu(hMenu, menuTools);
	for (int tool_i = 0; tool_i < toolMax; ++tool_i) {
		MENUITEMINFO mii;
		mii.cbSize = sizeof(MENUITEMINFO);
		mii.fMask = MIIM_DATA;
		if (::GetMenuItemInfo(hToolsMenu, IDM_TOOLS+tool_i, FALSE, &mii) && mii.dwItemData) {
			if (SciTEKeys::MatchKeyCode(reinterpret_cast<long&>(mii.dwItemData), wParam, modifiers)) {
				SciTEBase::MenuCommand(IDM_TOOLS+tool_i);
				return 1l;
			}
		}
	}

	// loop through the keyboard short cuts defined by user.. if found
	// exec it the command defined
	for (int cut_i = 0; cut_i < shortCutItems; cut_i++) {
		if (KeyMatch(shortCutItemList[cut_i].menuKey, wParam, modifiers)) {
			int commandNum = SciTEBase::GetMenuCommandAsInt(shortCutItemList[cut_i].menuCommand);
			if (commandNum != -1) {
				// its possible that the command is for scintilla directly
				// all scintilla commands are larger then 2000
				if (commandNum < 2000) {
					SciTEBase::MenuCommand(commandNum);
				} else {
					SciTEBase::SendFocused(commandNum);
				}
				return 1l;
			}
		}
	}

	return 0l;
}

LRESULT SciTEWin::KeyUp(WPARAM) {
	return 0l;
}

void SciTEWin::AddToPopUp(const char *label, int cmd, bool enabled) {
	SString localised = LocaliseString(label);
	HMENU menu = reinterpret_cast<HMENU>(popup.GetID());
	if (0 == localised.length())
		::AppendMenu(menu, MF_SEPARATOR, 0, "");
	else if (enabled)
		::AppendMenu(menu, MF_STRING, cmd, localised.c_str());
	else
		::AppendMenu(menu, MF_STRING | MF_DISABLED | MF_GRAYED, cmd, localised.c_str());
}

LRESULT SciTEWin::ContextMenuMessage(UINT iMessage, WPARAM wParam, LPARAM lParam) {
	Window w = wEditor;
	Point pt = Point::FromLong(lParam);
	if ((pt.x == -1) && (pt.y == -1)) {
		// Caused by keyboard so display menu near caret
		if (wOutput.HasFocus())
			w = wOutput;
		int position = SendFocused(SCI_GETCURRENTPOS);
		pt.x = SendFocused(SCI_POINTXFROMPOSITION, 0, position);
		pt.y = SendFocused(SCI_POINTYFROMPOSITION, 0, position);
		POINT spt = {pt.x, pt.y};
		::ClientToScreen(static_cast<HWND>(w.GetID()), &spt);
		pt = Point(spt.x, spt.y);
	} else {
		PRectangle rcEditor = wEditor.GetPosition();
		if (!rcEditor.Contains(pt)) {
			PRectangle rcOutput = wOutput.GetPosition();
			if (rcOutput.Contains(pt)) {
				w = wOutput;
			} else {	// In frame so use default.
				return ::DefWindowProc(MainHWND(), iMessage, wParam, lParam);
			}
		}
	}
	menuSource = ::GetDlgCtrlID(reinterpret_cast<HWND>(w.GetID()));
	ContextMenu(w, pt, wSciTE);
	return 0;
}

LRESULT SciTEWin::WndProc(UINT iMessage, WPARAM wParam, LPARAM lParam) {
	//Platform::DebugPrintf("start wnd proc %x %x\n",iMessage, MainHWND());
	LRESULT uim = uniqueInstance.CheckMessage(iMessage, wParam, lParam);
	if (uim != 0) {
		return uim;
	}

	switch (iMessage) {

	case WM_CREATE:
		Creation();
		break;

	case WM_COMMAND:
		Command(wParam, lParam);
		break;

	case WM_CONTEXTMENU:
		return ContextMenuMessage(iMessage, wParam, lParam);

	case WM_ENTERMENULOOP:
		if (!wParam)
			menuSource = 0;
		break;

	case WM_SYSCOMMAND:
		if ((wParam == SC_MINIMIZE) && props.GetInt("minimize.to.tray")) {
			MinimizeToTray();
			return 0;
		}
		return ::DefWindowProc(MainHWND(), iMessage, wParam, lParam);

	case SCITE_TRAY:
		if (lParam == WM_LBUTTONDBLCLK) {
			RestoreFromTray();
			::ShowWindow(MainHWND(), SW_RESTORE);
			::FlashWindow(MainHWND(), FALSE);
		}
		break;

	case WM_NOTIFY:
		Notify(reinterpret_cast<SCNotification *>(lParam));
		break;

	case WM_KEYDOWN:
		return KeyDown(wParam);

	case WM_KEYUP:
		return KeyUp(wParam);

	case WM_SIZE:
		//Platform::DebugPrintf("size %d %x %x\n",iMessage, wParam, lParam);
		if (wParam != 1)
			SizeSubWindows();
		break;

	case WM_MOVE:
		SendEditor(SCI_CALLTIPCANCEL);
		break;

	case WM_GETMINMAXINFO: {
			MINMAXINFO *pmmi = reinterpret_cast<MINMAXINFO *>(lParam);
			if (fullScreen) {
				// Last constants for both x and y are just fiddles - don't know why they are needed
				pmmi->ptMaxSize.x = ::GetSystemMetrics(SM_CXSCREEN) +
				                    2 * ::GetSystemMetrics(SM_CXSIZEFRAME) +
				                    2;
				pmmi->ptMaxSize.y = ::GetSystemMetrics(SM_CYSCREEN) +
				                    ::GetSystemMetrics(SM_CYCAPTION) +
				                    ::GetSystemMetrics(SM_CYMENU) +
				                    2 * ::GetSystemMetrics(SM_CYSIZEFRAME) +
				                    3;
				pmmi->ptMaxTrackSize.x = pmmi->ptMaxSize.x;
				pmmi->ptMaxTrackSize.y = pmmi->ptMaxSize.y;
				return 0;
			} else {
				return ::DefWindowProc(MainHWND(), iMessage, wParam, lParam);
			}
		}

	case WM_INITMENU:
		CheckMenus();
		break;

	case WM_CLOSE:
		QuitProgram();
		return 0;

	case WM_DESTROY:
		break;

	case WM_SETTINGCHANGE:
		//Platform::DebugPrintf("** Setting Changed\n");
		SendEditor(WM_SETTINGCHANGE, wParam, lParam);
		SendOutput(WM_SETTINGCHANGE, wParam, lParam);
		break;

	case WM_SYSCOLORCHANGE:
		//Platform::DebugPrintf("** Color Changed\n");
		SendEditor(WM_SYSCOLORCHANGE, wParam, lParam);
		SendOutput(WM_SYSCOLORCHANGE, wParam, lParam);
		break;

	case WM_PALETTECHANGED:
		//Platform::DebugPrintf("** Palette Changed\n");
		if (wParam != reinterpret_cast<WPARAM>(MainHWND())) {
			SendEditor(WM_PALETTECHANGED, wParam, lParam);
			//SendOutput(WM_PALETTECHANGED, wParam, lParam);
		}

		break;

	case WM_QUERYNEWPALETTE:
		//Platform::DebugPrintf("** Query palette\n");
		SendEditor(WM_QUERYNEWPALETTE, wParam, lParam);
		//SendOutput(WM_QUERYNEWPALETTE, wParam, lParam);
		return TRUE;

	case WM_ACTIVATEAPP:
		SendEditor(SCI_HIDESELECTION, !wParam);
		// Do not want to display dialog yet as may be in middle of system mouse capture
		::PostMessage(MainHWND(), WM_COMMAND, IDM_ACTIVATE, wParam);
		break;

	case WM_ACTIVATE:
		//Platform::DebugPrintf("Focus: w:%x l:%x %x e=%x o=%x\n", wParam, lParam, ::GetFocus(), wEditor.GetID(), wOutput.GetID());
		if (wParam != WA_INACTIVE) {
			::SetFocus(wFocus);
		}
		break;

	case WM_DROPFILES:
		DropFiles(reinterpret_cast<HDROP>(wParam));
		break;

	case WM_COPYDATA:
		return uniqueInstance.CopyData(reinterpret_cast<COPYDATASTRUCT *>(lParam));

	default:
		//Platform::DebugPrintf("default wnd proc %x %d %d\n",iMessage, wParam, lParam);
		return ::DefWindowProc(MainHWND(), iMessage, wParam, lParam);
	}
	//Platform::DebugPrintf("end wnd proc\n");
	return 0l;
}

// Take care of 32/64 bit pointers
#ifdef GetWindowLongPtr
static void *PointerFromWindow(HWND hWnd) {
	return reinterpret_cast<void *>(::GetWindowLongPtr(hWnd, 0));
}
static void SetWindowPointer(HWND hWnd, void *ptr) {
	::SetWindowLongPtr(hWnd, 0, reinterpret_cast<LONG_PTR>(ptr));
}
#else
static void *PointerFromWindow(HWND hWnd) {
	return reinterpret_cast<void *>(::GetWindowLong(hWnd, 0));
}
static void SetWindowPointer(HWND hWnd, void *ptr) {
	::SetWindowLong(hWnd, 0, reinterpret_cast<LONG>(ptr));
}
#endif

LRESULT PASCAL SciTEWin::TWndProc(
    HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	//Platform::DebugPrintf("W:%x M:%d WP:%x L:%x\n", hWnd, iMessage, wParam, lParam);

	// Find C++ object associated with window.
	SciTEWin *scite = reinterpret_cast<SciTEWin *>(PointerFromWindow(hWnd));
	// scite will be zero if WM_CREATE not seen yet
	if (scite == 0) {
		if (iMessage == WM_CREATE) {
			LPCREATESTRUCT cs = reinterpret_cast<LPCREATESTRUCT>(lParam);
			scite = reinterpret_cast<SciTEWin *>(cs->lpCreateParams);
			scite->wSciTE = hWnd;
			SetWindowPointer(hWnd, scite);
			return scite->WndProc(iMessage, wParam, lParam);
		} else
			return ::DefWindowProc(hWnd, iMessage, wParam, lParam);
	} else
		return scite->WndProc(iMessage, wParam, lParam);
}

LRESULT SciTEWin::WndProcI(UINT iMessage, WPARAM wParam, LPARAM lParam) {
	switch (iMessage) {

	case WM_COMMAND:
		Command(wParam, lParam);
		break;

	case WM_NOTIFY:
		Notify(reinterpret_cast<SCNotification *>(lParam));
		break;

	case WM_PAINT: {
			PAINTSTRUCT ps;
			::BeginPaint(reinterpret_cast<HWND>(wContent.GetID()), &ps);
			Surface *surfaceWindow = Surface::Allocate();
			if (surfaceWindow) {
				surfaceWindow->Init(ps.hdc, wContent.GetID());
				PRectangle rcPaint(ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right, ps.rcPaint.bottom);
				Paint(surfaceWindow, rcPaint);
				surfaceWindow->Release();
				delete surfaceWindow;
			}
			::EndPaint(reinterpret_cast<HWND>(wContent.GetID()), &ps);
			return 0;
		}

	case WM_LBUTTONDOWN:
		ptStartDrag = Point::FromLong(lParam);
		capturedMouse = true;
		heightOutputStartDrag = heightOutput;
		::SetCapture(reinterpret_cast<HWND>(wContent.GetID()));
		//Platform::DebugPrintf("Click %x %x\n", wParam, lParam);
		break;

	case WM_MOUSEMOVE:
		if (capturedMouse) {
			MoveSplit(Point::FromLong(lParam));
		}
		break;

	case WM_LBUTTONUP:
		if (capturedMouse) {
			MoveSplit(Point::FromLong(lParam));
			capturedMouse = false;
			::ReleaseCapture();
		}
		break;

	case WM_CAPTURECHANGED:
		capturedMouse = false;
		break;

	case WM_SETCURSOR:
		if (ControlIDOfCommand(lParam) == HTCLIENT) {
			Point ptCursor;
			::GetCursorPos(reinterpret_cast<POINT *>(&ptCursor));
			Point ptClient = ptCursor;
			::ScreenToClient(MainHWND(), reinterpret_cast<POINT *>(&ptClient));
			if ((ptClient.y > (visHeightTools + visHeightTab)) && (ptClient.y < visHeightTools + visHeightTab + visHeightEditor)) {
				PRectangle rcScintilla = wEditor.GetPosition();
				PRectangle rcOutput = wOutput.GetPosition();
				if (!rcScintilla.Contains(ptCursor) && !rcOutput.Contains(ptCursor)) {
					wSciTE.SetCursor(splitVertical ? Window::cursorHoriz : Window::cursorVert);
					return TRUE;
				}
			}
		}
		return ::DefWindowProc(MainHWND(), iMessage, wParam, lParam);

	default:
		//Platform::DebugPrintf("default wnd proc %x %d %d\n",iMessage, wParam, lParam);
		return ::DefWindowProc(reinterpret_cast<HWND>(wContent.GetID()),
		                       iMessage, wParam, lParam);
	}
	//Platform::DebugPrintf("end wnd proc\n");
	return 0l;
}

LRESULT PASCAL SciTEWin::IWndProc(
    HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	//Platform::DebugPrintf("W:%x M:%d WP:%x L:%x\n", hWnd, iMessage, wParam, lParam);

	// Find C++ object associated with window.
	SciTEWin *scite = reinterpret_cast<SciTEWin *>(::PointerFromWindow(hWnd));
	// scite will be zero if WM_CREATE not seen yet
	if (scite == 0) {
		if (iMessage == WM_CREATE) {
			LPCREATESTRUCT cs = reinterpret_cast<LPCREATESTRUCT>(lParam);
			scite = reinterpret_cast<SciTEWin *>(cs->lpCreateParams);
			scite->wContent = hWnd;
			SetWindowPointer(hWnd, scite);
			return scite->WndProcI(iMessage, wParam, lParam);
		} else
			return ::DefWindowProc(hWnd, iMessage, wParam, lParam);
	} else
		return scite->WndProcI(iMessage, wParam, lParam);
}

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int) {

#ifdef NO_EXTENSIONS
	Extension *extender = 0;
#else
	MultiplexExtension multiExtender;
	Extension *extender = &multiExtender;

#ifndef NO_LUA
	SingleThreadExtension luaAdapter(LuaExtension::Instance());
	multiExtender.RegisterExtension(luaAdapter);
#endif

#ifndef NO_FILER
	multiExtender.RegisterExtension(DirectorExtension::Instance());
#endif
#endif
	//Platform::DebugPrintf("Command line is \n%s\n<<", lpszCmdLine);

	SciTEWin::Register(hInstance);
#ifdef STATIC_BUILD

	Scintilla_LinkLexers();
	Scintilla_RegisterClasses(hInstance);
#else

	HMODULE hmod = ::LoadLibrary("SciLexer.DLL");
	if (hmod == NULL)
		::MessageBox(NULL, "The Scintilla DLL could not be loaded.  SciTE will now close", "Error loading Scintilla", MB_OK | MB_ICONERROR);
#endif

	MSG msg;
	msg.wParam = 0;
	{
		SciTEWin MainWind(extender);
		MainWind.Run(lpszCmdLine);
		bool going = true;
		while (going) {
			going = ::GetMessage(&msg, NULL, 0, 0);
			if (going) {
				if (!MainWind.ModelessHandler(&msg)) {
					if (::TranslateAccelerator(reinterpret_cast<HWND>(MainWind.GetID()), MainWind.GetAcceleratorTable(), &msg) == 0) {
						::TranslateMessage(&msg);
						::DispatchMessage(&msg);
					}
				}
			}
		}
	}

#ifdef STATIC_BUILD
	Scintilla_ReleaseResources();
#else

	::FreeLibrary(hmod);
#endif

	return msg.wParam;
}