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
<form METHOD="post" enctype="multipart/form-data" ACTION=upload.lua?to=server>
<p>Select your hex file: <input type="file" size=80 name="file"></p>
<p>Upload to server:<input Type=submit  Value="Upload"></p>
</form>
<form METHOD="get"  ACTION=upload.lua>
<p><input type="hidden" name="to" id="to" value="arduino" />
  <input Type=submit  Value="Upload"> to <select name="board">
    <option value="atmega2560">Mega 2560</option>
     <option value="atmega328p">UNO</option>
     
        </select> </p>
</form>

<a href="../index.html">Application admin</a>
</br>
]]

---- end form test



if get_data.to == "server" then
  print "<h1>Uploading to server</h1>\n"
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
    print "data arrived </br>\n"
  
    local f = assert(io.open ("/tmp/upload.hex", "wb"))
    s = assert( f:write (HEX_DATA))  
    f:close ()  -- close it
    print [[ hex file dumped in /tmp/upload.hex </br> ]]
  end

  elseif get_data.to == "arduino" then
    print "<h1>Uploading to arduino</h1>\n"
    
    if  get_data.board == "atmega2560"  then
      programmer = "-cstk500v2"
    else
      programmer = "-arduino"    
    end
    --PortName = os.getenv("PortName")

            
    PortName = "/dev/tts/1"

    io.write( "run_avrdude -v -p"..get_data.board.." > /tmp/upload.out 2>&1</br>")
  os.execute("rm -p /tmp/value.txt") 
  os.execute("pkill -f \".*ParseArduinoInput\" > /tmp/upload.out 2>&1")
  os.execute("echo p >> "..PortName.."")
  os.execute("echo Admin.logLevel=0 >> "..PortName..";sleep 3")
  os.execute("run_avrdude -v -p"..get_data.board.." >> /tmp/upload.out 2>&1")
  os.execute("echo upload done >> /tmp/upload.out")
  os.execute("/www/cgi-bin/jantje/startReadingArduino.sh  >> /tmp/upload.out 2>&1")
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
          
          
