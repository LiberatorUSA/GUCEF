@echo off

SET COPYGUCEFBINARIES_THEBATCHDIR=%~dp0
SET GUCEFLOADERBINS=%COPYGUCEFBINARIES_THEBATCHDIR%\GUCEF\0.0\

ECHO *** Enable command extensions ***

VERIFY OTHER 2>nul
SETLOCAL ENABLEEXTENSIONS
IF ERRORLEVEL 1 (
  REM ERROR: Unable to enable extensions
  PAUSE
)
ENDLOCAL

IF NOT DEFINED COMPILERTAG (
  ECHO *** Defining compiler tag to default ***
  SET %COMPILERTAG%=MVC9
)


echo Go to binary output folder for Debug
CD ..\..\..\common\bin\%COMPILERTAG%\bin\Debug

echo Copy all binaries we need for the test

copy gucefMT_d.dll %GUCEFLOADERBINS%\gucefMT\0.0
copy gucefCORE_d.dll %GUCEFLOADERBINS%\gucefCORE\0.0
copy gucefCOMCORE_d.dll %GUCEFLOADERBINS%\gucefCOMCORE\0.0
copy gucefCOM_d.dll %GUCEFLOADERBINS%\gucefCOM\0.0
copy gucefVFS_d.dll %GUCEFLOADERBINS%\gucefVFS\0.0
copy gucefIMAGE_d.dll %GUCEFLOADERBINS%\gucefIMAGE\0.0
copy gucefINPUT_d.dll %GUCEFLOADERBINS%\gucefINPUT\0.0
copy gucefDRN_d.dll %GUCEFLOADERBINS%\gucefDRN\0.0
copy gucefGUI_d.dll %GUCEFLOADERBINS%\gucefGUI\0.0
copy gucefMATH_d.dll %GUCEFLOADERBINS%\gucefMATH\0.0
copy gucefPATCHER_d.dll %GUCEFLOADERBINS%\gucefPATCHER\0.0

echo Go to binary output folder for Release
CD %COPYGUCEFBINARIES_THEBATCHDIR%
CD ..\..\..\common\bin\%COMPILERTAG%\bin\Release

echo Copy all binaries we need for the test

copy gucefMT.dll %GUCEFLOADERBINS%\gucefMT\0.0
copy gucefCORE.dll %GUCEFLOADERBINS%\gucefCORE\0.0
copy gucefCOMCORE.dll %GUCEFLOADERBINS%\gucefCOMCORE\0.0
copy gucefCOM.dll %GUCEFLOADERBINS%\gucefCOM\0.0
copy gucefVFS.dll %GUCEFLOADERBINS%\gucefVFS\0.0
copy gucefIMAGE.dll %GUCEFLOADERBINS%\gucefIMAGE\0.0
copy gucefINPUT.dll %GUCEFLOADERBINS%\gucefINPUT\0.0
copy gucefDRN.dll %GUCEFLOADERBINS%\gucefDRN\0.0
copy gucefGUI.dll %GUCEFLOADERBINS%\gucefGUI\0.0
copy gucefMATH.dll %GUCEFLOADERBINS%\gucefMATH\0.0
copy gucefPATCHER.dll %GUCEFLOADERBINS%\gucefPATCHER\0.0