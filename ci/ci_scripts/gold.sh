#!/usr/bin/env bash
set -e
echo "tag postcommit image to gold image."
${CI_SRC}/startdocker.sh &
mkdir -p ~/.docker && cp /root/.docker/config.json ~/.docker/ || true
trap "kill -9 $(ps aux | grep dockerd | grep -v grep | awk '{print $2}')" ERR
sleep $(echo ${SECOND_WAIT_DOCKERD})
export SOURCE="postcommit"
export TARGET="gold"
ARCH=$(uname -i)
# tag and push kundb
docker pull ${DOCKER_REPO_URL}/${SOURCE}/${COMPONENT_BASE}:${ARCH}-${BRANCH_NAME}
docker tag ${DOCKER_REPO_URL}/${SOURCE}/${COMPONENT_BASE}:${ARCH}-${BRANCH_NAME} ${DOCKER_REPO_URL}/${TARGET}/${COMPONENT_BASE}:${ARCH}-${BRANCH_NAME}
docker push ${DOCKER_REPO_URL}/${TARGET}/${COMPONENT_BASE}:${ARCH}-${BRANCH_NAME}
# stop docker in docker
echo "kill dockerd before exit"
kill -9 $(ps aux | grep dockerd | grep -v grep | awk '{print $2}')
