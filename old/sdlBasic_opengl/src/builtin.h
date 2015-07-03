/*
    Name:       builtin.h
    Purpose:    builtin routines for wxBasic
    Author:     David Cuny
    Copyright:  (c) 2001 David Cuny <dcuny@lanset.com>
    Licence:    LGPL
*/

void notYet( char *name);
void basPrintStack(void);
void basConcat(void);
void basConvBase(void);
void basIndexes(void);
//_________________________________________________________________________________________________________________________
// Run time
void basArgc(void);
void basArgv(void);
void basCommand(void);
void basRun(void);
void basShell(void);
//_________________________________________________________________________________________________________________________
// Array
void basLBound(void);
void basQuickSort(void);
void basUBound(void);
//_________________________________________________________________________________________________________________________
//Strings
void basAsc(void);
void basChr(void);
void basFormat(void);
void basInsert(void);
void basInstr(void);
void basLCase(void);
void basLeft(void);
void basLen(void);
void basLTrim(void);
void basMid(void);
void basReplace(void);
void basReplaceSubStr(void);
void basReverse(void);
void basRight(void);
void basRInstr(void);
void basRTrim(void);
void basSpace(void);
void basStr(void);
void basStrF(void);
void basString(void);
void basTrim(void);
void basTally(void);
void basTypeOf(void);
void basUCase(void);
void basVal(void);

//_________________________________________________________________________________________________________________________
//Maths
void basAbs(void);
void basACos(void);
void basASin(void);
void basATan(void);
void basCos(void);
void basExp(void);
void basFix(void);
void basFrac(void);
void basHex(void);
void basInt(void);
void basLog(void);
void basRandomize(void);
void basRnd(void);
void basRound(void);
void basSgn(void);
void basSin(void);
void basSqr(void);
void basTan(void);

//_________________________________________________________________________________________________________________________
//Files
void basEof(void);
void basFileExists(void);
void basFileCopy(void);
void basFileMove(void);
void basFileRename(void);
void basFreeFile(void);
void basKill(void);
void basLoc(void);
void basLof(void);
void basReadByte(void);
void basRename(void);
void basSeek(void);
void basWriteByte(void);

//_________________________________________________________________________________________________________________________
//Dirs
void basChDir(void);
void basDir(void);
void basDirExists(void);
void basDirFirst(void);
void basDirNext(void);
void basMkDir(void);
void basRmDir(void);

//_________________________________________________________________________________________________________________________
//Date and time
void basDate(void);
void basTicks(void);
void basTime(void);


//_________________________________________________________________________________________________________________________
//
//---- SDLengine --------------------------------------------------------------------------------------------------
//_________________________________________________________________________________________________________________________
// Screen
void basSetDisplay(void);
void basDisplayWidth(void);
void basDisplayHeight(void);
void basDisplayBpp(void);

void basScreen(void);
void basDirectScreen(void);

void basScreenOpen(void);
void basScreenClose(void);
void basScreenClone(void);

void basScreenCopy(void);
void basScreenFade(void);
void basScreenFadein(void);
void basScreenCrossFade(void);
void basScreenAlpha(void);
void basScreenLock(void);
void basScreenUnlock(void);

void basScreenRect(void);
void basXScreenRect(void);
void basYScreenRect(void);
void basWScreenRect(void);
void basHScreenRect(void);
void basFlagScreenRect(void);

void basScreenWidth(void);
void basScreenHeight(void);

void basOffset(void);
void basXOffset(void);
void basYOffset(void);

void basCls(void);
void basScreenSwap(void);
void basAutoBack(void);
void basDualPlayfield(void);
void basWaitVbl(void);
void basFps(void);
//_________________________________________________________________________________________________________________________
// Graphics
void basRgb(void);
void basColor(void);
void basPalette(void);
void basInk(void);

void basPoint(void);
void basDot(void);
void basPlot(void);
void basLine(void);
void basBox(void);
void basBar(void);
void basCircle(void);
void basFillCircle(void);
void basEllipse(void);
void basFillEllipse(void);
void basPaint(void);
//_________________________________________________________________________________________________________________________
// File graphics input output
void basLoadImage(void);
void basSaveImage(void);
void basLoadSound(void);
void basSoundExists(void);
void basLoadMusic(void);
void basMusicExists(void);
//_________________________________________________________________________________________________________________________
// Image manipulation
void basHotSpot(void);
void basImageExists(void);
void basImageWidth(void);
void basImageHeight(void);
void basDeleteImage(void);
void basCopyImage(void);
void basSetAlpha(void);
void basZoomImage(void);
void basRotateImage(void);
void basRotozoomImage(void);
//_________________________________________________________________________________________________________________________
// Blitting
void basBlt(void);
void basPasteBob(void);
void basPasteIcon(void);
void basGrab(void);
//_________________________________________________________________________________________________________________________
// Sprite system
void basSpriteClip(void);
void basSprite(void);
void basDeleteSprite(void);
void basXSprite(void);
void basYSprite(void);
void basSpriteWidth(void);
void basSpriteHeight(void);
void basFrSprite(void);
void basLiveSprite(void);
void basSpriteHit(void);
void basAutoUpdateSprite(void);
void basUpdateSprite(void);
//_________________________________________________________________________________________________________________________
// Bob system
void basSetBob(void);
void basBob(void);
void basDeleteBob(void);
void basXBob(void);
void basYBob(void);
void basBobWidth(void);
void basBobHeight(void);
void basFrBob(void);
void basLiveBob(void);
void basBobHit(void);
void basAutoUpdateBob(void);
void basUpdateBob(void);
//_________________________________________________________________________________________________________________________
//Text draw
void basText(void);
void basSetFont(void);
void basTextRender(void);
//_________________________________________________________________________________________________________________________
// Print screen
void basPen(void);
void basPaper(void);
void basfPrints(void);
void basPrints(void);
void basLocate(void);
void basAtx(void);
void basAty(void);
void basCursOn(void);
void basCursOff(void);
void basInputs(void);
void basZoneInputs(void);
//_________________________________________________________________________________________________________________________
// Sounds and music
void basPlaySound(void);
void basPlayFreqSound(void);
void basVolumeSound(void);
void basStopSound(void);
void basPauseSound(void);
void basResumeSound(void);
void basVuMeterSound(void);
void basPositionSound(void);
void basSoundChannels(void);

void basPlayMusic(void);
void basPositionMusic(void);
void basStopMusic(void);
void basPauseMusic(void);
void basResumeMusic(void);
void basRewindMusic(void);
void basFadeMusic(void);
void basVolumeMusic(void);
void basSpeedMusic(void);
//_________________________________________________________________________________________________________________________
// Keyboard
void basKey(void);
void basInkey(void);
void basWaitKey(void);
//_________________________________________________________________________________________________________________________
// Mouse
void basXMouse(void);
void basYMouse(void);
void basXMouseScreen(void);
void basYMouseScreen(void);
void basBMouse(void);
void basMouseShow(void);
void basMouseHide(void);
void basMouseZone(void);
//_________________________________________________________________________________________________________________________
// Joystick
void basXJoy(void);
void basYJoy(void);
void basBJoy(void);
//_________________________________________________________________________________________________________________________
// SDLtime
void basWait(void);
void basTimer(void);

//_________________________________________________________________________________________________________________________
//
//---- GLengine --------------------------------------------------------------------------------------------------
//_________________________________________________________________________________________________________________________
// GLscreen
void basGLsetdisplay(void);
void basGLscreenswap(void);
void basGLscreen2D(void);
void basGLscreenrect(void);
//_________________________________________________________________________________________________________________________
// GLcamera
void basGLcameraposition(void);
void basGLcameraangle(void);
void basGLcameralook(void);
//_________________________________________________________________________________________________________________________
// GLlight
//_________________________________________________________________________________________________________________________
//    GLgraphics
void basGLcolor(void);
//_________________________________________________________________________________________________________________________
// GLtextures
void basGLloadtexture(void);
void basGLmaketexture(void);
void basGLtexture(void);
//_________________________________________________________________________________________________________________________
// GLobject
void basGLmakecube(void);
void basGLmakesphere(void);
void basGLmakesurface(void);
//_________________________________________________________________________________________________________________
// GLmatrix
void basGLbegin(void);
void basGLend(void);
void basGLnormal(void);
void basGLtexcoord(void);
void basGLsetvertex(void);
void basGLpushmatrix(void);
void basGLpopmatrix(void);
void basGLflush(void);
//_________________________________________________________________________________________________________________
// GLlowlevel
void basGLvertex(void);

void basGLtranslate(void);
void basGLrotate(void);
void basGLenable(void);



//_________________________________________________________________________________________________________________
void addBuiltin( char *name, void (*routine)(void), int args, int optargs );
void initBuiltins(void);


static struct {     /* builtin routines */
    char    *name;
    void    (*routine)(void);
    int     args;
    int     optargs;
} builtin[] = {
    {"printstack",      basPrintStack,      0,  0},
    {"convbase",        basConvBase,        2,  0},
    {"convbase$",       basConvBase,        2,  0},

//    Run time
    {"argc",            basArgc,            0,  0},
    {"argv",            basArgv,            1,  0},
    {"command",         basCommand,         0,  1},
    {"command$",        basCommand,         0,  1},
    {"run",             basRun,             1,  0},
    {"shell",           basShell,           1,  0},

//    Array
    {"indexes",         basIndexes,         1,  0},
    {"lbound",          basLBound,          2,  0},
    {"quicksort",       basQuickSort,       3,  0},
    {"ubound",          basUBound,          2,  0},

//    Strings
    {"chr",             basChr,             1,  0},
    {"chr$",            basChr,             1,  0},
    {"format",          basFormat,          1,  20},
    {"format$",         basFormat,          1,  20},
    {"insert",          basInsert,          3,  0},
    {"insert$",         basInsert,          3,  0},
    {"instr",           basInstr,           2,  1},
    {"lcase",           basLCase,           1,  0},
    {"lcase$",          basLCase,           1,  0},
    {"left",            basLeft,            2,  0},
    {"left$",           basLeft,            2,  0},
    {"len",             basLen,             1,  0},
    {"length",          basLen,             1,  0},
    {"ltrim",           basLTrim,           1,  0},
    {"ltrim$",          basLTrim,           1,  0},
    {"mid",             basMid,             2,  1},
    {"mid$",            basMid,             2,  1},
    {"replace",         basReplace,         2,  1},
    {"replace$",        basReplace,         2,  1},
    {"replacesubstr",   basReplaceSubStr,   3,  0},
    {"replacesubstr$",  basReplaceSubStr,   3,  0},
    {"reverse",         basReverse,         1,  0},
    {"reverse$",        basReverse,         1,  0},
    {"right",           basRight,           2,  0},
    {"right$",          basRight,           2,  0},
    {"rinstr",          basRInstr,          2,  1},
    {"rtrim",           basRTrim,           1,  0},
    {"rtrim$",          basRTrim,           1,  0},
    {"space",           basSpace,           1,  0},
    {"space$",          basSpace,           1,  0},
    {"str",             basStr,             1,  0},
    {"str$",            basStr,             1,  0},
    {"strf",            basStrF,            4,  0},
    {"strf$",           basStrF,            4,  0},
    {"string",          basString,          2,  0},
    {"string$",         basString,          2,  0},
    {"trim",            basTrim,            1,  0},
    {"trim$",           basTrim,            1,  0},
    {"tally",           basTally,           2,  1},
    {"ucase",           basUCase,           1,  0},
    {"ucase$",          basUCase,           1,  0},
    {"typeof$",         basTypeOf,          1,  0},
    {"typeof",          basTypeOf,          1,  0},

//    Maths
    {"abs",             basAbs,             1,  0},
    {"acos",            basACos,            1,  0},
    {"asc",             basAsc,             1,  0},
    {"asin",            basASin,            1,  0},
    {"atan",            basATan,            1,  0},
    {"cos",             basCos,             1,  0},
    {"exp",             basExp,             1,  0},
    {"fix",             basFix,             1,  0},
    {"floor",           basFix,             1,  0},
    {"frac",            basFrac,            1,  0},
    {"hex",             basHex,             1,  0},
    {"hex$",            basHex,             1,  0},
    {"int",             basInt,             1,  0},
    {"log",             basLog,             1,  0},
    {"randomize",       basRandomize,       0,  1},
    {"rnd",             basRnd,             0,  1},
    {"round",           basRound,           1,  0},
    {"sgn",             basSgn,             1,  0},
    {"sin",             basSin,             1,  0},
    {"sqr",             basSqr,             1,  0},
    {"tan",             basTan,             1,  0},
    {"val",             basVal,             1,  0},

//    Files
    {"eof",             basEof,             1,  0},
    {"fileexists",      basFileExists,      1,  0},
    {"filecopy",        basFileCopy  ,      2,  0},
    {"filemove",        basFileMove  ,      2,  0},
    {"filerename",      basFileRename,      2,  0},
    {"freefile",        basFreeFile,        0,  0},
    {"kill",            basKill,            1,  0},
    {"loc",             basLoc,             1,  0},
    {"lof",             basLof,             1,  0},
    {"readbyte",        basReadByte,        1,  0},
    {"rename",          basRename,          2,  0},
    {"seek",            basSeek,            1,  1},
    {"writebyte",       basWriteByte,       2,  0},

//    Dirs
    {"chdir",           basChDir,           1,  0},
    {"dir",		basDir,             0,  0},
    {"dir$",		basDir,             0,  0},
    {"dirfirst",        basDirFirst,        1,  0},
    {"dirnext",		basDirNext,         0,  0},
    {"direxists",       basDirExists,       1,  0},
    {"mkdir",           basMkDir,           1,  0},
    {"rmdir",           basRmDir,           1,  0},

//    Date and time
    {"date",            basDate,            0,  0},
    {"date$",           basDate,            0,  0},
    {"ticks",           basTicks,           0,  0},
    {"time",            basTime,            0,  0},
    {"time$",           basTime,            0,  0},


/***************** SDLengine *********************/
//    Screen
    {"setdisplay",      basSetDisplay,      4,  0},
    {"displaywidth",    basDisplayWidth,    0,  0},
    {"displayheight",   basDisplayHeight,   0,  0},
    {"displaybpp",      basDisplayBpp,      0,  0},

    {"screen",          basScreen,	    0,  1},
    {"directscreen",    basDirectScreen,    0,  0},
    {"screenopen",      basScreenOpen,	    8,  0},
    {"screenclose",     basScreenClose,	    1,  0},
    {"screenclone",     basScreenClone,	    7,  0},

    {"screencopy",      basScreenCopy,	    8,  0},
    {"screenfade",      basScreenFade,	    2,  0},
    {"screenfadein",    basScreenFadein,    3,  0},
    {"screencrossfade", basScreenCrossFade, 3,  0},
    {"screenalpha",     basScreenAlpha,     2,  0},
    {"screenlock",      basScreenLock,      1,  0},
    {"screenunlock",    basScreenUnlock,    1,  0},

    {"screenrect",      basScreenRect,	    5,  0},
    {"xscreenrect",     basXScreenRect,	    0,  0},
    {"yscreenrect",     basYScreenRect,	    0,  0},
    {"wscreenrect",     basWScreenRect,	    0,  0},
    {"hscreenrect",     basHScreenRect,	    0,  0},
    {"flagscreenrect",  basFlagScreenRect,  0,  0},

    {"screenwidth",     basScreenWidth,	    0,  0},
    {"screenheight",    basScreenHeight,    0,  0},

    {"offset",   		basOffset,	    2,  0},
    {"xoffset",   		basXOffset,   	    0,  0},
    {"yoffset",   		basYOffset,         0,  0},

    {"cls",				basCls,     	    0,  0},
    {"screenswap",      basScreenSwap,      0,  0},
    {"setautoback",     basAutoBack,        0,  1},
    {"autoback",    	basAutoBack,        0,  1},
    {"dualplayfield",   basDualPlayfield,   0,  1},
    {"waitvbl",     	basWaitVbl,         0,  0},
    {"fps",         	basFps,             0,  1},

//    Graphics
    {"rgb",     	basRgb,      	    3,  0},
    {"color",     	basColor,      	    1,  1},
    {"palette",     	basPalette,         0,  255},
    {"ink",     	basInk,      	    1,  0},

    {"point",     	basPoint,      	    2,  0},
    {"dot",     	basDot,      	    2,  0},
    {"plot",     	basPlot,      	    3,  0},
    {"line",     	basLine,      	    4,  0},
    {"box",     	basBox,      	    4,  0},
    {"bar",     	basBar,      	    4,  0},
    {"circle",     	basCircle,     	    3,  0},
    {"fillcircle",      basFillCircle, 	    3,  0},
    {"ellipse",         basEllipse,   	    4,  0},
    {"fillellipse",     basFillEllipse,	    4,  0},
    {"paint",		basPaint,	    2,  0},

//    File graphics input output

    {"loadimage",       basLoadImage,	    2,  0},
    {"saveimage",       basSaveImage,	    2,  0},
    {"loadsound",       basLoadSound,	    2,  0},
    {"soundexists",     basSoundExists,	    1,  0},
    {"loadmusic",       basLoadMusic,	    1,  0},
    {"musicexists",  	basMusicExists,	    0,  0},

//    Image manipulation
    {"hotspot",      	basHotSpot,	    3,  0},
    {"imageexists",  	basImageExists,	    1,  0},
    {"imagewidth",   	basImageWidth,	    1,  0},
    {"imageheight",  	basImageHeight,	    1,  0},
    {"deleteimage",  	basDeleteImage,	    1,  0},
    {"copyimage",    	basCopyImage,	    2,  0},
    {"setalpha",     	basSetAlpha,	    2,  0},
    {"zoomimage",     	basZoomImage,	    3,  0},
    {"rotateimage",    	basRotateImage,	    2,  0},
    {"rotozoomimage",  	basRotozoomImage,   3,  0},

//    Blitting
    {"blt", 	      	basBlt,	            7,  0},
    {"pastebob",      	basPasteBob,        3,  0},
    {"pasteicon",     	basPasteIcon,       3,  0},
    {"grab",          	basGrab,            5,  0},

//    Sprite system
    {"spriteclip",    	basSpriteClip,      4,  0},
    {"sprite",        	basSprite,          4,  0},
    {"deletesprite",  	basDeleteSprite,    1,  0},
    {"xsprite",       	basXSprite,         1,  0},
    {"ysprite",       	basYSprite,         1,  0},
    {"spritewidth",   	basSpriteWidth,     1,  0},
    {"spriteheight",  	basSpriteHeight,    1,  0},
    {"frsprite",      	basFrSprite,        1,  0},
    {"livesprite",    	basLiveSprite,      1,  0},
    {"spritehit",     	basSpriteHit,       1,  1},
    {"autoupdatesprite",basAutoUpdateSprite,1,  0},
    {"updatesprite",  	basUpdateSprite,    0,  0},

//    Bob system
    {"setbob",        	basSetBob,          2,  0},
    {"bob",           	basBob    ,         4,  0},
    {"deletebob",     	basDeleteBob,       1,  0},
    {"xbob",          	basXBob,            1,  0},
    {"ybob",          	basYBob,            1,  0},
    {"bobwidth",      	basBobWidth,        1,  0},
    {"bobheight",     	basBobHeight,       1,  0},
    {"frbob",         	basFrBob,           1,  0},
    {"livebob",       	basLiveBob,         1,  0},
    {"bobhit",        	basBobHit,          1,  1},
    {"autoupdatebob", 	basAutoUpdateBob,   1,  0},
    {"updatebob",     	basUpdateBob,       0,  0},

//    Text draw
    {"text",          	basText,            4,  0},
    {"setfont",       	basSetFont,         1,  0},
    {"textrender",    	basTextRender,      3,  0},

//    Print screen
    {"pen",    	      	basPen,             0,  1},
    {"paper",         	basPaper,           0,  1},
    {"fprints",       	basfPrints,         1,  0},
    {"prints",        	basPrints,          1,  0},
    {"locate",        	basLocate,          2,  0},
    {"atx",           	basAtx,             0,  0},
    {"aty",           	basAty,             0,  0},
    {"curson",        	basCursOn,          0,  0},
    {"cursoff",       	basCursOff,         0,  0},
    {"inputs",        	basInputs,          2,  0},
    {"zoneinputs",    	basZoneInputs,      4,  0},

//    Sounds and music
    {"playsound",     	basPlaySound,       2,  1},
    {"playfreqsound",   basPlayFreqSound,   3,  1},
    {"volumesound",   	basVolumeSound,     1,  1},
    {"stopsound",     	basStopSound,       0,  1},
    {"pausesound",     	basPauseSound,      0,  1},
    {"resumesound",     basResumeSound,     0,  1},
    {"vumetersound",    basVuMeterSound,    0,  1},
    {"positionsound",    basPositionSound,  3,  0},
    {"soundchannels",    basSoundChannels,  1,  0},

    {"playmusic",     	basPlayMusic,       1,  0},
    {"positionmusic",  	basPositionMusic,   1,  0},
    {"stopmusic",     	basStopMusic,       0,  0},
    {"pausemusic",     	basPauseMusic,      0,  0},
    {"resumemusic",    	basResumeMusic,     0,  0},
    {"rewindmusic",    	basRewindMusic,     0,  0},
    {"fademusic",     	basFadeMusic,       1,  0},
    {"volumemusic",   	basVolumeMusic,     0,  1},
    {"speedmusic",   	basSpeedMusic,      1,  0},

//    Keyboard
    {"key",           	basKey,             1,  0},
    {"inkey",         	basInkey,           0,  0},
    {"waitkey",       	basWaitKey,         0,  1},

//    Mouse
    {"xmouse",        	basXMouse,          0,  0},
    {"ymouse",        	basYMouse,          0,  0},
    {"xmousescreen",  	basXMouseScreen,    1,  0},
    {"ymousescreen",  	basYMouseScreen,    1,  0},
    {"bmouse",        	basBMouse,          0,  0},
    {"mouseshow",     	basMouseShow,       0,  0},
    {"mousehide",     	basMouseHide,       0,  0},
    {"mousezone",     	basMouseZone,       4,  0},

//    Joystick
    {"xjoy",          	basXJoy,            0,  0},
    {"yjoy",          	basYJoy,            0,  0},
    {"bjoy",          	basBJoy,            0,  0},

//    SDLtime
    {"wait",          	basWait,            1,  0},
    {"timer",         	basTimer,           0,  0},

/***************** GLengine *********************/
//    GLscreen
    {"glsetdisplay",    basGLsetdisplay,    4,  0},
    {"glscreenswap",    basGLscreenswap,    0,  0},
    {"glscreen2d",      basGLscreen2D,      0,  1},
    {"glscreenrect",    basGLscreenrect,    4,  0},

//    GLcamera
    {"glcameraposition",basGLcameraposition,3,  0},
    {"glcameraangle",	basGLcameraangle   ,3,  0},
    {"glcameralook",	basGLcameralook   ,3,  0},
//    GLlight


//    GLgraphics
    {"glcolor",        basGLcolor,      3,  0},


//    GLtextures
    {"glloadtexture",   basGLloadtexture,   2,  0},
    {"glmaketexture",   basGLmaketexture,   2,  0},
    {"gltexture",   	basGLtexture,       1,  0},

//    GLobject
    {"glmakecube",      basGLmakecube,     4,  0},
    {"glmakesphere",      basGLmakesphere,     4,  0},
    {"glmakesurface",  basGLmakesurface, 4,  0},

//    GLmatrix
    {"glbegin",  basGLbegin, 1,  0},
    {"glend",  basGLend, 0,  0},
    {"glnormal",  basGLnormal, 3,  0},
    {"gltexcoord",  basGLtexcoord, 2,  0},
    {"glsetvertex",  basGLsetvertex, 1,  2},
    {"glpushmatrix",  basGLpushmatrix, 0,  0},
    {"glpopmatrix",  basGLpopmatrix, 0,  0},
    {"glflush",  basGLflush, 0,  0},

//    GLlowlevel
    {"glvertex",        basGLvertex  ,      4,  0},

    {"gltranslate",     basGLtranslate,    3,  0},
    {"glrotate",      	basGLrotate,       4,  0},
    {"glenable",      	basGLenable,       1,  0},



    {NULL,              NULL,               0,  0}
};

