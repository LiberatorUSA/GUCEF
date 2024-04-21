@echo off

SET GUCEF_THEBATCHDIR=%~dp0

ECHO *** Perform common environment variable setup ***

cd..
cd..

IF NOT DEFINED GUCEF_HOME (
  ECHO GUCEF environment variable not found, setting it
  SET GUCEF_HOME=%CD%
  ECHO GUCEF_HOME=%CD%
)

SET SRCROOTDIR=%CD%
SET OUTPUTDIR=%GUCEF_HOME%\common\bin\Arduino
SET GUCEF_SOURCE_DIR=%SRCROOTDIR%

cd %GUCEF_THEBATCHDIR%

ECHO SRCROOTDIR: %SRCROOTDIR%
ECHO GUCEF_HOME: %GUCEF_HOME%
ECHO root OUTPUTDIR: %OUTPUTDIR%


cd "%GUCEF_THEBATCHDIR%"


REM -----------------------------------------------------

:END

IF NOT DEFINED NOPAUSE (
  PAUSE
)