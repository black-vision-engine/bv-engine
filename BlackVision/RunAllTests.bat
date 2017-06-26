SETLOCAL ENABLEEXTENSIONS
setlocal enabledelayedexpansion

set arch=%1
set configuration=%2
set toolset=%3
set scriptDirectory=%~dp0
set testOutputDir=%4
set consoleReport=""

if "%4"=="" set consoleReport=-ReportToConsole


cd %scriptDirectory%

set TEST_EXECUTABLES_DIRECTORY=_Builds\%arch%-%toolset%-%configuration%\Tests\


cd %scriptDirectory%%TEST_EXECUTABLES_DIRECTORY%

for /r %%g in (*.exe) do (
	set fileDir=%%~dpg
	set fileName=%%~ng
	set fileWithExt=%%~nxg
	
	cd !fileDir!
	echo Working directory: !fileDir!
	
	echo Executing test: !fileName!
	call !fileWithExt! -o %scriptDirectory%%testOutputDir%!fileName!.xml -FileLog %scriptDirectory%%testOutputDir%Logs/!fileName!Log.txt debug -DisableDefaultLog %consoleReport%
	echo Execution ended
	
	cd %scriptDirectory%%TEST_EXECUTABLES_DIRECTORY%
)

