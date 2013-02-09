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

IF NOT DEFINED SKIP_GUCEF_PREMAKE4INFOGENERATION (
  ECHO *** Generate Premake4 files ***
  CALL GeneratePremake4Info.bat
)
IF DEFINED SKIP_GUCEF_PREMAKE4INFOGENERATION (
  ECHO Skipping GUCEF's Premake4 file generation
)

cd "%BATCHSTARTDIR%"

ECHO *** Perform common Premake4 environment variable setup ***

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
