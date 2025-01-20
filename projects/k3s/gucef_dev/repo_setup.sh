#!/bin/bash

# Directory where the repository should be located
REPO_DIR="/code/GUCEF"

# URL of the repository to clone
REPO_URL="https://github.com/DinandVanvelzen/GUCEF.git"

# Check if the directory exists and is a Git repository
if [ -d "$REPO_DIR" ] && [ -d "$REPO_DIR/.git" ]; then
    echo "The GUCEF repository already exists in $REPO_DIR."
else
    echo "The GUCEF repository does not exist in $REPO_DIR. Cloning the repository..."
    git clone "$REPO_URL" "$REPO_DIR"
fi
