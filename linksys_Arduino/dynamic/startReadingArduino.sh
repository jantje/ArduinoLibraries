#!/bin/ash
#This scripts starts the whole arduino openwrt communication.
# start with DUMP the first time so that a web page is created.


pkill -f ".*ParseArduinoInput.sh"
cd /www/cgi-bin/jantje
./ParseArduinoInput.sh < /dev/ttyACM0 2>&1 >>/tmp/error.log&
 
 sleep 1
 echo P>> /dev/ttyACM0
 echo LOG HEADER>> /dev/ttyACM0
 echo SET>> /dev/ttyACM0
if [ "$1" = "DUMP" ]; then 
  echo DUMP>> /dev/ttyACM0
  echo LOG VALUE>> /dev/ttyACM0
else
  echo NODUMP
fi
