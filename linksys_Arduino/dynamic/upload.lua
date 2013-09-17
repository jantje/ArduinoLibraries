#! /usr/bin/lua

-- HTTP header
print [[
Content-Type: text/html; charset=iso-8859-1
<?xml version="1.0" encoding="iso-8859-1"?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">

]]

print [[<html lang="en">
<head> 
<title>Arduino system page</title>
</head>
<body>
<h1>Arduino Control Center</h1>
<form METHOD="post" enctype="multipart/form-data" ACTION=http://192.168.1.102/cgi-bin/robot/cgiutil/upload.lua?to=server>
<p>Select your hex file: <input type="file" size=80 name="file"></p>
<p>Upload to server:<input Type=submit  Value="Upload"></p>
</form>
<form METHOD="get"  ACTION=http://192.168.1.102/cgi-bin/robot/cgiutil/upload.lua>
<p><input type="hidden" name="to" id="to" value="arduino" />
  <input Type=submit  Value="Upload"> to <select name="board">
    <option value="atmega2560">Mega 2560</option>
     <option value="atmega328p">UNO</option>
     
        </select> </p>
</form>

<a href="/robot/robot.html"> application admin</a>
</br>
]]

---- end form test




--local QUERY_STRING = os.getenv ("QUERY_STRING")
--print (QUERY_STRING)
--print (get_data.board)
--print (get_data["to"])
--if QUERY_STRING == "to=server" then
if get_data.to == "server" then

  POST_DATA = io.read ("*all")  -- read all of stdin
  contentType = os.getenv ("CONTENT_TYPE")
  if contentType then
    pos = string.find (contentType, "=", 2, true) 
    if pos then
      pos =string.find(POST_DATA, "\r\n\r\n",1,true) + 4
      if pos then
        lastpos  = string.find(POST_DATA,  "\r\n\r\n" , pos,true)
        HEX_DATA = string.sub(POST_DATA, pos, lastpos +1 )
      end
    end
  end         
  if HEX_DATA then
    print "<h1>POST data</h1>\n"
  
    local f = assert(io.open ("/tmp/upload.hex", "wb"))
    s = assert( f:write (HEX_DATA))  
    f:close ()  -- close it
    print [[ hex file dumped in /tmp/upload.hex </br> ]]
  end

--elseif QUERY_STRING == "to=arduino" then
  elseif get_data.to == "arduino" then
  if  get_data.board == "atmega2560"  then
    programmer = "-cstk500v2"
  else
    programmer = "-arduino"    
  end 
  io.write( "/mnt/pkg/usr/bin/avrdude -p"..get_data.board.." "..programmer.." -P/dev/ttyS1 -b115200 -Uflash:w:/tmp/upload.hex:a -C/mnt/pkg/etc/avrdude.conf >> upload.out 2>&1</br>")
--  os.execute("cp /mnt/pkg/usr/bin/avrdude /tmp/avrdude") 
--  os.execute("cp /mnt/pkg/etc/avrdude.conf /tmp/avrdude.conf")
  os.execute("rm /tmp/value.txt") 
  os.execute("pkill -f \".*ParseArduinoInput\" > /tmp/upload.out 2>&1")
  os.execute("stty -F /dev/ttyS1 1401:0:18b2:0:3:1c:7f:15:1:5:0:0:11:13:1a:0:12:f:17:16:4:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0")
  os.execute("echo RESET >> /dev/ttyS1")
  os.execute("echo RESET >> /dev/ttyS1;sleep 1")
--  os.execute("/www/cgi-bin/robot/flushSerial </dev/ttyS1 >> /tmp/upload.out 2>&1")
  os.execute("echo N >> /dev/ttyS1;/tmp/avrdude -p"..get_data.board.." "..programmer.." -P/dev/ttyS1 -b115200 -Uflash:w:/tmp/upload.hex:a -C/tmp/avrdude.conf >> /tmp/upload.out 2>&1")
  os.execute("echo upload done >> /tmp/upload.out")
  os.execute("/www/cgi-bin/robot/startReadingArduino.sh & >> upload.out 2>&1")
  f= assert(io.open ("/tmp/upload.out", "r"))
  t = f:read("*all") 
  t= string.gsub( fixhtml(t), "\n", "</br>\n")
  io.write ( t )
  f:close ()
end
          
          print [[
          </body>
          </html>
          ]]
          
          
