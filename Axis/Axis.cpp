/*
 *	This Source Code Form is subject to the terms of the Mozilla Public
 *	License, v. 2.0.If a copy of the MPL was not distributed with this
 *	file, You can obtain one at http ://mozilla.org/MPL/2.0/.
 */

/********
 * Fichier: Axis_Status.cpp
 * Auteurs: M.-A Martel
 * Date: 06 Fevrier 2019 (creation)
 * Description:
********/

#include "Axis.h"
using namespace std;

#if defined(__OPENCM904__)
  #define DEVICE_NAME "3" //Dynamixel on Serial3(USART3)  <-OpenCM 485EXP
#elif defined(__OPENCR__)
  #define DEVICE_NAME ""
#endif

// **************** Class Constructor ****************

Axis::Axis(uint8_t AxisID, uint32_t baud, int new_model, int MinSoft, int MaxSoft)
{
	ID		= AxisID;
	uint8_t get_id[16];
	uint8_t scan_cnt = 0;

	isFreeToMove = false;
	torqueControlEnable = false;
	blink_timer=0;

	MaxSoftlimit = MaxSoft;
	MinSoftlimit = MinSoft;

	Sts_Homed 	= 0;
	Sts_Homing	= 0;
	HomeOffset 	= 0;
	dontMoveBackward  = 0 ;
	dontMoveForward   = 0 ;

	torque_counter_filter = NULL;
	moving_counter_filter = NULL;

	if ((new_model == 350) or (new_model == 250))
	{
		model = new_model;
	}
	else
	{
		Serial.println("New model unknown. We'll consider it to be 350");
		model = 350;
	}

	// **** Initialisation de la communication avec les moteurs ****
	result = dxl.init(DEVICE_NAME, 57600);
	if (result == false)
	{
	Serial.println(log);
	Serial.println("Failed to init");
	}
	else
	{
	Serial.print("Succeed to init : ");
	Serial.println(baud);
	}

	// **** Scan du reseau pour lire les moteurs ****
	uint8_t range = 10;
	result = dxl.scan(get_id, &scan_cnt, range);
	if (result == false)
	{
	Serial.println(log);
	Serial.println("Failed to scan");
	}
	else
	{
	Serial.print("Find ");
	Serial.print(scan_cnt);
	Serial.println(" Dynamixels");
	}

	for (int cnt = 0; cnt < scan_cnt; cnt++)
	{
		Serial.print("id : ");
		Serial.print(get_id[cnt]);
		Serial.print(" model name : ");
		Serial.println(dxl.getModelName(get_id[cnt]));
	}
}

// **************** Class Destructor ****************

Axis::~Axis()
{
	delete torque_counter_filter;
	delete moving_counter_filter;
}


// *****************************************************************************************************
// ***************************************** PUBLIC METHODS ********************************************
// *****************************************************************************************************

// **************** Enabling Methods ****************

void Axis::Enable()
{
	result = dxl.torqueOn(ID, &log);
	if(debugMode == 1)
	{
		if (result == false)
		{
			Serial.println(log);
			return;
		}
		else
		{
			Serial.println(log);
		}
	}
	if(result)
	{
		Sts_Enabled = 1;
	}
}

void Axis::Disable()
{
	result = dxl.torqueOff(ID, &log);
	if(debugMode == 1)
	{
		if (result == false)
		{
			Serial.println(log);
			return;
		}
		else
		{
			Serial.println(log);
		}
	}
	if(result)
	{
		Sts_Enabled = 0;
	}
}

// **************** Moving Methods ****************

/*
void Axis::Zero()
{
	result = dxl.jointMode(ID, 0, 0, &log);

	if(debugMode == 1)
	{
		if (result == false)
		{
			Serial.println(log);
			return;
		}
		else
		{
			Serial.println(log);
		}
	}

	result = dxl.goalPosition(ID, (int32_t)0, &log);

	if(debugMode == 1)
	{
		if (result == false)
		{
			Serial.println(log);
			return;
		}
		else
		{
			Serial.println(log);
		}
	}
}
*/

void Axis::HomeRequest(bool *HomeSW)
{
	Sts_Homed = 0;
	HomeOffset = 0;

	//writeRegister("Homing_Offset", convertAngle2Value(HomeOffset));

	if(Sts_Homing == 0)
	{
		moveAtSpeed(String(-50));
	}

	Sts_Homing = 1;

	if(*HomeSW)
	{
		stopCmd();
		if (getMovingStatus() == 0)
        {
			HomeOffset = 0 - getPosition();
			Sts_Homing = 0;
			Sts_Homed = 1;
			Serial.println(log);
			Serial.println("Home Offset =");
			Serial.println(HomeOffset);
			Serial.println("Actual Pos =");
			Serial.println(Sts_ActualPosition);
		}
		dontMoveBackward = 1;

		//writeRegister("Homing_Offset", convertAngle2Value(HomeOffset));

	}
}

void Axis::stopCmd()
{
	moveAtSpeed("0");
}

void Axis::verifGoalAchieve(void) // Modifier les get pour la variable directement si modif de readStatus
{
    if (getMovingStatus()!= 0)
    {
        if(getVelocity() > 0) // verification si velocity a des valeurs positives ou négatives
        {
            if (getPosition() >= (Sts_GoalPosition-2) || getPosition() >= MaxSoftlimit)
            {
                stopCmd();
            }
        }
        else
        {
            if (getPosition() <= (Sts_GoalPosition+2) || getPosition() <= MinSoftlimit)
            {
                stopCmd();
            }
        }
    }
}

/*
void Axis::moveTo(String cmd)
{
	uint16_t position = cmd.toInt();

	if(position > MaxSoftlimit)
	{
		position = MaxSoftlimit;
	}
	else if (position < MinSoftlimit)
	{
		position = MinSoftlimit;
	}

	result = dxl.jointMode(ID, 0, 0, &log);

	if(debugMode == 1)
	{
		if (result == false)
		{
			Serial.println(log);
			return;
		}
		else
		{
			Serial.println(log);
		}
	}

	result = dxl.goalPosition(ID, (int32_t)position, &log);

	if(debugMode == 1)
	{
		if (result == false)
		{
			Serial.println(log);
			return;
		}
		else
		{
			Serial.println(log);
		}
	}
}
*/

void Axis::moveAtSpeed(String cmd)
{

	int32_t vitesse  = cmd.toInt();

    if (vitesse < 0)
	{
			dontMoveForward = 0;

			if (dontMoveBackward)
					{
							Serial.println ("You can't move backward");
							return;
					}

	}

	if (vitesse > 0)
	{
			dontMoveBackward = 0;

			if (dontMoveForward)
					{
							Serial.println("You can't move Forward");
							return;
					}

	}

	result = dxl.wheelMode(ID, 0, &log);

	if(debugMode == 1)
	{
		if (result == false)
		{
				Serial.println(log);
				return;
		}
		else
		{
				Serial.println(log);
		}
	}

	result = dxl.goalVelocity(ID, (int32_t)vitesse, &log);

	if(debugMode == 1)
	{
		if (result == false)
		{
				Serial.println(log);
				return;
		}
		else
		{
				Serial.println(log);
		}
	}
}

// **************** Set Parameters Methods ****************

void Axis::setPermissionForward()
{
    dontMoveForward = 1;
}

void Axis::setPermissionBackward()
{
    dontMoveBackward = 1;
}

void Axis::setGoalPosition(int goalP)
{
    Sts_GoalPosition = goalP;
}

void Axis::setMaxSoftlimit(String cmd)
{
	int32_t value = cmd.toInt();

	if(value >= MinSoftlimit && value <= 359 && value >= 0)
	{
		MaxSoftlimit = value;
		writeRegister("Max_Position_Limit", value);

		if(debugMode == 1)
		{
			Serial.println('Succed to set Maximum Softlimit');
		}
	}
	else
	{
		if(debugMode == 1)
		{
			Serial.println('Fail to set Maximum Softlimit');
		}
	}
}

void Axis::setMinSoftlimit(String cmd)
{
	int32_t value = cmd.toInt();

	if(value <= MaxSoftlimit && value <= 359 && value >= 0)
	{
		MinSoftlimit = value;
		writeRegister("Min_Position_Limit", value);

		if(debugMode == 1)
		{
			Serial.println('Succed to set Minimum Softlimit');
		}
	}
	else
	{
		if(debugMode == 1)
		{
			Serial.println('Fail to set Minimum Softlimit');
			Serial.println('Invalid data for Minimum Softlimit');
		}
	}
}

void Axis::setTorqueFilter(float new_reference, float new_maxDifference, int new_counterBeforeTrigger)
{
	if (torque_counter_filter != NULL)
	{
		delete torque_counter_filter;
	}
	torque_counter_filter = new counter_filter(new_reference, new_maxDifference, new_counterBeforeTrigger);
}

void Axis::setMovingFilter(float new_reference, float new_maxDifference, int new_counterBeforeTrigger)
{
	if (moving_counter_filter != NULL)
	{
		delete moving_counter_filter;
	}
	moving_counter_filter = new counter_filter(new_reference, new_maxDifference, new_counterBeforeTrigger);
}

void Axis::blink(blink_state new_blink_state, unsigned long time_open_millis)
{
	bool led_status = readRegister("LED");

	if (new_blink_state != STOP_BLINK)
	{
		unsigned long actual_time = millis();
		if (abs(actual_time - blink_timer) > time_open_millis)
		{
			blink_timer = actual_time;
			if (led_status == true)
			{
				dxl.ledOff(ID, nullptr);
			}
			else
			{
				dxl.ledOn(ID, nullptr);
			}
		}
	}
	else
	{
		if (led_status = true)
		{
			dxl.ledOff(ID, nullptr);
		}
	}
}

// **************** Read Paramters Methods ****************

void Axis::readStatus()
{
	int dummy = 0;
	dummy = getPosition();
	dummy = getCurrent();
	dummy = getTorque();
	dummy = getVelocity();
	dummy = getMovingStatus();
}

int Axis::getPosition()
{
	Sts_ActualPosition = (convertValue2Angle(readRegister("Present_Position")));

	if(Sts_ActualPosition>360)
	{
		float temp = (Sts_ActualPosition/360)-(int(Sts_ActualPosition/360));
		Sts_ActualPosition = (temp*360);
	}
	else if((-1*Sts_ActualPosition)>360)
	{
		Sts_ActualPosition = -1*Sts_ActualPosition;
		float temp = (Sts_ActualPosition/360)-(int(Sts_ActualPosition/360));
		Sts_ActualPosition = (-1*temp*360);
	}
	Sts_ActualPosition = Sts_ActualPosition + HomeOffset;

	if(debugMode == 1)
	{
		Serial.println("Actual Position is : ");
		Serial.println(Sts_ActualPosition);
	}

	return Sts_ActualPosition;
}

int Axis::getCurrent()
{
	Sts_ActualCurrent = dxl.convertValue2Current(readRegister("Present_Current"));

	if(debugMode == 1)
	{
		Serial.println("Actual Current is : ");
		Serial.println(Sts_ActualCurrent);
		Serial.println(" mA");
	}

	return Sts_ActualCurrent;
}

int Axis::getTorque()
{

	if (model == 350)
	{
		short actual_current = readRegister("Present_Current");
		Sts_ActualTorque = actual_current;
	}
	else if (model == 250)
	{
		Sts_ActualTorque = int(readRegister("Present_Load"));
	}
	else
	{
		Serial.println("Model unknow: can not get torque");
	}

	if(debugMode == 1)
	{
		Serial.println("Actual Torque is : ");
		Serial.println((short)Sts_ActualTorque);
		Serial.println(" %");
	}

	return Sts_ActualTorque;
}

int Axis::getVelocity()
{
	Sts_ActualVelocity = dxl.convertValue2Velocity(ID,readRegister("Present_Velocity"));

	if(debugMode == 1)
	{
		Serial.println("Actual Velocity is : ");
		Serial.println(Sts_ActualVelocity);
		Serial.println(" RPM");
	}

	return Sts_ActualVelocity;
}

int Axis::getMovingStatus()
{
	Sts_Moving = readRegister("Moving");

	if(debugMode == 1)
	{
		if(Sts_Moving)
		{
			Serial.println("Motor is Running");
		}
		else
		{
			Serial.println("Motor is stopped");
		}
	}

	return Sts_Moving;
}

bool Axis::getPermissionForward()
{

    if (debugMode == 1)
        {
            if(dontMoveForward)
            {
                Serial.println("Can't move Forward");
            }

            else
            {
                Serial.println("You can move Forward");
            }
        }
    return dontMoveForward;
}

bool Axis::getPermissionBackward()
{

    if (debugMode == 1)
        {
            if(dontMoveBackward)
            {
                Serial.println("Can't move Forward");
            }

            else
            {
                Serial.println("You can move Forward");
            }
        }
    return dontMoveBackward;
}

// **************** Read/Write Register Methods ****************

int Axis::readRegister(String regName)
{
	int32_t data = 0;


	int result = dxl.itemRead(ID, regName.c_str(), &data, &log);

	if(debugMode == 1)
	{
		if (result == false)
		{
			Serial.println(log);	//test
			Serial.println("Failed to read");
		}
		else
		{
			Serial.println(log);	//test
			Serial.print("read data : ");
			Serial.println(data);
		}
	}

	return data;
}

void Axis::writeRegister(String regName, int32_t value)
{
	if(Sts_Enabled)
	{
		Disable();
		int result = dxl.writeRegister(ID, regName.c_str(), value, &log);
		Enable();
	}
	else
	{
		int result = dxl.writeRegister(ID, regName.c_str(), value, &log);
	}

	if(debugMode == 1)
	{
		if (result == false)
		{
			Serial.println(log);
			Serial.println("Failed to write");
		}
		else
		{
			Serial.println(log);
			Serial.print("Succeed to write data : ");
			Serial.println(value);
		}
	}
}

// *****************************************************************************************************
// **************************************** PRIVATE METHODS ********************************************
// *****************************************************************************************************

// **************** Convertion Methods ****************

float Axis::convertValue2Angle(int value)
{
    return (value*360/4095);
}

int Axis::convertAngle2Value(float angle)
{
    return (angle*4095/360);
}
