#include "Obstacles.h"


#include "Util.h"
#include "SoundManager.h"
#include "TextureManager.h"

DestructibleObstacle::DestructibleObstacle(std::string fileNameAndType, std::string FileName)
{
	m_fileName = FileName;
	TextureManager::Instance()->load("../Assets/textures/" + fileNameAndType, FileName);

	auto size = TextureManager::Instance()->getTextureSize(FileName);
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(300.0f, 300.0f);

	setType(DESTRUCTIBLE_OBSTACLE);
	getRigidBody()->isColliding = false;


	m_healthBarDestRect = new SDL_Rect;
	m_healthBarDestRect->x = getTransform()->position.x + (size.x / 2) - 67.5f;
	m_healthBarDestRect->y = getTransform()->position.y - (size.y / 1.5);
	m_healthBarDestRect->h = 4;
	m_healthBarDestRect->w = m_healthBar.getHealthPoints();
	m_healthBar.setDest(m_healthBarDestRect);

	m_dbgMode = false;
}

DestructibleObstacle::~DestructibleObstacle()
= default;

void DestructibleObstacle::draw()
{
	TextureManager::Instance()->draw(m_fileName,
		getTransform()->position.x, getTransform()->position.y, 0, 255, false);

	if (m_dbgMode)
		Util::DrawRect(getTransform()->position, getWidth(), getHeight(), glm::vec4(255, 0, 0, 255));

	m_healthBar.draw();
}

void DestructibleObstacle::update()
{
	m_healthBarDestRect->x = getTransform()->position.x + (getWidth() / 2) - 45.5f;
	m_healthBarDestRect->y = getTransform()->position.y - (getHeight() / 2.5);
	m_healthBarDestRect->h = 4;
	m_healthBarDestRect->w = m_healthBar.getHealthPoints();
	m_healthBar.setDest(m_healthBarDestRect);
}

void DestructibleObstacle::clean()
{
}

void DestructibleObstacle::flipDbg()
{
	m_dbgMode = !m_dbgMode;
}

HealthBar& DestructibleObstacle::getHealthBar()
{
	return m_healthBar;
}