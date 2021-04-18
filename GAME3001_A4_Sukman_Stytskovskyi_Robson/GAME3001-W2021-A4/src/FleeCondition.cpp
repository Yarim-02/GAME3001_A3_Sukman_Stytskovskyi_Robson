#include "FleeCondition.h"

FleeCondition::FleeCondition(const bool LOS)
{
	setFleeing(LOS);
	name = "Flee Condition";
}

FleeCondition::~FleeCondition()
= default;

void FleeCondition::setFleeing(const bool state)
{
	m_isFleeing = state;
	data = state;
}

bool FleeCondition::Condition()
{
	return m_isFleeing;
}
