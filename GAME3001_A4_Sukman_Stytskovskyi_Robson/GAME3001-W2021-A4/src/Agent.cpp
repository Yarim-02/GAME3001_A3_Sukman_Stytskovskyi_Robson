#include "Agent.h"

#include "Util.h"

Agent::Agent()
{
}

Agent::~Agent()
= default;

glm::vec2 Agent::getTargetPosition() const
{
	return m_targetPosition;
}

glm::vec2 Agent::getCurrentDirection() const
{
	return m_currentDirection;
}

float Agent::getLOSDistance() const
{
	return m_LOSDistance;
}

float Agent::getDetectionRadius() const
{
	return m_DetectionRadius;
}

bool Agent::hasLOS() const
{
	return m_hasLOS;
}

bool Agent::inDR() const
{
	return m_inDR;
}

float Agent::getCurrentHeading() const
{
	return m_currentHeading;
}

glm::vec4 Agent::getLOSColour() const
{
	return m_LOSColour;
}

glm::vec4 Agent::getDRColour() const
{
	return m_DRColour;
}

void Agent::setTargetPosition(const glm::vec2 new_position)
{
	m_targetPosition = new_position;
}

void Agent::setCurrentDirection(const glm::vec2 new_direction)
{
	m_currentDirection = new_direction;
}

void Agent::setLOSDistance(const float distance)
{
	m_LOSDistance = distance;
}

void Agent::setDetectionRadius(float radius)
{
	m_DetectionRadius = radius;
}

void Agent::setHasLOS(const bool state)
{
	m_hasLOS = state;
	m_LOSColour = (m_hasLOS) ? glm::vec4(0, 1, 0, 1) : glm::vec4(1, 0, 0, 1);
}

void Agent::setInDR(const bool state)
{
	m_inDR = state;
	m_DRColour = (m_inDR) ? glm::vec4(0, 1, 0, 1) : glm::vec4(1, 0, 0, 1);
}

void Agent::setCurrentHeading(const float heading)
{
	m_currentHeading = heading;
	m_changeDirection();
}

void Agent::setLOSColour(const glm::vec4 colour)
{
	m_LOSColour = colour;
}

void Agent::setDRColour(glm::vec4 colour)
{
	m_DRColour = colour;
}

void Agent::m_changeDirection()
{
	const auto x = cos(m_currentHeading * Util::Deg2Rad);
	const auto y = sin(m_currentHeading * Util::Deg2Rad);
	m_currentDirection = glm::vec2(x, y);
}

void Agent::setGoingRight(bool b)
{
	m_bGoingRight = b;
}

void Agent::setFleeing(bool b)
{
	m_isFleeing = b;
}

bool Agent::getGoingRight()
{
	return m_bGoingRight;
}

bool Agent::getFleeing()
{
	return m_isFleeing;
}
