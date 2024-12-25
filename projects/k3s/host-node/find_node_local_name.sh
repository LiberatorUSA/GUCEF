#!/bin/bash

# Try to get the hostname
host_name=$(hostname)

# Try avahi-resolve (mDNS)
if command -v avahi-resolve &> /dev/null; then
    local_name=$(avahi-resolve -a $(hostname -I) | awk '{print $2}')
fi

# If avahi-resolve doesn't give a result, try nslookup
if [ -z "$local_name" ] && command -v nslookup &> /dev/null; then
    local_name=$(nslookup $(hostname -I) | grep name | awk '{print $4}')
fi

# Try dig as a fallback
if [ -z "$local_name" ] && command -v dig &> /dev/null; then
    local_name=$(dig -x $(hostname -I) +short)
fi

# Output the found name or default to hostname if no name is found
echo "Hostname: $host_name"
if [ -z "$local_name" ]; then
    echo "No .local or .lan name found"
else
    echo ".local or .lan name: $local_name"
fi
