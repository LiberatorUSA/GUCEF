@echo off

REM SET BUIDUSINGNDK_THEBATCHDIR=%CD%
SET BUIDUSINGNDK_THEBATCHDIR=%~dp0

ECHO.
ECHO *** Starting build from MS Windows ***
ECHO.

IF NOT DEFINED GUCEF_HOME (
  ECHO GUCEF environment variable not found, setting it
  SET GUCEF_HOME=%BUIDUSINGNDK_THEBATCHDIR%..\..\..
)
REM ECHO GUCEF_HOME="%GUCEF_HOME%"

ECHO Script location for FindCygwin: %GUCEF_HOME%\projects\Android\FindCygwin.bat

ECHO Invoking FindCygwin script
ECHO.

SET OLD_NOPAUSE=%NOPAUSE%
SET NOPAUSE="true"

CALL %GUCEF_HOME%\projects\Android\FindCygwin.bat

SET NOPAUSE=%OLD_NOPAUSE%

IF DEFINED CYGWIN_ROOT (
  
  ECHO.
  ECHO *** Invoking BuildUsingNDK.sh using bash ***
  ECHO.
  
  rem ECHO Adding Cygwin binary paths to PATH
  rem SET PATH="%BUIDUSINGNDK_THEBATCHDIR%";"%CYGWIN_ROOT%\bin";"%PATH%"

  REM ECHO Changing working directory to cygwin's bin
  REM CD /D "%CYGWIN_ROOT%\bin"

  CD /D %BUIDUSINGNDK_THEBATCHDIR%

  ECHO Undefining GUCEF_HOME so we don't mess up the bash version
  SET GUCEF_HOME=

  %CYGWIN_ROOT%\bin\bash.exe BuildUsingNDK.sh --login -i
)

IF NOT DEFINED CYGWIN_ROOT (

  ECHO.
  ECHO Failure: The FindCygwin script was unable to locate cygwin, we cannot proceed!
)


IF NOT DEFINED NOPAUSE (
  PAUSE
)