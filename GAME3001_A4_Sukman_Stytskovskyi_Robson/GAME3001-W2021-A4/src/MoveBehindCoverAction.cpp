#include "MoveBehindCoverAction.h"
#include <iostream>

MoveBehindCoverAction::MoveBehindCoverAction()
{
	name = "Move Behind Cover Action";
}

MoveBehindCoverAction::~MoveBehindCoverAction()
= default;

void MoveBehindCoverAction::Action()
{
	std::cout << "Performing Move Behind Cover Action" << std::endl;
}
