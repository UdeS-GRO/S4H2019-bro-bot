/*
 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include <DynamixelWorkbench.h>
#include "p_monitor.h"
#include "Axis.h"
#include "HandControl.h"


#if defined(__OPENCM904__)
  #define DEVICE_NAME "3" //Dynamixel on Serial3(USART3)  <-OpenCM 485EXP
#elif defined(__OPENCR__)
  #define DEVICE_NAME ""
#endif

//Constant declarations and definitions
#define TORQUE_COUNTER_REFERENCE  0
#define TORQUE_MAX_DIFFERENCE     40
#define TORQUE_CNT_BEFORE_TRIGGER 150
#define MOVING_CNT_BEFORE_TRIGGER 150
#define NUMBER_OF_AXIS            4   //+ 1 because there is no axis 0
#define NUMBER_OF_FINGERS         3

#define FINGER1_PIN               A0

//Global variables
String cmd[64]={"begin","57600"};
String cmd_tx[20];                //String to send to the motor after computing

HandControl hand_control(NUMBER_OF_FINGERS);

Axis *Axis_table[NUMBER_OF_AXIS];


// Hard Limit Switches inputs declaration
int inMinLS01 = 2;
int inMaxLS01 = 3;
int inMinLS02 = 4;
int inMaxLS02 = 5;
int inMinLS03 = 6;
int inMaxLS03 = 7;

// Limit Switches input status
bool MinLS[NUMBER_OF_AXIS];
bool MaxLS[NUMBER_OF_AXIS];


//Function declarations
void read_serial(void);
void read_radio(void);
void torque_control(Axis * axis);
void fix_com(void);
void ack_msg(void);
void change_finger_control_mode(String new_mode);
void finger_control(int finger_number);

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

    Axis_table[2]->setTorqueFilter(TORQUE_COUNTER_REFERENCE,TORQUE_MAX_DIFFERENCE,TORQUE_CNT_BEFORE_TRIGGER);  
    Axis_table[2]->setMovingFilter(0, 0,MOVING_CNT_BEFORE_TRIGGER);
    
    Axis_table[3]->setTorqueFilter(TORQUE_COUNTER_REFERENCE,TORQUE_MAX_DIFFERENCE,TORQUE_CNT_BEFORE_TRIGGER);  
    Axis_table[3]->setMovingFilter(0, 0,MOVING_CNT_BEFORE_TRIGGER);

    // Pinout Attribution for Limit Swtiches
    pinMode(inMinLS01, INPUT);
    pinMode(inMaxLS01, INPUT);
    pinMode(inMinLS02, INPUT);
    pinMode(inMaxLS02, INPUT);
    pinMode(inMinLS03, INPUT);
    pinMode(inMaxLS03, INPUT);
}

void loop() 
{
  short axis_index;
  
  //Axis_table[1]->readStatus();
  //Axis_table[2]->readStatus();
  //Axis_table[3]->readStatus();
    
  // Limits Switch digital Read
  MinLS[1] = digitalRead(inMinLS01);
 /* MinLS[2] = digitalRead(inMinLS02);
  MinLS[3] = digitalRead(inMinLS03);
  MaxLS[1] = digitalRead(inMaxLS01);
  MaxLS[2] = digitalRead(inMaxLS02);
  MaxLS[3] = digitalRead(inMaxLS03); */ // decommenter lorsque les pins seront utilises. Sinon ce sont des valeurs randoms qui sont donnes             

//  bool test = Axis_table[1]->HomeRequest(&MinLS[1]); // Test homing Command
  
  //Read message
  read_serial();
  read_radio();

// stoping by limit switch
/*for (axis_index =1; axis_index < NUMBER_OF_AXIS ; axis_index++)      // decommenter lorsque les pins seront utilises. Sinon ce sont des valeurs randoms qui sont donnes
  {
    if (MinLS[axis_index] || MaxLS[axis_index])
    {
      Serial.print("that is weird");
      stopBySwitch(Axis_table[axis_index]);
    }
  }*/
//
//// Juste pour faire des tests
//if (MinLS[1] && Axis_table[3]->Sts_Homing == 0)
//{ 
//  Serial.println("Wouhou! it is working !!");
//  stopBySwitch(Axis_table[3]);
//}
//
//if( Axis_table[3]->Sts_Homing == 1)
//{
//  Axis_table[3]->HomeRequest(&MinLS[1]);
//}


  //======Computing======
  // Torque control
  for (axis_index =1; axis_index < NUMBER_OF_AXIS ; axis_index++)
  {
    torque_control(Axis_table[axis_index]);
  }

  // Finger control
  int finger_index;
  for (finger_index =1; finger_index <= NUMBER_OF_FINGERS; finger_index++)
  {
    finger_control(finger_index);
  }
}

void stopBySwitch(Axis * axis)
{
  axis->stopCmd();
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
     //Serial.print("Torque_cnt: ");                                          //test: counter
     //Serial.println(present_load_f);
     //Serial.println(axis->torque_counter_filter->counter);
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
        int ID = cmd[1].toInt();
        
        axis = Axis_table[ID];
      
      
        if(cmd[0] == "joint")
          { 
             int temp = cmd[2].toInt()*4095/360;
             cmd[2] = String(temp);
             axis->moveTo(cmd[2]);
          }  
        else if(cmd[0] == "zero")
         {
            //axis->Zero();
            axis->HomeRequest(&MinLS[1]);
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
        else if (cmd[0] == "torque_control_disable")
        {
          axis->torqueControlEnable = false;
        }
      }
      
      /* Message not destined to motors */
      /* Finger control message */
      if (cmd[0] == "finger_mode")
      {
          change_finger_control_mode(cmd[1]);
      }
      else if (cmd[0] == "finger_move")
      {
        /* Read the PWM value from gui for a specific finger */
        hand_control.setFingerGuiValue(cmd[1].toInt(),cmd[2].toInt());
      } 
    }   
}


void change_finger_control_mode(String new_mode)
{
  bool doModeExist =false;
  int mode_debug;
  
  if(new_mode == "LOCK")
  {
    hand_control.setMode(LOCK);
    doModeExist = true;
  }
  else if(new_mode == "FREE")
  {
    hand_control.setMode(FREE);
    doModeExist = true;
  }
  else if(new_mode == "GUI")
  {
    hand_control.setMode(GUI);
    doModeExist = true;
  }
  else if(new_mode == "GLOVE")
  {
    hand_control.setMode(GLOVE);
    doModeExist = true;
  }
  else
  {/* NOP */}

  if (doModeExist)
  {
    Serial.print(String("New finger control mode: " + new_mode));
    Serial.println(String( ": " + String(hand_control.getMode())));
  }
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

void finger_control(int finger_number)
{
  int new_PWM_cmd =0;
  if( hand_control.getMode() == LOCK)
  {
    /* do nothing and keep last position */
  }
  else if (hand_control.getMode() == FREE)
  {
    /* Set the PWM to 0 to disable the servo*/          //TODO :Add the pwm control
  }
    else if (hand_control.getMode() == GLOVE)
  {
    /* Read the commands sent by radio*/
    new_PWM_cmd = hand_control.getFingerGloveValue(finger_number);
    Serial.println(String("Radio:" +String(new_PWM_cmd)));       //TODO :Add the pwm control
  }
    else if (hand_control.getMode() == GUI)
  {
    /* Read the last command sent by the GUI*/
     new_PWM_cmd = hand_control.getFingerGuiValue(finger_number);
     Serial.println(String("GUI:" +String(new_PWM_cmd)));      //TODO :Add the pwm control
  }
}


