#pragma once
#ifndef __BULLET__
#define __BULLET__
#include "DisplayObject.h"

class Bullet : public DisplayObject
{
public:
	// constructor
	Bullet(glm::vec2 position,glm::vec2 direction);

	//destructor
	~Bullet();

	// life cycle functions
	void draw();
	void update();
	void clean();

	// Setters and Getters
	void setDestination(glm::vec2 destination);
	void setMaxSpeed(float speed);
	glm::vec2 getOrientation() const;
	void setOrientation(glm::vec2 orientation);
	float getRotation() const;
	void setRotation(float angle);
	float getTurnRate() const;
	void setTurnRate(float rate);
	float getAccelerationRate() const;
	void setAccelerationRate(float rate);
private:
	glm::vec2 m_destination;
	glm::vec2 m_targetDirection;
	glm::vec2 m_orientation;

	float m_rotationAngle;
	float m_maxSpeed;
	float m_turnRate;
	float m_accelerationRate;
};
#endif // __BULLET__