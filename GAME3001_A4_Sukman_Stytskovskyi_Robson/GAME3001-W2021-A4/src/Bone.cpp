#include "Bone.h"

#include "TextureManager.h"

Bone::Bone(glm::vec2 position, glm::vec2 direction) : Bullet(position, direction)
{
	TextureManager::Instance()->load("../Assets/textures/circle.png", "circle");

	setMaxSpeed(7.5f);
	setAccelerationRate(7.5f);
}

Bone::~Bone()
{
}

void Bone::draw()
{
	TextureManager::Instance()->draw("circle", getTransform()->position.x, getTransform()->position.y);
}
