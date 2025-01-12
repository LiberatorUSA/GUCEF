#!/bin/bash

# Apply Longhorn disk configuration
. ./generate-longhorn-disks-config.sh
sudo kubectl apply -f ./longhorn-disks.yaml -n longhorn-system

# Now that the disks are defined we can define storage classes which will use them
sudo kubectl apply -f storage_class_jbod_disks.yaml -n longhorn-system
sudo kubectl apply -f storage_class_mirror_disks.yaml -n longhorn-system
sudo kubectl apply -f storage_class_raid_disks.yaml -n longhorn-system

# After setting up the storage you still need to explicitly mark the disks as usable ('schedulable' in longhorn lingo)
. ./mark-disks-schedulable.sh