#!/bin/bash

# Jenkins info
JENKINS_URL="http://localhost:32000"
ADMIN_USER="admin"
ADMIN_PASS="admin_password"

# New user info
CICD_USER_NAME="auto_cicd"
CICD_USER_PASS="auto_cicd_password"
CICD_USER_FULLNAME="Auto CICD"
CICD_USER_EMAIL=""

# Jenkins CLI jar path
JENKINS_CLI_JAR="./jenkins-cli.jar"

# Check if the CLI jar exists
if [ ! -f ${JENKINS_CLI_JAR} ]; then
    echo "Error: jenkins-cli.jar not found at ${JENKINS_CLI_JAR}.Attempting download."
	./copy_jenkins_client_cli_from_pod.sh
    if [ ! -f ${JENKINS_CLI_JAR} ]; then	
        echo "Error: jenkins-cli.jar not found at ${JENKINS_CLI_JAR}. Please ensure the file exists."
        exit 1
	fi
fi

# Create the user
java -jar ${JENKINS_CLI_JAR} -s ${JENKINS_URL} -auth ${ADMIN_USER}:${ADMIN_PASS} groovy = <<EOF
jenkins.model.Jenkins.instance.securityRealm.createAccount('${CICD_USER_NAME}', '${CICD_USER_PASS}')
EOF

echo "User ${CICD_USER_NAME} created successfully."

# Write the contents of the variable to a text file
echo "$CICD_USER_NAME" > ./CICD_USER_NAME.txt
echo "$CICD_USER_PASS" > ./CICD_USER_PASS.txt

export CICD_USER_NAME
export CICD_USER_PASS
