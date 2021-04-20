#pragma once
#ifndef __WAIT_CONDITION__
#define __WAIT_CONDITION__
#include "ConditionNode.h"

class WaitCondition : public ConditionNode
{
public:
	WaitCondition(bool state = false);
	virtual ~WaitCondition();

	// Getters and Setters
	void setWaiting(bool state);

	virtual bool Condition() override;
private:
	bool m_Waiting;
};

#endif /* defined (__FLEE_CONDITION__) */