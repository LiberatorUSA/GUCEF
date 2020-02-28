@echo off

SET GUCEF_THEBATCHDIR=%~dp0
GOTO FIND_GUCEF_CMAKE_SLN_DEBUG_MVC14_CMAKELISTGENERATOR


REM -----------------------------------------------------

:FIND_GUCEF_RELEASE_CMAKELISTGENERATOR

SET GENERATORPATH=%GUCEF_THEBATCHDIR%\..\..\tools\ProjectGenerator\bin\ReleasedBins\Win32\2020.02.28\
SET GENERATOREXE=ProjectGenerator.exe
SET EXETEST=%GENERATORPATH%\%GENERATOREXE%

ECHO Test path = "%EXETEST%"
IF EXIST "%EXETEST%" (
  ECHO Using released version of ProjectGenerator dated Feb 28th 2020
  GOTO RUN_CMAKELISTGENERATOR
)

IF NOT EXIST "%EXETEST%" (
  ECHO Error: Unable to locate GUCEF released ProjectGenerator
  GOTO END
)
    
cd "%GUCEF_THEBATCHDIR%"
GOTO RUN_CMAKELISTGENERATOR

REM -----------------------------------------------------

:FIND_GUCEF_CMAKE_SLN_DEBUG_MVC14_CMAKELISTGENERATOR

SET GENERATORPATH=%GUCEF_THEBATCHDIR%\..\..\common\bin\MVC14\bin\Debug
SET GENERATOREXE=ProjectGenerator.exe
SET EXETEST=%GENERATORPATH%\%GENERATOREXE%

ECHO Test path = "%EXETEST%"
IF EXIST "%EXETEST%" (
  ECHO Warning: Using VC14 CMake debug development version of the ProjectGenerator
  GOTO RUN_CMAKELISTGENERATOR
)

IF NOT EXIST "%EXETEST%" (
  ECHO Cannot locate VC14 CMake debug development version of the ProjectGenerator
  GOTO FIND_GUCEF_RELEASE_CMAKELISTGENERATOR
)

cd "%GUCEF_THEBATCHDIR%"
GOTO RUN_CMAKELISTGENERATOR

REM -----------------------------------------------------

:RUN_CMAKELISTGENERATOR

SET CMAKE_ROOT="C:\Program Files (x86)\CMake"

SET PATH="%GENERATORPATH%";%PATH%

cd %GUCEF_THEBATCHDIR%\..\..\

IF NOT DEFINED GUCEF_HOME (
  ECHO GUCEF environment variable not found, setting it
  SET GUCEF_HOME=%CD%
  ECHO GUCEF_HOME=%CD%
)

%GENERATOREXE% *ConfigPath=%GUCEF_THEBATCHDIR%\ProjectGenerator.ini*
cd "%GUCEF_THEBATCHDIR%"
GOTO END


REM -----------------------------------------------------

:END

IF NOT DEFINED NOPAUSE (
  PAUSE
)