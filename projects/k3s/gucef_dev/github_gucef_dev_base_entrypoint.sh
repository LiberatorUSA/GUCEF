#!/bin/sh
set -e

# Put something else here as needed ...

# Run the repository setup script
/repo_setup.sh

# Start supervisord
exec /usr/bin/supervisord -c /etc/supervisor/conf.d/supervisord.conf
