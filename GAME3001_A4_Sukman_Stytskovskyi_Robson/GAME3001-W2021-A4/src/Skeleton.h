#pragma once
#ifndef __Skeleton__
#define __Skeleton__

#include "TextureManager.h"
#include <glm/vec4.hpp>
#include "HealthBar.h"

#include "Agent.h"

class Skeleton final : public Agent
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
	bool getTakingDamage();
	HealthBar& getHealthBar();

	// setters
	void setMaxSpeed(float newSpeed);
	void flipDbg();
	void flipTakingDamage();
	void setAnimationState(std::string animationState);
	void setCurrentAction(std::string currentAction);

private:
	void m_checkBounds();
	void m_reset();

	float m_maxSpeed;
	float m_turnRate;

	bool m_dbgMode;
	bool m_takingDamage = false;

	std::string m_currentAction;
	int m_frameCounter = 0;
	int m_frameCounter2 = 0;
	std::string m_animationState;

	HealthBar m_healthBar;
	glm::vec2 m_position;
	SDL_Rect* m_healthBarDestRect;


};




#endif /* defined (__Skeleton__) */

