#pragma once
/*
	This Source Code Form is subject to the terms of the Mozilla Public
	License, v. 2.0.If a copy of the MPL was not distributed with this
	file, You can obtain one at http ://mozilla.org/MPL/2.0/.
*/


#ifndef HAND_CONTROL_H
#define HAND_CONTROL_H


enum ControlFingerState
{
	LOCK = 1,
	FREE,
	GLOVE,
	GUI
};

class HandControl
{
private:
	int* finger_gui_cmd_;
	int* finger_glove_cmd_;
	int nb_of_finger;
	ControlFingerState mode;

public:
	/* Constructor and destructor*/
	HandControl(int number_of_fingers);
	~HandControl();

	void setMode(ControlFingerState new_mode);
	ControlFingerState getMode(void);

	void setFingerGuiValue(int finger_number, int new_value);
	int getFingerGuiValue(int finger_number);

	void setFingerGloveValue(int finger_number, int new_value);
	int getFingerGloveValue(int finger_number);
};



#endif