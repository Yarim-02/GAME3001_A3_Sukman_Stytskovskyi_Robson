#pragma once
#ifndef __FLEE_CONDITION__
#define __FLEE_CONDITION__
#include "ConditionNode.h"

class FleeCondition : public ConditionNode
{
public:
	FleeCondition(bool LOS = false);
	virtual ~FleeCondition();

	// Getters and Setters
	void setFleeing(bool state);

	virtual bool Condition() override;
private:
	bool m_isFleeing;
};

#endif /* defined (__FLEE_CONDITION__) */