#!/bin/bash
subfolder=jantje
export PATH=${PATH}:/www/cgi-bin/${subfolder}
SerialSpeed=115200
Port=ttyATH0
PortName=/dev/${Port}
UploadPortName=/dev/${Port}
HOST_NAME=`hostname`
HugeRemoteStorageServer=nas.local
WebLocationDynamic=/www/cgi-bin/${subfolder}
WebLocationStatic=/www/${subfolder}
LogFileSuffix=.csv
staticfolderURL=/${subfolder}/

#the root of the storages
RamDiskStorage=/tmp/${subfolder}
CommonBigStorage=/mnt/sda1/${HOST_NAME}/${subfolder}
CommonHugeRemoteMountPoint=/mnt/nas/



#the root of the storages per port (and system)
FastSmallStorage=${RamDiskStorage}/${Port}
SlowBigStorage=${CommonBigStorage}/${Port}
HugeRemoteStorage=/mnt/nas/${HOST_NAME}/${Port}


backupLocation=/mnt/nas/${HOST_NAME}/backup

    
    
OutputHtml=${SlowBigStorage}/generated.html
LogHeaderFile=${SlowBigStorage}/header.txt



BeginHtml=${WebLocationStatic}/input.begin.html
EndHtml=${WebLocationStatic}/input.end.html
defaulthtmlFile=${WebLocationStatic}/default.html
            

LogFilePrefix=${SlowBigStorage}/log/Arduino
ErrorFolder=${SlowBigStorage}/log
ErrorLog=${ErrorFolder}/error.log



export DumpFile=${FastSmallStorage}/dump.txt
export OutputValue=${FastSmallStorage}/value.txt
export logFile=${FastSmallStorage}/Arduino.log


#script locations
MakeWepPage=${WebLocationDynamic}/MakeWebPage.sh

SttyCommand="stty -F ${PortName}  ${SerialSpeed}  raw -clocal -echo icrnl"

