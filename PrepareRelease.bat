SETLOCAL ENABLEEXTENSIONS
setlocal enabledelayedexpansion

set buildsDir=%1
set outputDir=%2

call python PrepareRelease.py %buildsDir% %outputDir%
