#include "MoveToRangeAction.h"
#include <iostream>

MoveToRangeAction::MoveToRangeAction()
{
	name = "Move To Range Action";
}

MoveToRangeAction::~MoveToRangeAction()
= default;

void MoveToRangeAction::Action()
{
	std::cout << "Performing Move To Range Action" << std::endl;
}
