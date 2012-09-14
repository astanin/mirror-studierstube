REM This batch file builds the documentation and opens the index page with explorer.
REM This batch file checks the existance of html directory.
REM if none it creates one.
REM Author: Michele Fiorentino
ECHO Argument (mandatory):  build or clean
ECHO   Make sure you have installed doxygen.
ECHO   If not, download it from www.doxygen.org

cd %STB4ROOT%\doc
set htmldir=doc\html

REM ************************************************************************
if %1==clean (goto cleanup)

REM ************************************************************************
:build

if exist %htmldir%\*.* goto htmldirexists
mkdir %htmldir%
REM ************************************************************************
:htmldirexists

doxygen.exe studierstube_v4.dox
echo Running doxygen (it may take a while)...
explorer %htmldir%\index.html
pause
exit 0

REM ************************************************************************
: cleanup
REM check extension for security (do not calcels code!)
echo Cleaning (it may take a while)...
del %htmldir%\*.png
del %htmldir%\*.html
del %htmldir%\*.gif
del %htmldir%\*.css
del %htmldir%\*.md5
del %htmldir%\*.dot
echo Cleaning Done!

exit 0