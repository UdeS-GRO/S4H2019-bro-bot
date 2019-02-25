#pragma once

#ifndef COUNTER_FILTER_H
#define COUNTER_FILTER_H
class counter_filter
{
public:
	counter_filter( float new_reference, float new_maxDifference, int new_counterBeforeTrigger);
	~counter_filter();

	bool compute(float present_value);
	bool isTriggered(void);

	void setReference(float new_reference);
	void setMaxDifference(float new_maxDifference);
	void setCounterBeforeTrigger(int new_counterBeforeTrigger);
	int reset_counter(void);
	int getCounter(void);
	int	counter;



private:
	float reference;
	float maxDifference;
	int counterBeforeTrigger;
	float absolute(float value);
};

#endif

