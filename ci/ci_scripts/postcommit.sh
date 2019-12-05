#!/usr/bin/env bash
set -e
source ${CI_SRC}/postcommit.env
trap "kill -9 $(ps aux | grep dockerd | grep -v grep | awk '{print $2}')" ERR
set -ex && bash ${MYSQL_SRC}/script/jenkins_job_build.sh
echo "kill dockerd before exit" && kill -9 $(ps aux | grep dockerd | grep -v grep | awk '{print $2}')
