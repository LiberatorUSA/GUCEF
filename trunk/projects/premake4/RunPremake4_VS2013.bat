for /f %%i in ("%0") do SET VS2013_BATCHPATH=%%~dpi

SET PM4OUTSUBDIR=PM4MVC12
SET NOPAUSE=TRUE

CALL Premake4Common.bat

ECHO *** Set MVC12 specifics and run Pemake 4 ***

CD %GUCEF_HOME%
%VS2013_BATCHPATH%\premake4.exe vs2013
PAUSE