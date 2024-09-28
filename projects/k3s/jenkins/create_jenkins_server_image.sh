#!/bin/bash

#if [ $# -eq 0 ]
#  then
#    tag='latest'
#  else
#    tag=$1
#fi


# first get a listing of the current container for analysis
docker run jenkins/jenkins --version
docker inspect jenkins/jenkins
#docker run jenkins/jenkins:lts --version
#docker inspect jenkins/jenkins:lts

# build the derived jenkins server image
docker build -f jenkins_server_build_image.dockerfile . -t vanvelzen/jenkins-server-base:latest

# save the image as a local tar file
docker save --output ./vanvelzen-jenkins-server-base-latest.tar vanvelzen/jenkins-server-base:latest

# Import the image into k3s
sudo k3s ctr images import ./vanvelzen-jenkins-server-base-latest.tar -t vanvelzen/jenkins-server-base:latest

