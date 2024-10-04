#!/bin/bash

# Kubernetes and Jenkins info
JENKINS_HOME="/var/jenkins_home"
LOCAL_JAR_PATH="./jenkins-cli.jar"

. ./wait_for_jenkins_pod.sh

# Copy jenkins-cli.jar from pod to local machine
echo "Attempting to copy jenkins-cli.jar from ${JENKINS_HOME} inside pod ${POD_NAME}"
kubectl cp ${NAMESPACE}/${POD_NAME}:${JENKINS_HOME}/jenkins-cli.jar ${LOCAL_JAR_PATH}

# Check if the copy was successful
if [ -f ${LOCAL_JAR_PATH} ]; then
    echo "jenkins-cli.jar copied successfully to ${LOCAL_JAR_PATH}."
else
    echo "Failed to copy jenkins-cli.jar."
    exit 1
fi
