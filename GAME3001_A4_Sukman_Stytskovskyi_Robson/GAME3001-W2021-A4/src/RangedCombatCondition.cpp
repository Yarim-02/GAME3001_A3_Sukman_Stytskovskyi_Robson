#include "RangedCombatCondition.h"

RangedCombatCondition::RangedCombatCondition(const bool within_combat_range)
{
	setIsWithinCombatRange(within_combat_range);
	name = "Ranged Combat Range Condition";
}

RangedCombatCondition::~RangedCombatCondition()
= default;

void RangedCombatCondition::setIsWithinCombatRange(const bool state)
{
	m_isWithinCombatRange = state;
	data = state;
}

bool RangedCombatCondition::Condition()
{
	return m_isWithinCombatRange;
}