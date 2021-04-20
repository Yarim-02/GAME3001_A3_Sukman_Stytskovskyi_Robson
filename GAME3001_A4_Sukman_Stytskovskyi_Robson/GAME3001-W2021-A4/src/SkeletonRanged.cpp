#include "Skeletons.h"

#include "SoundManager.h"

SkeletonRanged::SkeletonRanged()
{
	m_variation = RANGE_COMBAT;

	m_healthBar.setHealthPoints(65);
	m_waitInCoverTimer = 0;
	m_timerNotOut = true;
	m_isBehindCover = false;
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

void SkeletonRanged::setWaitInCoverTimer(int seconds)
{
	m_waitInCoverTimer = seconds;
}

int SkeletonRanged::getWaintInCoverTimer()
{
	return m_waitInCoverTimer;
}

void SkeletonRanged::setIsBehindCover(bool state)
{
	m_isBehindCover = state;
}

bool SkeletonRanged::getIsBehindCover()
{
	return m_isBehindCover;
}

void SkeletonRanged::setTimerNotOut(bool state)
{
	m_timerNotOut = state;
}

bool SkeletonRanged::getTimerNotOut()
{
	return m_timerNotOut;
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
