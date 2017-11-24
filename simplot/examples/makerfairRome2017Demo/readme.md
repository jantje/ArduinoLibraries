requirements for the rome make fair demo   
  
1-> code completion    
2-> compiler warnings    
3-> library manager    
4-> boardmanager     
5-> multi build configuration    
6-> firmware upload    
7-> serial console    
8-> serial plotter    
9- git(hub)    
10-> debug    
    
Idea demo    
Part one simple out of the box with uno    
Start with blinkwithout delay with teensy.    
demo 1,2,6,7    
    
part 2 hardware debug  (zero and esp8266)    
Both bords need to be installed using the boardsmanager (4)    
Install the simplot library. (3)    
create a new zero sketch based on a this example.    
Upload    
demo plotter 8    
Demo multiple serial connnection on serial monitor as demo 1 is still runnning    
Create new config for debugging with zero (5)    
Upload and short debug demo (10)   
Create a new config for esp8266    
verify compile upload show plotter (and blinking led on esp8266)    

part 3 local debug unit test    
Create new config for local debug    
Debug quickly    
Create new config for local unit test    
run unit test (still to be made)    
    
Part 4 github    
checkin the project in github    






