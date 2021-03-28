#pragma once
#ifndef __OBSTACLE__
#define __OBSTACLE__
#include "DisplayObject.h"

class Obstacle final : public DisplayObject
{
public:
	// constructors
	Obstacle(std::string fileNameAndType, std::string FileName);

	// destructor
	~Obstacle();

	// life cycle functions
	void draw() override;
	void update() override;
	void clean() override;
private:
	std::string m_fileName;

};

#endif /* defined (__OBSTACLE__) */