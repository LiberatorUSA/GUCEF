#!/bin/bash

# Get the remote URL of the Git repository
GIT_REMOTE_URL=$(git config --get remote.origin.url)

# Check if the remote URL was retrieved successfully
if [ -z "$GIT_REMOTE_URL" ]; then
    echo "Error: Could not retrieve the remote URL. Ensure this script is run from within a Git repository."
    exit 1
fi

# Output the remote URL (for debugging purposes)
echo "The remote URL of the Git repository is: $GIT_REMOTE_URL"

# You can now use the GIT_REMOTE_URL variable in your script
