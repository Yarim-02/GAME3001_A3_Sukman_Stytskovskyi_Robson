
#pragma once
#ifndef __HEALTH_BAR__
#define __HEALTH_BAR__
#include <SDL_rect.h>

#include "DisplayObject.h"
#include "TextureManager.h"

class HealthBar : public DisplayObject
{
public:
	HealthBar();
	~HealthBar();

	void draw() override;
	void update() override;
	void clean() override;

	int getHealthPoints();
	void setHealthPoints(int hp);
	void setDest(SDL_Rect* dest);

private:
	int m_healthPoints;
	SDL_Rect* m_dest;
	
	
};





#endif	//defined (__HEALTH_BAR__)