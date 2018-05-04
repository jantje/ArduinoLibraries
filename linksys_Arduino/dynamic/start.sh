#!/bin/ash 

. ./SetGlobalSettings.sh  
${SttyCommand} >/dev/null
echo >> ${PortName}
echo "LOG HEADER">> ${PortName}
sleep 60
echo DUMP>> ${PortName}