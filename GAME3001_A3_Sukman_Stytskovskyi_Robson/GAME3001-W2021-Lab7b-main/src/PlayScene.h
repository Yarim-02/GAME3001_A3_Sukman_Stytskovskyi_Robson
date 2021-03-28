#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Button.h"
#include "DecisionTree.h"
#include "Label.h"
#include "Obstacle.h"
#include "ship.h"
#include "SpaceShip.h"
#include "Target.h"
#include "Tile.h"

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;

	

private:
	// IMGUI Function
	void GUI_Function();
	std::string m_guiTitle;
	glm::vec2 m_mousePosition;
	bool m_isGridEnabled;

	Target* m_pTarget;
	Ship* m_pShip;
	std::vector<Obstacle*> m_pObstacle;
	Obstacle* m_pObstacle2;
	Obstacle* m_pObstacle3;
	void m_CheckShipLOS(DisplayObject* object);
	void m_CheckShipDR(DisplayObject* object);

	DecisionTree* decisionTree;

	//Tile Stuff
	std::vector<Tile*> m_pGrid;
	void m_setGridEnabled(bool state);
	bool m_getGridEnabled() const;
	Tile* m_getTile(int col, int row);
	Tile* m_getTile(glm::vec2 grid_position);

	std::vector<Tile*> m_pPatrolPath;
	void m_buildGrid();

	void damageActor(Ship* actor);

	int m_frameCounter = 0;
	int m_patrolPathPosition = 0;
	glm::vec2 m_lastEnemyPosition;

	bool m_randomSwitch = 0;
};



#endif /* defined (__PLAY_SCENE__) */
