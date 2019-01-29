
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
    Serial.begin(9600);
    while(!Serial);     // Wait until the serial is ready
    
    cmd[0]="help";
}

void loop() 
{
    if(Serial.available()>0)
    {
      String read_string = Serial.readStringUntil('\n');
      Serial.println("[CMD] : " + String(read_string));
      
      read_string.trim();

      split(read_string, ' ', cmd);

      dynamixel_command(cmd);
    }
}
