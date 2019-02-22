#include <DynamixelWorkbench.h>
#include "p_monitor.h"
#include "Axis.h"

#if defined(__OPENCM904__)
  #define DEVICE_NAME "3" //Dynamixel on Serial3(USART3)  <-OpenCM 485EXP
#elif defined(__OPENCR__)
  #define DEVICE_NAME ""
#endif

//Constant declarations and definitions
#define TORQUE_COUNTER_REFERENCE -10
#define TORQUE_MAX_DIFFERENCE      50
#define TORQUE_CNT_BEFORE_TRIGGER 150
#define MOVING_CNT_BEFORE_TRIGGER 100

//Global variables
String cmd[64]={"begin","57600"};
String cmd_tx[20];              //String to send to the motor after computing
 
Axis *Axis01;


//Function declarations
void read_serial(void);
void read_radio(void);

// Initialisation
void setup()
{
    // Initialisation
    Serial.begin(57600);
    while(!Serial);     // Wait until the serial is ready
    Axis01 = new Axis(1,57600); 
    Axis01->Zero();

    /*Init for the torque control*/
    Axis01->setTorqueFilter(TORQUE_COUNTER_REFERENCE,TORQUE_MAX_DIFFERENCE,TORQUE_CNT_BEFORE_TRIGGER);
    Axis01->setTorqueFilter(0, 0,MOVING_CNT_BEFORE_TRIGGER);
}

void loop() 
{
  //Read message
  read_serial();
  read_radio();


  //Computing
  torque_control(Axis01);
    
}


void torque_control(Axis * axis)
{
  if (axis->torqueControlEnable ==true)
  {
    float present_load = float(axis->getTorque());
    float present_moving = float(axis->getMovingStatus());
    bool triggered =false;
    
  
    if(axis->isFreeToMove == false)
    {
      /*Verify if something tries to force against the motor */
      triggered = axis->torque_counter_filter->compute(present_load);
      if(triggered)
      {
        cmd_tx[0] = String("torque_off "+  String(axis->ID));                                                   //TODO : Implementer un système de queue de message 
        axis->isFreeToMove = true;
        axis->blink(GREEN_BLINK,500);   //Blink 500 ms
      }
    }
    else 
    { 
      /*Verify if something/someone is still moving the motor*/   
      triggered = axis->moving_counter_filter->compute(present_moving);  
      if(triggered)
      {
        cmd_tx[0] = String("torque_on");
        cmd_tx[1] = String(axis->ID); 
        dynamixel_command(cmd_tx);
        
        axis->isFreeToMove = false;
        int actual_position = axis->getPosition();
        //Convert angle: Demander a Jean-Michel si cest normal quil n'y a pas de conversion d'angle
        //axis->moveTo(String (actual_position));           //Commented because of the issue above
        axis->blink(STOP_BLINK,500);   //Blink 500 ms
      }
    }
  }
  else
  {
    axis->isFreeToMove = false;
    axis->torque_counter_filter->reset_counter();
    axis->moving_counter_filter->reset_counter();
    axis->blink(STOP_BLINK,500);   //Blink 500 ms
    
    cmd_tx[0] = String("torque_on");
    cmd_tx[1] = String(axis->ID); 
    dynamixel_command(cmd_tx);
  }
}

void read_radio(void)
{
  //À rajouter
}

void read_serial(void)
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
