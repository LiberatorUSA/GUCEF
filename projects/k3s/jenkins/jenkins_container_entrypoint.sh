#!/bin/sh
set -e

# Execute our custom boostrap script
. ./usr/local/bin/jenkins_container_entrypoint_bootstrap.sh

# Start Jenkins
exec /usr/local/bin/jenkins.sh
