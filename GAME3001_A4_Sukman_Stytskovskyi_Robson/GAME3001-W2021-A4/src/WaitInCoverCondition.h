#pragma once
#ifndef __WAIT_IN_COVER_CONDITION__
#define __WAIT_IN_COVER_CONDITION__
#include "ConditionNode.h"

class WaitInCoverCondition : public ConditionNode
{
public:
	WaitInCoverCondition(bool timer_out = false);
	virtual ~WaitInCoverCondition();

	// Getters and Setters
	void setIsTimerOut(bool state);

	virtual bool Condition() override;
private:
	bool m_timerOut;
};

#endif /* defined (__WAIT_IN_COVER_CONDITION__) */
