#include "Melee.h"
#include "Game.h"
#include "Util.h"
#include "TextureManager.h"

Melee::Melee(glm::vec2 position, glm::vec2 direction)
{
	TextureManager::Instance()->load("../Assets/textures/bullet.png", "bullet");

	auto size = TextureManager::Instance()->getTextureSize("bullet");
	setWidth(size.x);
	setHeight(size.y);

	setDestination(direction);
	getTransform()->position = position;
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);

	setOrientation(direction);
	setRotation(Util::signedAngle(getOrientation(), m_targetDirection));
	setMaxSpeed(5.0f);
	setAccelerationRate(5.0f);
	setTurnRate(10.0f);

	setOrientation(m_destination - getTransform()->position);// muy importanto, amigos!
}

Melee::~Melee()
= default;

void Melee::draw()
{
	TextureManager::Instance()->draw("bullet", getTransform()->position.x, getTransform()->position.y);
}

void Melee::update()
{
	auto deltaTime = TheGame::Instance()->getDeltaTime();
	
	getRigidBody()->acceleration = getOrientation() * getAccelerationRate();

	// using the formula pf = pi + vi*t + 0.5ai*t^2
	getRigidBody()->velocity += getOrientation() * (deltaTime)+
		0.5f * getRigidBody()->acceleration * (deltaTime);

	getRigidBody()->velocity = Util::clamp(getRigidBody()->velocity, m_maxSpeed);

	getTransform()->position += getRigidBody()->velocity;
}

void Melee::setDestination(const glm::vec2 destination)
{
	m_destination = destination;
}

void Melee::setMaxSpeed(const float speed)
{
	m_maxSpeed = speed;
}

glm::vec2 Melee::getOrientation() const
{
	return m_orientation;
}

void Melee::setOrientation(const glm::vec2 orientation)
{
	m_orientation = orientation;
}

float Melee::getTurnRate() const
{
	return m_turnRate;
}

void Melee::setTurnRate(const float rate)
{
	m_turnRate = rate;
}

void Melee::setRotation(const float angle)
{
	m_rotationAngle = angle;

	const auto offset = -90.0f;
	const auto angle_in_radians = (angle + offset) * Util::Deg2Rad;

	const auto x = cos(angle_in_radians);
	const auto y = sin(angle_in_radians);

	// convert the angle to a normalized vector and store it in Orientation
	setOrientation(glm::vec2(x, y));
}

float Melee::getAccelerationRate() const
{
	return m_accelerationRate;
}

void Melee::setAccelerationRate(const float rate)
{
	m_accelerationRate = rate;
}

float Melee::getRotation() const
{
	return m_rotationAngle;
}



void Melee::clean()
{
}
