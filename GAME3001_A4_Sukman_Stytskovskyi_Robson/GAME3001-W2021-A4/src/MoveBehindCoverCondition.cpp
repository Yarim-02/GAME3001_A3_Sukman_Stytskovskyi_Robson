#include "MoveBehindCoverCondition.h"

MoveBehindCoverCondition::MoveBehindCoverCondition(const bool hit)
{
	setGotHit(hit);
	name = "Move Behind Cover Condition";
}

MoveBehindCoverCondition::~MoveBehindCoverCondition()
= default;

void MoveBehindCoverCondition::setGotHit(const bool state)
{
	m_GotHit = state;
	data = state;
}

bool MoveBehindCoverCondition::Condition()
{
	return m_GotHit;
}