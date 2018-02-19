SETLOCAL ENABLEEXTENSIONS
setlocal enabledelayedexpansion

set buildsDir=%1
set outputDir=%2

set scriptDirectory=%~dp0


cd %scriptDirectory%

set buildsInputDir=%buildsDir%
set buildsOutputDir=%outputDir%

call python PrepareRelease.py %buildsInputDir% %buildsOutputDir%
