set PFILES=c:\\programmi
mkdir %PROGRAMFILES%\sdlBasic
copy sdlBasic.exe %PROGRAMFILES%\sdlBasic
copy ..\..\shared\*.ttf %WINDIR%\fonts

echo Windows Registry Editor Version 5.00 >sdlBasic.reg

echo [HKEY_CLASSES_ROOT\sdlBasicFile] >>sdlBasic.reg
echo @="sdlBasic program runtime" >>sdlBasic.reg

echo [HKEY_CLASSES_ROOT\sdlBasicFile\DefaultIcon] >>sdlBasic.reg
echo @="%PFILES%\\sdlBasic\\sdlBasic.exe,3" >>sdlBasic.reg

echo [HKEY_CLASSES_ROOT\sdlBasicFile\shell] >>sdlBasic.reg
echo @="open" >>sdlBasic.reg

echo [HKEY_CLASSES_ROOT\sdlBasicFile\shell\open] >>sdlBasic.reg

echo [HKEY_CLASSES_ROOT\sdlBasicFile\shell\open\command] >>sdlBasic.reg
echo @="%PFILES%\\sdlBasic\\sdlBasic.exe \"%%1\"" >>sdlBasic.reg


start sdlBasic.reg
