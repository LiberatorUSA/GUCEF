#!/bin/bash
#


# Turn on tracing, we want to see what's going on
#set -x

clear
echo "*** Invoking CMakeCommon ***"
. CMakeCommon.sh
echo "*** Invoking CMake ***"
ANIX_OUTPUTDIR="$OUTPUTDIR/ANDROID"
echo "Output dir for this CMake target = $ANIX_OUTPUTDIR"
echo "CMake will use source root: $SRCROOTDIR"


[ -d android ] || mkdir android
cd android
[ -f Makefile ] || export VERBOSE="VERBOSE=1"
[ -f Makefile ] || cmake -DCMAKE_TOOLCHAIN_FILE=Android.cmake -G "Unix Makefiles" -H"$SRCROOTDIR" -B$ANIX_OUTPUTDIR

#Do we want to actually build?
#make -j2 $VERBOSE $*

echo "Press enter to continue..."
line=""
read line