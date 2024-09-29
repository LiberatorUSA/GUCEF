#!/bin/sh
set -e

preinstalled_files=$(find /jenkins_home_backup/ | sed -e "s/[^-][^\/]*\//  |/g" -e "s/|\([^ ]\)/|-\1/")
echo "preinstalled files in jenkins_home_backup: ${preinstalled_files}"

preinstalled_files=$(find /var/jenkins_home/ | sed -e "s/[^-][^\/]*\//  |/g" -e "s/|\([^ ]\)/|-\1/")
echo "preinstalled files in /var/jenkins_home: ${preinstalled_files}"

cp -r -f -n /jenkins_home_backup/* /var/jenkins_home/

echo "Moving pre-installed plugins to mounted data volume which is our new JENKINS_HOME"
mkdir -p /var/jenkins_home/data/plugins
mv -f /var/jenkins_home/plugins/* /var/jenkins_home/data/plugins/ || echo "No plugins to move"

echo "Moving pre-installed config to mounted casc volume if no config is present there"
mv -vn /var/jenkins_home/jenkins_config.yaml /var/jenkins_home/casc_configs/jenkins_config.yaml

jenkins_home_contents=$(find ${JENKINS_HOME}/ | sed -e "s/[^-][^\/]*\//  |/g" -e "s/|\([^ ]\)/|-\1/")
echo "files structure now in JENKINS_HOME: ${jenkins_home_contents}"
