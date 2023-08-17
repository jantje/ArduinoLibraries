#!/bin/ash
ScriptFolder=$(dirname $SCRIPT_FILENAME)
cd ${ScriptFolder}
. ./SetGlobalSettings.sh

echo  >> ${PortName} 
echo $QUERY_STRING  >> ${PortName}
