#include "WaitInCoverAction.h"
#include <iostream>

WaitInCoverAction::WaitInCoverAction()
{
	name = "Wait In Cover Action";
}

WaitInCoverAction::~WaitInCoverAction()
= default;

void WaitInCoverAction::Action()
{
	std::cout << "Performing Wait In Cover Action" << std::endl;
}
