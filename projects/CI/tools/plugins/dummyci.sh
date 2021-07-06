#!/bin/bash

# Documentation 
read -r -d '' USAGE_TEXT << EOM
Usage: dummyci.sh command [<param>...]
Run given command in dummy test.

Available dummy commands:  
    build <project_name>    start build of given project
                            outputs build request id
                            requires: GITHUB_REF
    status <build_number>   get status of build identified by given build number
                            outputs one of: success | failed | null
    kill <build_number>     kills running build identified by given build number                            
    hash <position>         get revision hash on given positions
                            available positions:
                                last        hash of last succesfull build commit
                                            only commits of 'build' job are considered
                                current     hash of current commit
    help                    display this usage text                             
EOM

set -e

# Functions

##
# Print message on stderr to do not affect stdout which can be used as input to another commands.
#
# Input:
#    MESSAGE - message to print
#
function log {
    MESSAGE=$1
    >&2 echo "$MESSAGE"
}

##
# Print error message and exit program with status code 1
#
# Input:
#   MESSAGE - error message to show
##
function fail {
    MESSAGE=$1
    log "ERROR: $MESSAGE"
    log "$USAGE_TEXT"
    exit 1
}

##
# Fast fail when given environment variable is not set.
#
# Input:
#   ENV_VAR - name of environment variable to check
##
function require_env_var {
    local ENV_VAR=$1
    if [[ -z "${!ENV_VAR}" ]]; then
        fail "$ENV_VAR is not set"
    fi  
}

##
# Fast fail when given parameter is empty
#
# Input:
#   MESSAGE - message to show when requirement is not met
#   PARAM - parameter which should be not null
##
function require_not_null {
    local MESSAGE=$1
    if [[ -z "$2" ]]; then
        fail "$MESSAGE"
    fi
}

##
# Make HTTP POST call to the void
#
# Input:
#   URL - part of URL after dummy repo base url
#   DATA - form data to post (optional)
##
function post {
	return 0
}

##
# Make HTTP GET call to the void
#
# Input:
#   URL - part of URL after dummy base url
##
function get {
    echo ""
	return 0
}

##
# Get current branch name
#
# Input:
#   REQUIRED - any not empty value to perform validation on existence of environment variable with branch name
##
function get_branch {
    echo "foobar_dummy_branch"
}

##
# Trigger build in the void
#
# Build in dummy land is never actually triggered
#
# Input:
#   PROJECT_NAME - name of project to start build for
#
# Output:
#   build number
##
function trigger_build {
    local PROJECT_NAME=$1
    require_not_null "Project name not speficied" ${PROJECT_NAME} 
    BRANCH=$(get_branch required)
    NOW=$(date -u +"%Y-%m-%dT%H:%M:%SZ")
    BODY="$(cat <<-EOM
    {
        "event_type": "build-${PROJECT_NAME}",
        "client_payload": {
            "job": "${PROJECT_NAME}"
        }
    }
EOM
    )"
    post dispatches "${BODY}"

    echo 'null'
}

##
# Get status of dummy build
#
# Input:
#   BUILD_ID - id of build (workflow run id)
#
# Output:
#   success | failed | null
##
function get_build_status {
	echo "success"
}


##
# Kill dummy build
#
# Input:
#   BUILD_ID - id of build (workflow run id)
##
function kill_build {
	return 0
}

##
# Get revision hash of last successful commit which invokes main monorepository build
#
# Output:
#   revision hash or null when there were no commits yet
##
function get_last_successful_commit {
    BRANCH=$(get_branch)
    if [[ -n "$BRANCH" ]]; then
        BRANCH_FILTER='and .head_branch == "'${BRANCH}'"'
    fi
    SELECTOR='.conclusion == "success" '${BRANCH_FILTER}''
    	
	echo "3f7442a022b9d969b141ec804458c4c12b53baef"
}

##
# Get revision hash of current commit
#
# Output:
#   revision hash or null when there were no commits yet
##
function get_current_commit {
    echo "3f7442a022b9d969b141ec804458c4c12b53baec"
}

##
# Main
##

# Validatate common requirements
#		There are none - dummy

# Parse command
case $1 in
    build)        
        trigger_build $2
        ;;
    status)
        get_build_status $2
        ;;
    kill)
        kill_build $2
        ;;    
    hash)
        case $2 in
            last)
                get_last_successful_commit
                ;;
            current)
                get_current_commit
                ;;
            *)
                fail "Unknown hash position $2"             
                ;;
        esac
        ;;        
    *)
        fail "Unknown command $1"
        ;;        
esac