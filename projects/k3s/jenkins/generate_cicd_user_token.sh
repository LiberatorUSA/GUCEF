#!/bin/bash

# Jenkins info
JENKINS_URL="http://localhost:32000"
ADMIN_USER="admin"
ADMIN_PASS="admin_password"

# Existing user info
CICD_USER_NAME="auto_cicd"

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

# Generate API token
CICD_USER_TOKEN=$(java -jar ${JENKINS_CLI_JAR} -s ${JENKINS_URL} -auth ${ADMIN_USER}:${ADMIN_PASS} groovy = <<EOF
println(jenkins.model.Jenkins.instance.getUser('${CICD_USER_NAME}').getProperty(jenkins.security.ApiTokenProperty).tokenStore.generateNewToken('auto_cicd_token').plainValue)
EOF
)

echo "Token for user ${CICD_USER_NAME} generated successfully."
echo "Token: ${CICD_USER_TOKEN}"

export CICD_USER_NAME
export CICD_USER_TOKEN

# Write the contents of the variable to a text file
echo "$CICD_USER_TOKEN" > ./CICD_USER_TOKEN.txt
