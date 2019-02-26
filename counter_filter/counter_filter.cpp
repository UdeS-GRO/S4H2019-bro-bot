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

	if (absolute(present_value - reference) > maxDifference)
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