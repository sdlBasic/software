; Script generated by the HM NIS Edit Script Wizard.

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "sdlBasic"
!define PRODUCT_VERSION "2005-01-29"
!define PRODUCT_PUBLISHER "__vroby__"
!define PRODUCT_WEB_SITE "http://sdlbasic.sourceforge.net"
;!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\makensis.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"
!define PRODUCT_STARTMENU_REGVAL "NSIS:StartMenuDir"

SetCompressor lzma

; MUI 1.67 compatible ------
!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; License page
!insertmacro MUI_PAGE_LICENSE "sdlBasic\doc\english\lgpl.txt"
; Components page
!insertmacro MUI_PAGE_COMPONENTS
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Start menu page
var ICONS_GROUP
!define MUI_STARTMENUPAGE_NODISABLE
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "sdlBasic"
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "${PRODUCT_STARTMENU_REGVAL}"
!insertmacro MUI_PAGE_STARTMENU Application $ICONS_GROUP
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
;!define MUI_FINISHPAGE_RUN "$INSTDIR\makensis.exe"
!define MUI_FINISHPAGE_SHOWREADME "$INSTDIR\doc\english\QuickHelp.html"
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "English"

; Reserve files
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS

; MUI end ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "sdlBasic-setup.exe"
InstallDir "$PROGRAMFILES\sdlBasic"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

Section "runtime" SEC01
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
;  File "${NSISDIR}\makensis.exe"
  CreateDirectory "$SMPROGRAMS\$ICONS_GROUP"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\clickme.lnk" '$INSTDIR\clickme.sdlbas"'
  File "${NSISDIR}\License.txt"
  SetOutPath "$SYSDIR"
  File "packages\sdlBasic-win32\SDL_ttf.dll"
  File "packages\sdlBasic-win32\SDL_net.dll"
  File "packages\sdlBasic-win32\SDL_mixer.dll"
  File "packages\sdlBasic-win32\smpeg.dll"
  File "packages\sdlBasic-win32\SDL_image.dll"
  File "packages\sdlBasic-win32\SDL.dll"
  File "packages\sdlBasic-win32\libpng1.dll"
  File "packages\sdlBasic-win32\jpeg.dll"
  File "packages\sdlBasic-win32\libtiff.dll"
  File "packages\sdlBasic-win32\zlib.dll"
  SetOutPath "$WINDIR\fonts"
  File "sdlBasic\shared\DejaVuSans.ttf"
  File "sdlBasic\shared\DejaVuSansMono-Roman.ttf"
  SetOutPath "$WINDIR"
  File "sdlBasic\installer\win32\sdlBasic.exe"
  SetOutPath "$INSTDIR"
  File "sdlBasic\sdlBasic.png"
  File "sdlBasic\sdlnow.gif"
  File "sdlBasic\lizard.mod"
  File "sdlBasic\clickme.sdlbas"
SectionEnd

Section "documentation" SEC02
  SetOutPath "$INSTDIR\doc\english"
  File "sdlBasic\doc\english\lgpl.txt"
  File "sdlBasic\doc\english\keycode.txt"
  File "sdlBasic\doc\english\sdlBasic.txt"
  File "sdlBasic\doc\english\QuickHelp.html"
  File "sdlBasic\doc\english\style.css"
  File "sdlBasic\doc\english\logo.jpg"
  File "sdlBasic\doc\english\motd.png"
  File "sdlBasic\doc\english\pattern.png"
  SetOutPath "$INSTDIR\doc\english\font"
  File "sdlBasic\doc\english\fonts\LICENSE"
  SetOutPath "$INSTDIR\doc\italiano"
  File "sdlBasic\doc\italiano\lgpl.txt"
  File "sdlBasic\doc\italiano\keycode.txt"
  File "sdlBasic\doc\italiano\sdlBasic.txt"
  SetOutPath "$INSTDIR\doc\italiano\font"
  File "sdlBasic\doc\italiano\fonts\LICENSE"
SectionEnd

Section -AdditionalIcons
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Website.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section -Post
  ;register association
  WriteRegStr HKCR ".sdlbas" "" "sdlBasicFile"
  WriteRegStr HKCR "sdlBasicFile" "" "sdlBasic program runtime"
  WriteRegStr HKCR "sdlBasicFile\shell" "" "open"
  WriteRegStr HKCR "sdlBasicFile\DefaultIcon" "" "$WINDIR\sdlBasic.exe,3"
  WriteRegStr HKCR "sdlBasicFile\shell\open\command" "" '$WINDIR\sdlBasic.exe "%1"'

  WriteRegStr HKCR ".sdlblib" "" "sdlBasicLibrary"
  WriteRegStr HKCR "sdlBasicLibrary" "" "sdlBasic program runtime"
  WriteRegStr HKCR "sdlBasicLibrary\DefaultIcon" "" "$WINDIR\sdlBasic.exe,1"






  WriteUninstaller "$INSTDIR\uninst.exe"
;  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\makensis.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
;  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\makensis.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "${PRODUCT_STARTMENU_REGVAL}" "$ICONS_GROUP"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd

; Section descriptions
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC01} "run time software"
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC02} "preliminary documentation (italiano-english available)"
!insertmacro MUI_FUNCTION_DESCRIPTION_END


Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) � stato completamente rimosso dal tuo computer."
FunctionEnd

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Sei sicuro di voler completamente rimuovere $(^Name) e tutti i suoi componenti?" IDYES +2
  Abort
FunctionEnd

Section Uninstall
  ReadRegStr $ICONS_GROUP ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "${PRODUCT_STARTMENU_REGVAL}"
  Delete "$INSTDIR\${PRODUCT_NAME}.url"
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\doc\italiano\font\COPYRIGHT.TXT"
  Delete "$INSTDIR\doc\italiano\sdlBasic.txt"
  Delete "$INSTDIR\doc\italiano\keycode.txt"
  Delete "$INSTDIR\doc\italiano\lgpl.txt"
  Delete "$INSTDIR\doc\italiano\Reference.txt"
  Delete "$INSTDIR\doc\english\font\COPYRIGHT.TXT"
  Delete "$INSTDIR\doc\english\sdlBasic.txt"
  Delete "$INSTDIR\doc\english\keycode.txt"
  Delete "$INSTDIR\doc\english\lgpl.txt"
  Delete "$INSTDIR\sdlBasic.ico"
  Delete "$INSTDIR\sdlBasic.bmp"
  Delete "$INSTDIR\sdlBasic.gif"
  Delete "$WINDIR\sdlbasic.bat"
  Delete "$WINDIR\sdlBasic.exe"
  Delete "$WINDIR\fonts\VeraMono.ttf"
  Delete "$WINDIR\fonts\Vera.ttf"
  Delete "$SYSDIR\zlib.dll"
  Delete "$SYSDIR\jpeg.dll"
  Delete "$SYSDIR\libpng1.dll"
  Delete "$SYSDIR\SDL.dll"
  Delete "$SYSDIR\SDL_image.dll"
  Delete "$SYSDIR\SDL_mixer.dll"
  Delete "$SYSDIR\SDL_net.dll"
  Delete "$SYSDIR\SDL_ttf.dll"
  Delete "$INSTDIR\License.txt"
  ;Delete "$INSTDIR\makensis.exe"

  Delete "$SMPROGRAMS\$ICONS_GROUP\Uninstall.lnk"
  Delete "$SMPROGRAMS\$ICONS_GROUP\Website.lnk"
  Delete "$DESKTOP\sdlBasic.lnk"
  Delete "$SMPROGRAMS\$ICONS_GROUP\sdlBasic.lnk"

  RMDir "$WINDIR\fonts"
  RMDir "$WINDIR"
  RMDir "$SYSDIR"
  RMDir "$SMPROGRAMS\$ICONS_GROUP"
  RMDir "$INSTDIR\doc\italiano\font"
  RMDir "$INSTDIR\doc\italiano"
  RMDir "$INSTDIR\doc\english\font"
  RMDir "$INSTDIR\doc\english"
  RMDir "$INSTDIR"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  ;DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd
