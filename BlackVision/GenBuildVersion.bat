SETLOCAL ENABLEEXTENSIONS
setlocal enabledelayedexpansion

set version=%1
set patch=%2
set scriptDirectory=%~dp0
cd %scriptDirectory%

set versionFilePath=%scriptDirectory%\LibBlackVision\Source\Version\BuildVersion.h


set content=const int BuildVersion = %version%; const int PatchVersion = %patch%;
echo %content%  > %versionFilePath%

