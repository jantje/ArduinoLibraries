#!/bin/ash
#stty -F /dev/ttyS1 1401:0:18b2:0:3:1c:7f:15:1:5:0:0:11:13:1a:0:12:f:17:16:4:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0
echo  >> /dev/ttyS1
echo SET $QUERY_STRING | sed 's/%20/ /g' |sed 's/%60/`/g'|sed 's/%3E/>/g'|sed 's/%22/"/g' |sed 's/%27/"/g'    >> /dev/ttyS1
echo LOG VALUE >> /dev/ttyS1
echo $QUERY_STRING
