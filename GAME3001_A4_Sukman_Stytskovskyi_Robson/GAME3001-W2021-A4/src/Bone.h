#pragma once

#include "Bullet.h"

class Bone : public Bullet
{
public:
	Bone(glm::vec2 position, glm::vec2 direction);
	~Bone();

	void draw();
private:

};