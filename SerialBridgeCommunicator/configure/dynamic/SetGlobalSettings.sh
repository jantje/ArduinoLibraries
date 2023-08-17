#!/bin/bash
subfolder=jantje
export PATH=${PATH}:/www/cgi-bin/${subfolder}
SerialSpeed=115200
Port=ttyATH0
PortName=/dev/${Port}
UploadPortName=/dev/${Port}
HOST_NAME=`hostname`
HugeRemoteStorageServer=nas.local
WebLocation=/www/cgi-bin/${subfolder}
StaticWebLocation=/www/${subfolder}
LogFileSuffix=.csv
staticfolder=/${subfolder}/

#the root of the storages
CommonFastStorage=/tmp/${subfolder}
CommonBigStorage=/mnt/sda1/${HOST_NAME}/${subfolder}
CommonHugeRemoteMountPoint=/mnt/nas/



#the root of the storages per port (and system)
FastSmallStorage=${CommonFastStorage}/${Port}
SlowBigStorage=${CommonBigStorage}/${Port}
HugeRemoteStorage=/mnt/nas/${HOST_NAME}/${Port}


backupLocation=/mnt/nas/${HOST_NAME}/backup

    
    
OutputHtml=${SlowBigStorage}/generated.html
LogHeaderFile=${SlowBigStorage}/header.txt



BeginHtml=${StaticWebLocation}/input.begin.html
EndHtml=${StaticWebLocation}/input.end.html
defaulthtmlFile=${StaticWebLocation}/default.html
            

LogFilePrefix=${SlowBigStorage}/log/Arduino
ErrorFolder=${SlowBigStorage}/log
ErrorLog=${ErrorFolder}/error.log



DumpFile=${FastSmallStorage}/dump.txt
OutputValue=${FastSmallStorage}/value.txt
logFile=${FastSmallStorage}/Arduino.log
pid=${FastSmallStorage}/pid


#script locations
MakeWepPage=${WebLocation}/MakeWebPage.sh

SttyCommand="stty -F ${PortName}  ${SerialSpeed}  raw -clocal -echo icrnl"

