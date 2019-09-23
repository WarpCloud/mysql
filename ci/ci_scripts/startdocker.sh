#!/bin/bash

CGROUP=/sys/fs/cgroup
if ! mountpoint -q ${CGROUP}; then
  mount -t tmpfs -o uid=0,gid=0,mode=0755 cgroup ${CGROUP}
fi

if [[ -d /sys/kernel/security ]] && ! mountpoint -q /sys/kernel/security; then
  mount -t securityfs none /sys/kernel/security || {
    echo "Could not mount /sys/kernel/security."
    echo "AppArmor detection and --privileged mode might break."
  }
fi

for SUBSYS in $(cut -d: -f2 /proc/1/cgroup); do
  [[ -d ${CGROUP}/${SUBSYS} ]] || mkdir ${CGROUP}/${SUBSYS}
  mountpoint -q ${CGROUP}/${SUBSYS} ||
    mount -n -t cgroup -o ${SUBSYS} cgroup ${CGROUP}/${SUBSYS}

  echo ${SUBSYS} | grep -q ^name= && {
    NAME=$(echo ${SUBSYS} | sed s/^name=//)
    ln -s ${SUBSYS} ${CGROUP}/${NAME}
  }

  [[ ${SUBSYS} == cpuacct,cpu ]] && ln -s ${SUBSYS} ${CGROUP}/cpu,cpuacct
done

if [[ ! -f /tmp/resolv_conf_checker ]]; then
  echo >>/etc/resolv.conf
  touch /tmp/resolv_conf_checker
fi

UUID=$(cat /proc/sys/kernel/random/uuid)
mkdir -p /var/lib/docker/${UUID}
mount -o bind /var/lib/docker/${UUID} /var/lib/docker/image/aufs/layerdb/mounts

DOCKER_CMD="docker daemon --insecure-registry 172.16.1.99 $EXTRA_OPTS"
if [[ $ENABLE_OVERLAY == "true" ]]; then
  DOCKER_CMD="$DOCKER_CMD -s overlay"
fi

${DOCKER_CMD}
