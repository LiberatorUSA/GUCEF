#!/bin/bash

# Define the port to check
PORT=22

# Check if the port is open
if nc -zv localhost $PORT 2>&1 | grep -q 'succeeded'; then
  echo "Port $PORT is open."

  # Check if the port is being used for SSH
  if ps aux | grep "sshd" | grep -q "$PORT"; then
    echo "Port $PORT is being used for SSH."
  else
    echo "Port $PORT is not being used for SSH."
  fi
else
  echo "Port $PORT is closed."
fi
