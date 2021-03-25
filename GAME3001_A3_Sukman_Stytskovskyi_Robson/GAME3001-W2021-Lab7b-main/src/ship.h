#pragma once
#ifndef __SHIP__
#define __SHIP__

#include "TextureManager.h"
#include <glm/vec4.hpp>

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
	
	// setters
	void setMaxSpeed(float newSpeed);
	void flipDbg();

private:
	void m_checkBounds();
	void m_reset();

	float m_maxSpeed;
	float m_turnRate;

	bool m_dbgMode;

	std::string m_currentAction;
	int m_frameCounter = 0;
};


#endif /* defined (__SHIP__) */

