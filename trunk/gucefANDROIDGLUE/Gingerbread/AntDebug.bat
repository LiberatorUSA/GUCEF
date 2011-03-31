@echo off

SET ANT_HOME=G:\Other\ant


IF NOT DEFINED ANT_HOME (
  ECHO "ANT_HOME is not defined! Cannot locate Ant, make sure you set ANT_HOME first to the location of the ant tool."
  ECHO "Ant is a java tool from Apache that you may need to download seperatly on your machine"
)
ECHO ANT_HOME = %ANT_HOME%


IF DEFINED ANT_HOME (
  CALL %ANT_HOME%\ant.bat debug
)

PAUSE