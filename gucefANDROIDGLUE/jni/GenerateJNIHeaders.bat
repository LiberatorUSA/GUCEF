@echo off

SET GENERATEJNIHEADERS_BATCHSTARTDIR=%CD%

REM -----------------------------------------------------

ECHO *** Enable command extensions ***

VERIFY OTHER 2>nul
SETLOCAL ENABLEEXTENSIONS
IF ERRORLEVEL 1 (
  REM ERROR: Unable to enable extensions
  PAUSE
)
ENDLOCAL


REM -----------------------------------------------------


ECHO *** Checking environment variables ***

IF NOT DEFINED JAVA_SDK_HOME (
  ECHO Error: Environment variable "JAVA_SDK_HOME" is not defined
  GOTO :END
)
SET JAVA_SDK_TOOLS=%JAVA_SDK_HOME%\bin

goto :commentedOut1
IF NOT DEFINED ANDROID_SDK_HOME (
  ECHO Error: Environment variable "ANDROID_SDK_HOME" is not defined
  GOTO :END
)
:commentedOut1

ECHO JAVA_SDK_HOME=%JAVA_SDK_HOME%
ECHO JAVA_SDK_TOOLS=%JAVA_SDK_TOOLS%
ECHO ANDROID_SDK_HOME=%ANDROID_SDK_HOME%

REM -----------------------------------------------------

ECHO *** Creating .class files using Java SDK javac tool ***

SET SOURCE_PATH=%GENERATEJNIHEADERS_BATCHSTARTDIR%\..\src\
ECHO Working dir = "%CD%"
cd "%SOURCE_PATH%"
"%JAVA_SDK_TOOLS%\javac.exe" -g -verbose -deprecation -d "%GENERATEJNIHEADERS_BATCHSTARTDIR%" -sourcepath "%SOURCE_PATH%" -classpath "%SOURCE_PATH%" @"%GENERATEJNIHEADERS_BATCHSTARTDIR%\sourcefiles.txt"

ECHO Return value error level: %ERRORLEVEL%

IF %ERRORLEVEL% NEQ 0 (
  ECHO Failed to generate *.class files from *.java files
  GOTO :END
) ELSE (
  ECHO Successfully generated *.class files from *.java files
)

REM -----------------------------------------------------

ECHO *** Creating .h files from .class files using Java SDK javah tool***

cd "%GENERATEJNIHEADERS_BATCHSTARTDIR%"
ECHO Working dir = "%CD%"
"%JAVA_SDK_TOOLS%\javah.exe" -verbose -force -stubs -d "%GENERATEJNIHEADERS_BATCHSTARTDIR%" -classpath "%GENERATEJNIHEADERS_BATCHSTARTDIR%"

ECHO Return value error level: %ERRORLEVEL%

IF %ERRORLEVEL% NEQ 0 (
  ECHO Failed to generate *.h files from *.class files
  GOTO :END
) ELSE (
  ECHO Successfully generated *.h files from *.class files
)

REM -----------------------------------------------------

:END

cd "%GENERATEJNIHEADERS_BATCHSTARTDIR%"
IF NOT DEFINED NOPAUSE (
  PAUSE
)