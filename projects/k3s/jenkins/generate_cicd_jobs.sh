#!/bin/bash

# Variables
JENKINS_URL="http://localhost:32000"
JENKINS_JOB_NAME="GUCEF"
CASC_CONFIG_FILE="./GUCEF_Build.xml"
. ./get_cicd_user_info.sh

# Convert relative paths to absolute paths
CASC_CONFIG_FILE=$(realpath "$CASC_CONFIG_FILE")

# Read the content of the configuration file
CASC_CONFIG_CONTENT=$(cat "$CASC_CONFIG_FILE")

# Function to check HTTP response code
check_response() {
    if [ $1 -ne 200 ]; then
        echo "Error: Received HTTP response code $1"
        exit 1
    fi
}

# Upload CasC configuration

# First check to see if the job already exists
echo "Checking if the job ${JENKINS_JOB_NAME} exist..."
RESPONSE=$(curl -s -o /dev/null -w "%{http_code}" -X GET "$JENKINS_URL/job/${JENKINS_JOB_NAME}/api/xml/config.xml" \
    --user "$CICD_USER_NAME:$CICD_USER_TOKEN" \
    -H "Content-Type: application/xml" \
    --data-binary "$CASC_CONFIG_CONTENT")
echo "Test returned http result code $RESPONSE"	
if [ $RESPONSE -eq 200 ]; then
	echo "Config for ${JENKINS_JOB_NAME} exists already, will perform an update instead of a create action"
	
	# Update the Jenkins job
	RESPONSE=$(curl -s -o /dev/null -w "%{http_code}" -X POST "$JENKINS_URL/job/${JENKINS_JOB_NAME}/api/xml/config.xml" \
		--user "$CICD_USER_NAME:$CICD_USER_TOKEN" \
		-H "Content-Type: application/xml" \
		--data-binary "$CASC_CONFIG_CONTENT")

	# Check the response
	check_response $RESPONSE	
	
	echo "Job successfully updated with response code $RESPONSE"
else
	echo "Config for ${JENKINS_JOB_NAME} is not available, will try to create a new job"
	
	# POST request to create the Jenkins job
	RESPONSE=$(curl -s -o /dev/null -w "%{http_code}" -X POST "$JENKINS_URL/createItem?name=${JENKINS_JOB_NAME}" \
		--user "$CICD_USER_NAME:$CICD_USER_TOKEN" \
		-H "Content-Type: application/xml" \
		--data-binary "$CASC_CONFIG_CONTENT")

	# Check the response
	check_response $RESPONSE

	echo "Job successfully created with response code $RESPONSE"
fi	
