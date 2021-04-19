#pragma once
#ifndef __OBSTACLES__
#define __OBSTACLES__
#include "DisplayObject.h"
#include "HealthBar.h"

class Obstacle : public DisplayObject
{
public:
	// constructors
	Obstacle(std::string fileNameAndType, std::string FileName);
	Obstacle() { };

	// destructor
	~Obstacle();

	// life cycle functions
	void draw() override;
	void update() override;
	void clean() override;

	void flipDbg();
protected:
	std::string m_fileName;
	bool m_dbgMode;

};


class DestructibleObstacle : public Obstacle
{
public:
	// constructors
	DestructibleObstacle(std::string fileNameAndType, std::string FileName);

	// destructor
	~DestructibleObstacle();

	// life cycle functions
	void draw() override;
	void update() override;
	void clean() override;

	HealthBar& getHealthBar();
private:
	HealthBar m_healthBar;
	SDL_Rect* m_healthBarDestRect;
};

#endif /* defined (__OBSTACLES__) */