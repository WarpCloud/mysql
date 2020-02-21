#!groovy
@Library('jenkins-library') _

pipeline {
  agent {
    kubernetes {
      defaultContainer 'builder'
      label UUID.randomUUID().toString()
      yaml kubernetesTemplate.kubernetesTemplate('172.16.1.99/gold/tos-package-builder', '12G')
    }
  }
  environment {
    DockerRegistry = '172.16.1.99'
    DOCKER_HOST = 'unix:///var/run/docker.sock'
    DOCKER_REPO = '172.16.1.99'
    COMPONENT_NAME = 'kundb'
    DOCKER_PROD_NS = 'kundb-ci'
    DOCKER_HARBOR = credentials('harbor')
  }
  options {
    disableConcurrentBuilds()
    buildDiscarder(logRotator(artifactDaysToKeepStr: '', artifactNumToKeepStr: '', daysToKeepStr: '60', numToKeepStr: '100'))
    gitLabConnection('gitlab-172.16.1.41')
    durabilityHint('PERFORMANCE_OPTIMIZED')
    timestamps()
    retry(2)
    timeout(time: 24, unit: 'HOURS')
    gitlabBuilds(builds: ['kundb-mysql-ci'])
  }
  parameters {
    booleanParam(defaultValue: false, description: '', name: 'RELEASE')
    string(defaultValue: env.BRANCH_NAME, description: '', name: 'RELEASE_TAG')
  }
  triggers {
    cron('H 23 * * *')
  }
  stages {
    stage('checkout') {
      steps {
        waitDocker {}
        updateGitlabCommitStatus(name: 'kundb-mysql-ci', state: 'pending')
        checkout scm
        // checkout scm: [$class: 'GitSCM', extensions: [[$class: 'CheckoutOption', timeout: 30]]]
      }
    }

    stage('compile x86_64 mysql') {
      steps {
        updateGitlabCommitStatus(name: 'kundb-mysql-ci', state: 'running')
        sh """#!/bin/bash -ex
          docker login -u \$DOCKER_HARBOR_USR -p \$DOCKER_HARBOR_PSW \$DockerRegistry

          REV=\$(git rev-parse HEAD)

          export IMG_NAME=\${DockerRegistry}/\${DOCKER_PROD_NS}/x86_64/mysql:${params.RELEASE_TAG}
          docker build --label CODE_REVISION=\${REV} \
            --label BRANCH=${params.RELEASE_TAG} \
            --label COMPILE_DATE=\$(date +%Y%m%d-%H%M%S) \
            -f ci/x86_64/Dockerfile -t \${IMG_NAME} .

          docker push \${IMG_NAME}
        """
      }
    }

    stage('compile arm64 mysql') {
      steps {
        updateGitlabCommitStatus(name: 'kundb-mysql-ci', state: 'running')
        sh """#!/bin/bash -ex
          docker login -u \$DOCKER_HARBOR_USR -p \$DOCKER_HARBOR_PSW \$DockerRegistry
          docker run --rm --privileged 172.16.1.99/transwarp/multiarch/qemu-user-static:register --reset

          REV=\$(git rev-parse HEAD)

          export IMG_NAME=\${DockerRegistry}/\${DOCKER_PROD_NS}/aarch64/mysql:${params.RELEASE_TAG}
          docker build --label CODE_REVISION=\${REV} \
            --label BRANCH=${params.RELEASE_TAG} \
            --label COMPILE_DATE=\$(date +%Y%m%d-%H%M%S) \
            -f ci/aarch64/Dockerfile -t \${IMG_NAME} .

          docker push \${IMG_NAME}
        """
      }
    }
  }
  post {
    success {
      updateGitlabCommitStatus name: 'kundb-mysql-ci', state: 'success'
    }
    failure {
      updateGitlabCommitStatus name: 'kundb-mysql-ci', state: 'failed'
    }
    always {
      script {
        sendMail2 {
          emailRecipients = "kundb-dev@transwarp.io"
          attachLog = false
        }
      }
    }
  }
}
