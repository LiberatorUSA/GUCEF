#!/bin/sh
set -e

# Put something else here as needed ...

# Start the SSH server
exec /usr/sbin/sshd -D
