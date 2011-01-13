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
    GUCEF_HOME="$FINDNDK_SCRIPTSTARTDIR/../.."
  fi
  echo "GUCEF_HOME = $GUCEF_HOME"

}

#------------------------------------------------------------------------------

function FindAndroidNdkR4 {

  # Check for ANDROIDNDK variable, if undefined set it to a copy in depencencies, we will just have to guess
  ANDROIDNDK=${ANDROIDNDK:=undefined}
  if [ "$ANDROIDNDK" = "undefined" ]; then
    echo "ANDROIDNDK environment variable not found, setting it to our default location"
    ANDROIDNDK="$GUCEF_HOME/dependencies/android-ndk-r4"

    # For NDK r4 we also check for the build script
    TEST_PATH="$ANDROIDNDK/ndk-build"
    echo "Testing for ndk-build existance @ $TEST_PATH"
    if [ -x "$TEST_PATH" ];
    then
      echo "Found NDK build script"
      ANDROIDNDKBUILD=$TEST_PATH
    else
      echo "Unable to locate NDK build script, invalid NDK location"
      ANDROIDNDK="undefined"   
    fi

  fi
  
}

#------------------------------------------------------------------------------

function FindAndroidNdk {

  ANDROIDNDK=${ANDROIDNDK:=undefined}
  ANDROIDNDKBUILD=${ANDROIDNDKBUILD:=undefined}
  
  # Right now we only support NDK release 4
  FindAndroidNdkR4
  
  echo " "
  echo "** Found the following for the NDK"
  echo "   ANDROIDNDK = $ANDROIDNDK"
  echo "   ANDROIDNDKBUILD = $ANDROIDNDKBUILD"
}

#------------------------------------------------------------------------------

# Remember the dir where we started out since we use relative paths
scriptPath="$(cd "${0%/*}" 2>/dev/null; echo "$PWD"/"${0##*/}")"
FINDNDK_SCRIPTSTARTDIR=${scriptPath%/*}
echo "FINDNDK_SCRIPTSTARTDIR = $FINDNDK_SCRIPTSTARTDIR"

# Make sure the GUCEF_HOME variable is set
SetGucefHome

# Locate the NDK
FindAndroidNdk


