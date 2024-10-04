#!/bin/bash

# Function to read a variable from a file
read_variable_from_file() {
    local file_name=$1
    if [ -f "$file_name" ]; then
        cat "$file_name"
    else
        echo ""
    fi
}

# Read variables from text files
CICD_USER_TOKEN=$(read_variable_from_file "./CICD_USER_TOKEN.txt")
CICD_USER_NAME=$(read_variable_from_file "./CICD_USER_NAME.txt")
CICD_USER_PASS=$(read_variable_from_file "./CICD_USER_PASS.txt")

# Check if any variable is empty and call the generate script if needed
if [ -z "$CICD_USER_TOKEN" ] || [ -z "$CICD_USER_NAME" ] || [ -z "$CICD_USER_PASS" ]; then
    echo "One or more credentials are missing. Generating new credentials..."
    ./generate_cicd_user.sh
    ./generate_cicd_user_token.sh

    # Re-read variables from text files after generating new credentials
    CICD_USER_TOKEN=$(read_variable_from_file "./CICD_USER_TOKEN.txt")
    CICD_USER_NAME=$(read_variable_from_file "./CICD_USER_NAME.txt")
    CICD_USER_PASS=$(read_variable_from_file "./CICD_USER_PASS.txt")
fi

# Output the read variables (for debugging purposes)
echo "CICD_USER_TOKEN: $CICD_USER_TOKEN"
echo "CICD_USER_NAME: $CICD_USER_NAME"
echo "CICD_USER_PASS: $CICD_USER_PASS"

export CICD_USER_TOKEN
export CICD_USER_NAME
export CICD_USER_PASS