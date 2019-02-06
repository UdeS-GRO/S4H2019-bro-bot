#include <DynamixelWorkbench.h>
#include "p_monitor.h"

#if defined(__OPENCM904__)
  #define DEVICE_NAME "3" //Dynamixel on Serial3(USART3)  <-OpenCM 485EXP
#elif defined(__OPENCR__)
  #define DEVICE_NAME ""
#endif

String cmd[64];

void setup()
{
    // Initialisation
    Serial.begin(57600);
    while(!Serial);     // Wait until the serial is ready
    cmd[0] = "help";
    dynamixel_command(cmd);
}

void loop() 
{
    if(Serial.available())
    {
      String read_string = Serial.readStringUntil('\n');
      read_string.trim();    
      split(read_string, ' ', cmd);
      Serial.println("-----------------"+cmd[2]+"-------------------");    
      if(cmd[0] == "joint")
       { 
          convertAngle(cmd);
       }
        
      dynamixel_command(cmd);
    }
}
void convertAngle(String* cmd)
{ 
        int temp = cmd[2].toInt();
        temp= temp*4095/360;
        cmd[2] = String(temp); 
}
