#!/bin/bash

# Output file where the YAML will be saved
output_file="persistent-volumes.yaml"

# Check if 'bc' is installed, if not, attempt to install it
if ! command -v bc &> /dev/null; then
    echo "'bc' command not found. Installing 'bc'..."
    sudo apt-get update && sudo apt-get install -y bc

    if [ $? -ne 0 ]; then
        echo "Failed to install 'bc'. Please install it manually and rerun the script."
        exit 1
    fi
fi

# Function to convert human-readable disk size to Gi using bc
convert_to_gi() {
    size=$1
    if [[ $size == *K ]]; then
        echo "scale=2; ${size%K} / (1024*1024)" | bc
    elif [[ $size == *M ]]; then
        echo "scale=2; ${size%M} / 1024" | bc
    elif [[ $size == *G ]]; then
        echo "${size%G}"
    elif [[ $size == *T ]]; then
        echo "scale=2; ${size%T} * 1024" | bc
    else
        echo "${size}"
    fi
}

# Clear the output file if it already exists
> $output_file

# Loop through all mounted drives under /mnt
for mount_point in /mnt/*; do
    if [ -d "$mount_point" ]; then
        # Get the drive name (the folder name under /mnt)
        drive_name=$(basename "$mount_point")
        
        # Get the total size of the mounted drive
        size=$(df -h "$mount_point" | awk 'NR==2 {print $2}')
        capacity=$(convert_to_gi "$size")
        
        # Append Gi unit to the capacity
        capacity="${capacity}Gi"
        
        # Determine storage class based on mount name
        if [[ "$drive_name" == *jbod* ]]; then
            storage_class="jbod"
        elif [[ "$drive_name" == *mirror* || "$drive_name" == *raid* ]]; then
            storage_class="raid"
        else
            storage_class="default"  # You can change this to whatever is appropriate
        fi
        
        # Generate YAML for PersistentVolume
        cat >> $output_file <<EOF
apiVersion: v1
kind: PersistentVolume
metadata:
  name: ${drive_name}-pv
spec:
  capacity:
    storage: $capacity
  accessModes:
    - ReadWriteMany
  hostPath:
    path: $mount_point
  storageClassName: $storage_class
---
EOF
    fi
done

echo "PersistentVolume YAML file generated: $output_file"
