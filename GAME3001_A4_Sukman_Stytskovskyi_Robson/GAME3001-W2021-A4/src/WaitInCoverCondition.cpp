#include "WaitInCoverCondition.h"

WaitInCoverCondition::WaitInCoverCondition(bool timer_out)
{
	setIsTimerOut(timer_out);
	name = "Move Behind Cover Condition";
}

WaitInCoverCondition::~WaitInCoverCondition()
= default;

void WaitInCoverCondition::setIsTimerOut(bool state)
{
	data = state;
	m_timerOut = state;
}

bool WaitInCoverCondition::Condition()
{
	return m_timerOut;
}
