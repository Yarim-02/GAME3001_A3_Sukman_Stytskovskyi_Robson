#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Button.h"
#include "DecisionTree.h"
#include "Label.h"
#include "Obstacles.h"
#include "Skeleton.h"
#include "SpaceShip.h"
#include "Target.h"
#include "Tile.h"
#include "Bullet.h"
#include "Melee.h"
#include "Gunner.h"
#include "PathNode.h"

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

	Target* m_pTarget;
	Skeleton* m_pSkeleton;
	Gunner* m_pPlayer;
	std::vector<Bullet*> m_pBullet;
	std::vector <Melee*> m_pMelee;
	std::vector<Obstacle*> m_pObstacle;
	std::vector<DestructibleObstacle*> m_pObstacleDestructible;
	Obstacle* m_pObstacle2;
	Obstacle* m_pObstacle3;
	
	bool m_CheckAgentLOS(Agent* agent, DisplayObject* object);
	void m_CheckPathNodeLOS();
	void m_CheckShipDR(DisplayObject* object);

	DecisionTree* decisionTree;

	std::vector<Label*> m_pGameStatus;
	int m_enemiesAlive,
		m_enemiesDead;

	int m_MeleeCounter,
		m_BulletCounter,
		m_PressCounter,
		m_LOSCounter;

	bool m_dbgMode,
		skeletonDead;

	//Tile Stuff
	std::vector<Tile*> m_pTileGrid;
	std::vector<Tile*> m_pPatrolPath;
	void m_buildTileGrid();
	Tile* m_getTile(int col, int row);
	Tile* m_getTile(glm::vec2 grid_position);

	//Path Node Stuff
	std::vector<PathNode*> m_pGrid;
	bool m_gridVisible;
	void m_buildGrid();
	void m_toggleGrid(bool state);

	PathNode* m_findClosestPathNode(Agent* agent);
	PathNode* m_findClosestLOSPathNode(Agent* agent);
	PathNode* m_findClosestCoverPathNode(Agent* agent);
	
	void damageActor(Skeleton* actor);

	int m_frameCounter = 0;
	bool m_randomSwitch = 0;
	int m_patrolPathPosition = 0;
	int mouseX, mouseY;

	glm::vec2 m_lastEnemyPosition;
};



#endif /* defined (__PLAY_SCENE__) */
