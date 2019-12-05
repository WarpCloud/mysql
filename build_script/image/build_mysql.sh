#!/bin/bash
function NOT_ALLOWED_BRANCH_TEST() {
  if [[ ${CI_PROJECT_NAMESPACE} != "OLTP" ]]; then
    for br in ${RELEASE_BR[@]}; do
      if [[ ${BRANCH_NAME} == ${br} ]]; then
        echo "merge your branch, ${BRANCH_NAME}@${CI_PROJECT_NAMESPACE}/mysql, into ${BRANCH_NAME}@OLTP/mysql"
        exit 1
      fi
    done
  fi
}

function build_mysql() {
  ## The function name should be the same as script's name
  set -e

  NOT_ALLOWED_BRANCH_TEST

  if [[ -n ${MYSQL_SRC} ]]; then
    cd ${MYSQL_SRC}
    chmod -R o=g *
  else
    echo "MYSQL_SRC not set, exit"
    exit 1
  fi

  ARCH=$(uname -i)
  docker build -f ci/${ARCH}/Dockerfile -t mariadb-${ARCH} .
  docker tag mariadb-${ARCH}:latest ${DOCKER_REPO_URL}/${BUILDER}/${COMPONENT_BASE}:${ARCH}-${IMAGE_TAG}
  docker push ${DOCKER_REPO_URL}/${BUILDER}/${COMPONENT_BASE}:${ARCH}-${IMAGE_TAG}
  docker tag ${DOCKER_REPO_URL}/${BUILDER}/${COMPONENT_BASE}:${ARCH}-${IMAGE_TAG} ${DOCKER_REPO_URL}/${BUILDER}/${COMPONENT_BASE}:${ARCH}-${BRANCH_NAME}
  docker push ${DOCKER_REPO_URL}/${BUILDER}/${COMPONENT_BASE}:${ARCH}-${BRANCH_NAME}
}
