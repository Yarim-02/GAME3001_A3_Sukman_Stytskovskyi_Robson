#include "Skeleton.h"
#include "glm/gtx/string_cast.hpp"
#include "PlayScene.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "Util.h"

Skeleton::Skeleton() : m_maxSpeed(10.0f)
{

	TextureManager::Instance()->load("../Assets/textures/IdleSkeleton1.png", "Skeleton");
	TextureManager::Instance()->load("../Assets/textures/IdleSkeleton1.png", "IdleSkeleton1");
	TextureManager::Instance()->load("../Assets/textures/IdleSkeleton2.png", "IdleSkeleton2");
	TextureManager::Instance()->load("../Assets/textures/IdleSkeleton3.png", "IdleSkeleton3");
	TextureManager::Instance()->load("../Assets/textures/RightWalkingSkeleton1.png", "RightWalkingSkeleton1");
	TextureManager::Instance()->load("../Assets/textures/RightWalkingSkeleton2.png", "RightWalkingSkeleton2");
	TextureManager::Instance()->load("../Assets/textures/RightWalkingSkeleton3.png", "RightWalkingSkeleton3");
	TextureManager::Instance()->load("../Assets/textures/LeftWalkingSkeleton1.png", "LeftWalkingSkeleton1");
	TextureManager::Instance()->load("../Assets/textures/LeftWalkingSkeleton2.png", "LeftWalkingSkeleton2");
	TextureManager::Instance()->load("../Assets/textures/LeftWalkingSkeleton3.png", "LeftWalkingSkeleton3");
	TextureManager::Instance()->load("../Assets/textures/BackwardSkeleton1.png", "BackwardSkeleton1");
	TextureManager::Instance()->load("../Assets/textures/BackwardSkeleton2.png", "BackwardSkeleton2");
	TextureManager::Instance()->load("../Assets/textures/BackwardSkeleton3.png", "BackwardSkeleton3");
	TextureManager::Instance()->load("../Assets/textures/BSdamage1.png", "BSdamage1");
	TextureManager::Instance()->load("../Assets/textures/BSdamage2.png", "BSdamage2");
	TextureManager::Instance()->load("../Assets/textures/BSdamage3.png", "BSdamage3");
	TextureManager::Instance()->load("../Assets/textures/FSdamage1.png", "FSdamage1");
	TextureManager::Instance()->load("../Assets/textures/FSdamage2.png", "FSdamage2");
	TextureManager::Instance()->load("../Assets/textures/FSdamage3.png", "FSdamage3");
	TextureManager::Instance()->load("../Assets/textures/RWSdamage1.png", "RWSdamage1");
	TextureManager::Instance()->load("../Assets/textures/RWSdamage2.png", "RWSdamage2");
	TextureManager::Instance()->load("../Assets/textures/RWSdamage3.png", "RWSdamage3");
	TextureManager::Instance()->load("../Assets/textures/LWSdamage1.png", "LWSdamage1");
	TextureManager::Instance()->load("../Assets/textures/LWSdamage2.png", "LWSdamage2");
	TextureManager::Instance()->load("../Assets/textures/LWSdamage3.png", "LWSdamage3");
	TextureManager::Instance()->load("../Assets/textures/DyingSkeleton1.png", "DyingSkeleton1");
	TextureManager::Instance()->load("../Assets/textures/DyingSkeleton2.png", "DyingSkeleton2");
	TextureManager::Instance()->load("../Assets/textures/DyingSkeleton3.png", "DyingSkeleton3");

	SoundManager::Instance().allocateChannels(6);
	SoundManager::Instance().load("../Assets/audio/skeleton_walk.ogg", "skeleton_walk", SOUND_SFX);
	SoundManager::Instance().setSoundVolume(60);

	auto size = TextureManager::Instance()->getTextureSize("Skeleton");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(400.0f, 300.0f);
	m_position = getTransform()->position;
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(SHIP);

	setCurrentHeading(0.0f);// current facing angle
	setCurrentDirection(glm::vec2(1.0f, 0.0f)); // facing right
	m_turnRate = 90.0f; // 90 degrees per frame

	setLOSDistance(400.0f); // 5 ppf x 80 feet
	setLOSColour(glm::vec4(1, 0, 0, 1));

	setDetectionRadius(50.f);
	setDRColour((glm::vec4(1, 0, 0, 1)));

	m_dbgMode = false;
	m_currentAction = "Patrol";

	m_healthBarDestRect = new SDL_Rect;
	m_healthBarDestRect->x = getTransform()->position.x + (size.x / 2) - 50;
	m_healthBarDestRect->y = getTransform()->position.y + 8;
	m_healthBarDestRect->w = m_healthBar.getHealthPoints();
	m_healthBarDestRect->h = 4;
}


Skeleton::~Skeleton()
= default;

void Skeleton::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;


	if (m_currentAction == "Patrol" || m_currentAction == "Wandering" || m_currentAction == "Taking Damage")
	{
		if (m_animationState == "WalkingDown")
		{
			if (m_frameCounter < 5)
			{
				TextureManager::Instance()->draw("IdleSkeleton1",
					x, y, 0.0, 255, true);
			}
			else if (m_frameCounter < 10)
			{
				TextureManager::Instance()->draw("IdleSkeleton2",
					x, y, 0.0, 255, true);
			}
			else if (m_frameCounter < 15)
			{
				TextureManager::Instance()->draw("IdleSkeleton3",
					x, y, 0.0, 255, true);
			}
		}
		if (m_animationState == "FWSdamage")
		{
			if (m_frameCounter < 5)
			{
				TextureManager::Instance()->draw("FSdamage1",
					x, y, 0.0, 255, true);
			}
			else if (m_frameCounter < 10)
			{
				TextureManager::Instance()->draw("FSdamage2",
					x, y, 0.0, 255, true);
			}
			else if (m_frameCounter < 15)
			{
				TextureManager::Instance()->draw("FSdamage3",
					x, y, 0.0, 255, true);

			}
		}
		if (m_animationState == "WalkingRight")
		{
			if (m_frameCounter < 5)
			{
				TextureManager::Instance()->draw("RightWalkingSkeleton1",
					x, y, 0.0, 255, true);
			}
			else if (m_frameCounter < 10)
			{
				TextureManager::Instance()->draw("RightWalkingSkeleton2",
					x, y, 0.0, 255, true);
			}
			else if (m_frameCounter < 15)
			{
				TextureManager::Instance()->draw("RightWalkingSkeleton3",
					x, y, 0.0, 255, true);

			}
		}
		if (m_animationState == "RWSdamage")
		{
			if (m_frameCounter < 5)
			{
				TextureManager::Instance()->draw("RWSdamage1",
					x, y, 0.0, 255, true);
			}
			else if (m_frameCounter < 10)
			{
				TextureManager::Instance()->draw("RWSdamage2",
					x, y, 0.0, 255, true);
			}
			else if (m_frameCounter < 15)
			{
				TextureManager::Instance()->draw("RWSdamage3",
					x, y, 0.0, 255, true);

			}
		}
		if (m_animationState == "WalkingLeft")
		{
			if (m_frameCounter < 5)
			{
				TextureManager::Instance()->draw("LeftWalkingSkeleton1",
					x, y, 0.0, 255, true);
			}
			else if (m_frameCounter < 10)
			{
				TextureManager::Instance()->draw("LeftWalkingSkeleton2",
					x, y, 0.0, 255, true);
			}
			else if (m_frameCounter < 15)
			{
				TextureManager::Instance()->draw("LeftWalkingSkeleton3",
					x, y, 0.0, 255, true);

			}
		}
		if (m_animationState == "LWSdamage")
		{
			if (m_frameCounter < 5)
			{
				TextureManager::Instance()->draw("LWSdamage1",
					x, y, 0.0, 255, true);
			}
			else if (m_frameCounter < 10)
			{
				TextureManager::Instance()->draw("LWSdamage2",
					x, y, 0.0, 255, true);
			}
			else if (m_frameCounter < 15)
			{
				TextureManager::Instance()->draw("LWSdamage3",
					x, y, 0.0, 255, true);

			}
		}
		if (m_animationState == "WalkingUp")
		{
			if (m_frameCounter < 5)
			{
				TextureManager::Instance()->draw("BackwardSkeleton1",
					x, y, 0.0, 255, true);
			}
			else if (m_frameCounter < 10)
			{
				TextureManager::Instance()->draw("BackwardSkeleton2",
					x, y, 0.0, 255, true);
			}
			else if (m_frameCounter < 15)
			{
				TextureManager::Instance()->draw("BackwardSkeleton3",
					x, y, 0.0, 255, true);

			}
		}
		if (m_animationState == "BWSdamage")
		{
			if (m_frameCounter < 5)
			{
				TextureManager::Instance()->draw("BSdamage1",
					x, y, 0.0, 255, true);
			}
			else if (m_frameCounter < 10)
			{
				TextureManager::Instance()->draw("BSdamage2",
					x, y, 0.0, 255, true);
			}
			else if (m_frameCounter < 15)
			{
				TextureManager::Instance()->draw("BSdamage3",
					x, y, 0.0, 255, true);

			}
		}
	}
	else if (m_currentAction == "Dying")
	{
		if (m_frameCounter < 5)
		{
			TextureManager::Instance()->draw("DyingSkeleton1",
				x, y, 0.0, 255, true);
		}
		else if (m_frameCounter < 10)
		{
			TextureManager::Instance()->draw("DyingSkeleton2",
				x, y, 0.0, 255, true);
		}
		else if (m_frameCounter < 15)
		{
			TextureManager::Instance()->draw("DyingSkeleton3",
				x, y, 0.0, 255, true);
		}
	}
	else if (m_currentAction == "Dead")
	{
	}
	else
	{
		if (m_frameCounter < 5)
		{
			TextureManager::Instance()->draw("IdleSkeleton1",
				x, y, 0.0, 255, true);
		}
		else if (m_frameCounter < 10)
		{
			TextureManager::Instance()->draw("IdleSkeleton2",
				x, y, 0.0, 255, true);
		}
		else if (m_frameCounter < 15)
		{
			TextureManager::Instance()->draw("IdleSkeleton3",
				x, y, 0.0, 255, true);
		}
	}


	if (m_dbgMode)
	{
		// draw LOS
		Util::DrawLine(getTransform()->position, getTransform()->position + getCurrentDirection() * getLOSDistance(), getLOSColour());
		// draw Detection Radius
		Util::DrawCircle(getTransform()->position, getDetectionRadius(), getDRColour());
	}

	m_healthBar.draw();
}


void Skeleton::update()
{
	auto size = TextureManager::Instance()->getTextureSize("Skeleton");
	/*move();
	m_checkBounds();*/
	if (m_frameCounter > 13)
		m_frameCounter = 0;
	if (m_frameCounter2 > 600)
		m_frameCounter2 = 0;

	if (m_frameCounter2 % 60 == 0 && (m_currentAction == "Patrol" || m_currentAction == "Wandering"))
	{
		SoundManager::Instance().playSound("skeleton_walk", 0, -1);
	}

	m_frameCounter2++;
	m_frameCounter++;
	m_position = getTransform()->position;

	m_healthBarDestRect->x = getTransform()->position.x + (size.x / 2) - 67.5f;
	m_healthBarDestRect->y = getTransform()->position.y - (size.y / 1.5);
	m_healthBarDestRect->h = 4;
	m_healthBarDestRect->w = m_healthBar.getHealthPoints();
	m_healthBar.setDest(m_healthBarDestRect);

}

void Skeleton::clean()
{
}

void Skeleton::turnRight()
{
	setCurrentHeading(getCurrentHeading() + m_turnRate);
	if (getCurrentHeading() >= 360)
	{
		setCurrentHeading(getCurrentHeading() - 360.0f);
	}
}

void Skeleton::turnLeft()
{
	setCurrentHeading(getCurrentHeading() - m_turnRate);
	if (getCurrentHeading() < 0)
	{
		setCurrentHeading(getCurrentHeading() + 360.0f);
	}
}

void Skeleton::moveForward()
{
	getRigidBody()->velocity = getCurrentDirection() * m_maxSpeed;
}

void Skeleton::moveBack()
{
	getRigidBody()->velocity = getCurrentDirection() * -m_maxSpeed;
}

void Skeleton::move()
{
	getTransform()->position += getRigidBody()->velocity;
	//getRigidBody()->velocity *= 0.9f;
}

float Skeleton::getMaxSpeed() const
{
	return m_maxSpeed;
}

std::string Skeleton::getAnimationState()
{
	return m_animationState;
}

std::string Skeleton::getCurrentAction()
{
	return m_currentAction;
}

bool Skeleton::getTakingDamage()
{
	return m_takingDamage;
}

HealthBar& Skeleton::getHealthBar()
{
	return m_healthBar;
}

void Skeleton::setMaxSpeed(float newSpeed)
{
	m_maxSpeed = newSpeed;
}

void Skeleton::flipDbg()
{
	m_dbgMode = !m_dbgMode;
}

void Skeleton::flipTakingDamage()
{
	m_takingDamage = !m_takingDamage;
}

void Skeleton::setAnimationState(std::string animationState)
{
	if (getCurrentAction() != "Taking Damage" && getCurrentAction() != "Dying")
	{
		m_animationState = animationState;
	}
}

void Skeleton::setCurrentAction(std::string currentAction)
{
	m_currentAction = currentAction;
}

void Skeleton::m_checkBounds()
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

void Skeleton::m_reset()
{
	getRigidBody()->isColliding = false;
	const int halfWidth = getWidth() * 0.5f;
	const auto xComponent = rand() % (640 - getWidth()) + halfWidth + 1;
	const auto yComponent = -getHeight();
	getTransform()->position = glm::vec2(xComponent, yComponent);
}
