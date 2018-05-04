#!/bin/ash  
#This scripts starts the whole arduino openwrt communication.
# start with DUMP the first time so that a web page is created.

ScriptFolder=$(dirname $0)
cd ${ScriptFolder}
. ./SetGlobalSettings.sh
#startBonjour

#make sure ParseArduinoInput.sh is stopped
stopReadingArduino.sh

./fixmount

mkdir -p ${backupLocation}
umount ${CommonHugeRemoteMountPoint}
mount.cifs ${HugeRemoteStorageServer}:/home ${CommonHugeRemoteMountPoint} -o user=linksys,pass=DE4Fkhd6dtRz,sec=ntlm


#reset the tty
${SttyCommand}

#start the parsing
cd ${WebLocation}
mkdir -p ${ErrorFolder}
mkdir -p ${FastSmallStorage}
./ParseArduinoInput.sh < ${PortName} 2>&1 >>${ErrorLog} &
 
 #send comand to arduino to setup the environment
 sleep 1
 echo P>> ${PortName}
 echo LOG HEADER>> ${PortName}
 echo SET>> ${PortName}
if [ "$1" = "DUMP" ]; then 
  sleep 1
  echo DUMP>> ${PortName}
  echo LOG VALUE>> ${PortName}
else
  echo NODUMP
fi
