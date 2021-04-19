#include "Skeletons.h"


SkeletonClose::SkeletonClose()
{
	m_variation = CLOSE_COMBAT;

	m_healthBar.setHealthPoints(135);
	setMaxSpeed(0.55f);
}

SkeletonClose::~SkeletonClose()
= default;

void SkeletonClose::setCloseCombatRange(bool state)
{
	m_closeCombatRange = state;
}

bool SkeletonClose::getCloseCombatRange()
{
	return m_closeCombatRange;
}

