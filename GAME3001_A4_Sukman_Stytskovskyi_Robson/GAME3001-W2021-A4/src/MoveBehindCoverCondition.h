#pragma once
#ifndef __MOVE_BEHIND_COVER_CONDITION__
#define __MOVE_BEHIND_COVER_CONDITION__
#include "ConditionNode.h"

class MoveBehindCoverCondition : public ConditionNode
{
public:
	MoveBehindCoverCondition(bool hit = false);
	virtual ~MoveBehindCoverCondition();

	// Getters and Setters
	void setGotHit(bool state);

	virtual bool Condition() override;
private:
	bool m_GotHit;
};

#endif /* defined (__MOVE_BEHIND_CONDITION__) */