#include "gunner.h"
#include "glm/gtx/string_cast.hpp"
#include "PlayScene.h"
#include "TextureManager.h"
#include "Util.h"

Gunner::Gunner() : m_maxSpeed(10.0f)
{
	TextureManager::Instance()->load("../Assets/textures/Player_1.png", "player_walking1");
	TextureManager::Instance()->load("../Assets/textures/Player_2.png", "player_walking2");

	auto size = TextureManager::Instance()->getTextureSize("player_walking1");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(400.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(SHIP);

	setCurrentHeading(0.0f);// current facing angle
	setCurrentDirection(glm::vec2(1.0f, 0.0f)); // facing right
	m_turnRate = 10.0f; // 5 degrees per frame
	m_maxSpeed = 5.0f;


	setLOSDistance(400.0f); // 5 ppf x 80 feet
	setLOSColour(glm::vec4(1, 0, 0, 1));

	setDetectionRadius(50.f);
	setDRColour((glm::vec4(1, 0, 0, 1)));

	m_dbgMode = false;
}


Gunner::~Gunner()
= default;

void Gunner::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the ship
	if (!m_isAnimating)
	{
		TextureManager::Instance()->draw("player_walking1", x, y, m_rotationAngle, 255, true);
	}
	else
	{
		if (m_frameCounter % 20 >= 0 && m_frameCounter % 20 <= 10)
			TextureManager::Instance()->draw("player_walking1", x, y, m_rotationAngle, 255, true);
		else
			TextureManager::Instance()->draw("player_walking2", x, y, m_rotationAngle, 255, true);
	}

	if (m_dbgMode)
	{
		// draw LOS
		Util::DrawLine(getTransform()->position, m_destination, getLOSColour());
		// draw Detection Radius
		Util::DrawCircle(getTransform()->position, getDetectionRadius(), getDRColour());
	}
}


void Gunner::update()
{
	/*move();
	m_checkBounds();*/

	m_Turn();

	m_frameCounter++;

	if (m_frameCounter >= 1000)
		m_frameCounter = 0;


}

void Gunner::clean()
{
}

void Gunner::turnRight()
{
	setCurrentHeading(getCurrentHeading() + m_turnRate);
	if (getCurrentHeading() >= 360)
	{
		setCurrentHeading(getCurrentHeading() - 360.0f);
	}
}

void Gunner::turnLeft()
{
	setCurrentHeading(getCurrentHeading() - m_turnRate);
	if (getCurrentHeading() < 0)
	{
		setCurrentHeading(getCurrentHeading() + 360.0f);
	}
}

void Gunner::moveForward()
{
	getRigidBody()->velocity = getCurrentDirection() * m_maxSpeed;
}

void Gunner::moveBack()
{
	getRigidBody()->velocity = getCurrentDirection() * -m_maxSpeed;
}

void Gunner::move()
{
	getTransform()->position += getRigidBody()->velocity;
	getRigidBody()->velocity *= 0.9f;
}

void Gunner::setDestination(const glm::vec2 destination)
{

	m_destination = destination;

}

void Gunner::setAnimating(bool state)
{
	m_isAnimating = state;
}

glm::vec2 Gunner::getOrientation() const
{
	return m_orientation;
}

void Gunner::setRotation(float angle)
{
	m_rotationAngle = angle;

	auto offset = -90.0f;
	auto angle_in_radians = (angle + offset) * Util::Deg2Rad;

	auto x = cos(angle_in_radians);
	auto y = sin(angle_in_radians);

	// convert the angle to a normalized vector and store it in Orientation
	setOrientation(glm::vec2(x, y));
}

float Gunner::getRotation()
{
	return m_rotationAngle;
}

void Gunner::setOrientation(const glm::vec2 orientation)
{
	m_orientation = orientation;
}

float Gunner::getMaxSpeed() const
{
	return m_maxSpeed;
}

void Gunner::setMaxSpeed(float newSpeed)
{
	m_maxSpeed = newSpeed;
}

void Gunner::flipDbg()
{
	m_dbgMode = !m_dbgMode;
}

void Gunner::m_checkBounds()
{

	if (getTransform()->position.x > Config::SCREEN_WIDTH)
	{
		getTransform()->position = glm::vec2(0.0f, getTransform()->position.y);
	}

	if (getTransform()->position.x < 0)
	{
		getTransform()->position = glm::vec2(800.0f, getTransform()->position.y);
	}

	if (getTransform()->position.y > Config::SCREEN_HEIGHT)
	{
		getTransform()->position = glm::vec2(getTransform()->position.x, 0.0f);
	}

	if (getTransform()->position.y < 0)
	{
		getTransform()->position = glm::vec2(getTransform()->position.x, 600.0f);
	}

}

void Gunner::m_reset()
{
	getRigidBody()->isColliding = false;
	const int halfWidth = getWidth() * 0.5f;
	const auto xComponent = rand() % (640 - getWidth()) + halfWidth + 1;
	const auto yComponent = -getHeight();
	getTransform()->position = glm::vec2(xComponent, yComponent);
}

void Gunner::m_Turn()
{
	// direction with magnitude
	m_targetDirection = Gunner::m_destination - getTransform()->position;


	// normalized direction
	m_targetDirection = Util::normalize(m_targetDirection);
	//std::cout << m_targetDirection.x << m_targetDirection.y << std::endl;
	std::cout << m_rotationAngle << std::endl;
	auto targetRotation = (Util::signedAngle(getOrientation(), m_targetDirection));
	auto turnSensitivity = 5.0f;
	if (abs(targetRotation) > turnSensitivity)
	{
		if (targetRotation > 0.0f)
		{
			setRotation(getRotation() + m_turnRate);
		}
		else if (targetRotation < 0.0f)
		{
			setRotation(getRotation() - m_turnRate);
		}
	}
}

