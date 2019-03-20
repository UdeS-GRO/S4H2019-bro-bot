/*
	This Source Code Form is subject to the terms of the Mozilla Public
	License, v. 2.0.If a copy of the MPL was not distributed with this
	file, You can obtain one at http ://mozilla.org/MPL/2.0/.
*/

#include "HandControl.h"

HandControl::HandControl(int number_of_fingers)
{
	finger_gui_cmd_ = new int[number_of_fingers];
	finger_glove_cmd_ = new int[number_of_fingers];
	mode = FREE;

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
	finger_gui_cmd_[finger_number - 1] = new_value;
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
	return finger_gui_cmd_[finger_number - 1];
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
	finger_glove_cmd_[finger_number - 1] = new_value;
}

/**
* Get the value of the PWM for a specific motor received from the GUI.
*
*
* @param The number of the finger (index-1)
* @return The PWM value of the corresponding finger.
*/
int HandControl::getFingerGloveValue(int finger_number)
{
	return finger_glove_cmd_[finger_number - 1];
}