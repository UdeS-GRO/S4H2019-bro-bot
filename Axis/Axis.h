#ifndef AXIS_H
#define AXIS_H

using namespace std;
#include <DynamixelWorkbench.h>
#include "WString.h"

class Axis
{
public:
  Axis(uint8_t ID, uint32_t baud);
  ~Axis();
  int tews;

	uint8_t ID;
  DynamixelWorkbench dxl;
	float Sts_ActualPosition;
	float Sts_ActualCurrent;
	float Sts_ActualTorque;
	float Sts_ActualVelocity;
  bool Sts_Moving;
  bool Sts_AtPosition;
  void  moveTo(String cmd);
  void  moveAtSpeed(String cmd);
  void  Zero();
  int	getPosition();
  int	getCurrent();
  int	getTorque();
  int	getVelocity();
  int	getMovingStatus();
  int readRegister(String regName);
  void writeRegister(String regName, int32_t value);

private:

	const char *log = NULL;
  bool result = false;


  float convertValue2Angle(int value);
  int convertAngle2Value(float angle);



};

#endif
