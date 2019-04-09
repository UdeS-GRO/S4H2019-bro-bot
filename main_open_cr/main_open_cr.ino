/*
 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include <DynamixelWorkbench.h>
//#include "p_monitor.h"  // À EFFACER LORSQUE LES TESTS SERONT TERMINÉS
#include "Axis.h"
#include "HandControl.h"
#include "Glove.h"


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


// Function declarations
void read_serial(void);
void torque_control(Axis * axis);
void fix_com(void);
void ack_msg(void);
void change_finger_control_mode(String new_mode);
void finger_control(int finger_number);
void stopBySwitch(Axis * axis);
void limitSwitch(void);
void split(String data, char separator, String* temp);
void glove_check(void);

// Initialisation
void setup()
{
    // Initialisation
    Serial.begin(57600);
    while(!Serial);     // Wait until the serial is ready
    
    //fix_com();    // fait bugger le opencr
    
    /* Axis creation*/
    Axis_table[0] = NULL; //There is no axis 0
    Axis_table[1] = new Axis(1,57600,250,0,359); 
    Axis_table[2] = new Axis(2,57600,350,5,225);
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
  
  Axis_table[1]->readStatus();
  Axis_table[2]->readStatus();
  Axis_table[3]->readStatus();
    
  // Limits Switch digital Read
  limitSwitch();           
  
  //Read message
  read_serial();
  
  glove_check();
  
  //======Computing======
  // Torque control
  /*for (axis_index =1; axis_index < NUMBER_OF_AXIS ; axis_index++)
  {
    Axis_table[axis_index]->verifGoalAchieve(); // faire un mode debug
    torque_control(Axis_table[axis_index]);
  }*/

  // Finger control
  int finger_index;
  for (finger_index =1; finger_index <= NUMBER_OF_FINGERS; finger_index++)
  {
    finger_control(finger_index);
  }
  
  // Moveto control
  for (axis_index =1; axis_index < NUMBER_OF_AXIS ; axis_index++)
  {
    //torque_control(Axis_table[axis_index]);
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
        axis->Disable();
        /*cmd_tx[0] = String("torque_off");                                                   //TODO : Implementer un système de queue de message 
        cmd_tx[1] = String(axis->ID);
        dynamixel_command(cmd_tx); */
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
        /*cmd_tx[0] = String("torque_on");
        cmd_tx[1] = String(axis->ID); 
        dynamixel_command(cmd_tx);*/
        axis->Enable();
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
    
    /*cmd_tx[0] = String("torque_on");
    cmd_tx[1] = String(axis->ID); 
    dynamixel_command(cmd_tx);*/
    axis->Enable();
  }
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
      
       
         if(cmd[0] == "zero")
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
            Serial.println(axis->getVelocity());
         }
         else if (cmd[0] == "stop")
          {
            short axis_index;
             // Set the motor velocity to 0 and indicates that it has reach is position
            for (axis_index =1; axis_index < NUMBER_OF_AXIS ; axis_index++)
              {
                Axis_table[axis_index]->stopCmd();
              } 
          }
        else if(cmd[0] == "moveto")
         {
            /*int goalpos = cmd[2].toFloat();
            Serial.println(goalpos);
            axis->setGoalPosition(goalpos);   //idée comme ça, on utilise une 4e commande pour la vitesse voulue. On pourrait même setuper 3 vitesses prédéfini genre slow, medium, fast ****
            if(goalpos < axis->getPosition())
            {
              axis->moveAtSpeed("-50");
              axis->setAtPosition(false);
            }
            if(goalpos > axis->getPosition())
            {
              axis ->moveAtSpeed("50");
              axis->setAtPosition(false);
            }*/

            axis->Moveto(cmd[2].toFloat());
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
      else if (cmd[0] == "stop")
      {
        
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

/*void fix_com(void)
{
    cmd_tx[0] = String("begin");    //Bug qui doit être régler
    cmd_tx[1] = String("57600"); 
    dynamixel_command(cmd_tx);
    cmd_tx[0] = String("scan");
    cmd_tx[1] = String("3"); 
    dynamixel_command(cmd_tx);
}*/

void ack_msg(void)
{
  Serial.println("nolidge");
}

void finger_control(int finger_number)
{
  int new_PWM_cmd =0;
  if( hand_control.getMode() == LOCK)
  {
    hand_control.getMotorValue();
    hand_control.setMotorValue();
    /* do nothing and keep last position */
  }
  else if (hand_control.getMode() == FREE)
  {
    /* Set the PWM to 0 to disable the servo*/          //TODO :Add the pwm control
  }
    else if (hand_control.getMode() == GLOVE)
  {
    /* Read the commands sent by radio*/
    hand_control.setMotorValue();
    //new_PWM_cmd = hand_control.getFingerGloveValue(finger_number);
    //Serial.println(String("Radio:" +String(new_PWM_cmd)));       //TODO :Add the pwm control
  }
    else if (hand_control.getMode() == GUI)
  {
    /* Read the last command sent by the GUI*/
    hand_control.setMotorValue();
     //new_PWM_cmd = hand_control.getFingerGuiValue(finger_number);
     //Serial.println(String("GUI:" +String(new_PWM_cmd)));      //TODO :Add the pwm control
  }
}

void limitSwitch(void)
{
  // Limits Switch digital Read
  MinLS[1] = digitalRead(inMinLS01);
  MinLS[2] = digitalRead(inMinLS02);
  //MinLS[3] = digitalRead(inMinLS03);
  MaxLS[1] = digitalRead(inMaxLS01);
  MaxLS[2] = digitalRead(inMaxLS02);
  //MaxLS[3] = digitalRead(inMaxLS03); */ // decommenter lorsque les pins seront utilises. Sinon ce sont des valeurs randoms qui sont donnes
  // stoping by limit switch
  
int axis_index;

for (axis_index =1; axis_index < NUMBER_OF_AXIS-1 ; axis_index++)      // ENLEVER LE -1 LORSQU'ELLES SERONT TOUS CONNECTÉS
  {
    if(Axis_table[axis_index]->getSwitchMode())
    {
        if(!MinLS[axis_index] && !MaxLS[axis_index])
        {
          Axis_table[axis_index]->setSwitchMode(false);
        }
    }
    else if(!Axis_table[axis_index]->getSwitchMode()) // à changer avec le SwitchMode dans axis.cpp
    {
      if (MinLS[axis_index] && Axis_table[axis_index]->Sts_Homing == 0 || MaxLS[axis_index] && Axis_table[axis_index]->Sts_Homing == 0)
      {
        stopBySwitch(Axis_table[axis_index]);
        
        if (MinLS[axis_index])
          {
            Serial.print("switch MIN ");
            Serial.println(axis_index);        
            Axis_table[axis_index]->setPermissionBackward();
          }
        else if (MaxLS[axis_index])
          {
           Serial.print("switch MAX ");
           Serial.println(axis_index);
           Axis_table[axis_index]->setPermissionForward();
          }
        else 
          {
            Serial.println("WHAAT IS HAPPENING HERE");
          }
      }
    }

// Juste pour faire des tests
  /*if (MinLS[1] && Axis_table[1]->Sts_Homing == 0)  // *************** Le test est pour le moteur 3 *************************
  { 
    //Serial.println("Wouhou! it is working !!");
    stopBySwitch(Axis_table[1]);
    Axis_table[1]->setPermissionForward();
  }

  
  if( Axis_table[1]->Sts_Homing == 1)
  {
   Axis_table[1]->HomeRequest(&MinLS[1]);
  }*/

  }
}

void split(String data, char separator, String* temp)
{
  int cnt = 0;
  int get_index = 0;

  String copy = data;
  
  while(true)
  {
    get_index = copy.indexOf(separator);

    if(-1 != get_index)
    {
      temp[cnt] = copy.substring(0, get_index);

      copy = copy.substring(get_index + 1);
    }
    else
    {
      temp[cnt] = copy.substring(0, copy.length());
      break;
    }
    ++cnt;
  }
}

void glove_check(void){
  if(hand_control.getMode()==GLOVE){
    if(radio.available()){
      hand_control.read_radio();
    }
  }
  if(hand_control.getMode()!=FREE){
    hand_control.attachMotor();
  }
}
