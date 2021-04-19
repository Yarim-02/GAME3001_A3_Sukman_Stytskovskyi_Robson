#include "Skeletons.h"

#include "SoundManager.h"

SkeletonRanged::SkeletonRanged()
{
	m_variation = RANGE_COMBAT;

	m_healthBar.setHealthPoints(65);
	setMaxSpeed(0.25f);
}

SkeletonRanged::~SkeletonRanged()
= default;

void SkeletonRanged::setRangedCombatRange(bool state)
{
	m_rangedCombatRange = state;
}

bool SkeletonRanged::getRangedCombatRange()
{
	return m_rangedCombatRange;
}

void SkeletonRanged::performAttack(glm::vec2 target)
{
	m_pBone.push_back(new Bone(getTransform()->position, target));

	for (int i = 0; i < m_pBone.size(); i++)
	{
		m_pBone[i]->draw();
		m_pBone[i]->update();
	}
	SoundManager::Instance().playSound("shoot_sound", 0, 1);
}
