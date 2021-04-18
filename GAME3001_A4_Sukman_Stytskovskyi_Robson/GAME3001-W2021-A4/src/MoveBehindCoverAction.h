#pragma once
#ifndef __MOVE_BEHIND_COVER_ACTION__
#define __MOVE_BEHIND_COVER_ACTION__
#include "ActionNode.h"


class MoveBehindCoverAction : public ActionNode
{
public:
	MoveBehindCoverAction();
	virtual ~MoveBehindCoverAction();

	virtual void Action() override;
private:

};

#endif /* defined (__MOVE_TO_LOS_ACTION__) */