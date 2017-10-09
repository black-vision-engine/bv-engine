SETLOCAL ENABLEEXTENSIONS
setlocal enabledelayedexpansion

set version=%1
set scriptDirectory=%~dp0
cd %scriptDirectory%

set versionFilePath=%scriptDirectory%\LibBlackVision\Source\Version\BuildVersion.h


set content=const int BuildVersion = %version%;
echo %content%  > %versionFilePath%

