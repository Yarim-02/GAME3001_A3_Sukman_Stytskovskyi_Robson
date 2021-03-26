#include "Bullet.h"
#include "Game.h"
#include "Util.h"
#include "TextureManager.h"

Bullet::Bullet(glm::vec2 position, glm::vec2 direction)
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
	setMaxSpeed(55.0f);
	setAccelerationRate(55.0f);
	setTurnRate(10.0f);
}

Bullet::~Bullet()
{
}

void Bullet::draw()
{
	TextureManager::Instance()->draw("bullet", getTransform()->position.x, getTransform()->position.y);
}

void Bullet::update()
{
	auto deltaTime = TheGame::Instance()->getDeltaTime();

	// direction with magnitude
	m_targetDirection = m_destination - getTransform()->position;

	// normalized direction
	m_targetDirection = Util::normalize(m_targetDirection);

	auto target_rotation = Util::signedAngle(getOrientation(), m_targetDirection);

	auto turn_sensitivity = 0.0f;

	if (abs(target_rotation) > turn_sensitivity)
	{
		if (target_rotation > 0.0f)
		{
			setRotation(getRotation() + getTurnRate());
		}
		else if (target_rotation < 0.0f)
		{
			setRotation(getRotation() - getTurnRate());
		}
	}

	getRigidBody()->acceleration = getOrientation() * getAccelerationRate();

	// using the formula pf = pi + vi*t + 0.5ai*t^2
	getRigidBody()->velocity += getOrientation() * (deltaTime)+
		0.5f * getRigidBody()->acceleration * (deltaTime);

	getRigidBody()->velocity = Util::clamp(getRigidBody()->velocity, m_maxSpeed);

	getTransform()->position += getRigidBody()->velocity;
}

void Bullet::setDestination(const glm::vec2 destination)
{
	m_destination = destination;
}

void Bullet::setMaxSpeed(const float speed)
{
	m_maxSpeed = speed;
}

float Bullet::getTurnRate() const
{
	return m_turnRate;
}

void Bullet::setTurnRate(const float rate)
{
	m_turnRate = rate;
}

void Bullet::setRotation(const float angle)
{
	m_rotationAngle = angle;

	const auto offset = -90.0f;
	const auto angle_in_radians = (angle + offset) * Util::Deg2Rad;

	const auto x = cos(angle_in_radians);
	const auto y = sin(angle_in_radians);

	// convert the angle to a normalized vector and store it in Orientation
	setOrientation(glm::vec2(x, y));
}

float Bullet::getAccelerationRate() const
{
	return m_accelerationRate;
}

void Bullet::setAccelerationRate(const float rate)
{
	m_accelerationRate = rate;
}

float Bullet::getRotation() const
{
	return m_rotationAngle;
}

glm::vec2 Bullet::getOrientation() const
{
	return m_orientation;
}

void Bullet::setOrientation(const glm::vec2 orientation)
{
	m_orientation = orientation;
}

void Bullet::clean()
{
}
