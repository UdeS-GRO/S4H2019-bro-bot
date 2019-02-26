#include <DynamixelWorkbench.h>
#include "p_monitor.h"
#include "Axis.h"
#include "counter_filter.h"

#define TORQUE_REFERENCE      (-10)
#define TORQUE_MAX_DIFFERENCE (50)
#define TORQUE_TRIGGER_LIMIT  (150) 

unsigned short torque_cnt=0;

Axis axis2(2,57600);


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
        temp= temp*4095/360;
        cmd[2] = String(temp); 
}

void torque_control(Axis * axis)
{

  if (torque_cnt > TORQUE_TRIGGER_LIMIT)
  {
    String temp_cmd=String("torque_off " + String(axis->ID));
    
  }
}

