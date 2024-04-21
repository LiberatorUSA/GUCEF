@echo off

SET BATCHSTARTDIR=%~dp0

ECHO *** Enable command extensions ***

VERIFY OTHER 2>nul
SETLOCAL ENABLEEXTENSIONS
IF ERRORLEVEL 1 (
  REM ERROR: Unable to enable extensions
  PAUSE
)
ENDLOCAL


ECHO *** Add CMake dir to PATH ***
SET CMAKE_EXE=%BATCHSTARTDIR%\cmake\bin
SET PATH=%PATH%;"%CMAKE_EXE%"
ECHO CMake binary is here: %CMAKE_EXE%

cd "%BATCHSTARTDIR%"

ECHO *** Perform common CMake environment variable setup ***

cd..
cd..

SET GUCEF_HOME=%CD%
SET SRCROOTDIR=%CD%
SET OUTPUTDIR=%GUCEF_HOME%\common\bin
SET GUCEF_SOURCE_DIR=%SRCROOTDIR%

ECHO SRCROOTDIR: %SRCROOTDIR%
ECHO GUCEF_HOME: %GUCEF_HOME%
ECHO root OUTPUTDIR: %OUTPUTDIR%


