#!/bin/sh
set -e

# Put something else here as needed ...

# Start supervisord
exec /usr/bin/supervisord -c /etc/supervisor/conf.d/supervisord.conf
