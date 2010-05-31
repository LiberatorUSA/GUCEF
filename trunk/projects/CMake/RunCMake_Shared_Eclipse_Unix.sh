#!/bin/bash
.CMakeCommon.sh
ENIX_OUTPUTDIR="$OUTPUTDIR\MVC8"
'cmake -DBUILD_SHARED_LIBS=ON -G"Eclipse CDT4 - Unix Makefiles" -H"$SRCROOTDIR" -B$ENIX_OUTPUTDIR'
echo "Press enter to continue"
read line
