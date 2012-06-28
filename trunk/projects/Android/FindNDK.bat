@echo off

SET FINDNDK_THEBATCHDIR=%~dp0

GOTO FIND_CMDCAPABLE_NDK

REM -----------------------------------------------------

:FIND_ANDROIDNDK_R8

SET NDKTESTROOT=%GUCEF_HOME%\dependencies\android-ndk-r8
SET CMDTEST=%NDKTESTROOT%\ndk-build.cmd

IF EXIST "%CMDTEST%" (
  ECHO Found NDK .cmd at %CMDTEST%
  SET CMDCAPABLE_NDK=%NDKTESTROOT%
  GOTO :eof
)

ECHO Unable to locate .cmd capable NDK at %NDKTESTROOT%
GOTO :eof

REM -----------------------------------------------------

:FIND_ANDROIDNDK_R7c

SET NDKTESTROOT=%GUCEF_HOME%\dependencies\android-ndk-r7c
SET CMDTEST=%NDKTESTROOT%\ndk-build.cmd

IF EXIST "%CMDTEST%" (
  ECHO Found NDK .cmd at %CMDTEST%
  SET CMDCAPABLE_NDK=%NDKTESTROOT%
  GOTO :eof
)

ECHO Unable to locate .cmd capable NDK at %NDKTESTROOT%
GOTO :eof

REM -----------------------------------------------------

:FIND_ANDROIDNDK_R7b

SET NDKTESTROOT=%GUCEF_HOME%\dependencies\android-ndk-r7b
SET CMDTEST=%NDKTESTROOT%\ndk-build.cmd

IF EXIST "%CMDTEST%" (
  ECHO Found NDK .cmd at %CMDTEST%
  SET CMDCAPABLE_NDK=%NDKTESTROOT%
  GOTO :eof
)

ECHO Unable to locate .cmd capable NDK at %NDKTESTROOT%
GOTO :eof

REM -----------------------------------------------------

:FIND_ANDROIDNDK_R7

SET NDKTESTROOT=%GUCEF_HOME%\dependencies\android-ndk-r7
SET CMDTEST=%NDKTESTROOT%\ndk-build.cmd

IF EXIST "%CMDTEST%" (
  ECHO Found NDK .cmd at %CMDTEST%
  SET CMDCAPABLE_NDK=%NDKTESTROOT%
  GOTO :eof
)

ECHO Unable to locate .cmd capable NDK at %NDKTESTROOT%
GOTO :eof

#------------------------------------------------------------------------------

:FIND_CMDCAPABLE_NDK

ECHO.
ECHO *** Starting search .cmd capable MS Windows NDK ***
ECHO.

IF NOT DEFINED GUCEF_HOME (
  ECHO GUCEF environment variable not found, setting it
  SET GUCEF_HOME=%FINDNDK_THEBATCHDIR%..\..
)
REM ECHO GUCEF_HOME="%GUCEF_HOME%"

REM Clear the NDK path variable
SET CMDCAPABLE_NDK=

CALL :FIND_ANDROIDNDK_R8

IF DEFINED CMDCAPABLE_NDK (
  ECHO Found .cmd capable NDK at %CMDCAPABLE_NDK%
  GOTO END_OF_BATCH
)

CALL :FIND_ANDROIDNDK_R7c

IF DEFINED CMDCAPABLE_NDK (
  ECHO Found .cmd capable NDK at %CMDCAPABLE_NDK%
  GOTO END_OF_BATCH
)

CALL :FIND_ANDROIDNDK_R7b

IF DEFINED CMDCAPABLE_NDK (
  ECHO Found .cmd capable NDK at %CMDCAPABLE_NDK%
  GOTO END_OF_BATCH
)

CALL :FIND_ANDROIDNDK_R7

IF DEFINED CMDCAPABLE_NDK (
  ECHO Found .cmd capable NDK at %CMDCAPABLE_NDK%
  GOTO END_OF_BATCH
)

ECHO Failed to locate any .cmd capable NDK
GOTO END_OF_BATCH

REM -----------------------------------------------------

:END_OF_BATCH

IF NOT DEFINED NOPAUSE PAUSE