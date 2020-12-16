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
SET OUTPUTDIR=%GUCEF_HOME%\common\bin
SET GUCEF_SOURCE_DIR=%SRCROOTDIR%

cd %GUCEF_THEBATCHDIR%

ECHO SRCROOTDIR: %SRCROOTDIR%
ECHO GUCEF_HOME: %GUCEF_HOME%
ECHO root OUTPUTDIR: %OUTPUTDIR%


GOTO FIND_GUCEF_CMAKE_SLN_DEBUG_MVC14_PROJECTGENERATOR


REM -----------------------------------------------------

:FIND_GUCEF_RELEASE_PROJECTGENERATOR

SET GENERATORPATH=%GUCEF_THEBATCHDIR%\..\..\tools\ProjectGenerator\bin\ReleasedBins\Win32\2020.02.28\
SET GENERATOREXE=ProjectGenerator.exe
SET EXETEST=%GENERATORPATH%\%GENERATOREXE%

ECHO Test path = "%EXETEST%"
IF EXIST "%EXETEST%" (
  ECHO Using released version of ProjectGenerator dated Feb 28th 2020
  GOTO RUN_PROJECTGENERATOR
)

IF NOT EXIST "%EXETEST%" (
  ECHO Error: Unable to locate GUCEF released ProjectGenerator
  GOTO END
)
    
cd "%GUCEF_THEBATCHDIR%"
GOTO RUN_PROJECTGENERATOR

REM -----------------------------------------------------

:FIND_GUCEF_CMAKE_SLN_DEBUG_MVC14_PROJECTGENERATOR

SET GENERATORPATH=%GUCEF_THEBATCHDIR%\..\..\common\bin\MVC14\bin\Debug
SET GENERATOREXE=ProjectGenerator.exe
SET EXETEST=%GENERATORPATH%\%GENERATOREXE%

ECHO Test path = "%EXETEST%"
IF EXIST "%EXETEST%" (
  ECHO Warning: Using VC14 CMake debug development version of the ProjectGenerator
  GOTO RUN_PROJECTGENERATOR
)

IF NOT EXIST "%EXETEST%" (
  ECHO Cannot locate VC14 CMake debug development version of the ProjectGenerator
  GOTO FIND_GUCEF_RELEASE_PROJECTGENERATOR
)

cd "%GUCEF_THEBATCHDIR%"
GOTO RUN_PROJECTGENERATOR

REM -----------------------------------------------------

:RUN_PROJECTGENERATOR

SET PATH="%GENERATORPATH%";%PATH%

cd %GUCEF_THEBATCHDIR%\..\..\

%GENERATOREXE% *ConfigPath=%GUCEF_THEBATCHDIR%\ProjectGenerator.ini* %GUCEF_XTRAPROJECTGENCMDS%
cd "%GUCEF_THEBATCHDIR%"
GOTO END


REM -----------------------------------------------------

:END

IF NOT DEFINED NOPAUSE (
  PAUSE
)