for /f %%i in ("%0") do SET CB_BATCHPATH=%%~dpi

SET PM5OUTSUBDIR=PM5CB
SET PM5TARGETSUBDIR=PM5CB\bin
SET NOPAUSE=TRUE

CALL Premake5Common.bat

ECHO *** Set CodeBlocks specifics and run Pemake 5 ***

CD %GUCEF_HOME%
%CB_BATCHPATH%\premake5.exe codeblocks
PAUSE