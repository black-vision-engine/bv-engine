SETLOCAL ENABLEEXTENSIONS
setlocal enabledelayedexpansion

set arch=%1
set configuration=%2
set toolset=%3
set benchmarksOutput=%4
set scriptDirectory=%~dp0


cd %scriptDirectory%

set testOutputDir=%scriptDirectory%%testOutputDir%

call python RunBenchmarks.py %arch% %configuration% %toolset% %benchmarksOutput%
