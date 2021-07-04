#!/bin/bash

# Documentation
read -r -d '' USAGE_TEXT << EOM
Usage:
    list-projects-to-builds.sh <revision range>

    List all projects which had some changes in given commit range.
    Project is identified with relative path to project's root directory from repository root.
    Output list is ordered respecting dependencies between projects (lower projects depends on upper).
    There can be multiple projects (separated by space) on single line which means they can be build on parallel.
   
    If one of commit messages in given commit range contains [rebuild-all] flag then all projects will be listed.

    <revision reange>       range of revision hashes where changes will be looked for
                            format is HASH1..HASH2
EOM

set -e

# Capture input parameter and validate it
COMMIT_RANGE=$1
COMMIT_RANGE_FOR_LOG="$(echo $COMMIT_RANGE | sed -e 's/\.\./.../g')"

if [[ -z $COMMIT_RANGE ]]; then
    echo "ERROR: You need to provide revision range in format HASH1..HASH2 as input parameter"
    echo "$USAGE_TEXT"
    exit 1
fi    

# Find script directory (no support for symlinks)
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

# Look for changes in given revision range
CHANGED_PATHS=$(git diff $COMMIT_RANGE --name-status)
echo -e "\n---------------------------------------\n"
echo -e "Changed paths:\n$CHANGED_PATHS" 
echo -e "\n---------------------------------------\n"

# Setup variables for output collecting 
CHANGED_PROJECTS=""
CHANGED_DEPENDENCIES=""

echo -e "\n---------------------------------------\n"
echo -e "Determining all changed projects...\n"
# If [rebuild-all] command passed it's enought to take all projects and all dependencies as changed
if [[ $(git log "$COMMIT_RANGE_FOR_LOG" | grep "\[rebuild-all\]") ]]; then
    echo -e "Determined that we need to consider all targets as changed projects\n"
	CHANGED_PROJECTS="$(${DIR}/list-all-targets-as-changed.sh)"
else    
    CHANGED_PROJECTS="$(${DIR}/list-changed-targets.sh)"
fi
echo -e "\n---------------------------------------\n"

# Build output 
#PROJECTS_TO_BUILD=$(echo -e "$CHANGED_DEPENDENCIES" | tsort | tac)
#for PROJECT in $(echo -e "$CHANGED_PROJECTS"); do
#    if [[ ! $(echo -e "$PROJECTS_TO_BUILD" | grep "$PROJECT") ]]; then    
#        PROJECTS_TO_BUILD="$PROJECT $PROJECTS_TO_BUILD"
#    fi
#done
PROJECTS_TO_BUILD=${CHANGED_PROJECTS}

# Print output
echo -e "$CHANGED_PROJECTS"
