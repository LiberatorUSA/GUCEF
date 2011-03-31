@echo off

IF NOT DEFINED JAVA_HOME (
  ECHO "JAVA_HOME is not defined! Cannot locate JDK, make sure you set JAVA_HOME first"
)

ant debug

PAUSE