@echo off

GOTO FIND_CYGWIN

REM -----------------------------------------------------

:FIND_CYGWIN_USING_REGISTRY_WIN32

ECHO Looking for cygwin entry in win32 windows registry
REGEDIT /E %Temp%.\tmp-cygwin.reg "HKEY_LOCAL_MACHINE\SOFTWARE\Cygnus Solutions\Cygwin\mounts v2\/"> nul

ECHO Checking if something has been found
if not exist %Temp%.\tmp-cygwin.reg GOTO CYGWIN_NOT_FOUND_IN_WIN32_REGISTRY

ECHO Getting the native key from windows registry result
FOR /F "tokens=1* delims==" %%A IN ('TYPE %Temp%.\tmp-cygwin.reg ^| FIND "native"') DO SET CYGWIN_ROOT=%%B> nul

ECHO Removing quotes
SET CYGWIN_ROOT=%CYGWIN_ROOT:"=%> nul

ECHO Removing temporary file
if exist %Temp%.\tmp-cygwin.reg del %Temp%.\tmp-cygwin.reg

ECHO Check if we now know where cygwin is
if "%CYGWIN_ROOT%" == "" goto CYGWIN_NOT_FOUND_IN_WIN32_REGISTRY

GOTO END_OF_BATCH

REM -----------------------------------------------------

:CYGWIN_NOT_FOUND_IN_WIN32_REGISTRY

echo no cygwin entry has been found in registry using Win32 path, trying WOW64 registry path
GOTO FIND_CYGWIN_USING_REGISTRY_WOW64

REM -----------------------------------------------------

:FIND_CYGWIN_USING_REGISTRY_WOW64

ECHO Looking for cygwin entry in wow64 windows registry
REGEDIT /E %Temp%.\tmp-cygwin.reg "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Cygwin\setup\/"> nul

ECHO Checking if something has been found
if not exist %Temp%.\tmp-cygwin.reg GOTO CYGWIN_NOT_FOUND_IN_WOW64_REGISTRY

ECHO Getting the native key from windows registry result
FOR /F "tokens=1* delims==" %%A IN ('TYPE %Temp%.\tmp-cygwin.reg ^| FIND "native"') DO SET CYGWIN_ROOT=%%B> nul

ECHO Removing quotes
SET CYGWIN_ROOT=%CYGWIN_ROOT:"=%> nul

ECHO Removing temporary file
if exist %Temp%.\tmp-cygwin.reg del %Temp%.\tmp-cygwin.reg

ECHO Check if we now know where cygwin is
if "%CYGWIN_ROOT%" == "" goto CYGWIN_NOT_FOUND_IN_WOW64_REGISTRY

GOTO END_OF_BATCH


REM -----------------------------------------------------

:CYGWIN_NOT_FOUND_IN_WOW64_REGISTRY

ECHO no cygwin entry has been found in registry using WOW64 path, testing default location
GOTO FIND_CYGWIN_IN_DEFAULT_LOCATION


REM -----------------------------------------------------

:FIND_CYGWIN_IN_DEFAULT_LOCATION

SET EXETEST=c:\cygwin\bin\bash.exe

IF EXIST "%EXETEST%" (
  ECHO Found Cygwin at the default install location C:\cygwin
  SET CYGWIN_ROOT=c:\cygwin
  GOTO END_OF_BATCH
)

ECHO Unable to locate Cygwin in the default install location c:\cygwin
GOTO END_OF_BATCH

REM -----------------------------------------------------

:FIND_CYGWIN

REM we start looking in the registry, one function will call the next etc so we only invoke the first function

ECHO.
ECHO *** Starting search for cygwin ***
ECHO.

GOTO FIND_CYGWIN_USING_REGISTRY_WIN32


REM -----------------------------------------------------

:END_OF_BATCH

IF DEFINED CYGWIN_ROOT (

  ECHO.
  ECHO Success: Cygwin has been located!
  ECHO CYGWIN_ROOT=%CYGWIN_ROOT%
  ECHO.

  REM ECHO Adding Cygwin binary paths to PATH
  REM SET PATH=".;%CYGWIN_ROOT%\bin;%CYGWIN_ROOT%\usr\X11R6\bin;%CYGWIN_ROOT%\opt\elinos\bin;%PATH%"
)

IF NOT DEFINED CYGWIN_ROOT (

  ECHO.
  ECHO Failure: Cygwin could not be located!
  ECHO.
)

IF NOT DEFINED NOPAUSE PAUSE
