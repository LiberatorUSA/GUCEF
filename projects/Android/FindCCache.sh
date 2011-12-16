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
    GUCEF_HOME="$FINDCCACHE_SCRIPTSTARTDIR/../.."
  fi
  echo "GUCEF_HOME = $GUCEF_HOME"

}

#------------------------------------------------------------------------------

function FindCygwinCCache {

  echo "Detected that we are running in Cygwin"

  if [ "$CCACHEPATH" = "undefined" ]; then
  
    echo "CCACHEPATH environment variable not found, checking our default Cygwin location"
    TEST_PATH="/cygdrive/c/cygwin/bin/ccache.exe"
    echo "Testing for ccache existance @ $TEST_PATH"

    if [ -x "$TEST_PATH" ];
    then
      echo "Found ccache binary"
      CCACHEPATH=$TEST_PATH
    else
      echo "Unable to locate ccache, invalid ccache location (or did you forget to install ccache in Cygwin?)"
      CCACHEPATH="undefined"   
    fi

  fi

  echo "CCache does not work properly under Cygwin at this time due to bad NDK - Cygwin interaction"
  echo "CCache useage is disabled"
  CCACHEPATH="undefined"
}

#------------------------------------------------------------------------------

function FindLinuxCCache {

  if [ "$CCACHEPATH" = "undefined" ]; then
      
    echo "CCACHEPATH environment variable not found, checking our default location"
    TEST_PATH="$GUCEF_HOME/dependencies/ccache/ccache"
    echo "Testing for ccache existance @ $TEST_PATH"

    if [ -x "$TEST_PATH" ];
    then
      echo "Found ccache binary"
      CCACHEPATH=$TEST_PATH
    else
      echo "Unable to locate ccache, invalid ccache location"
      CCACHEPATH="undefined"   
    fi
             
  fi

#  if [ "$CCACHEPATH" = "undefined" ]; then
#    echo "Setting ccache value to the default detection value"
#    CCACHEPATH="ccache"
#  fi

}

#------------------------------------------------------------------------------

function FindCCache {

  # if not yet defined initialize the path to undefined
  CCACHEPATH=${CCACHEPATH:=undefined}

  # Check if we are running under Cygwin
  if [ "$OSTYPE" = "cygwin" ]; then    
    FindCygwinCCache  
  else
    FindLinuxCCache
  fi

  echo " "
  echo "** Found the following for CCache"
  echo "   CCACHEPATH = $CCACHEPATH"
}

#------------------------------------------------------------------------------

# Remember the dir where we started out since we use relative paths
scriptPath="$(cd "${0%/*}" 2>/dev/null; echo "$PWD"/"${0##*/}")"
FINDCCACHE_SCRIPTSTARTDIR=${scriptPath%/*}
echo "FINDCCACHE_SCRIPTSTARTDIR = $FINDCCACHE_SCRIPTSTARTDIR"

# Make sure the GUCEF_HOME variable is set
SetGucefHome

# Locate the optional CCache tooling
FindCCache

if [ "$CCACHEPATH" != "undefined" ]; then

  # Export the variable for child processes
  export NDK_CCACHE=$CCACHEPATH

fi

