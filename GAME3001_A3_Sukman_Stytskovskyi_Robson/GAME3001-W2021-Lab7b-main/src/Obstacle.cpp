#include "Obstacle.h"


#include "SoundManager.h"
#include "TextureManager.h"

Obstacle::Obstacle(std::string fileNameAndType, std::string fileName)
{
	m_fileName = fileName;
	TextureManager::Instance()->load("../Assets/textures/" + fileNameAndType, fileName);

	auto size = TextureManager::Instance()->getTextureSize(fileName);
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(300.0f, 300.0f);

	setType(OBSTACLE);
	getRigidBody()->isColliding = false;

	SoundManager::Instance().load("../Assets/audio/yay.ogg", "yay", SOUND_SFX);
}

Obstacle::~Obstacle()
= default;

void Obstacle::draw()
{
	TextureManager::Instance()->draw(m_fileName, 
		getTransform()->position.x, getTransform()->position.y, 0, 255, false);
}

void Obstacle::update()
{
}

void Obstacle::clean()
{
}
