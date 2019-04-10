/*
*	This Source Code Form is subject to the terms of the Mozilla Public
*	License, v. 2.0.If a copy of the MPL was not distributed with this
*	file, You can obtain one at http ://mozilla.org/MPL/2.0/.
*/

#include "counter_filter.h"


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

void counter_filter::setReference(float new_reference)
{
	reference = new_reference;
}

void counter_filter::setMaxDifference(float new_maxDifference)
{
	maxDifference = new_maxDifference;
}

void counter_filter::setCounterBeforeTrigger(int new_counterBeforeTrigger)
{
	counterBeforeTrigger = new_counterBeforeTrigger;
}

int counter_filter::reset_counter(void)
{
	int counter_value = counter;
	counter = 0;
	return counter_value;
}

int counter_filter::getCounter(void)
{
	return counter;
}

float counter_filter::absolute(float value)
{
	if (value < 0)
	{
		return -value;
	}
	return value;
}