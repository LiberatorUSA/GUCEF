# syntax=docker/dockerfile:1.3-labs

# First create base image
FROM jenkins/jenkins AS jenkins-server-base

# install build tooling in derived base image
FROM jenkins-server-base
ENV JENKINS_HOME=/var/jenkins_home
ENV JENKINS_WAR=/usr/share/jenkins/jenkins.war
ENV JENKINS_URL=http://localhost:8080/jenkins/
ENV CASC_JENKINS_CONFIG=/var/jenkins_home/jenkins_config.yaml

# Copy our script to obtain the jenkins client cli and make it available to the 'user' 
# this is useful for kubectrl exec later
COPY ["extract_jenkins_client_cli.sh", "/var/jenkins_home/"]
RUN /var/jenkins_home/extract_jenkins_client_cli.sh
ENV JENKINS_WAR=${JENKINS_WAR}

# Already copy the jenkins init config into the image
# We can always still update it later if needed via kubectrl cp
COPY ["jenkins_config.yaml", "/var/jenkins_home/"]

# Copy our plugin bootstrap helper script into the image and execute it
COPY ["jenkins-plugin-stuffer.sh", "/var/jenkins_home/"]
#RUN /var/jenkins_home/jenkins-plugin-stuffer.sh configuration-as-code
RUN jenkins-plugin-cli --plugins configuration-as-code

# Switch to root to perform file operations
USER root

# we use a 'backup' folder to ensure we have a staging place for files that wont be impacted by mounting of volumes
RUN mkdir /jenkins_home_backup
RUN mkdir /jenkins_home_backup/plugins
RUN cp -r -f -n /usr/share/jenkins/ref/plugins/* /jenkins_home_backup/plugins
RUN cp -r -f -n /var/jenkins_home/* /jenkins_home_backup

# Copy the custom entrypoint script
COPY ["jenkins_container_entrypoint.sh", "jenkins_container_entrypoint_bootstrap.sh", "jenkins_container_entrypoint_hello.sh", "/usr/local/bin/"]
RUN chmod +x /usr/local/bin/jenkins_container_entrypoint.sh
RUN chmod +x /usr/local/bin/jenkins_container_entrypoint_bootstrap.sh
RUN chmod +x /usr/local/bin/jenkins_container_entrypoint_hello.sh

# Set the entrypoint
ENTRYPOINT ["/usr/local/bin/jenkins_container_entrypoint.sh"]

# Switch back to normal jenkins user
USER jenkins
