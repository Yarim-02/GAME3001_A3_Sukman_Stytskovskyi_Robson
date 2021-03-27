#pragma once
#ifndef __SHIP__
#define __SHIP__

#include "TextureManager.h"
#include <glm/vec4.hpp>
#include "HealthBar.h"

#include "Agent.h"

class Ship final : public Agent
{
	
public:
	Ship();
	~Ship();

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
	
	// setters
	void setMaxSpeed(float newSpeed);
	void flipDbg();
	void setAnimationState(std::string animationState);
	void setCurrentAction(std::string currentAction);

private:
	void m_checkBounds();
	void m_reset();

	float m_maxSpeed;
	float m_turnRate;

	bool m_dbgMode;

	std::string m_currentAction;
	int m_frameCounter = 0;
	std::string m_animationState;

	HealthBar m_healthBar;
	glm::vec2 m_position;
	SDL_Rect* m_healthBarDestRect;
	
};




#endif /* defined (__SHIP__) */

