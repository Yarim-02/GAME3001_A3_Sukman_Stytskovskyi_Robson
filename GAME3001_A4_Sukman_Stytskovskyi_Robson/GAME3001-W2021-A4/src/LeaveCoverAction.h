#pragma once
#ifndef __LEAVE_COVER_ACTION__
#define __LEAVE_COVER_ACTION__
#include "ActionNode.h"

class LeaveCoverAction : public ActionNode
{
public:
	LeaveCoverAction();
	virtual ~LeaveCoverAction();

	virtual void Action() override;
private:

};

#endif /* defined (__LEAVE_COVER_ACTION__) */
