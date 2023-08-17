#!/bin/ash 

. ./SetGlobalSettings.sh

InDump=0
NUM=0
EVENT_SEPARATOR=;
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
 if [ ! -f ${LogHeaderFile} ]; 
 then
    #The header file is missing so ask the header from arduino
    echo P>> ${PortName}
    echo LOG HEADER>> ${PortName}
 else
   cp ${LogHeaderFile} ${logFile}
 fi
}


MoveLogFile
while true
do
while read line
do
  let DUMPLINE+=1
  echo "${line}" >> $logFile

  if [ "$InDump" -eq 1 ]; then
    echo $line >> $DumpFile
#    if [ "$line" = "Dump done" ]; then
#        ${MakeWepPage} <$DumpFile > $OutputHtml
#        chmod 755  $OutputHtml
#        InDump=0
#    fi
    #OK it is 2 times the same thing but I changed the string wanted backwards compatibility
    #and didn't know how to do the if
   if [ "$line" = "done" ]; then
       ${MakeWepPage} <$DumpFile > $OutputHtml &
       chmod 755  $OutputHtml
       InDump=0
   fi
                                    
  else      
    if [ $DUMPLINE -gt 10000 ]; then
        let DUMPLINE=0
        MoveLogFile
    fi
    if [ "$line" = "Dumping all fields" ]; then
        InDump=1
        echo "" > $DumpFile
    else
        FIRST=`echo $line | cut -d';' -f1 |cut -d':' -f1`
        if [ "$FIRST" = "LOG VALUE" ]; then
            echo "$line" > $OutputValue
            chmod 755  $OutputValue
        elif [ "$FIRST" = "LOG HEADER" ]; then
             echo "$line" > $LogHeaderFile
        elif [ "$FIRST" = "EXEC" ]; then
#wait for this command to return. If the command is asynchronous the command must start the async action itself
          COMMAND=`echo $line | cut -d':' -f2`
          ${WebLocation}/${COMMAND} >> ${PortName} & 
        fi
    fi
  fi
done
sleep 1
done
