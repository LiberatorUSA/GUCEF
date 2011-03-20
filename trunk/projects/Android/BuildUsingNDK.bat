@echo off

REM SET BUIDUSINGNDK_THEBATCHDIR=%CD%
SET BUIDUSINGNDK_THEBATCHDIR=%~dp0

ECHO.
ECHO *** Starting build from MS Windows ***
ECHO.

IF NOT DEFINED GUCEF_HOME (
  ECHO GUCEF environment variable not found, setting it
  SET GUCEF_HOME=%BUIDUSINGNDK_THEBATCHDIR%..\..
)
REM ECHO GUCEF_HOME="%GUCEF_HOME%"

ECHO Script location for FindCygwin: %GUCEF_HOME%\projects\Android\FindCygwin.bat

ECHO Invoking FindCygwin script
ECHO.

SET OLD_NOPAUSE=%NOPAUSE%
SET NOPAUSE="true"

CALL %GUCEF_HOME%\projects\Android\FindCygwin.bat

SET NOPAUSE=%OLD_NOPAUSE%

IF NOT DEFINED CYGWIN_ROOT (

  ECHO.
  ECHO Failure: The FindCygwin script was unable to locate cygwin, we cannot proceed!
)

IF DEFINED CYGWIN_ROOT (
  
  ECHO.
  ECHO *** Invoking BuildUsingNDK.sh using bash ***
  ECHO.
  
  REM Cygwin needs its bin dirs to be able to execute commands
  REM /usr/local/bin;/usr/bin;/bin
  
  ECHO Adding Cygwin binary paths to PATH
  SET PATH=%CYGWIN_ROOT%\local\bin;%CYGWIN_ROOT%\usr\bin;%CYGWIN_ROOT%\bin;"%PATH%"

  ECHO Undefining GUCEF_HOME so we don't mess up the bash version
  SET GUCEF_HOME=
  REM SET GNUMAKE=

  SET WIN32_BASH_PATH=%CYGWIN_ROOT%\bin
  REM SET CYGWIN_ROOT=

  CD /D %BUIDUSINGNDK_THEBATCHDIR%

  REM echo %WIN32_BASH_PATH%
  %CYGWIN_ROOT%\bin\bash "%BUIDUSINGNDK_THEBATCHDIR%\BuildUsingNDK.sh" --login -i
)


IF NOT DEFINED NOPAUSE (
  PAUSE
)