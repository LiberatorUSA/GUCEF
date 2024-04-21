@echo off

SET BATCHSTARTDIR=%CD%

ECHO *** Enable command extensions ***

VERIFY OTHER 2>nul
SETLOCAL ENABLEEXTENSIONS
IF ERRORLEVEL 1 (
  REM ERROR: Unable to enable extensions
  PAUSE
)
ENDLOCAL

ECHO *** Perform common Premake5 environment variable setup ***

cd..
cd..

SET GUCEF_HOME=%CD%
SET SRCROOTDIR=%CD%
SET OUTPUTDIR=%GUCEF_HOME%\common\bin

IF NOT DEFINED OIS_HOME (
  ECHO OIS environment variable not found, setting it
  SET OIS_HOME=%GUCEF_HOME%\dependencies\OIS
)
ECHO OIS_HOME = %OIS_HOME%

IF NOT DEFINED FREEIMAGE_HOME (
  ECHO FreeImage environment variable not found, setting it
  SET FREEIMAGE_HOME=%GUCEF_HOME%\dependencies\FreeImage
)
ECHO FREEIMAGE_HOME = %FREEIMAGE_HOME%
  
IF NOT DEFINED DEVIL_HOME (
  ECHO DevIL environment variable not found, setting it
  SET DEVIL_HOME=%GUCEF_HOME%\dependencies\Devil
)
ECHO DEVIL_HOME = %DEVIL_HOME%

IF NOT DEFINED ZLIB_HOME (
  ECHO ZLib environment variable not found, setting it
  SET ZLIB_HOME=%GUCEF_HOME%\dependencies\zlib
)
ECHO ZLIB_HOME = %ZLIB_HOME%

IF NOT DEFINED ZZIPLIB_HOME (
  ECHO zzipLib environment variable not found, setting it
  SET ZZIPLIB_HOME=%GUCEF_HOME%\dependencies\zziplib
)
ECHO ZZIPLIB_HOME = %ZZIPLIB_HOME%

IF NOT DEFINED PM5OUTPUTDIR (
  ECHO PM5OUTPUTDIR environment variable not found, setting it
  SET PM5OUTPUTDIR=%GUCEF_HOME%\common\bin\premake5
)
IF DEFINED PM5OUTSUBDIR (
  ECHO PM5OUTSUBDIR environment variable found, appending premake4 output path
  SET PM5OUTPUTDIR=%PM5OUTPUTDIR%\%PM5OUTSUBDIR%
)

IF NOT DEFINED PM5TARGETDIR (
  ECHO PM5TARGETDIR environment variable not found, setting it
  SET PM5TARGETDIR=%GUCEF_HOME%\common\bin\premake5
)
IF DEFINED PM5TARGETSUBDIR (
  ECHO PM5TARGETSUBDIR environment variable found, appending premake5 output path
  SET PM5TARGETDIR=%PM5TARGETDIR%\%PM5TARGETSUBDIR%
)



cd "%BATCHSTARTDIR%"

