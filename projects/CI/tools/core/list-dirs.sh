#!/bin/bash

##
# List all dirs in the target path
#
# Usage:
#   list-dirs.sh <path>
##

set -e

cwd=$(pwd)

# Capture input parameter and validate it
ROOT_PATH_TO_LIST=$1
cd $ROOT_PATH_TO_LIST

for f in *; do
    if [ -d "$f" ]; then
        # Will not run if no directories are available
        echo "$f"
    fi
done

cd $cwd