#!/bin/bash

# Define the directory to search
BASE_DIR="/mnt"

# Define the output configuration file
OUTPUT_FILE="./longhorn-disks.yaml"

# get the node name as variabke KS_NODE_NAME for use below
. ./../host-node/get_ks_node_name.sh

# Initialize arrays for different disk types
declare -A disk_paths
disk_paths["raid"]=""
disk_paths["mirror"]=""
disk_paths["jbod"]=""

# Define keywords
keywords=("jbod" "raid" "mirror")

# Scan mounts under /mnt
for mount in /mnt/*; do
  for keyword in "${keywords[@]}"; do
    if [[ "$mount" =~ $keyword ]]; then
      # Add mount path to corresponding keyword in the disk_paths array
      disk_paths["$keyword"]+="$mount "
    fi
  done
done

# Create longhorn-disks.yaml
cat > longhorn-disks.yaml <<EOF
apiVersion: longhorn.io/v1beta1
kind: Node
metadata:
  name: ${KS_NODE_NAME}
spec:
  disks:
EOF

# Add disk paths to longhorn-disks.yaml
for keyword in "${!disk_paths[@]}"; do
  for path in ${disk_paths["$keyword"]}; do
    cat >> longhorn-disks.yaml <<EOF
    ${keyword}-disk-$(basename $path):
      path: $path
      allowScheduling: true
      tags:
        - $keyword
EOF
  done
done

echo "${OUTPUT_FILE} generated successfully."

