SETLOCAL ENABLEEXTENSIONS
setlocal enabledelayedexpansion

set arch=%1
set configuration=%2
set toolset=%3
set scriptDirectory=%~dp0
set testOutputDir=%4
set consoleReport=""


cd %scriptDirectory%

set TEST_EXECUTABLES_DIRECTORY=_Builds\%arch%-%toolset%-%configuration%\Tests\


cd %TEST_EXECUTABLES_DIRECTORY%

for /r %%g in (*.exe) do (
	set fileDir=%%~dpg
	set fileName=%%~ng
	
	cd !fileDir!
	
	echo Executing test: !fileName!
	call %%g -o %testOutputDir%!fileName!.xml -FileLog %testOutputDir%Logs/!fileName!Log.txt debug -DisableDefaultLog
	echo Execution ended
	
	cd %scriptDirectory%%TEST_EXECUTABLES_DIRECTORY%
)

