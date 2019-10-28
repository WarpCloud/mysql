#!/usr/bin/env bash
function IS_RELEASE_BRANCH() {
  local release="false"
  RELEASE_BR_ARR=(${RELEASE_BR})
  for br in ${RELEASE_BR_ARR[@]}; do
    if [[ ${BRANCH_NAME} == ${br} ]]; then
      release="true"
      break
    fi
  done
  if [[ ${release} == "false" ]]; then
    echo "will not generate release image, because ${BRANCH_NAME} is not release branch"
    exit 0
  fi
}

set -e
IS_RELEASE_BRANCH
echo "tag release image, push to 172.16.1.99/kundb repo."
${CI_SRC}/startdocker.sh &
mkdir -p ~/.docker && cp /root/.docker/kundb.json ~/.docker/ || true
trap "kill -9 $(ps aux | grep dockerd | grep -v grep | awk '{print $2}')" ERR
sleep $(echo ${SECOND_WAIT_DOCKERD})
export SOURCE="gold"
export TARGET="kundb"
ARCH=$(uname -i)
# tag and push kundb
docker pull ${DOCKER_REPO_URL}/${SOURCE}/${COMPONENT_BASE}:${ARCH}-${BRANCH_NAME}
docker tag ${DOCKER_REPO_URL}/${SOURCE}/${COMPONENT_BASE}:${ARCH}-${BRANCH_NAME} ${DOCKER_REPO_URL}/${TARGET}/${ARCH}/${COMPONENT_BASE}:${BRANCH_NAME}
docker push ${DOCKER_REPO_URL}/${TARGET}/${ARCH}/${COMPONENT_BASE}:${BRANCH_NAME}
# stop docker in docker
echo "kill dockerd before exit"
kill -9 $(ps aux | grep dockerd | grep -v grep | awk '{print $2}')
