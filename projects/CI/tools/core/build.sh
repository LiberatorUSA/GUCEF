#!/bin/bash

##
# Main entry for monorepository build.
# Triggers builds for all modified projects in order respecting their dependencies.
# 
# Usage:
#   build.sh
##

set -e

# Find script directory
SCRIPTPATH="$(cd "${0%/*}" 2>/dev/null; echo "$PWD"/"${0##*/}")"
SCRIPTDIR=${SCRIPTPATH%/*}

# Source extra helper functions
. $SCRIPTDIR/targetfuncs.sh

# Configuration with default values
: "${CI_TOOL:=circleci}"
: "${CI_PLUGIN:=$SCRIPTDIR/../plugins/${CI_TOOL}.sh}"

# Resolve commit range for current build 
LAST_SUCCESSFUL_COMMIT=$(${CI_PLUGIN} hash last)
echo "Last commit: ${LAST_SUCCESSFUL_COMMIT}"
if [[ ${LAST_SUCCESSFUL_COMMIT} == "null" ]]; then
    COMMIT_RANGE="origin/master"
else
    COMMIT_RANGE="$(${CI_PLUGIN} hash current)..${LAST_SUCCESSFUL_COMMIT}"
fi
echo "Commit range: $COMMIT_RANGE"

# Ensure we have all changes from last successful build
if [[ -f $(git rev-parse --git-dir)/shallow ]]; then
    if [[ ${LAST_SUCCESSFUL_COMMIT} == "null" ]]; then
        git fetch --unshallow
    else 
        DEPTH=1
        until git show ${LAST_SUCCESSFUL_COMMIT} > /dev/null 2>&1
        do
            DEPTH=$((DEPTH+5))
            echo "Last commit not fetched yet. Fetching depth $DEPTH."
            git fetch --depth=$DEPTH
        done
    fi
fi

# Collect all modified projects
PROJECTS_TO_BUILD=""
list_all_changed_targets_for_commit_range "PROJECTS_TO_BUILD" "$COMMIT_RANGE"

# If nothing to build inform and exit
if [[ -z "$PROJECTS_TO_BUILD" ]]; then
    echo -e "\n---------------------------------------\n"
	echo "No projects to build"
	echo -e "\n---------------------------------------\n"
    exit 0
fi

echo -e "\n---------------------------------------\n"
echo    "The following projects need to be built:"
echo -e "$PROJECTS_TO_BUILD"
echo -e "\n---------------------------------------\n"

# Build all modified projects
echo -e "$PROJECTS_TO_BUILD" | while read PROJECT; do
	CI_PLUGIN=${CI_PLUGIN} $SCRIPTDIR/build-projects.sh ${PROJECT}
done;
