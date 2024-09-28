#!/bin/bash

# Set the namespace where Jenkins is running
NAMESPACE="devops-tools"

# Find the Jenkins pod name by looking for the pod with the "jenkins-" prefix
POD_NAME=$(sudo kubectl get pods -n $NAMESPACE --no-headers -o custom-columns=":metadata.name" | grep '^jenkins-')

# Check if the pod was found
if [[ -z "$POD_NAME" ]]; then
  echo "Jenkins pod not found in namespace $NAMESPACE"
  exit 1
fi
echo "Found Jenkins pod: $POD_NAME"
export POD_NAME

echo "Waiting up to 60secs for the pod to be ready"
TIMEOUT=60 # Timeout in seconds
start_time=$(date +%s)
while true; do
  sudo kubectl get pod "$POD_NAME" -n "$NAMESPACE" | grep -q "Running"
  if [ $? -eq 0 ]; then
    echo "Pod $POD_NAME is ready!"
    break
  fi

  elapsed_time=$(($(date +%s) - $start_time))
  if [ $elapsed_time -gt $TIMEOUT ]; then
    echo "Timeout waiting for pod $POD_NAME to become ready."
    exit 1
  fi

  sleep 5
done
