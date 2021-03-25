#include "ship.h"
#include "glm/gtx/string_cast.hpp"
#include "PlayScene.h"
#include "TextureManager.h"
#include "Util.h"

Ship::Ship() : m_maxSpeed(10.0f)
{
	TextureManager::Instance()->load("../Assets/textures/IdleSkeleton1.png","Skeleton");
	TextureManager::Instance()->load("../Assets/textures/RightWalkingSkeleton1.png", "RightWalkingSkeleton1");
	TextureManager::Instance()->load("../Assets/textures/RightWalkingSkeleton2.png", "RightWalkingSkeleton2");
	TextureManager::Instance()->load("../Assets/textures/RightWalkingSkeleton3.png", "RightWalkingSkeleton3");

	auto size = TextureManager::Instance()->getTextureSize("Skeleton");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(400.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(SHIP);

	setCurrentHeading(0.0f);// current facing angle
	setCurrentDirection(glm::vec2(1.0f, 0.0f)); // facing right
	m_turnRate = 5.0f; // 5 degrees per frame

	setLOSDistance(400.0f); // 5 ppf x 80 feet
	setLOSColour(glm::vec4(1, 0, 0, 1));

	setDetectionRadius(50.f);
	setDRColour((glm::vec4(1, 0, 0, 1)));

	m_dbgMode = false;
	m_currentAction = "Patrol";
}


Ship::~Ship()
= default;

void Ship::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;


	if(m_currentAction == "Patrol")
	{
		if (m_frameCounter < 5)
		{
			TextureManager::Instance()->draw("RightWalkingSkeleton1",
				x, y, getCurrentHeading(), 255, true);
		}
		else if (m_frameCounter < 10)
		{
			TextureManager::Instance()->draw("RightWalkingSkeleton2",
				x, y, getCurrentHeading(), 255, true);
		}
		else if (m_frameCounter < 15)
		{
			TextureManager::Instance()->draw("RightWalkingSkeleton3",
				x, y, getCurrentHeading(), 255, true);
	
		}
	}
	else
	{
		TextureManager::Instance()->draw("Skeleton", x, y, getCurrentHeading(), 255, true);
	}
	
	if (m_dbgMode)
	{
		// draw LOS
		Util::DrawLine(getTransform()->position, getTransform()->position + getCurrentDirection() * getLOSDistance(), getLOSColour());
		// draw Detection Radius
		Util::DrawCircle(getTransform()->position, getDetectionRadius(), getDRColour());
	}
}


void Ship::update()
{
	/*move();
	m_checkBounds();*/
	if (m_frameCounter > 13)
		m_frameCounter = 0;

	m_frameCounter++;
}

void Ship::clean()
{
}

void Ship::turnRight()
{
	setCurrentHeading(getCurrentHeading() + m_turnRate);
	if (getCurrentHeading() >= 360) 
	{
		setCurrentHeading(getCurrentHeading() - 360.0f);
	}
}

void Ship::turnLeft()
{
	setCurrentHeading(getCurrentHeading() - m_turnRate);
	if (getCurrentHeading() < 0)
	{
		setCurrentHeading(getCurrentHeading() + 360.0f);
	}
}

void Ship::moveForward()
{
	getRigidBody()->velocity = getCurrentDirection() * m_maxSpeed;
}

void Ship::moveBack()
{
	getRigidBody()->velocity = getCurrentDirection() * -m_maxSpeed;
}

void Ship::move()
{
	getTransform()->position += getRigidBody()->velocity;
	getRigidBody()->velocity *= 0.9f;
}

float Ship::getMaxSpeed() const
{
	return m_maxSpeed;
}

void Ship::setMaxSpeed(float newSpeed)
{
	m_maxSpeed = newSpeed;
}

void Ship::flipDbg()
{
	m_dbgMode = !m_dbgMode;
}

void Ship::m_checkBounds()
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

void Ship::m_reset()
{
	getRigidBody()->isColliding = false;
	const int halfWidth = getWidth() * 0.5f;
	const auto xComponent = rand() % (640 - getWidth()) + halfWidth + 1;
	const auto yComponent = -getHeight();
	getTransform()->position = glm::vec2(xComponent, yComponent);
}

