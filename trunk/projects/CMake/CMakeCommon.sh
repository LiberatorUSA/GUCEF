#!/bin/bash
#
# Turn on tracing, we want to see what's going on
#set -x
#
# Remember the dir where we started out since we use relative paths
CMAKECOMMON_SCRIPTSTARTDIR=pwd
#
SKIP_GUCEF_CMAKELISTSFILEGENERATION=${SKIP_GUCEF_CMAKELISTSFILEGENERATION:=undefined}
if [[ "$SKIP_GUCEF_CMAKELISTSFILEGENERATION" != "undefined" ]];
then 
  echo "*** Generate CMakeLists.txt files ***"
  .GenerateCMakeLists.sh
else 
  echo "Skipping GUCEF's CMakeLists file generation"
fi
#
cd "$CMAKECOMMON_SCRIPTSTARTDIR"
#
echo "*** Perform common CMake environment variable setup ***"
#
cd ".."
cd ".."
#
GUCEF_HOME=pwd
SRCROOTDIR=pwd
OUTPUTDIR="$GUCEF_HOME\common\bin"
#
OIS_HOME=${OIS_HOME:=undefined}
if [ "$OIS_HOME" = "undefined" ]; then
  echo "OIS environment variable not found, setting it"
  OIS_HOME="$GUCEF_HOME/gucefINPUT/plugins/inputdriverOIS/dependency/OIS"
fi
#
echo "OIS_HOME = $OIS_HOME"
#
FREEIMAGE_HOME=${FREEIMAGE_HOME:=undefined}
if [ "$FREEIMAGE_HOME" = "undefined" ]; then
  echo "FreeImage environment variable not found, setting it"
  FREEIMAGE_HOME="$GUCEF_HOME/gucefIMAGE/plugins/imgpluginFreeImage/dependencies/FreeImage"
fi
#
echo "FREEIMAGE_HOME = $FREEIMAGE_HOME"
#
DEVIL_HOME=${DEVIL_HOME:=undefined}
if [ "$DEVIL_HOME" = "undefined" ]; then
  echo "DevIL environment variable not found, setting it"
  DEVIL_HOME="$GUCEF_HOME/gucefIMAGE/plugins/imgpluginDEVIL/dependancy/Devil"
fi
#
echo "DEVIL_HOME = $DEVIL_HOME"
#
ZLIB_HOME=${ZLIB_HOME:=undefined}
if [ "$ZLIB_HOME" = "undefined" ]; then
  echo "ZLib environment variable not found, setting it"
  ZLIB_HOME="$GUCEF_HOME/gucefVFS/plugins/vfspluginZIP/dependencies/zlib"
fi
#
echo "ZLIB_HOME = $ZLIB_HOME"
#
ZZIPLIB_HOME=${ZZIPLIB_HOME:=undefined}
if [ "$ZZIPLIB_HOME" = "undefined" ]; then
  echo "ZZipLib environment variable not found, setting it"
  ZZIPLIB_HOME="$GUCEF_HOME/gucefVFS/plugins/vfspluginZIP/dependencies/zziplib"
fi
#
echo "ZZIPLIB_HOME = $ZZIPLIB_HOME"
#
