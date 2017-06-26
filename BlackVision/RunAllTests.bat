SETLOCAL ENABLEEXTENSIONS
setlocal enabledelayedexpansion

set arch=%1
set configuration=%2
set toolset=%3
set scriptDirectory=%~dp0
set testOutputDir=%4
set consoleReport=""
set gtestParams=

cd %scriptDirectory%

set testOutputDir=%scriptDirectory%%testOutputDir%

if "%4"=="" set consoleReport=-ReportToConsole
if NOT "%4"=="" set gtestParams=--gtest_output=xml:%testOutputDir%

REM Clean output directory only if test output directory is valid.
REM Otherwise user wants to have all reports in console and we don't have to clean this directory.
if NOT "%4"=="" @RD /S /Q "%testOutputDir%"

set GTEST_EXECUTABLES_DIRECTORY=_Builds\%arch%-%toolset%-%configuration%\Tests\GTest\
set FRAMEWORKTEST_EXECUTABLES_DIRECTORY=_Builds\%arch%-%toolset%-%configuration%\Tests\Framework\


REM Call all framework tests

cd %scriptDirectory%%FRAMEWORKTEST_EXECUTABLES_DIRECTORY%

for /r %%g in (*.exe) do (
	set fileDir=%%~dpg
	set fileName=%%~ng
	set fileWithExt=%%~nxg
	
	cd !fileDir!
	echo Working directory: !fileDir!
	
	echo Executing test: !fileName!
	call !fileWithExt! -o %testOutputDir%!fileName!.xml -FileLog %testOutputDir%Logs/!fileName!Log.txt debug -DisableDefaultLog %consoleReport%
	echo Execution ended
	
	cd %scriptDirectory%%FRAMEWORKTEST_EXECUTABLES_DIRECTORY%
)


REM Call all gtests

cd %scriptDirectory%%GTEST_EXECUTABLES_DIRECTORY%

for /r %%g in (*.exe) do (
	set fileDir=%%~dpg
	set fileName=%%~ng
	set fileWithExt=%%~nxg
	
	cd !fileDir!
	echo Working directory: !fileDir!
	
	echo Executing test: !fileName!
	call !fileWithExt! %gtestParams%
	echo Execution ended
	
	cd %scriptDirectory%%GTEST_EXECUTABLES_DIRECTORY%
)

