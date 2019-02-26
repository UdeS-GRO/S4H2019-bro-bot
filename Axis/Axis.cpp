<<<<<<< HEAD

=======
>>>>>>> 3f32f45dcee8bcf9f178077325f960f86193b91e
/********
 * Fichier: Axis_Status.cpp
 * Auteurs: M.-A Martel
 * Date: 06 Février 2019 (creation)
 * Description: Implementation des methodes des classes decrites dans
 *    forme.h. Les methodes de la classe Forme ne doivent pas etre
 *    modifiees. Ce fichier fait partie de la distribution de Graphicus.
********/

#include "Axis.h"
using namespace std;

#if defined(__OPENCM904__)
  #define DEVICE_NAME "3" //Dynamixel on Serial3(USART3)  <-OpenCM 485EXP
#elif defined(__OPENCR__)
  #define DEVICE_NAME ""
#endif

Axis::Axis(uint8_t AxisID, uint32_t baud, int new_model)
{
	ID		= AxisID;
	uint8_t get_id[16];
	uint8_t scan_cnt = 0;

	isFreeToMove = false;
	torqueControlEnable = false;
	blink_timer=0;

	torque_counter_filter = NULL;
	moving_counter_filter = NULL;

	if ((new_model == 350) or (new_model == 250))
	{ 
		model = new_model;
	}
	else
	{
		Serial.println("New model unknow. Will consider it to be 350");
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

Axis::~Axis()
{
	delete torque_counter_filter;
	delete moving_counter_filter;
}

int Axis::readRegister(String regName)
{
	int32_t data = 0;


	int result = dxl.itemRead(ID, regName.c_str(), &data, &log);

	if (result == false)
        {
          //Serial.println(log);	//test
         // Serial.println("Failed to read");
        }
        else
        {
          //Serial.println(log);	//test
          //Serial.print("read data : ");
          //Serial.println(data);
        }

	return data;
}

void Axis::writeRegister(String regName, int32_t value)
{
	int result = dxl.writeRegister(ID, regName.c_str(), value, &log);
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

void Axis::Zero()
{
    result = dxl.jointMode(ID, 0, 0, &log);
        if (result == false)
        {
          Serial.println(log);
          return;
        }
        else
        {
          Serial.println(log);
        }

        result = dxl.goalPosition(ID, (int32_t)0, &log);
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

void Axis::moveTo(String cmd)
{
    uint16_t position = cmd.toInt();
    result = dxl.jointMode(ID, 0, 0, &log);
        if (result == false)
        {
          Serial.println(log);
          return;
        }
        else
        {
          Serial.println(log);
        }

        result = dxl.goalPosition(ID, (int32_t)position, &log);
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


void Axis::moveAtSpeed(String cmd)
{

    int32_t vitesse  = cmd.toInt();

    result = dxl.wheelMode(ID, 0, &log);
    if (result == false)
    {
        Serial.println(log);
        return;
    }
    else
    {
        Serial.println(log);
    }

    result = dxl.goalVelocity(ID, (int32_t)vitesse, &log);
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


int Axis::getPosition()
{
	Sts_ActualPosition = convertValue2Angle(readRegister("Present_Position"));

	if(Sts_ActualPosition>360)
	{
		float temp = (Sts_ActualPosition/360)-(int(Sts_ActualPosition/360));
		Sts_ActualPosition = (temp*360);
	}

	Serial.println("Actual Position is : ");
	Serial.println(Sts_ActualPosition);

	return Sts_ActualPosition;
}

int Axis::getCurrent()
{
	Sts_ActualCurrent = dxl.convertValue2Current(readRegister("Present_Current"));

	Serial.println("Actual Current is : ");
	Serial.println(Sts_ActualCurrent);
	Serial.println(" mA");

	return Sts_ActualCurrent;
}

int Axis::getTorque()
{
	
	if (model == 350)
	{
		Sts_ActualTorque = int(dxl.convertValue2Load(readRegister("Present_Current")));
	}
	else if (model == 250)
	{
		Sts_ActualTorque = int(readRegister("Present_Load"));
	}
	else 
	{
		Serial.println("Model unknow: can not get torque");
	}

	//Serial.println(" %");					//test: 
	//Serial.println(" %");					//test: 
	//Serial.println("Actual Torque is : ");
	//Serial.println((short)Sts_ActualTorque);
	//Serial.println(" %");

	return Sts_ActualTorque;
}

int Axis::getVelocity()
{
	Sts_ActualVelocity = dxl.convertValue2Velocity(ID,readRegister("Present_Velocity"));

	Serial.println("Actual Velocity is : ");
	Serial.println(Sts_ActualVelocity);
	Serial.println(" RPM");

	return Sts_ActualVelocity;
}

int Axis::getMovingStatus()
{
	Sts_Moving = readRegister("Moving");

	if(Sts_Moving)
	{
		//Serial.println("Motor is Running");		//test
	}
	else
	{
		//Serial.println("Motor is stopped");		//test
	}


	return Sts_Moving;
}

float Axis::convertValue2Angle(int value)
{
    return (value*360/4095);
}

int Axis::convertAngle2Value(float angle)
{
    return (angle*4095/360);
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