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
    GUCEF_HOME="$FINDSDK_SCRIPTSTARTDIR/../.."
  fi
  echo "GUCEF_HOME = $GUCEF_HOME"

}

#------------------------------------------------------------------------------

function FindAndroidSdkx86Windows {

  # Check for ANDROIDSDK variable, if undefined set it to a copy in depencencies, we will just have to guess
  ANDROIDSDK=${ANDROIDSDK:=undefined}
  if [ "$ANDROIDSDK" = "undefined" ]; then
    echo "ANDROIDSDK environment variable not found, setting it to our default location"
    ANDROIDSDK="$GUCEF_HOME/dependencies/android-sdk-windows"

    # We also check for the android script
    TEST_PATH="$ANDROIDSDK/tools/android.bat"
    echo "Testing for sdk android script existance @ $TEST_PATH"
    if [ -x "$TEST_PATH" ];
    then
      echo "Found SDK build script"
      ANDROIDSDKSCRIPT=$TEST_PATH
    else
      echo "Unable to locate SDK android script, invalid SDK location"
      ANDROIDSDK="undefined"   
    fi

  fi
  
}

#------------------------------------------------------------------------------

function FindAndroidSdkx86Linux {

  # Check for ANDROIDSDK variable, if undefined set it to a copy in depencencies, we will just have to guess
  ANDROIDSDK=${ANDROIDSDK:=undefined}
  if [ "$ANDROIDSDK" = "undefined" ]; then
    echo "ANDROIDSDK environment variable not found, setting it to our default location"
    ANDROIDSDK="$GUCEF_HOME/dependencies/android-sdk-linux_86"

    # We also check for the android script
    TEST_PATH="$ANDROIDSDK/tools/android"
    echo "Testing for sdk android script existance @ $TEST_PATH"
    if [ -x "$TEST_PATH" ];
    then
      echo "Found SDK build script"
      ANDROIDSDKSCRIPT=$TEST_PATH
    else
      echo "Unable to locate SDK android script, invalid SDK location"
      ANDROIDSDK="undefined"   
    fi

  fi
  
}

#------------------------------------------------------------------------------

function FindAndroidSdk {

  ANDROIDSDK=${ANDROIDSDK:=undefined}
  ANDROIDSDKBUILD=${ANDROIDSDKBUILD:=undefined}

  if [ "$ANDROIDSDK" = "undefined" ]; then

  # Check for linux SDK for x86
  FindAndroidSdkx86Linux
  
  if [ "$ANDROIDSDK" = "undefined" ]; then

  echo "Unable to locate linux SDK for x86, will try windows version" 

  # Check for Windows SDK for x86
  FindAndroidSdkx86Windows
  
  if [ "$ANDROIDSDK" = "undefined" ]; then

  echo "Unable to locate windows SDK for x86" 
        
  fi
  fi
  fi
  
  echo " "
  echo "** Found the following for the SDK"
  echo "   ANDROIDSDK = $ANDROIDSDK"
  echo "   ANDROIDSDKSCRIPT = $ANDROIDSDKSCRIPT"
}

#------------------------------------------------------------------------------

# Remember the dir where we started out since we use relative paths
scriptPath="$(cd "${0%/*}" 2>/dev/null; echo "$PWD"/"${0##*/}")"
FINDSDK_SCRIPTSTARTDIR=${scriptPath%/*}
echo "FINDSDK_SCRIPTSTARTDIR = $FINDSDK_SCRIPTSTARTDIR"

# Make sure the GUCEF_HOME variable is set
SetGucefHome

# Locate the SDK
FindAndroidSdk

# Export the variable for child processes
export ANDROIDSDK
export ANDROIDSDKSCRIPT  

#echo "Press enter to continue..."
#line=""
#read line

