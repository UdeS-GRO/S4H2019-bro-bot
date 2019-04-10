/*
	This Source Code Form is subject to the terms of the Mozilla Public
	License, v. 2.0.If a copy of the MPL was not distributed with this
	file, You can obtain one at http ://mozilla.org/MPL/2.0/.
*/

#include "HandControl.h"

/**
* Constructor of the Handcontrol class
*
* Initialize the size of the finger data arrays and the mode to the FREE mode
*
* @param The number of fingers
* @return Nothing.
*/
HandControl::HandControl(int number_of_fingers)
{
	finger_gui_cmd_ = new int[number_of_fingers];
	finger_glove_cmd_ = new int[number_of_fingers];
	mode = FREE;
	nb_of_finger = number_of_fingers;
	thumb.attach(pin_m1);
  	INDEX.attach(pin_m2);
  	middle.attach(pin_m3);

	// Initialisation of glove communication
    	radio = new RF24(7, 8); // CE, CSN
    	const byte address[6] = "gant0";
    	radio->begin();
    	radio->openReadingPipe(0, address);
    	radio->setPALevel(RF24_PA_MIN);
    	radio->startListening();

	int index;
	for (index = 0; index < number_of_fingers; index++)
	{
		finger_gui_cmd_[index] = 0;
		finger_glove_cmd_[index] = 0;
	}
}


HandControl::~HandControl()
{
	delete[] finger_gui_cmd_;
	delete[] finger_glove_cmd_;
}

/**
* Set the mode in wich the hand will be controlled.
*
* @param The new mode.
* @return Nothing.
*/
void HandControl::setMode(ControlFingerState new_mode)
{
	mode = new_mode;
}

/**
* Get the actual mode in wich the hand is controlled.
*
* @param Nothing.
* @return Actual mode.
*/
ControlFingerState HandControl::getMode(void)
{
	return mode;
}

/**
* Set the value of the PWM to send to a finger by the GUI.
*
* The specified value of PWM will be send inside in a private specific array.
* The array index is the finger number -1.
*
* @param The number of the finger (index-1) and the new value to insert.
* @return Nothing.
*/
void HandControl::setFingerGuiValue(int finger_number, int new_value)
{
	if ((finger_number > 0) && (finger_number <= nb_of_finger))
	{
		finger_gui_cmd_[finger_number - 1] = new_value;
	}
}

/**
* Get the value of the PWM for a specific motor received from the GUI.
*
*
* @param The number of the finger (index-1)
* @return The PWM value of the corresponding finger.
*/
int HandControl::getFingerGuiValue(int finger_number)
{
	if ((finger_number > 0) && (finger_number <= nb_of_finger))
	{
		return finger_gui_cmd_[finger_number - 1];
	}
}

/**
* Set the value of the PWM to send to a finger by the Glove.
*
* The specified value of PWM will be send inside in a private specific array.
* The array index is the finger number -1.
*
* @param The number of the finger (index-1) and the new value to insert.
* @return Nothing.
*/
void HandControl::setFingerGloveValue(int finger_number, int new_value)
{
	if ((finger_number > 0) && (finger_number <= nb_of_finger))
	{
		finger_glove_cmd_[finger_number - 1] = new_value;
	}
}

/**
* Get the value of the actual position of a specified finger
*
* @param The number of the finger (index-1)
* @return The PWM value of the corresponding finger.
*/
int HandControl::getFingerGloveValue(int finger_number)
{
	if ((finger_number > 0) && (finger_number <= nb_of_finger))
	{
		return finger_glove_cmd_[finger_number - 1];
	}
}

/**
* Get the value of the actual position of each servos 
*
* @param The new mode.
* @return Nothing.
*/
void HandControl::getMotorValue(void)
{
	//read servos angle
	finger_glove_cmd_[0] = thumb.read();             
	finger_glove_cmd_[1] = INDEX.read();
	finger_glove_cmd_[2] = middle.read();
}

/**
* send the commands to the servos related to the fingers
*
* @param The new mode.
* @return Nothing.
*/
void HandControl::setMotorValue(void)
{
	//move servos to set angles
	thumb.write(finger_glove_cmd_[0]);             
	INDEX.write(finger_glove_cmd_[1]);
	middle.write(finger_glove_cmd_[2]);
}

/**
* translate the command receive by the radio to work with the servos. 
*
*
* @param The new mode.
* @return Nothing.
*/
void HandControl::read_radio(void)
{
  radio->read(&text_radio, sizeof(text_radio));
  Serial.println("je suis dans read_radio");
  int i = 0;
  int finger_id = 0;

  for(i=0;text_radio[i]!=';';i++){
    finger_value_str += text_radio[i];
    if(text_radio[i]==','){
      finger_glove_cmd_[finger_id] = finger_value_str.toInt();
      finger_id += 1;
      finger_value_str = "";
    }
  }
}

/**
* detach the srevos related to each fingers.
* Used when it is in Free mode
*
*
* @param The new mode.
* @return Nothing.
*/
void HandControl::detachMotor(void)
{
	thumb.detach();
  	INDEX.detach();
  	middle.detach();
}

/**
* Attach a servo for each finger 
*
* @param The new mode.
* @return Nothing.
*/
void HandControl::attachMotor(void)
{
	thumb.attach(pin_m1);
  	INDEX.attach(pin_m2);
  	middle.attach(pin_m3);
}
