@echo off

SET GUCEF_THEBATCHDIR=%CD%
GOTO FIND_GUCEF_CMAKE_SLN_DEBUG_MVC10_CMAKELISTGENERATOR


REM -----------------------------------------------------

:FIND_GUCEF_RELEASE_CMAKELISTGENERATOR

SET GENERATORPATH=%GUCEF_THEBATCHDIR%\..\..\tools\ProjectGenerator\bin\ReleasedBins\Win32\22January2013\
SET GENERATOREXE=ProjectGenerator.exe
SET EXETEST=%GENERATORPATH%\%GENERATOREXE%

ECHO Test path = "%EXETEST%"
IF EXIST "%EXETEST%" (
  ECHO Using released version of ProjectGenerator dated 22'nd January 2013
  GOTO RUN_CMAKELISTGENERATOR
)

IF NOT EXIST "%EXETEST%" (
  ECHO Error: Unable to locate GUCEF released CMakeListGenerator
  GOTO END
)
    
cd "%GUCEF_THEBATCHDIR%"
GOTO RUN_CMAKELISTGENERATOR

REM -----------------------------------------------------

:FIND_GUCEF_CMAKE_SLN_DEBUG_MVC10_CMAKELISTGENERATOR

SET GENERATORPATH=%GUCEF_THEBATCHDIR%\..\..\common\bin\MVC10\bin\Debug
SET GENERATOREXE=ProjectGenerator.exe
SET EXETEST=%GENERATORPATH%\%GENERATOREXE%

ECHO Test path = "%EXETEST%"
IF EXIST "%EXETEST%" (
  ECHO Warning: Using VC10 CMake debug development version of the ProjectGenerator
  GOTO RUN_CMAKELISTGENERATOR
)

IF NOT EXIST "%EXETEST%" (
  ECHO Cannot locate VC10 CMake debug development version of the ProjectGenerator
  GOTO FIND_GUCEF_RELEASE_CMAKELISTGENERATOR
)

cd "%GUCEF_THEBATCHDIR%"
GOTO RUN_CMAKELISTGENERATOR

REM -----------------------------------------------------

:RUN_CMAKELISTGENERATOR

SET PATH="%GENERATORPATH%";%PATH%

cd %GUCEF_THEBATCHDIR%\..\..\

IF NOT DEFINED GUCEF_HOME (
  ECHO GUCEF environment variable not found, setting it
  SET GUCEF_HOME=%CD%
  ECHO GUCEF_HOME=%CD%
)

rem %GENERATOREXE% *rootDir=%GUCEF_HOME%* *dirsToIgnore=.svn;_svn* *projectName=GUCEF* *generators=cmake* *outputDir=%GUCEF_HOME%* *cmakgen:TemplateDir=%GUCEF_HOME%\projects\CMake\ProjectGenTemplates*
cd "%GUCEF_THEBATCHDIR%"
GOTO END


REM -----------------------------------------------------

:END

IF NOT DEFINED NOPAUSE (
  PAUSE
)