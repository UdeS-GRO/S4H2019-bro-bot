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
	Axis(uint8_t ID, uint32_t baud, int new_model = 350);
	~Axis();
	int tews;

	uint8_t ID;
	int model;

	bool isFreeToMove;						//Used for the torque control
	bool torqueControlEnable;				//Used for the torque control

	DynamixelWorkbench dxl;
	float Sts_ActualPosition;
	float Sts_ActualCurrent;
	int Sts_ActualTorque;
	float Sts_ActualVelocity;
	bool Sts_Moving;
	bool Sts_AtPosition;
	void  moveTo(String cmd);
	void  moveAtSpeed(String cmd);
	void  Zero();
	int	getPosition();
	int	getCurrent();
	int getTorque();
	int	getVelocity();
	int	getMovingStatus();
	int readRegister(String regName);
	void writeRegister(String regName, int32_t value);
	void setTorqueFilter(float new_reference, float new_maxDifference, int new_counterBeforeTrigger);
	void setMovingFilter(float new_reference, float new_maxDifference, int new_counterBeforeTrigger);
	counter_filter* torque_counter_filter;	//Used for the torque control
	counter_filter* moving_counter_filter;	//Used for the torque control
	void blink(blink_state new_blink_state, unsigned long time_open_millis);

private:
	unsigned long blink_timer;
	const char *log = NULL;
	bool result = false;

	float convertValue2Angle(int value);
	int convertAngle2Value(float angle);



};

#endif
