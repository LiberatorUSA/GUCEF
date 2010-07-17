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
    GUCEF_HOME="$GENERATECMAKELISTS_SCRIPTSTARTDIR/../.."
  fi
  echo "GUCEF_HOME = $GUCEF_HOME"

}

#------------------------------------------------------------------------------

function FindGucefDebugCMakeListGenerator {
  
  TEST_PATH="$GUCEF_HOME/common/bin/CBNIX/bin/CMakeListGenerator"
  echo "Testing for executable binary @ $TEST_PATH"
  if [ -x "$TEST_PATH" ];
  then
    echo "Found Debug version of GUCEF CMakeListGenerator"
    GENERATORPATH=$TEST_PATH
  fi

}

#------------------------------------------------------------------------------

function FindGucefReleaseCMakeListGenerator {
  
  TEST_PATH="$GUCEF_HOME/tools/CMakeListGenerator/bin/ReleasedBins/Linux/26June2010/CMakeListGenerator"
  echo "Testing for executable binary @ $TEST_PATH"
  if [ -x "$TEST_PATH" ];
  then
    echo "Found Release version of GUCEF CMakeListGenerator dated 26'th of June 2010"
    GENERATORPATH=$TEST_PATH
  fi

}

#------------------------------------------------------------------------------

function FindCMakeListGenerator {

  GENERATORPATH=${GENERATORPATH:=undefined}
  FindGucefDebugCMakeListGenerator
  if [ "$GUCEF_HOME" = "undefined" ]; 
  then
    FindGucefReleaseCMakeListGenerator
  fi
}

#------------------------------------------------------------------------------

# Remember the dir where we started out since we use relative paths
scriptPath="$(cd "${0%/*}" 2>/dev/null; echo "$PWD"/"${0##*/}")"
GENERATECMAKELISTS_SCRIPTSTARTDIR=${scriptPath%/*}
echo "GENERATECMAKELISTS_SCRIPTSTARTDIR = $GENERATECMAKELISTS_SCRIPTSTARTDIR"

# Make sure the GUCEF_HOME variable is set
SetGucefHome

# Determine a path to a CMakeListGenerator binary
GENERATORPATH=${GENERATORPATH:=undefined}
FindCMakeListGenerator
if [ "$GENERATORPATH" = "undefined" ]; then
  echo "Error: Unable to find a CMakeListGenerator binary"
  exit
fi
echo "GENERATORPATH = $TEST_PATH"

# Enable monitor job control
#set -o monitor

# invoke the generator
$GENERATORPATH 'rootDir=$GUCEF_HOME'




