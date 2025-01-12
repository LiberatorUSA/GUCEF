#!/bin/bash

# Define variables
NAMESPACE="longhorn-system"

# Get some info from other helper scripts
# get the node name as variabke KS_NODE_NAME for use below
. ./../host-node/get_ks_node_name.sh

# Get the pod name but also wait until its ready
. ./wait_for_longhorn_manager_pod.sh
# Turn the pod name into an IP we can use to access the REST API
. ./../host-node/get_ip_for_pod.sh ${POD_NAME} ${NAMESPACE}
LONGHORN_MANAGER_IP=${POD_IP}
echo "LONGHORN_MANAGER_IP=${LONGHORN_MANAGER_IP}"

# Define variables
LONGHORN_API_URL="http://${LONGHORN_MANAGER_IP}:9500/v1"

# Get the node details
NODE_JSON=$(curl -s ${LONGHORN_API_URL}/nodes/${KS_NODE_NAME})

if [[ $? -ne 0 ]]; then
  echo "Failed to fetch node details from Longhorn API."
  exit 1
fi

if [[ -z $NODE_JSON ]]; then
  echo "Node details are empty. Please check the node name and Longhorn API URL."
  exit 1
fi

echo "Fetched node details successfully."

# Extract disk names from the node details
DISK_NAMES=$(echo ${NODE_JSON} | jq -r '.disks | keys[]')

if [[ -z $DISK_NAMES ]]; then
  echo "No disks found for the node ${KS_NODE_NAME}."
  exit 1
fi

# Loop through each disk and mark it as schedulable
for DISK_NAME in ${DISK_NAMES}; do
  # Get disk details
  DISK_JSON=$(echo ${NODE_JSON} | jq -r ".disks.\"${DISK_NAME}\"")

  if [[ -z $DISK_JSON ]]; then
    echo "Failed to fetch details for disk ${DISK_NAME}."
    continue
  fi

  # Update disk's allowScheduling flag to true
  UPDATED_DISK_JSON=$(echo ${DISK_JSON} | jq '.allowScheduling = true')

  # Update the node's disk configuration
  UPDATED_NODE_JSON=$(echo ${NODE_JSON} | jq ".disks.\"${DISK_NAME}\" = ${UPDATED_DISK_JSON}")

  # Send the updated node configuration to Longhorn API
  response=$(curl -s -X PUT -H "Content-Type: application/json" \
    -d "${UPDATED_NODE_JSON}" \
    ${LONGHORN_API_URL}/nodes/${KS_NODE_NAME})

  if [[ $? -ne 0 ]]; then
    echo "Failed to update disk ${DISK_NAME}."
    continue
  fi

  if echo "$response" | grep -q "error"; then
    echo "Error occurred while updating disk ${DISK_NAME}: $response"
    continue
  fi

  echo "Successfully updated disk ${DISK_NAME} and marked it as schedulable."
done

echo "Script execution completed."
