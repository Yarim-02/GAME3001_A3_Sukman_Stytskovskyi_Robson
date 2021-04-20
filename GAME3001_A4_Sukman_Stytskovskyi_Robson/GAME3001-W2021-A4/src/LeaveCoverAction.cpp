#include "LeaveCoverAction.h"
#include <iostream>

LeaveCoverAction::LeaveCoverAction()
{
	name = "Leave Cover Action";
}

LeaveCoverAction::~LeaveCoverAction()
= default;

void LeaveCoverAction::Action()
{
	std::cout << "Performing Leave Cover Action" << std::endl;
}
