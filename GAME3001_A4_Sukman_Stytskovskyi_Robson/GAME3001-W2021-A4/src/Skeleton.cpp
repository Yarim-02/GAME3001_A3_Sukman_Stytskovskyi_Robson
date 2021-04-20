#include "Skeletons.h"

#include "Game.h"
#include "glm/gtx/string_cast.hpp"
#include "PlayScene.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "Util.h"

Skeleton::Skeleton() : m_maxSpeed(20.0f)
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

	TextureManager::Instance()->load("../Assets/textures/FRS1.png", "FRS1");
	TextureManager::Instance()->load("../Assets/textures/FRS2.png", "FRS2");
	TextureManager::Instance()->load("../Assets/textures/FRS3.png", "FRS3");
	TextureManager::Instance()->load("../Assets/textures/BRS1.png", "BRS1");
	TextureManager::Instance()->load("../Assets/textures/BRS2.png", "BRS2");
	TextureManager::Instance()->load("../Assets/textures/BRS3.png", "BRS3");
	TextureManager::Instance()->load("../Assets/textures/RWRS1.png", "RWRS1");
	TextureManager::Instance()->load("../Assets/textures/RWRS2.png", "RWRS2");
	TextureManager::Instance()->load("../Assets/textures/RWRS3.png", "RWRS3");
	TextureManager::Instance()->load("../Assets/textures/LWRS1.png", "LWRS1");
	TextureManager::Instance()->load("../Assets/textures/LWRS2.png", "LWRS2");
	TextureManager::Instance()->load("../Assets/textures/LWRS3.png", "LWRS3");


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
	setType(AGENT);

	m_variation = BASE;

	setCurrentHeading(0.0f);// current facing angle
	setCurrentDirection(glm::vec2(1.0f, 0.0f)); // facing right
	m_turnRate = 90.0f; // 90 degrees per frame

	setLOSDistance(400.0f); // 5 ppf x 80 feet
	setLOSColour(glm::vec4(1, 0, 0, 1));

	setDetectionRadius(150.f);
	setDRColour((glm::vec4(1, 0, 0, 1)));

	m_dbgMode = false;
	m_takingDamage = false;

	setFleeing(false);
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


	if (m_currentAction == "Patrol Action" || m_currentAction == "Wandering" || m_currentAction == "Taking Damage")
	{
		if (m_animationState == "WalkingDown")
		{
			if (m_variation == CLOSE_COMBAT)
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
			else if (m_variation == RANGE_COMBAT)
			{
				if (m_frameCounter < 5)
				{
					TextureManager::Instance()->draw("FRS1",
						x, y, 0.0, 255, true);
				}
				else if (m_frameCounter < 10)
				{
					TextureManager::Instance()->draw("FRS2",
						x, y, 0.0, 255, true);
				}
				else if (m_frameCounter < 15)
				{
					TextureManager::Instance()->draw("FRS3",
						x, y, 0.0, 255, true);
				}
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
			if (m_variation == CLOSE_COMBAT)
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
			else if (m_variation == RANGE_COMBAT)
			{
				if (m_frameCounter < 5)
				{
					TextureManager::Instance()->draw("RWRS1",
						x, y, 0.0, 255, true);
				}
				else if (m_frameCounter < 10)
				{
					TextureManager::Instance()->draw("RWRS2",
						x, y, 0.0, 255, true);
				}
				else if (m_frameCounter < 15)
				{
					TextureManager::Instance()->draw("RWRS3",
						x, y, 0.0, 255, true);

				}
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
			if (m_variation == CLOSE_COMBAT)
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
			else if (m_variation == RANGE_COMBAT)
			{
				if (m_frameCounter < 5)
				{
					TextureManager::Instance()->draw("LWRS1",
						x, y, 0.0, 255, true);
				}
				else if (m_frameCounter < 10)
				{
					TextureManager::Instance()->draw("LWRS2",
						x, y, 0.0, 255, true);
				}
				else if (m_frameCounter < 15)
				{
					TextureManager::Instance()->draw("LWRS3",
						x, y, 0.0, 255, true);

				}
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
			if (m_variation == CLOSE_COMBAT)
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
			else if (m_variation == RANGE_COMBAT)
			{
				if (m_frameCounter < 5)
				{
					TextureManager::Instance()->draw("BRS1",
						x, y, 0.0, 255, true);
				}
				else if (m_frameCounter < 10)
				{
					TextureManager::Instance()->draw("BRS2",
						x, y, 0.0, 255, true);
				}
				else if (m_frameCounter < 15)
				{
					TextureManager::Instance()->draw("BRS3",
						x, y, 0.0, 255, true);

				}
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
		if (m_variation == CLOSE_COMBAT)
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
		else if (m_variation == RANGE_COMBAT)
		{
			if (m_frameCounter < 5)
			{
				TextureManager::Instance()->draw("FRS1",
					x, y, 0.0, 255, true);
			}
			else if (m_frameCounter < 10)
			{
				TextureManager::Instance()->draw("FRS2",
					x, y, 0.0, 255, true);
			}
			else if (m_frameCounter < 15)
			{
				TextureManager::Instance()->draw("FRS3",
					x, y, 0.0, 255, true);
			}
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
	/*move();*/

	//if (m_GotHitCounter >= 0)
	//	m_GotHitCounter--;
	//
	//if (m_GotHitCounter <= 0)
	//	m_takingDamage = false;
	//
	//if (getTakingDamage() && m_GotHitCounter <= 0)
	//{ 
	//	m_GotHitCounter = 600;
	//}

	if (m_currentAction != "Flee Action")
		m_checkBounds();

	if (m_frameCounter > 13)
		m_frameCounter = 0;
	if (m_frameCounter2 > 600)
		m_frameCounter2 = 0;

	if (m_frameCounter2 % 60 == 0 && (getCurrentAction() == "Patrol Action" || getCurrentAction() == "Wandering" ||
		getCurrentAction() == "Taking Damage" || getCurrentAction() == "Move To Player Action" ||
		getCurrentAction() == "Flee Action"))
	{
		SoundManager::Instance().playSound("skeleton_walk", 0, -1);
	}
	auto distance = abs(abs(m_arrivalPoint.x - getTransform()->position.x) - abs(m_arrivalPoint.y - getTransform()->position.y));

	if (m_arrivalPoint.x > -10)
	{
		if (distance < 50)
		{
			setMaxSpeed(0.5f);
			//std::cout << distance << std::endl;
		}
		else
			setMaxSpeed(02.0f);
	}
	//std::cout << m_maxSpeed;

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

bool Skeleton::getHadLOS()
{
	return m_bHadLOS;
}

bool Skeleton::getTakingDamage()
{
	return m_takingDamage;
}

HealthBar& Skeleton::getHealthBar()
{
	return m_healthBar;
}

TYPE Skeleton::getVariation()
{
	return m_variation;
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

void Skeleton::setTakingDamage(bool state)
{
	m_takingDamage = state;
}

void Skeleton::setHadLOS(bool b)
{
	m_bHadLOS = b;
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

	if (getTransform()->position.x > Config::SCREEN_WIDTH - 40.f)
	{
		getTransform()->position = glm::vec2(760.0f, getTransform()->position.y);
	}

	if (getTransform()->position.x < 40.f)
	{
		getTransform()->position = glm::vec2(40.0f, getTransform()->position.y);
	}

	if (getTransform()->position.y > Config::SCREEN_HEIGHT +40.f)
	{
		getTransform()->position = glm::vec2(getTransform()->position.x, 640.0f);
	}

	if (getTransform()->position.y < 40.f)
	{
		getTransform()->position = glm::vec2(getTransform()->position.x, 40.0f);
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

void Skeleton::m_Seek(Agent* target)
{
	auto deltaTime = TheGame::Instance()->getDeltaTime();

	// direction with magnitude
	auto m_targetDirection = target->getTransform()->position - this->getTransform()->position;

	// normalized direction
	m_targetDirection = Util::normalize(m_targetDirection);

	auto target_rotation = Util::signedAngle(getCurrentDirection(), m_targetDirection);

	auto turn_sensitivity = 5.0f;

	if (abs(target_rotation) > turn_sensitivity)
	{
		if (target_rotation > 0.0f)
		{
			this->setCurrentHeading(getCurrentHeading() + 15.0f);
		}
		else if (target_rotation < 0.0f)
		{
			this->setCurrentHeading(getCurrentHeading() - 15.0f);
		}
	}

	getRigidBody()->acceleration = getCurrentDirection() * 5.0f;

	// using the formula pf = pi + vi*t + 0.5ai*t^2
	getRigidBody()->velocity += getCurrentDirection() * (deltaTime)+
		0.5f * getRigidBody()->acceleration * (deltaTime);

	getRigidBody()->velocity = Util::clamp(getRigidBody()->velocity, m_maxSpeed);

	getTransform()->position += getRigidBody()->velocity;
}

void Skeleton::setArrivalPoint(glm::vec2 point)
{
	m_arrivalPoint = point;
}

