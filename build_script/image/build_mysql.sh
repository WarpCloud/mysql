#!/bin/bash
function build_mysql
## The function name should be the same as script's name
{
    set -e

    # 1.if $MYSQL_SRC exists, cd
    if [ $MYSQL_SRC ]; then
        cd $MYSQL_SRC
        chmod -R o=g *
    else
        echo "MYSQL_SRC not set, exit"
        exit 1
    fi

    # 2. docker build
    docker build -f docker/Dockerfile -t mysql .

    # 3. rename image && push
    docker tag mysql:latest ${DOCKER_REPO_URL}/${BUILDER}/${COMPONENT_BASE}:${IMAGE_TAG}
    docker push ${DOCKER_REPO_URL}/${BUILDER}/${COMPONENT_BASE}:${IMAGE_TAG}
}
