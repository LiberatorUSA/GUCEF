for /f %%i in ("%0") do SET VS2010_BATCHPATH=%%~dpi

SET PM4OUTSUBDIR=PM4MVC10
SET NOPAUSE=TRUE

CALL Premake4Common.bat

ECHO *** Set MVC10 specifics and run Pemake 4 ***

CD %GUCEF_HOME%
%VS2010_BATCHPATH%\premake4.exe vs2010
PAUSE