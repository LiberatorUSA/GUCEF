#!/bin/bash
#
# Turn on tracing, we want to see what's going on
#set -x

#------------------------------------------------------------------------------

function SetGucefHome {

  # Set GUCEF_HOME if it is not set yet
  GUCEF_HOME=${GUCEF_HOME:=undefined}
  if [ "$GUCEF_HOME" = "undefined" ]; then
    echo "GUCEF_HOME environment variable not found, setting it"
    GUCEF_HOME="$GUCEF_THESCRIPTDIR/.."
  fi
  echo "GUCEF_HOME = $GUCEF_HOME"

}

#------------------------------------------------------------------------------

function FindGucefDebugProjectGenerator {

  TEST_PATH="$GUCEF_HOME/common/bin/CBNIX/bin/ProjectGenerator"
  echo "Testing for executable binary @ $TEST_PATH"
  if [ -x "$TEST_PATH" ];
  then
    echo "Found Debug version of GUCEF ProjectGenerator"
    GENERATORPATH=$TEST_PATH
  fi

}

#------------------------------------------------------------------------------

function FindGucefReleaseProjectGenerator {

  TEST_PATH="$GUCEF_HOME/tools/ProjectGenerator/bin/ReleasedBins/Linux/20January2012/ProjectGenerator"
  echo "Testing for executable binary @ $TEST_PATH"
  if [ -x "$TEST_PATH" ];
  then
    echo "Found Release version of GUCEF ProjectGenerator dated 20th of January 2012"
    GENERATORPATH=$TEST_PATH
  fi

}

#------------------------------------------------------------------------------

function FindProjectGenerator {

  GENERATORPATH=${GENERATORPATH:=undefined}
  FindGucefDebugProjectGenerator
  if [ "$GENERATORPATH" = "undefined" ];
  then
    FindGucefReleaseProjectGenerator
  fi
}

#------------------------------------------------------------------------------

# Remember the dir where we started out since we use relative paths
scriptPath="$(cd "${0%/*}" 2>/dev/null; echo "$PWD"/"${0##*/}")"
GUCEF_THESCRIPTDIR=${scriptPath%/*}
echo "GUCEF_THESCRIPTDIR = $GUCEF_THESCRIPTDIR"

# Make sure the GUCEF_HOME variable is set
SetGucefHome

# Determine a path to a ProjectGenerator binary
GENERATORPATH=${GENERATORPATH:=undefined}
FindProjectGenerator
if [ "$GENERATORPATH" = "undefined" ]; then
  echo "Error: Unable to find a ProjectGenerator binary"
  exit
fi
echo "GENERATORPATH = $TEST_PATH"

# invoke the generator
#$GENERATORPATH *ConfigPath=$GUCEF_THESCRIPTDIR/ProjectGenerator.ini* $GUCEF_EXTRAPROJECTGENCMDS





