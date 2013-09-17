#!/bin/ash
#stty -F /dev/ttyS1 1401:0:18b2:0:3:1c:7f:15:1:5:0:0:11:13:1a:0:12:f:17:16:4:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0
echo  >> /dev/ttyS1
echo $QUERY_STRING  >> /dev/ttyS1
if [ "$QUERY_STRING" = "RESET" ];
then  
  echo N >>  /dev/ttyS1
else
  echo bummer >>  /dev/ttyS1
fi
