#include <Process.h>

// switch on/off Philis Hue lamp via Yun

void setup()
{
  Bridge.begin();   // Initialize the Bridge
}

void loop()
{
 
  Process p;
  p.runShellCommand("curl -X PUT -d '{\"on\":true, \"ct\" : 153, \"bri\":254 }' http://192.168.0.100/api/EE2Md-XKTaIjMj9J78ztSPZruD9l9P6A0SknxsJd/lights/1/state");  // switch on, ct 6500 K, brightness max
  while(p.running()); 
  
  delay(5000);  
  
  p.runShellCommand("curl -X PUT -d '{\"on\":false}' http://192.168.0.100/api/EE2Md-XKTaIjMj9J78ztSPZruD9l9P6A0SknxsJd/lights/2/state"); // switch off
  while(p.running()); 
   
  delay(5000);  
  
  p.runShellCommand("curl -X PUT -d '{\"on\":true,\"hue\":0, \"bri\":50 }' http://192.168.0.100/api/EE2Md-XKTaIjMj9J78ztSPZruD9l9P6A0SknxsJd/lights/1/state"); // switch on, Hue red, brightness 50
  while(p.running()); 
  
  delay(5000);  
}
