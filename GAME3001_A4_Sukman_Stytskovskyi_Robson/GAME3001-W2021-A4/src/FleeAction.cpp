#include "FleeAction.h"
#include <iostream>

FleeAction::FleeAction()
{
	name = "Flee Action";
}

FleeAction::~FleeAction()
= default;

void FleeAction::Action()
{
	std::cout << "Performing Flee Action" << std::endl;
}
