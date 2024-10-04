#!/bin/bash

# First generate the cicd user account using our admin account
./generate_cicd_user.sh

# After creating the CICD user account lets get an API token for it
./generate_cicd_user_token.sh

