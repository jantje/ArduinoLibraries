#!/bin/bash  
#This scripts starts the whole arduino openwrt communication.
# start with DUMP the first time so that a web page is created.

. SetGlobalSettings.sh

mkdir -p ${CommonHugeRemoteMountPoint}
umount ${CommonHugeRemoteMountPoint}
mount -t cifs //${HugeRemoteStorageServer}/home ${CommonHugeRemoteMountPoint} -o user=Marvin2,pass=Marvin2
mkdir -p ${backupLocation}

mv  ${SlowBigStorage)/log/* ${backupLocation}/log
