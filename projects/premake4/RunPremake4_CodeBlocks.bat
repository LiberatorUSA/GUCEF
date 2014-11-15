for /f %%i in ("%0") do SET CB_BATCHPATH=%%~dpi

SET PM4OUTSUBDIR=PM4CB
SET PM4TARGETSUBDIR=PM4CB\bin
SET NOPAUSE=TRUE

CALL Premake4Common.bat

ECHO *** Set CodeBlocks specifics and run Pemake 4 ***

CD %GUCEF_HOME%
%CB_BATCHPATH%\premake4.exe codeblocks
PAUSE