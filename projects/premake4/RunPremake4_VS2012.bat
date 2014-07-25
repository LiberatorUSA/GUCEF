for /f %%i in ("%0") do SET VS2012_BATCHPATH=%%~dpi

SET PM4OUTSUBDIR=PM4MVC11
SET NOPAUSE=TRUE

CALL Premake4Common.bat

ECHO *** Set MVC10 specifics and run Pemake 4 ***

CD %GUCEF_HOME%
%VS2012_BATCHPATH%\premake4.exe vs2012
PAUSE