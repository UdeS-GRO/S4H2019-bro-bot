/*
*	This Source Code Form is subject to the terms of the Mozilla Public
*	License, v. 2.0.If a copy of the MPL was not distributed with this
*	file, You can obtain one at http ://mozilla.org/MPL/2.0/.
*/

#ifndef AXIS_H
#define AXIS_H

using namespace std;
#include <DynamixelWorkbench.h>
#include "WString.h"
#include "counter_filter.h"
#include "Arduino.h"

enum blink_state 
{
	STOP_BLINK = 0,
	GREEN_BLINK = 1,
};

class Axis
{
public:

	// *********************************************
	// **** Class constructor and destructor ****

	Axis(uint8_t ID, uint32_t baud, int new_model = 350, int MinSoft = -20, int MaxSoft = 359);
	~Axis();


	// ************************
	// **** Public Tags ****

	int tews;
	uint8_t ID;
	int model;
	bool debugMode = 0;


	// ********************************
	// **** Torque Control Tags ****

	bool isFreeToMove;
	bool torqueControlEnable;
	counter_filter* torque_counter_filter;
	counter_filter* moving_counter_filter;


	// ************************
	// **** Status Tags ****

	float 	Sts_ActualPosition;
	float 	Sts_ActualCurrent;
	int 	Sts_ActualTorque;
	float 	Sts_ActualVelocity;
	bool 	Sts_Moving;
	bool 	Sts_AtPosition;
	bool 	Sts_Homed;
	bool 	Sts_Homing;


	// ****************************
	// **** Softlimits Tags ****

	int MaxSoftlimit;
	int MinSoftlimit;


	// *****************************
	// **** Enabling Methods ****

	void Enable();
	void Disable();


	// ***************************
	// **** Moving Methods ****

	void Zero();
	void HomeRequest(bool *HomeSW);
	void stopCmd();
	void moveTo(String cmd);
	void moveAtSpeed(String cmd);


	// ***********************************
	// **** Set Parameters Methods ****

	void setMaxSoftlimit(String cmd);
	void setMinSoftlimit(String cmd);

	void setTorqueFilter(float new_reference, float new_maxDifference, int new_counterBeforeTrigger);
	void setMovingFilter(float new_reference, float new_maxDifference, int new_counterBeforeTrigger);
	void blink(blink_state new_blink_state, unsigned long time_open_millis);


	// ************************************
	// **** Read Parameters Methods ****

	int	getPosition();
	int	getCurrent();
	int getTorque();
	int	getVelocity();
	int	getMovingStatus();

	void readStatus();
	

	// ****************************************
	// **** Read/Write Register Methods ****

	int readRegister(String regName);
	void writeRegister(String regName, int32_t value);

private:

	DynamixelWorkbench dxl;
	unsigned long 	blink_timer;
	const char 		*log = NULL;
	bool 			result = false;
	float			HomeOffset = 0;


	// *******************************
	// **** Convertion Methods ****

	float convertValue2Angle(int value);
	int convertAngle2Value(float angle);



};

#endif
