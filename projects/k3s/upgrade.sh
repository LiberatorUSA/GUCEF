#!/bin/bash

NAMESPACE="system-upgrade"
PLAN_NAME="server-plan"  # Replace with your actual plan name
CHECK_INTERVAL=15  # Time interval in seconds between checks

echo "Monitoring K3s upgrade progress..."

# Function to check if the upgrade is complete
is_upgrade_complete() {
  # Check if all nodes have completed the upgrade
  completed=$(kubectl get plan ${PLAN_NAME} -n ${NAMESPACE} -o jsonpath='{.status.applying}' 2>/dev/null)
  if [ -z "$completed" ]; then
    echo "All nodes have been upgraded successfully!"
    return 0
  else
    return 1
  fi
}

# Function to display the current status of Jobs
print_job_status() {
  echo "Fetching Job statuses..."
  kubectl get jobs -n ${NAMESPACE} --no-headers | while read -r line; do
    job_name=$(echo "$line" | awk '{print $1}')
    completions=$(echo "$line" | awk '{print $2}')
    status=$(kubectl get job $job_name -n ${NAMESPACE} -o jsonpath='{.status.conditions[?(@.type=="Complete")].status}')
    if [ "$status" == "True" ]; then
      echo "Job: $job_name has completed successfully."
    else
      echo "Job: $job_name is still in progress..."
    fi
  done
}

# Function to display the status of the nodes
print_node_status() {
  echo "Checking node statuses..."
  kubectl get nodes --no-headers | while read -r line; do
    node_name=$(echo "$line" | awk '{print $1}')
    status=$(echo "$line" | awk '{print $2}')
    echo "Node: $node_name is currently $status."
  done
}

#------------------------

# get a listing of the plans and jobs already in the system
echo "Currently active upgrade plans and jobs:"
kubectl -n system-upgrade get plans -o yaml
kubectl -n system-upgrade get jobs -o yaml

# Ensure you are running as root
if [ "$EUID" -ne 0 ]; then
  echo "Please run as root"
  exit
fi

# Create the system-upgrade namespace if it doesn't exist
kubectl create namespace system-upgrade || true

# Create the Upgrade Plan (apply the prepared plan yaml)
kubectl apply -f ./k3s_upgrade_plan.yaml

# get a listing of the plans and jobs now in the system
echo "Currently active upgrade plans and jobs:"
kubectl -n system-upgrade get plans -o yaml
kubectl -n system-upgrade get jobs -o yaml

echo "Upgrade plan applied. You can monitor the upgrade by checking the plan and pod status in the 'system-upgrade' namespace."

# Monitor the upgrade
echo "Monitoring the k3s server upgrade..."
while true; do
  echo "---------------------"
  print_job_status
  print_node_status
  
  # Check if the upgrade is complete
  if is_upgrade_complete; then
    echo "Upgrade complete!"
    break
  else
    echo "Upgrade still in progress..."
  fi
  
  # Wait for the defined interval before checking again
  sleep ${CHECK_INTERVAL}
done

# also check the agent upgrade
PLAN_NAME="agent-plan"

# Monitor the upgrade
echo "Monitoring the k3s agent upgrade..."
while true; do
  echo "---------------------"
  print_job_status
  print_node_status
  
  # Check if the upgrade is complete
  if is_upgrade_complete; then
    echo "Upgrade complete!"
    break
  else
    echo "Upgrade still in progress..."
  fi
  
  # Wait for the defined interval before checking again
  sleep ${CHECK_INTERVAL}
done
