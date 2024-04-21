@echo off

SET GUCEF_THEBATCHDIR=%~dp0

SET PROJECTGENERATOR_CONFIG_FILE="ProjectGenerator.ini"

GOTO FIND_GUCEF_CMAKE_SLN_DEBUG_VS2022_PROJECTGENERATOR


REM -----------------------------------------------------

:FIND_GUCEF_RELEASE_PROJECTGENERATOR

SET GENERATORPATH=%GUCEF_THEBATCHDIR%\..\tools\ProjectGenerator\bin\ReleasedBins\Win32\2022.05.15\
SET GENERATOREXE=ProjectGenerator.exe
SET EXETEST=%GENERATORPATH%\%GENERATOREXE%

ECHO Test path = "%EXETEST%"
IF EXIST "%EXETEST%" (
  ECHO Using released version of ProjectGenerator dated May 15th 2022
  SET PROJECTGENERATOR_CONFIG_FILE="ProjectGenerator.ini"
  GOTO RUN_PROJECTGENERATOR
)

IF NOT EXIST "%EXETEST%" (
  ECHO Error: Unable to locate GUCEF released ProjectGenerator
  GOTO END
)
    
cd "%GUCEF_THEBATCHDIR%"
GOTO RUN_PROJECTGENERATOR

REM -----------------------------------------------------

:FIND_GUCEF_CMAKE_SLN_DEBUG_VS2022_X64_PROJECTGENERATOR

SET GENERATORPATH=%GUCEF_THEBATCHDIR%\..\common\bin\VS2022_x64_ProjectGenerator\bin\Debug
SET GENERATOREXE=ProjectGenerator.exe
SET EXETEST=%GENERATORPATH%\%GENERATOREXE%

ECHO Test path = "%EXETEST%"
IF EXIST "%EXETEST%" (
  ECHO Warning: Using x64 VS2022 debug development version of the ProjectGenerator
  SET PROJECTGENERATOR_CONFIG_FILE="ProjectGenerator_d.ini"
  GOTO RUN_PROJECTGENERATOR
)

IF NOT EXIST "%EXETEST%" (
  ECHO Cannot locate x64 VS2022 debug development version of the ProjectGenerator
  GOTO FIND_GUCEF_RELEASE_PROJECTGENERATOR
)

cd "%GUCEF_THEBATCHDIR%"
GOTO RUN_PROJECTGENERATOR

REM -----------------------------------------------------

:FIND_GUCEF_CMAKE_SLN_DEBUG_VS2022_PROJECTGENERATOR

SET GENERATORPATH=%GUCEF_THEBATCHDIR%\..\common\bin\VS2022_ProjectGenerator\bin\Debug
SET GENERATOREXE=ProjectGenerator.exe
SET EXETEST=%GENERATORPATH%\%GENERATOREXE%

ECHO Test path = "%EXETEST%"
IF EXIST "%EXETEST%" (
  ECHO Warning: Using VS2022 debug development version of the ProjectGenerator
  SET PROJECTGENERATOR_CONFIG_FILE="ProjectGenerator_d.ini"  
  GOTO RUN_PROJECTGENERATOR
)

IF NOT EXIST "%EXETEST%" (
  ECHO Cannot locate VS2022 debug development version of the ProjectGenerator
  GOTO FIND_GUCEF_CMAKE_SLN_DEBUG_VS2022_X64_PROJECTGENERATOR
)

cd "%GUCEF_THEBATCHDIR%"
GOTO RUN_PROJECTGENERATOR

REM -----------------------------------------------------

:RUN_PROJECTGENERATOR

SET CMAKE_ROOT="C:\Program Files (x86)\CMake"

SET PATH="%GENERATORPATH%";%PATH%

cd %GUCEF_THEBATCHDIR%\..\

IF NOT DEFINED GUCEF_HOME (
  ECHO GUCEF environment variable not found, setting it
  SET GUCEF_HOME=%CD%
  ECHO GUCEF_HOME=%CD%
)

%GENERATOREXE% *ConfigPath=%GUCEF_THEBATCHDIR%\%PROJECTGENERATROR_CONFIG_FILE%* %GUCEF_EXTRAPROJECTGENCMDS%
cd "%GUCEF_THEBATCHDIR%"
GOTO END


REM -----------------------------------------------------

:END

IF NOT DEFINED NOPAUSE (
  PAUSE
)