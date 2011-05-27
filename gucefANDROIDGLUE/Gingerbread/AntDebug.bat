@echo off

SET ANTDEBUGSCRIPT_THEBATCHDIR=%~dp0

REM IF NOT DEFINED ANT_HOME (
  SET ANT_HOME=%ANTDEBUGSCRIPT_THEBATCHDIR%..\..\dependencies\ant
  ECHO ANT_HOME was not defined, using default guess which relies on the ant tool being placed in the archive dependencies dir
REM )


IF NOT DEFINED ANT_HOME (
  ECHO ANT_HOME is not defined! Cannot locate Ant, make sure you set ANT_HOME first to the location of the ant tool.
  ECHO Ant is a java tool from Apache that you may need to download seperatly on your machine
)
ECHO ANT_HOME = %ANT_HOME%


IF DEFINED ANT_HOME (
  CALL %ANT_HOME%\bin\ant.bat debug
)

PAUSE