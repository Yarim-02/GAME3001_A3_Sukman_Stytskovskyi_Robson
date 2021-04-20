#include "IsBehindCoverCondition.h"

IsBehindCoverCondition::IsBehindCoverCondition(bool state)
{
	setBehindCover(state);
	name = "Is Behind Cover Condition";
}

IsBehindCoverCondition::~IsBehindCoverCondition()
= default;

void IsBehindCoverCondition::setBehindCover(bool state)
{
	m_isBehindCover = state;
	data = state;
}

bool IsBehindCoverCondition::Condition()
{
	return m_isBehindCover;
}
