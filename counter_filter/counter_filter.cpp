/*
*	This Source Code Form is subject to the terms of the Mozilla Public
*	License, v. 2.0.If a copy of the MPL was not distributed with this
*	file, You can obtain one at http ://mozilla.org/MPL/2.0/.
*/

#include "counter_filter.h"

/**
* Constructor of the counter_filter class
*
* Initialize the values needed for the counter
*
* @param the reference value that represents a "normal torque", the max difference of torque needed to start the counter,
            the value the counter must reach to trigger
* @return Nothing.
*/
counter_filter::counter_filter(float new_reference, float new_maxDifference, int new_counterBeforeTrigger)
{
	reference = new_reference;
	maxDifference = new_maxDifference;
	counterBeforeTrigger = new_counterBeforeTrigger;
	counter = 0;
}


counter_filter::~counter_filter()
{
}

/**
* Function which verify if the actual difference of the torque with the reference value is higher than the max difference value
* When it is, the counter is incremented by one. When it's not, it is decremented by one until it reaches 0. If the counter
* reaches the counterBeforeTrigger value, it returns true to indicated it.
*
*
* @param the actual value of the torque
* @return true when the counter before trigger is reached
*/
bool counter_filter::compute(float present_value)
{
	bool value_to_return = false;
	float diff = (float)absolute(present_value - reference);

	if ((diff - maxDifference) > 0.001)
	{
		counter++;
	}
	else if (counter > 0)
	{
		counter--;
	}
	else
	{
		counter = 0;
	}

	if (counter > counterBeforeTrigger)
	{
		value_to_return = true;
	}

	return value_to_return;
}

/**
* Function used to know if the counter is triggered
*
*
* @param Nothing
* @return if it is trigger or not, true mean triggers.
*/
bool counter_filter::isTriggered(void)
{
	if (counter > counterBeforeTrigger)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/**
* Function used to set a new reference value
*
*
* @param the new reference
* @return Nothing.
*/
void counter_filter::setReference(float new_reference)
{
	reference = new_reference;
}

/**
* Function used to set a new max difference value
*
*
* @param the new max difference with the reference value to start the counter
* @return Nothing.
*/
void counter_filter::setMaxDifference(float new_maxDifference)
{
	maxDifference = new_maxDifference;
}

/**
* Function used to set a the new counterBeforeTrigger value
*
*
* @param the new value the counter need to reach before it triggers
* @return Nothing.
*/
void counter_filter::setCounterBeforeTrigger(int new_counterBeforeTrigger)
{
	counterBeforeTrigger = new_counterBeforeTrigger;
}

/**
* Function used to reset the counter
*
*
* @param Nothing
* @return the reset counter
*/
int counter_filter::reset_counter(void)
{
	int counter_value = counter;
	counter = 0;
	return counter_value;
}

/**
* Function to get the actual counter
*
*
* @param Nothing
* @return the actual value of the counter
*/
int counter_filter::getCounter(void)
{
	return counter;
}

/**
* Function that makes negative value become positive values
*
*
* @param value to put positive
* @return positive value.
*/
float counter_filter::absolute(float value)
{
	if (value < 0)
	{
		return -value;
	}
	return value;
}
