#!/bin/bash

set -e

# Find script directory (no support for symlinks)
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

# Source extra helper functions
. $DIR/../plugins/helperfuncs.sh

# Find script directory (no support for symlinks)
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

#-----------------------------------------------------------------------------------------------

function list_all_targets {
	
	local RESULT_VAR="$1"
	local TARGETS_LIST=""
	local DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

	# For all known projects check if there was a change and look for all dependant projects
	for PLATFORM_NAME in $(${DIR}/list-dirs.sh ${DIR}/../../targets); do        
		for TARGET_NAME in $(${DIR}/list-dirs.sh ${DIR}/../../targets/$PLATFORM_NAME); do		
			
			local TARGET_JOB_NAME=$TARGET_NAME-$PLATFORM_NAME		
			TARGETS_LIST="$TARGETS_LIST\n$TARGET_JOB_NAME"	
			
		done
	done
	
	# set result variable
	read -i "${RESULT_VAR}" < <( echo "${TARGETS_LIST}" )
}

#-----------------------------------------------------------------------------------------------

function list_all_changed_targets_foo {

	local RESULT_VAR="$1"
	local CHANGED_PATHS=$2
	local TARGETS_LIST=""
	local DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
	
			local TARGET_JOB_NAME="foobar-win32"   
					TARGETS_LIST="$TARGETS_LIST\n$TARGET_JOB_NAME"	
	
	# set result variable
	read "${RESULT_VAR}" < <( echo "${TARGETS_LIST}" )
}

#-----------------------------------------------------------------------------------------------

function list_all_changed_targets_for_paths {

	local RESULT_VAR="$1"
	local CHANGED_PATHS=$2
	local TARGETS_LIST=""
	local DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
	
	# For all known projects check if there was a change and look for all dependant projects
	for PLATFORM_NAME in $(${DIR}/list-dirs.sh ${DIR}/../../targets); do        
		for TARGET_NAME in $(${DIR}/list-dirs.sh ${DIR}/../../targets/$PLATFORM_NAME); do		
			
			local TARGET_JOB_NAME=$TARGET_NAME-$PLATFORM_NAME
			echo -e "Checking $TARGET_JOB_NAME paths for changes"
			local FILE_WITH_PATHS=${DIR}/../../targets/$PLATFORM_NAME/$TARGET_NAME/globpaths.txt
			
			local -a PATHARRAY		
			load_text_file_lines_as_array "PATHARRAY" $FILE_WITH_PATHS 
			#printf "%s\n" "${PATHARRAY[@]}"
			
			# Check the paths of the target against the changed paths
			local -i targetWasChanged=0
			for TARGET_PATH in "${PATHARRAY[@]}"
			do			
				if [[ $(echo -e "$CHANGED_PATHS" | grep "$TARGET_PATH") ]]; then                
					TARGETS_LIST="$TARGETS_LIST\n$TARGET_JOB_NAME"
					targetWasChanged=1
					echo -e "Determined that target $TARGET_JOB_NAME changed due to detection of change using path $TARGET_PATH"
					break
				fi 
			done
			
			if [ 0 -eq "$targetWasChanged" ]
			then
				echo -e "Determined that target $TARGET_JOB_NAME did not change"
			fi
			
		done
	done
	
	# set result variable
	read -i "${RESULT_VAR}" < <( echo "${TARGETS_LIST}" )
}

#-----------------------------------------------------------------------------------------------

function list_all_changed_targets_for_commit_range {

	# set up variables
	local RESULT_VAR="$1"
	local COMMIT_RANGE=$2
	local COMMIT_RANGE_FOR_LOG="$(echo $COMMIT_RANGE | sed -e 's/\.\./.../g')"
	local DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
	local CHANGED_TARGETS=""
	
	# validate input parameter
	if [[ -z $COMMIT_RANGE ]]; then
		echo "ERROR: You need to provide revision range in format HASH1..HASH2 as input parameter"
		echo "$USAGE_TEXT"
		return 1
	fi    

	# Look for changes in given revision range
	local CHANGED_PATHS=$(git diff $COMMIT_RANGE --name-status)
	echo -e "\n---------------------------------------\n"
	echo -e "Changed paths:\n$CHANGED_PATHS" 
	echo -e "\n---------------------------------------\n"
	
	# Now use the changed files to find the relevant targets
	echo -e "Determining all changed projects..."
	# If [rebuild-all] command passed it's enought to take all projects and all dependencies as changed
	if [[ $(git log "$COMMIT_RANGE_FOR_LOG" | grep "\[rebuild-all\]") ]]; then
		echo -e "Determined that we need to consider all targets as changed projects"
		list_all_targets "CHANGED_TARGETS"
	else    
		echo -e "Determined that we need to perform glob pattern matching against known targets..."
		list_all_changed_targets_for_paths "CHANGED_TARGETS" "$CHANGED_PATHS"
	fi

	# Print output
	echo -e "\n---------------------------------------\n"
	echo -e "Targets impacted:\n$CHANGED_TARGETS"
	
	# set result variable
	read "${RESULT_VAR}" < <( echo "${CHANGED_TARGETS}" )
}

#-----------------------------------------------------------------------------------------------
