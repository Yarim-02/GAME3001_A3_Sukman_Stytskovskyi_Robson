#pragma once
#ifndef __AGENT__
#define __AGENT__

#include <glm/vec4.hpp>
#include "DisplayObject.h"

class Agent : public DisplayObject
{
public:
	Agent();
	~Agent();

	// Inherited via GameObject
	void draw() override = 0;
	void update() override = 0;
	void clean() override = 0;

	// getters
	glm::vec2 getTargetPosition() const;
	glm::vec2 getCurrentDirection() const;
	float getLOSDistance() const;
	float getDetectionRadius() const;
	bool hasLOS() const;
	bool inDR() const;
	float getCurrentHeading() const;
	glm::vec4 getLOSColour() const;
	glm::vec4 getDRColour() const;
	bool getGoingRight();

	// setters
	void setTargetPosition(glm::vec2 new_position);
	void setCurrentDirection(glm::vec2 new_direction);
	void setLOSDistance(float distance);
	void setDetectionRadius(float radius);
	void setHasLOS(bool state);
	void setInDR(bool state);
	void setCurrentHeading(float heading);
	void setLOSColour(glm::vec4 colour);
	void setDRColour(glm::vec4 colour);
	void setGoingRight(bool b);

private:
	void m_changeDirection();
	float m_currentHeading;
	glm::vec2 m_currentDirection;
	glm::vec2 m_targetPosition;

	// LOS
	float m_LOSDistance;
	float m_DetectionRadius;
	bool m_hasLOS;
	bool m_inDR;
	bool m_bGoingRight;
	glm::vec4 m_LOSColour;
	glm::vec4 m_DRColour;
};



#endif /* defined ( __AGENT__) */