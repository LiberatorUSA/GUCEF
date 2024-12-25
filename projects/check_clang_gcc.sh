#!/bin/bash

# The purpose of this script is to match up the Clang 'selected' version of GCC with an actually installed GCC
# For some reason Clang can end up selecting versions that are not installed leading to compiler errors such as:
#               cannot find -lstdc++: No such file or directory
# This is very confusing since GCC might actually be installed and working just fine so you would not expect the above error message
# The issue is that the version Clang selects as per 'clang -v' would list for example:
#               Selected GCC installation: /usr/bin/../lib/gcc/x86_64-linux-gnu/12
# If the GCC version installed is thus not 12 it wont work resulting in the error noted above.
# To remedy we have to check which version Clang selected in all its wisdom and make sure that version is actually installed
# That is what the script code below does:

# Get the selected GCC version from clang -v output
echo "Obtaining the version of GCC selected by Clang in all its wisdom..."
selected_gcc_version=$(clang -v 2>&1 | grep "Selected GCC installation" | grep -oP 'x86_64-linux-gnu/\K\d+')
echo "Clang has selected GCC version $selected_gcc_version"

# Check if the selected GCC version is installed
if ! gcc-$selected_gcc_version --version &> /dev/null; then
    echo "GCC version $selected_gcc_version is not installed. Installing..."
    sudo apt update
    sudo apt install -y g++-$selected_gcc_version
else
    echo "GCC version $selected_gcc_version is already installed."
fi
