#!/bin/ash
#These settings are environment dependent.
let NumberOfLineTerminatorsToSkip=1
#end of environment specific section


InDump=0
NUM=0
OutputHtml=/www/robot/robot.html
LogFilePrefix=/mnt/log/Arduino
LogFileSuffix=.csv
MakeWepPage=/www/cgi-bin/robot/MakeWebPage.sh

DumpFile=/tmp/dump.txt
OutputValue=/tmp/value.txt
LogHeaderFile=/tmp/header.txt
logFile=/tmp/Arduino.log

BeginHtml=input.begin.html
EndHtml=input.end.html


let LOGINDEX=0

PAGENUM=0
SPACE=" "
let DUMPLINE=0

MoveLogFile()
{
 let NUM=0
 thetime=`date +%Y_%m_%d_%H_%M`
 dumpFile=${LogFilePrefix}${thetime}${LogFileSuffix}
 while [ -f ${dumpFile} ]
 do
  let NUM+=1
  dumpFile=${LogFilePrefix}${thetime}${NUM}${LogFileSuffix}
 done
 echo End Of Log>>${logFile}
 mv ${logFile} ${dumpFile}
 cp ${LogHeaderFile} ${logFile}
}


MoveLogFile
while read line
do
# change NumberOfLineTerminatorsToSkip at the top of the script
# if the input lines seem to be cut or contains trailers
  let tmp=0
  while [ $tmp -lt $NumberOfLineTerminatorsToSkip ]
  do
    let tmp+=1
    line=${line%%?} #this is needed on openwrt but not on my nas
  done
                                  
  let DUMPLINE+=1
  if [ $DUMPLINE -gt 1000 ]; then
    let DUMPLINE=0
    MoveLogFile
  fi
  echo "${line}" >> $logFile
  if [ "$line" = "Dumping all fields" ]; then
    InDump=1
    echo "" > $DumpFile
    
  elif [ "$line" = "Dump done" ]; then
      ${MakeWepPage} <$DumpFile > $OutputHtml
      chmod 755  $OutputHtml
      InDump=0
  else
    FIRST=`echo $line | cut -d';' -f1`
    if [ "$FIRST" = "LOG VALUE" ]; then
      echo "$line" > $OutputValue
      chmod 755  $OutputValue
    elif [ "$FIRST" = "LOG HEADER" ]; then
        echo "$line" > $LogHeaderFile
    else
      FIRST=`echo $line | cut -d' ' -f1`
      #Arduino wants linksys to run a command
      if [ "$FIRST" = "COMMAND" ]; then 
       VALUE=`echo $line | cut -d' ' -f2-20`
       eval  ${VALUE} >> /dev/ttyS1
       #todo the above code is for when arduino sends a command to linksys
# for instance to check wether the is allowed to start
      fi 
    fi
  fi

  if [ "$InDump" -eq 1 ]; then
    echo $line >> $DumpFile
  fi
done
