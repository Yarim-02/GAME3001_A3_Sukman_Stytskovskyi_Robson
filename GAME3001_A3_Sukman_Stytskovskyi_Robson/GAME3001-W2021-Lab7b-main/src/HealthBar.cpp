#include "HealthBar.h"
#include "TextureManager.h"
#include "Game.h"
#include "Renderer.h"

HealthBar::HealthBar()
{
	
}

HealthBar::~HealthBar()
= default;

void HealthBar::draw()
{
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 0, 0, 255);
	//SDL_RenderDrawRect(Renderer::Instance()->getRenderer(), m_dest);
	SDL_RenderFillRect(Renderer::Instance()->getRenderer(), m_dest);
	SDL_RenderPresent(Renderer::Instance()->getRenderer());
}

void HealthBar::update()
{

}

void HealthBar::clean()
{
}

int HealthBar::getHealthPoints()
{
	return m_healthPoints;
}

void HealthBar::setHealthPoints(int hp)
{
	m_healthPoints = hp;
}

void HealthBar::setDest(SDL_Rect* dest)
{
	m_dest = dest;
}

