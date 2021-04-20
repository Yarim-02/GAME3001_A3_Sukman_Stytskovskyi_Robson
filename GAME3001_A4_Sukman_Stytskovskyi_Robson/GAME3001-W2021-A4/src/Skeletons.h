#pragma once
#ifndef __Skeletons__
#define __Skeletons__

#include "TextureManager.h"
#include <glm/vec4.hpp>
#include "HealthBar.h"
#include "Bone.h"
#include "Agent.h"

enum TYPE {BASE, RANGE_COMBAT, CLOSE_COMBAT};

class Skeleton : public Agent
{

public:
	Skeleton();
	~Skeleton();

	// Inherited via GameObject
	void draw() override;
	void update() override;
	void clean() override;

	void turnRight();
	void turnLeft();
	void moveForward();
	void moveBack();

	void move();

	// getters
	float getMaxSpeed() const;
	std::string getAnimationState();
	std::string getCurrentAction();
	bool getHadLOS();
	bool getTakingDamage();
	HealthBar& getHealthBar();
	TYPE getVariation();

	// setters
	void setMaxSpeed(float newSpeed);
	void flipDbg();
	void flipTakingDamage();
	void setTakingDamage(bool state);
	void setHadLOS(bool b);
	void setAnimationState(std::string animationState);
	void setCurrentAction(std::string currentAction);
	void m_Seek(Agent* target);
	void setArrivalPoint(glm::vec2 point);

protected:
	void m_checkBounds();
	void m_reset();

	float m_maxSpeed;
	float m_turnRate;

	bool m_dbgMode;
	bool m_takingDamage = false;

	std::string m_currentAction;
	int m_frameCounter = 0;
	int m_frameCounter2 = 0;
	int m_GotHitCounter = 0;
	std::string m_animationState;

	TYPE m_variation;

	HealthBar m_healthBar;
	glm::vec2 m_position;
	glm::vec2 m_arrivalPoint;
	SDL_Rect* m_healthBarDestRect;
	
	bool m_bHadLOS;
};

class SkeletonRanged : public Skeleton
{
public:
	SkeletonRanged();
	~SkeletonRanged();


	void setRangedCombatRange(bool state);
	bool getRangedCombatRange();

	void setWaitInCoverTimer(int seconds);
	int getWaintInCoverTimer();

	void setIsBehindCover(bool state);
	bool getIsBehindCover();

	void setIsTimerOut(bool state);
	bool getIsTimerOut();

	void performAttack(glm::vec2 target);
private:
	bool m_rangedCombatRange = false;
	bool m_isBehindCover = false;
	bool m_isTimerOut;
	int m_waitInCoverTimer;
	

	std::vector<Bone*> m_pBone;
};

class SkeletonClose : public Skeleton
{
public:
	SkeletonClose();
	~SkeletonClose();

	void setCloseCombatRange(bool state);
	bool getCloseCombatRange();
private:
	bool m_closeCombatRange = false;
};

#endif /* defined (__Skeletons__) */

