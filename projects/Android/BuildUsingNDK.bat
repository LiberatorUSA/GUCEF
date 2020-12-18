@echo off

REM SET BUIDUSINGNDK_THEBATCHDIR=%CD%
SET BUIDUSINGNDK_THEBATCHDIR=%~dp0

CALL %BUIDUSINGNDK_THEBATCHDIR%\FindGucefHome.bat

IF NOT DEFINED GUCEF_HOME (
  ECHO GUCEF environment variable not found, setting it
  SET GUCEF_HOME=%BUIDUSINGNDK_THEBATCHDIR%..\..
)

SET OLD_NOPAUSE=%NOPAUSE%
SET NOPAUSE="true"

CALL %GUCEF_HOME%\projects\Android\FindNDK.bat

SET NOPAUSE=%OLD_NOPAUSE%

IF NOT DEFINED MSWIN_MAINANDROIDMAKEDIR (
  ECHO MSWIN_MAINANDROIDMAKEDIR environment variable not found, setting it
  SET MSWIN_MAINANDROIDMAKEDIR=%GUCEF_HOME%\projects\Android\jni
)

IF DEFINED CMDCAPABLE_NDK (

    ECHO.
    ECHO Changing current directory to Android project dir relative to %MSWIN_MAINANDROIDMAKEDIR%
    ECHO.

    CD %MSWIN_MAINANDROIDMAKEDIR%

    ECHO.
    ECHO *** Invoking build ***
    ECHO.

    CALL %CMDCAPABLE_NDK%\ndk-build.cmd

    PAUSE
)

IF NOT DEFINED CMDCAPABLE_NDK (

    ECHO.
    ECHO Unable to locate a NDK package which supports .cmd based compilation, Falling back to Cygwin ...
    ECHO.

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

    rem IF DEFINED CYGWIN_ROOT (
  
      ECHO.
      ECHO *** Invoking BuildUsingNDK.sh using bash ***
      ECHO.
  
      REM Cygwin needs its bin dirs to be able to execute commands
      REM /usr/local/bin;/usr/bin;/bin
  
      ECHO Adding Cygwin binary paths to PATH
      SET PATH="%CYGWIN_ROOT%\local\bin;%CYGWIN_ROOT%\usr\bin;%CYGWIN_ROOT%\bin;%PATH%"

      ECHO Undefining GUCEF_HOME so we don't mess up the bash version
      SET GUCEF_HOME=
      REM SET GNUMAKE=

      SET WIN32_BASH_PATH=%CYGWIN_ROOT%\bin
      REM SET CYGWIN_ROOT=

      CD /D %BUIDUSINGNDK_THEBATCHDIR%

      REM echo %WIN32_BASH_PATH%
      %CYGWIN_ROOT%\bin\bash "%BUIDUSINGNDK_THEBATCHDIR%\BuildUsingNDK.sh" --login -i
    rem )
)

IF NOT DEFINED NOPAUSE (
  PAUSE
)