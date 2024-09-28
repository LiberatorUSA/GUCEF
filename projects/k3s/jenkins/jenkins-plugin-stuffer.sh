#!/usr/bin/env bash

## This script pulls in plugins for Jenkins manually and drops them into the plugins directory

set -e
## set -x  ## Uncomment for debugging

# Function to create directories if they don't exist
create_dir_if_not_exists() {
  if [ ! -d "$1" ]; then
    mkdir -p "$1"
  fi
}

JENKINS_PLUGIN_HOME=${JENKINS_HOME:=/var/jenkins_home}
JENKINS_PLUGIN_HOME=${JENKINS_PLUGIN_HOME}/plugins/
echo "JENKINS_PLUGIN_HOME=${JENKINS_PLUGIN_HOME}"

echo ""
echo -e "Starting plugin downloads...\n"
create_dir_if_not_exists "${JENKINS_PLUGIN_HOME}"

for PLUGIN in "$@"
do
  echo "Pulling plugin file for ${PLUGIN}..."
  ## Rename to .jpi because that is what they do for system included plugins...yeah...whatever
  curl -v -L -sS -o "${JENKINS_PLUGIN_HOME}${PLUGIN}.jpi" "https://updates.jenkins.io/latest/${PLUGIN}.hpi"
done