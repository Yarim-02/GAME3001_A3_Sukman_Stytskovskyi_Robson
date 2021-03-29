#pragma once
#ifndef __GUNNER__
#define __GUNNER__

#include "TextureManager.h"
#include <glm/vec4.hpp>
#include "HealthBar.h"
#include "Agent.h"

class Gunner final : public Agent
{
public:
	Gunner();
	~Gunner();

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
	glm::vec2 getOrientation() const;
	float getRotation();
	HealthBar& getHealthBar();

	// setters
	void setMaxSpeed(float newSpeed);
	void flipDbg();
	void setOrientation(glm::vec2 orientation);
	void setRotation(float angle);
	void setDestination(glm::vec2 destination);
	void setAnimating(bool state);

private:
	void m_checkBounds();
	void m_reset();

	glm::vec2 m_destination;
	glm::vec2 m_targetDirection;
	glm::vec2 m_orientation;

	float m_maxSpeed;
	float m_turnRate;
	float m_rotationAngle;

	bool m_dbgMode;
	bool m_isAnimating;

	int m_frameCounter;

	void m_Turn();

	HealthBar m_healthBar;
	SDL_Rect* m_healthBarDestRect;
};


#endif /* defined (__SHIP__) */