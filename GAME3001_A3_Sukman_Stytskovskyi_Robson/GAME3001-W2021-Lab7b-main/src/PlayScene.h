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
#include "Bullet.h"
#include "Melee.h"
#include "Gunner.h"

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
	Gunner* m_pPlayer;
	std::vector<Bullet*> m_pBullet;
	std::vector <Melee*> m_pMelee;
	std::vector<Obstacle*> m_pObstacle;
	Obstacle* m_pObstacle2;
	Obstacle* m_pObstacle3;
	void m_CheckShipLOS(DisplayObject* object);
	void m_CheckShipDR(DisplayObject* object);

	DecisionTree* decisionTree;

	Label* m_pGameStatus;
	int m_enemiesAlive,
		m_enemiesDead;

	int m_MeleeCounter,
	 m_BulletCounter,
	 m_PressCounter;

	bool m_dbgMode;

	//Tile Stuff
	std::vector<Tile*> m_pGrid;
	std::vector<Tile*> m_pPatrolPath;

	void m_setGridEnabled(bool state);
	bool m_getGridEnabled() const;
	Tile* m_getTile(int col, int row);
	Tile* m_getTile(glm::vec2 grid_position);

	void m_buildGrid();

	void damageActor(Ship* actor);

	int m_frameCounter = 0;
	bool m_randomSwitch = 0;
	int m_patrolPathPosition = 0;
	int mouseX, mouseY;

	glm::vec2 m_lastEnemyPosition;
};



#endif /* defined (__PLAY_SCENE__) */
