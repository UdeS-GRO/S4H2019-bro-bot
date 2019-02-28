#include <DynamixelWorkbench.h>
#include "p_monitor.h"
#include "Axis.h"

#if defined(__OPENCM904__)
  #define DEVICE_NAME "3" //Dynamixel on Serial3(USART3)  <-OpenCM 485EXP
#elif defined(__OPENCR__)
  #define DEVICE_NAME ""
#endif

//Constant declarations and definitions
#define TORQUE_COUNTER_REFERENCE  0
#define TORQUE_MAX_DIFFERENCE     40
#define TORQUE_CNT_BEFORE_TRIGGER 300
#define MOVING_CNT_BEFORE_TRIGGER 300
#define NUMBER_OF_AXIS            4   //+ 1 because there is no axis 0

//Global variables
String cmd[64]={"begin","57600"};
String cmd_tx[20];              //String to send to the motor after computing
Axis *Axis_table[NUMBER_OF_AXIS];


//Function declarations
void read_serial(void);
void read_radio(void);
void torque_control(Axis * axis);
void fix_com(void);
void ack_msg(void);


// Initialisation
void setup()
{
    // Initialisation
    Serial.begin(57600);
    while(!Serial);     // Wait until the serial is ready
    
    fix_com();
    
    /* Axis creation*/
    Axis_table[0] = NULL; //There is no axis 0
    Axis_table[1] = new Axis(1,57600,250,0,359); 
    Axis_table[2] = new Axis(2,57600,350,25,250);
    Axis_table[3] = new Axis(3,57600,250);
    
    /*Init for the torque control*/
    Axis_table[1]->setTorqueFilter(TORQUE_COUNTER_REFERENCE,TORQUE_MAX_DIFFERENCE,TORQUE_CNT_BEFORE_TRIGGER);  
    Axis_table[1]->setMovingFilter(0, 0,MOVING_CNT_BEFORE_TRIGGER);

}

void loop() 
{
  //Read message
  read_serial();
  read_radio();


  //Computing
  short axis_index;
  for (axis_index =1; axis_index < NUMBER_OF_AXIS ; axis_index++)
  {
    torque_control(Axis_table[axis_index]);
  }

  
}


void torque_control(Axis * axis)
{
  if (axis->torqueControlEnable ==true)
  {
    short present_load = (short)axis->getTorque();  //COnvert to char to have negative value
    float present_load_f = (float)present_load;     //Convert to float
    float present_moving = (float)(1-axis->getMovingStatus());
    bool triggered =false;
  
    if(axis->isFreeToMove == false)
    {
      /*Verify if something tries to force against the motor */
     triggered = axis->torque_counter_filter->compute(present_load_f);        //test#0
//     Serial.print("Torque_cnt: ");                                          //test: counter
//     Serial.println(axis->torque_counter_filter->counter);
      if(triggered)
      {
        axis->moving_counter_filter->reset_counter();
        cmd_tx[0] = String("torque_off");                                                   //TODO : Implementer un système de queue de message 
        cmd_tx[1] = String(axis->ID);
        dynamixel_command(cmd_tx); 
        axis->isFreeToMove = true;
        axis->blink(GREEN_BLINK,500);   //Blink 500 ms
      }
    }
    else 
    { 
      /*Verify if something/someone is still moving the motor*/   
      triggered = axis->moving_counter_filter->compute(present_moving);
//      Serial.print("Moving_cnt: ");                                          //test: counter
//      Serial.println(axis->moving_counter_filter->counter);  
      axis->blink(GREEN_BLINK,500);   //Blink 500 ms
      
      if(triggered)
      {
        cmd_tx[0] = String("torque_on");
        cmd_tx[1] = String(axis->ID); 
        dynamixel_command(cmd_tx);
        axis->torque_counter_filter->reset_counter();
        
        axis->isFreeToMove = false;

        
        axis->blink(STOP_BLINK,500);   //Blink 500 ms
      }
    }
  }
  else if (axis->isFreeToMove == true)
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

      Axis* axis;
      /* Is the motor available */
      if ((cmd[1].toInt()  >= 1)  && (cmd[1].toInt()  < NUMBER_OF_AXIS))
      {
        /* Select the right motor to talk to*/
        axis = Axis_table[cmd[1].toInt()];
      
      
        if(cmd[0] == "joint")
          { 
             int temp = cmd[2].toInt()*4095/360;
             cmd[2] = String(temp);
             axis->moveTo(cmd[2]);
          }  
        else if(cmd[0] == "zero")
         {
            axis->Zero();
         }
         
        else if(cmd[0] == "speed")
         {
            axis->moveAtSpeed(cmd[2]);
         }
         
        else if(cmd[0] == "rd")
         {
            axis->readRegister(cmd[2]);
         }
        else if(cmd[0] == "wr")
         {
            axis->writeRegister(cmd[1],cmd[2].toInt());
         }
        else if(cmd[0] == "Pos")
         {
            Serial.print(axis->getPosition());
         }
        else if(cmd[0] == "Move")
         {
            axis->getMovingStatus();
         }
        else if(cmd[0] == "Cur")
         {
            axis->getCurrent();
         }
        else if(cmd[0] == "Torq")
         {
            axis->getTorque();
         }
        else if(cmd[0] == "Vel")
         {
            axis->getVelocity();
         }
        else if(cmd[0] == "movefrom")
         {
            int startpos = axis->getPosition();
            int depPos   = cmd[2].toInt();
            int endpos = depPos+startpos;
            if (endpos > 360)
            {
              endpos-=360;
              axis->moveAtSpeed("-100");
              while(1)
              {
                if(axis->getPosition() <= endpos)
                {
                  axis->moveAtSpeed("0");
                  break;
                }
              }
              
            }
            else
            {
              axis->moveAtSpeed("100");
              while(1)
              {
                if(axis->getPosition() >= endpos)
                {
                  axis->moveAtSpeed("0");
                  break;
                }
              }
            }
           }
        else if(cmd[0] == "moveto")
         {
            int goalpos = cmd[2].toInt();
            Serial.println(goalpos);
            if(goalpos > axis->getPosition())
            {
              axis->moveAtSpeed("100");
              while(1)
              {
                if((axis->getPosition() >= (goalpos-2))||(axis->getPosition() >= axis->MaxSoftlimit))
                {
                   axis->moveAtSpeed("0");
                   break;
                }
              }
            }
            else
            {
              axis->moveAtSpeed("-100");
              while(1)
              {
                if(axis->getPosition() <= (goalpos+2)||axis->getPosition()< axis->MinSoftlimit)
                {
                   axis->moveAtSpeed("0");
                   break;
                }
              }
            
            }
            //Position achieve. Send an ack flag to the py
            ack_msg();
         }
        else if (cmd[0] == "torque_control_enable")
        {
          axis->torqueControlEnable = true;
        }
    } 
    }   
}
void convertAngle(String* cmd)
{ /*
        int temp = cmd[1].toInt();
        temp= temp*4095/360;
        cmd[1] = String(temp); 

        if (cmd[0] == "torque_control_disable")
        {
          axis->torqueControlEnable = false;
        }
        
        else 
          {
            dynamixel_command(cmd);
          }
        
      
      else 
      {
         dynamixel_command(cmd);
      }
    }*/

}


void fix_com(void)
{
    cmd_tx[0] = String("begin");    //Bug qui doit être régler
    cmd_tx[1] = String("57600"); 
    dynamixel_command(cmd_tx);
    cmd_tx[0] = String("scan");
    cmd_tx[1] = String("3"); 
    dynamixel_command(cmd_tx);
}

void ack_msg(void)
{
  Serial.println("nolidge");
}
