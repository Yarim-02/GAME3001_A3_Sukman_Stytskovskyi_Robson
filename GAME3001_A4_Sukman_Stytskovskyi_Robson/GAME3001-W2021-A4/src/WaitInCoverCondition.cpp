#include "WaitInCoverCondition.h"

WaitInCoverCondition::WaitInCoverCondition(bool timer_out)
{
	setIsTimerOut(timer_out);
	name = "Wait In Cover Condition";
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
