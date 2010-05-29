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

cd "%GENERATEJNIHEADERS_BATCHSTARTDIR%\..\src\com\VanvelzenSoftware\gucefANDROIDGLUE"
ECHO Working dir = "%CD%"
REM "%JAVA_SDK_TOOLS%\javac.exe" com\VanvelzenSofware\gucefANDROIDGLUE\GUCEFApplicationGlue.java -classpath %ANDROID_SDK_HOME%\out\host\common\obj\JAVA_LIBRARIES\layoutlib_intermediates\classes
"%JAVA_SDK_TOOLS%\javac.exe" GUCEFApplicationGlue.java

IF EXIST GUCEFApplicationGlue.class (
  ECHO Successfully generated GUCEFApplicationGlue.class from GUCEFApplicationGlue.java
)

IF NOT EXIST GUCEFApplicationGlue.class (
  ECHO Failed to generate GUCEFApplicationGlue.class from GUCEFApplicationGlue.java
  GOTO :END
)

REM -----------------------------------------------------

ECHO *** Creating .h files from .class files using Java SDK javah tool***

cd "%GENERATEJNIHEADERS_BATCHSTARTDIR%\..\src\com\VanvelzenSoftware\gucefANDROIDGLUE"
ECHO Working dir = "%CD%"

"%JAVA_SDK_TOOLS%\javah.exe" -jni GUCEFApplicationGlue


REM -----------------------------------------------------

:END

cd "%GENERATEJNIHEADERS_BATCHSTARTDIR%"
IF NOT DEFINED NOPAUSE (
  PAUSE
)