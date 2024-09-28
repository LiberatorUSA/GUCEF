#!/bin/bash

# Define the directory to search
BASE_DIR="/mnt"

# Define the output configuration file
OUTPUT_FILE="./local-path-config-updated.json"

# Initialize arrays to store paths for each storageClass
declare -A storage_paths
storage_paths["raid"]=""
storage_paths["jbod"]=""

# Fetch the current configmap and extract the config.json using kubectl
current_config=$(sudo kubectl get configmap local-path-config -n kube-system -o json | jq -r '.data["config.json"]')

# Backup the current config to a file before modifying
echo "$current_config" > ./local-path-config-backup.json

# Parse the current nodePathMap section from the config
node_path_map=$(echo "$current_config" | jq '.nodePathMap')

# Loop through directories under /mnt
for dir in "$BASE_DIR"/*; do
  if [ -d "$dir" ]; then
    # Check if directory name contains "mirror" or "raid"
    if [[ "$dir" == *mirror* || "$dir" == *raid* ]]; then
      # Add the path to the raid array if it's not already there
      if [[ ! "${storage_paths["raid"]}" =~ "$dir" ]]; then
        storage_paths["raid"]="${storage_paths["raid"]} \"$dir\","
        echo "  Adding $dir to storageClass 'raid'"
      fi
    fi

    # Check if directory name contains "jbod"
    if [[ "$dir" == *jbod* ]]; then
      # Add the path to the jbod array if it's not already there
      if [[ ! "${storage_paths["jbod"]}" =~ "$dir" ]]; then
        storage_paths["jbod"]="${storage_paths["jbod"]} \"$dir\","
        echo "  Adding $dir to storageClass 'jbod'"
      fi
    fi
  fi
done

# Function to update or add storageClass paths in the nodePathMap
update_paths() {
  local storage_class=$1
  local new_paths=$2
  local updated_config

  if [ -n "$new_paths" ]; then
    # Remove trailing comma from new paths
    new_paths=$(echo "$new_paths" | sed 's/,$//')

    # Check if the storageClass already exists in the config
    storage_class_exists=$(echo "$node_path_map" | jq --arg sc "$storage_class" '.[] | select(.storageClass == $sc)')

    if [ -n "$storage_class_exists" ]; then
      # If the storageClass exists, merge the new paths
      updated_config=$(echo "$node_path_map" | jq --arg sc "$storage_class" --argjson paths "[$new_paths]" \
        '(.[] | select(.storageClass == $sc) | .paths) += $paths')
    else
      # If the storageClass doesn't exist, add a new entry
      new_entry=$(jq -n --arg sc "$storage_class" --argjson paths "[$new_paths]" \
        '{ "node": "DEFAULT_PATH_FOR_NON_LISTED_NODES", "storageClass": $sc, "paths": $paths }')

      updated_config=$(echo "$node_path_map" | jq --argjson entry "$new_entry" '. += [$entry]')
    fi

    # Update nodePathMap with the new entry or merged paths
    node_path_map="$updated_config"
  fi
}

# Update paths for "raid" and "jbod" storage classes
update_paths "raid" "${storage_paths["raid"]}"
update_paths "jbod" "${storage_paths["jbod"]}"

# Add the default path entry if it's not already in the config
default_exists=$(echo "$node_path_map" | jq '.[] | select(.storageClass == null)')
if [ -z "$default_exists" ]; then
  node_path_map=$(echo "$node_path_map" | jq '. += [{"node": "DEFAULT_PATH_FOR_NON_LISTED_NODES", "paths":["/var/lib/rancher/k3s/storage"]}]')
fi

# Merge the updated nodePathMap back into the original config
updated_config=$(echo "$current_config" | jq --argjson newNodePathMap "$node_path_map" '.nodePathMap = $newNodePathMap')

# Write the updated config to a file
echo "$updated_config" > "$OUTPUT_FILE"

echo "Updated configuration written to $OUTPUT_FILE"
