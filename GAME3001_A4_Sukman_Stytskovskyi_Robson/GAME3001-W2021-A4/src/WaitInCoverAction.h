#pragma once
#ifndef __WAIT_IN_COVER_ACTION__
#define __WAIT_IN_COVER_ACTION__
#include "ActionNode.h"


class WaitInCoverAction : public ActionNode
{
public:
	WaitInCoverAction();
	virtual ~WaitInCoverAction();

	virtual void Action() override;
private:

};

#endif /* defined (__WAIT_IN_COVER_ACTION__) */
