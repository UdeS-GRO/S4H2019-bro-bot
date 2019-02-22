#include <DynamixelWorkbench.h>
#include "p_monitor.h"
#include "Axis.h"

#if defined(__OPENCM904__)
  #define DEVICE_NAME "3" //Dynamixel on Serial3(USART3)  <-OpenCM 485EXP
#elif defined(__OPENCR__)
  #define DEVICE_NAME ""
#endif

String cmd[64]={"begin","57600"};

Axis *Axis01;

void setup()
{
    // Initialisation
    Serial.begin(57600);
    while(!Serial);     // Wait until the serial is ready
    Axis01 = new Axis(1,57600); 
    Axis01->Zero();
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
           int temp = cmd[1].toInt()*4095/360;
           cmd[1] = String(temp);
           Axis01->moveTo(cmd[1]);
        }
     
      if(cmd[0] == "zero")
       {
          Axis01->Zero();
       }
       
      if(cmd[0] == "speed")
       {
          Axis01->moveAtSpeed(cmd[1]);
       }
       
      if(cmd[0] == "rd")
       {
          Axis01->readRegister(cmd[1]);
       }
      else if(cmd[0] == "wr")
       {
          Axis01->writeRegister(cmd[1],cmd[2].toInt());
       }
      else if(cmd[0] == "Pos")
       {
          Axis01->getPosition();
       }
      else if(cmd[0] == "Move")
       {
          Axis01->getMovingStatus();
       }
      else if(cmd[0] == "Cur")
       {
          Axis01->getCurrent();
       }
      else if(cmd[0] == "Torq")
       {
          Axis01->getTorque();
       }
      else if(cmd[0] == "Vel")
       {
          Axis01->getVelocity();
       }
      else if(cmd[0] == "movefrom")
       {
          int startpos = Axis01->getPosition();
          int depPos   = cmd[1].toInt();
          int endpos = depPos+startpos;
          if (endpos > 360)
          {
            endpos-=360;
            Axis01->moveAtSpeed("-100");
            while(1)
            {
              if(Axis01->getPosition() <= endpos)
              {
                Axis01->moveAtSpeed("0");
                break;
              }
            }
            
          }
          else
          {
            Axis01->moveAtSpeed("100");
            while(1)
            {
              if(Axis01->getPosition() >= endpos)
              {
                Axis01->moveAtSpeed("0");
                break;
              }
            }
          }
         }
       
      else if(cmd[0] == "moveto")
       {
          int goalpos = cmd[1].toInt();
          Serial.println(goalpos);

          if(goalpos > Axis01->getPosition())
          {
            Axis01->moveAtSpeed("100");
            while(1)
            {
              if(Axis01->getPosition() >= (goalpos+2))
              {
                 Axis01->moveAtSpeed("0");
                 break;
              }
            }
          }
          else
          {
            Axis01->moveAtSpeed("-100");
            while(1)
            {
              if(Axis01->getPosition() <= (goalpos+2))
              {
                 Axis01->moveAtSpeed("0");
                 break;
              }
            }
          
          }

       }
      else 
        {
          dynamixel_command(cmd);
        }
      
    }
    
}
