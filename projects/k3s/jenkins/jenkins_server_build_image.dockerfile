# syntax=docker/dockerfile:1

# First create base image
FROM jenkins/jenkins AS jenkins-server-base

# install build tooling in derived base image
FROM jenkins-server-base
ENV JENKINS_HOME=/var/jenkins_home
ENV JENKINS_WAR=/usr/share/java/jenkins.war
ENV JENKINS_URL=http://localhost:8080/jenkins/
ENV CASC_JENKINS_CONFIG=/var/jenkins_home/jenkins_config.yaml

# Copy our script to obtain the jenkins client cli and make it available to the 'user' 
# this is useful for kubectrl exec later
COPY ["extract_jenkins_client_cli.sh", "/var/jenkins_home/"]
RUN /var/jenkins_home/extract_jenkins_client_cli.sh
ENV JENKINS_WAR=${JENKINS_WAR}

# Copy our plugin bootstrap helper script into the image and execute it
COPY ["jenkins-plugin-stuffer.sh", "/var/jenkins_home/"]
RUN /var/jenkins_home/jenkins-plugin-stuffer.sh configuration-as-code

# Already copy the jenkins init config into the image
# We can always still update it later if needed via kubectrl cp
COPY ["jenkins_config.yaml", "/var/jenkins_home/"]
