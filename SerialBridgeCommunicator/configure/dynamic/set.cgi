#!/bin/ash
ScriptFolder=$(dirname $SCRIPT_FILENAME)
cd ${ScriptFolder}
. ./SetGlobalSettings.sh

echo  >> ${PortName}
echo SET $QUERY_STRING | sed 's/%20/ /g' |sed 's/%60/`/g'|sed 's/%3E/>/g'|sed 's/%22/"/g' |sed 's/%27/"/g'    >>  ${PortName}
echo LOG VALUE >>  ${PortName} 
echo $QUERY_STRING
