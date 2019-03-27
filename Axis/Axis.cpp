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

/**
* Constructor of the Axis class
*
* Initialize all the states of the motor to 0, the 2 soft limits and the motor according to his model.
* It also check if the motor communicate with the OpenCR
*
* @param the ID of the motor, the baudrate, the model number, the minimum software limit and the maximum
* @return Nothing.
*/
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
	Sts_AtPosition = true;

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
/**
* Enable the motor by setting his torque on.
*
*
* @param Nothing
* @return Nothing.
*/
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
/**
* Disable the motor by setting his torque off.
*
*
* @param Nothing
* @return Nothing.
*/
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
/**
* Initialize the zero position of the motor by setting an offset to his position
*
*
* @param a pointer to the bool of the switch (1 = switch pressed)
* @return Nothing.
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
/**
* Stop the motor from spinning
*
*
* @param Nothing
* @return Nothing.
*/
void Axis::stopCmd()
{
    Sts_AtPosition = true;
	moveAtSpeed("0");
}
/**
* check if the motor achieve his goal position when moveto cmd is sent on the main_open_cr.ino
*
*
* @param Nothing
* @return Nothing.
*/
void Axis::verifGoalAchieve()
{
    if (!Sts_AtPosition)
    {
        if(Sts_ActualVelocity > 0) // verification si velocity a des valeurs positives ou négatives
        {
            if (Sts_ActualPosition >= (Sts_GoalPosition-2) || Sts_ActualPosition >= MaxSoftlimit)
            {
                stopCmd();
                Sts_AtPosition = 1;
            }
        }
        else if (Sts_ActualVelocity < 0)
        {
            if (Sts_ActualPosition <= (Sts_GoalPosition+2) || Sts_ActualPosition <= MinSoftlimit)
            {
                stopCmd();
                Sts_AtPosition = 1;
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
/**
* make the motor spin to a given speed. Negative = clockwise ***********VÉRIFIER CE STATEMENT
* It also put the variable dontMoveForward or dontMoveBackward to zero depending on the sense the motor turns
* This is to follow the event of a switch being activate and then forbiding to going in the sense the motor
* was spinning
*
* @param string of a number representing a certain speed [ 1 unity is 0.229 rpm ]
* @return Nothing.
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
/**
* By setting dontMoveForward to 1, this fonction forbid the motor to go anti-clockwise ************ à VÉRIFIER
*
*
* @param Nothing
* @return Nothing.
*/
void Axis::setPermissionForward()
{
    dontMoveForward = 1;
}
/**
* By setting dontMoveBackward to 1, this fonction forbid the motor to go clockwise ************ à VÉRIFIER
*
*
* @param Nothing
* @return Nothing.
*/
void Axis::setPermissionBackward()
{
    dontMoveBackward = 1;
}
/**
* Set the goal position in degrees according to the home offset of the fct HomeRequest
*
*
* @param the desired position in degrees
* @return Nothing.
*/
void Axis::setGoalPosition(float goalP)
{
    Sts_GoalPosition = goalP;
}

void Axis::setAtPosition(bool Reached)
{
    Sts_AtPosition = Reached;
}

/**
* Set the new maximum software limit to the motor
*
*
* @param the desired new max soft limit in degrees
* @return Nothing.
*/
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
/**
* Set the new minimum software limit to the motor
*
*
* @param the desired new min soft limit in degrees
* @return Nothing.
*/

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

/**
* Set a new counter from counter_filter.h.
* Used to trigger the torque off after a certain amount of time of applying an external torque.
* At this moment, the motor is easily manageable for teaching for instance.
*
* @param reference of a normal torque without external force, the maximum difference between the reference
*         and the actual torque to exceed for the counter to add up. The number to reach by the counter to trigger the action.
* @return Nothing.
*/
void Axis::setTorqueFilter(float new_reference, float new_maxDifference, int new_counterBeforeTrigger)
{
	if (torque_counter_filter != NULL)
	{
		delete torque_counter_filter;
	}
	torque_counter_filter = new counter_filter(new_reference, new_maxDifference, new_counterBeforeTrigger);
}

/**
* Set a new counter from counter_filter.h.
* Used to trigger the torque on after a certain amount of time of NOT applying an external torque.
* At this moment, the motor isn't easily manageable anymore and can now receive new command.
*
@param reference of a normal torque without external force, the maximum difference between the reference
*         and the actual torque to exceed for the counter to add up. The number to reach by the counter to trigger the action.
* @return Nothing.
*/
void Axis::setMovingFilter(float new_reference, float new_maxDifference, int new_counterBeforeTrigger)
{
	if (moving_counter_filter != NULL)
	{
		delete moving_counter_filter;
	}
	moving_counter_filter = new counter_filter(new_reference, new_maxDifference, new_counterBeforeTrigger);
}

/**
* Set the LED to blink or not. We use it to show when the motor is in teaching mode after the trigger of the torque_counter_filter
*
*
* @param new blink state(on/off), the time the LED is on for each blink
* @return Nothing.
*/
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
/**
* Read the actual status of the position, Current, torque, velocity and the moving status of the motor
*
*
* @param Nothing
* @return Nothing.
*/
void Axis::readStatus()
{
	int dummy = 0;
	dummy = getPosition();
	dummy = getCurrent();
	dummy = getTorque();
	dummy = getVelocity();
	dummy = getMovingStatus();
}
/**
* Get the actuel position of the motor in degrees according to the home offset of the fct HomeRequest
*
*
* @param Nothing
* @return the actual Position [degrees]
*/

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
/**
* Get the actual current of the motor. Used with the torque control of the 350 model
*
*
* @param Nothing
* @return the actual current [1 unit = 2.69 [mA] ]
*/
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
/**
* Get the actual torque of the motor. Used with the torque control of the 250 model
*
*
* @param Nothing
* @return actual torque [1 unit = 0.1%]
*/
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

/**
* Get the actual velocity of the motor.
*
*
* @param Nothing
* @return actual velocity [1 unit = 0.229 [rpm]]
*/

int Axis::getVelocity()
{
	Sts_ActualVelocity = readRegister("Present_Velocity");


	if(debugMode == 1)
	{
		Serial.println("Actual Velocity is : ");
		Serial.println(Sts_ActualVelocity);
		Serial.println(" RPM");
	}

	return Sts_ActualVelocity;
}
/**
* Get the actual moving status. If it is moving or not
*
*
* @param Nothing
* @return actual moving status. Even if its a int, it's either 1 or 0
*/

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

/**
* Get the variable dontMoveForward. To know if the motor can move anti-clockwise ***********
*
*
* @param Nothing
* @return if it can or not move forward (1 = CAN'T)
*/
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
/**
* Get the variable dontMoveBackward. To know if the motor can move clockwise ***********
*
*
* @param Nothing
* @return if it can or not move backward (1 = CAN'T)
*/
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
/**
* Function used in many other fct. Read the registers of the motor
*
*
* @param the register to read
* @return the data in the desired register
*/
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
/**
* Function used in many other fct. write in registers of the motor
*
*
* @param the register in which you want to write and the value to write in.
* @return Nothing
*/
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
/**
* Convert the value of the register in degree
*
*
* @param the value to convert
* @return value converted in degrees
*/
float Axis::convertValue2Angle(int value)
{
    return (value*360/4095);
}
/**
* Convert the value in degrees into the value suitable for the register. Used when writing in register
*
* @param the angle to convert
* @return the value suitable for the register
*/
int Axis::convertAngle2Value(float angle)
{
    return (angle*4095/360);
}
