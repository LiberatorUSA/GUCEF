#!/bin/bash

# Function to check multiple file locations and set the result in a variable
find_file_location() {
    local filename="$1"
    local result_var="$2"
    shift 2
    local locations=("$@")
    
    for location in "${locations[@]}"; do
        if [ -f "$location/$filename" ]; then
            eval "$result_var='$location/$filename'"
            return 0
        fi
    done
    
    eval "$result_var=''"
    return 1
}

# Find where the jenkins war file is located
file_location="${JENKINS_WAR}"
if [ -f "$file_location" ]; then
    echo "jenkins.war is at location specified by JENKINS_WAR env var and found at: $file_location"
else
    find_file_location "jenkins.war" file_location "/usr/share/jenkins" "/usr/lib/jenkins" "/var/lib/jenkins" "/opt/jenkins"
    if [ -n "$file_location" ]; then
        echo "File found at: $file_location"
        JENKINS_WAR=$file_location
    else
        echo "ERROR: File jenkins.war not found in any of the specified locations."
        return 1
    fi
fi

# extract its contents
mkdir -p /tmp/jenkinswar/
cd /tmp/jenkinswar/
cp "${JENKINS_WAR}" /tmp/jenkinswar/
jar --extract --file "/tmp/jenkinswar/jenkins.war"
cd /tmp/jenkinswar/
JENKINS_CLIJAR=$(find . -iname "cli-*")
echo "Using the following jar as the jenkins-cli.jar: ${JENKINS_CLIJAR}"

# Copy the Jenkins CLI JAR to the user bin for convenient use
mkdir -p /usr/local/bin/
cp "${JENKINS_CLIJAR}" -o /usr/local/bin/jenkins-cli.jar
chmod +x /usr/local/bin/jenkins-cli.jar

# cleanup our tmp folder, we are done using it
find /tmp/jenkinswar/ -type f -delete