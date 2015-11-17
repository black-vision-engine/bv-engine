@ECHO OFF

:: Check for Python Installation
python --version 2>NUL
if ERRORLEVEL 1 GOTO NOPYTHON

:: Generate latest gl version loader and copy files to LibBVGL\Source\GLCore with bindless extension
:: (generator tested with Python 2.7.8)
python ..\Dep\3rdParty\glad\main.py --generator=c --out-path="..\LibBVGL\Source\GLCore" --profile=core --api="gl=" --extensions=GL_ARB_bindless_texture
if ERRORLEVEL 1 GOTO GLADERROR

ECHO GLCore files generated successfully.

GOTO :EOF

:NOPYTHON
ECHO Error: Python not installed.

:GLADERROR
ECHO Error: GLCore files not generated.