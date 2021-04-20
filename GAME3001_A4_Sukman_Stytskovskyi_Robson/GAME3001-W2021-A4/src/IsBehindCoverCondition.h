#pragma once
#ifndef __IS_BEHIND_COVER_CONDITION__
#define __IS_BEHIND_COVER_CONDITION__
#include "ConditionNode.h"

class IsBehindCoverCondition : public ConditionNode
{
public:
	IsBehindCoverCondition(bool state = false);
	virtual ~IsBehindCoverCondition();

	// Getters and Setters
	void setBehindCover(bool state);

	virtual bool Condition() override;
private:
	bool m_isBehindCover;
};

#endif /* defined (__FLEE_CONDITION__) */