#!/bin/bash

##
# List all known targets.
# Targets are identified using the ProjectGenerator produced dir structure
# Each target has the relevant paths for said target in a simple globpaths.txt
# We check the paths as glob patterns to compare with the changed paths
#
# Usage:
#   list-targets.sh
##

#set -e

# Find script directory (no support for symlinks)
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

# Source extra helper functions
. $DIR/../plugins/helperfuncs.sh

# For all known projects check if there was a change and look for all dependant projects
for PLATFORM_NAME in $(${DIR}/list-dirs.sh ${DIR}/../../targets); do        
	for TARGET_NAME in $(${DIR}/list-dirs.sh ${DIR}/../../targets/$PLATFORM_NAME); do		
		
		TARGET_JOB_NAME=$TARGET_NAME-$PLATFORM_NAME
		echo Checking $TARGET_JOB_NAME paths for changes
		FILE_WITH_PATHS=${DIR}/../../targets/$PLATFORM_NAME/$TARGET_NAME/globpaths.txt
		
		declare -a PATHARRAY		
		load_text_file_lines_as_array $FILE_WITH_PATHS "PATHARRAY"		
		#printf "%s\n" "${PATHARRAY[@]}"
		
		# Check the paths of the target against the changed paths
		declare -i targetWasChanged=0
		for TARGET_PATH in "${PATHARRAY[@]}"
		do			
			if [[ $(echo -e "$CHANGED_PATHS" | grep "$TARGET_PATH") ]]; then                
			    CHANGED_TARGETS="$CHANGED_TARGETS\n$TARGET_JOB_NAME"
				targetWasChanged=1
				echo Determined that target $TARGET_JOB_NAME changed due to detection of change using path $TARGET_PATH
				break
			fi 
		done
		
		if [ 0 -eq "$targetWasChanged" ]
		then
			echo Determined that target $TARGET_JOB_NAME did not change
		fi
		
	done
done
