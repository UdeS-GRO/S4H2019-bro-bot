#include <DynamixelWorkbench.h>
#include "p_monitor.h"

#if defined(__OPENCM904__)
  #define DEVICE_NAME "3" //Dynamixel on Serial3(USART3)  <-OpenCM 485EXP
#elif defined(__OPENCR__)
  #define DEVICE_NAME ""
#endif

String cmd[64]={"begin","57600"};

void setup()
{
    // Initialisation
    Serial.begin(57600);
    while(!Serial);     // Wait until the serial is ready
    dynamixel_command(cmd);
    cmd[0]="scan";
    cmd[1]= "6";
    dynamixel_command(cmd);
    zeroJoint(); 
}

void loop() 
{
    if(Serial.available())
    {
      String read_string = Serial.readStringUntil('\n');
      read_string.trim();    
      split(read_string, ' ', cmd);   
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
        if(temp < 0);
          {
            temp = 360 - temp; // ajouter une fonction sens et mettre un sens anti-clockwise
            // mettre anti-clockwise
          }
        temp= temp*4095/360;
        cmd[2] = String(temp); 
}
void zeroJoint(void)
{
  cmd[0]="joint";
  cmd[1]="2";
  cmd[2]="0";
  dynamixel_command(cmd);
}
