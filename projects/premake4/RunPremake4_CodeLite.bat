for /f %%i in ("%0") do SET VS2010_BATCHPATH=%%~dpi

SET PM4OUTSUBDIR=PM4CODELITE
SET PM4TARGETSUBDIR=PM4CODELITE\bin
SET NOPAUSE=TRUE

CALL Premake4Common.bat

ECHO *** Set CodeLite specifics and run Pemake 4 ***

CD %GUCEF_HOME%
%VS2010_BATCHPATH%\premake4.exe codelite
PAUSE