for /f %%i in ("%0") do SET PM5_BATCHPATH=%%~dpi

SET PM5OUTSUBDIR=PM5CODELITE
SET PM5TARGETSUBDIR=PM5CODELITE\bin
SET NOPAUSE=TRUE

CALL Premake5Common.bat

ECHO *** Set CodeLite specifics and run Pemake 5 ***

CD %GUCEF_HOME%
%PM5_BATCHPATH%\premake5.exe codelite
PAUSE