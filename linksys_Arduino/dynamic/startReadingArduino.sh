#!/bin/ash
#This scripts starts the whole arduino openwrt communication.
# start with DUMP the first time so that a web page is created.
mount.cifs 192.168.1.100:/linksys /mnt/ -o user=linksys,pass=DE4Fkhd6dtRz
if [ ! -f /tmp/avrdude ]; then
  cp /mnt/pkg/usr/bin/avrdude /tmp/avrdude &
fi
if [ ! -f /tmp/avrdude.conf ]; then
  cp /mnt/pkg/etc/avrdude.conf /tmp/avrdude.conf &
fi

pkill -f ".*ParseArduinoInput.sh"
stty -F /dev/ttyS1 1401:0:18b2:0:3:1c:7f:15:1:5:0:0:11:13:1a:0:12:f:17:16:4:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0
cd /www/cgi-bin/robot
./ParseArduinoInput.sh < /dev/ttyS1 2>&1 >>/mnt/log/error.log&
 
 sleep 1
 echo P>> /dev/ttyS1
 echo LOG HEADER>> /dev/ttyS1
 echo SET>> /dev/ttyS1
if [ "$1" = "DUMP" ]; then 
  echo DUMP>> /dev/ttyS1
  echo LOG VALUE>> /dev/ttyS1
else
  echo NODUMP
fi
