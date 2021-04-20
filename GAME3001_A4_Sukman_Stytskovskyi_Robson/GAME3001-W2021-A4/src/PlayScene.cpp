#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"
#include "stdlib.h"
#include "time.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{	
	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);

	for (int row = 0; row < Config::ROW_NUM; ++row)
	{
		for (int col = 0; col < Config::COL_NUM; ++col)
		{
			TextureManager::Instance()->draw("grass", m_getTile(col, row)->getTransform()->position.x + offset.x, m_getTile(col, row)->getTransform()->position.y + offset.y, 0, 255, true);
			if (m_getTile(col, row)->getNeighbourTile(TOP_TILE) == nullptr)
			{
				TextureManager::Instance()->draw("tree", m_getTile(col, row)->getTransform()->position.x + offset.x, m_getTile(col, row)->getTransform()->position.y + offset.y, 0, 255, true);
				m_getTile(col, row)->setTileStatus(IMPASSABLE);
				m_getTile(col, row)->setType(OBSTACLE_TILE);
			}
			if (m_getTile(col, row)->getNeighbourTile(RIGHT_TILE) == nullptr)
			{
				TextureManager::Instance()->draw("tree", m_getTile(col, row)->getTransform()->position.x + offset.x, m_getTile(col, row)->getTransform()->position.y + offset.y, 0, 255, true);
				m_getTile(col, row)->setTileStatus(IMPASSABLE);
				m_getTile(col, row)->setType(OBSTACLE_TILE);
			}
			if (m_getTile(col, row)->getNeighbourTile(LEFT_TILE) == nullptr)
			{
				TextureManager::Instance()->draw("tree", m_getTile(col, row)->getTransform()->position.x + offset.x, m_getTile(col, row)->getTransform()->position.y + offset.y, 0, 255, true);
				m_getTile(col, row)->setTileStatus(IMPASSABLE);
				m_getTile(col, row)->setType(OBSTACLE_TILE);
			}
			if (m_getTile(col, row)->getNeighbourTile(BOTTOM_TILE) == nullptr)
			{
				TextureManager::Instance()->draw("log", m_getTile(col, row)->getTransform()->position.x + offset.x, m_getTile(col, row)->getTransform()->position.y + offset.y, 0, 255, true);
				m_getTile(col, row)->setTileStatus(IMPASSABLE);
				m_getTile(col, row)->setType(OBSTACLE_TILE);
			}
		}
	}

	if(EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();	
	}

	drawDisplayList();

	

	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);

}

void PlayScene::update()
{
	m_randomSwitch = 0 + rand() % 2;

	SDL_GetMouseState(&mouseX, &mouseY);

	m_pPlayer->setDestination(glm::vec2(mouseX, mouseY));


	//for normal KeyPresses
	if (m_PressCounter != 6)
			m_PressCounter++;

	//for melee attacks
	m_MeleeCounter++;
	if (m_MeleeCounter >= 12 && !m_pMeleePlayer.empty())
	{
		for (int i = 0; i < m_pMeleePlayer.size(); i++)
		{
			removeChild(m_pMeleePlayer[i]);
			m_pMeleePlayer[i] = nullptr;
			m_pMeleePlayer.erase(m_pMeleePlayer.begin() + i);
			m_pMeleePlayer.shrink_to_fit();
		}
	}

	//for bullet delition
	m_BulletCounter++;
	if (m_BulletCounter >= 36 && !m_pBullet.empty())
	{
		for (int i = 0; i < m_pBullet.size(); i++)
		{
			removeChild(m_pBullet[i]);
			m_pBullet[i] = nullptr;
			m_pBullet.erase(m_pBullet.begin() + i);
			m_pBullet.shrink_to_fit();
		}
	}
	
	// Actors and Obstacles
	for (int i = 0; i < m_pObstacle.size(); i++)
	{
		CollisionManager::ObstacleColCheck(m_pPlayer, m_pObstacle[i]);
		CollisionManager::ObstacleColCheck(m_pSkeletonClose, m_pObstacle[i]);
	}

	for (int i = 0; i < m_pObstacleDestructible.size(); i++)
	{
		CollisionManager::ObstacleColCheck(m_pPlayer, m_pObstacleDestructible[i]);
		CollisionManager::ObstacleColCheck(m_pSkeletonClose, m_pObstacleDestructible[i]);
	}

	//Projectiles and Obstacles

	for (int i = 0; i < m_pObstacle.size(); i++)
	{
		for (int j = 0; j < m_pBullet.size(); j++)
		{
			if (CollisionManager::AABBCheck(m_pObstacle[i], m_pBullet[j]))
			{
				removeChild(m_pBullet[j]);
				m_pBullet[j] = nullptr;
				m_pBullet.erase(m_pBullet.begin() + j);
				m_pBullet.shrink_to_fit();
			}
		}
	}

	for (int i = 0; i < m_pObstacleDestructible.size(); i++)
	{
		for (int j = 0; j < m_pBullet.size(); j++)
		{
			if (CollisionManager::AABBCheck(m_pObstacleDestructible[i], m_pBullet[j]))
			{
				removeChild(m_pBullet[j]);
				m_pBullet[j] = nullptr;
				m_pBullet.erase(m_pBullet.begin() + j);
				m_pBullet.shrink_to_fit();

				m_pObstacleDestructible[i]->getHealthBar().setHealthPoints
				(m_pObstacleDestructible[i]->getHealthBar().getHealthPoints() - 10);
			}
		}
	}

	for (int i = 0; i < m_pObstacleDestructible.size(); i++)
	{
		if (m_pObstacleDestructible[i]->getHealthBar().getHealthPoints() <= 0)
		{
			removeChild(m_pObstacleDestructible[i]);
			m_pObstacleDestructible[i] = nullptr;
			m_pObstacleDestructible.erase(m_pObstacleDestructible.begin() + i);
			m_pObstacleDestructible.shrink_to_fit();
		}
	}

	//Bullet and Enemy collision
	for (int i = 0; i < m_pBullet.size(); i++)
	{
		if (CollisionManager::circleAABBCheck(m_pBullet[i], m_pSkeletonClose) && !skeletonCloseDead)
		{
			damageSkeleton(m_pSkeletonClose);
			m_pSkeletonClose->setTakingDamage(true);
			//m_pSkeletonClose->flipTakingDamage();

			removeChild(m_pBullet[i]);
			m_pBullet[i] = nullptr;
			m_pBullet.erase(m_pBullet.begin() + i);
			m_pBullet.shrink_to_fit();
		}
		else if (CollisionManager::circleAABBCheck(m_pBullet[i], m_pSkeletonRanged) && !skeletonRangedDead)
		{
			damageSkeleton(m_pSkeletonRanged);
			m_pSkeletonRanged->setTakingDamage(true);
			//m_pSkeletonRanged->flipTakingDamage();

			removeChild(m_pBullet[i]);
			m_pBullet[i] = nullptr;
			m_pBullet.erase(m_pBullet.begin() + i);
			m_pBullet.shrink_to_fit();
		}
	}

	//Melee and Enemy collision
	for (int i = 0; i < m_pMeleePlayer.size(); i++)
	{
		if (CollisionManager::circleAABBCheck(m_pMeleePlayer[i], m_pSkeletonClose) && !skeletonCloseDead)
		{
			damageSkeleton(m_pSkeletonClose);
			m_pSkeletonClose->setTakingDamage(true);
			//m_pSkeletonClose->flipTakingDamage();
		}
		if (CollisionManager::circleAABBCheck(m_pMeleePlayer[i], m_pSkeletonRanged) && !skeletonRangedDead)
		{
			damageSkeleton(m_pSkeletonRanged);
			m_pSkeletonRanged->setTakingDamage(true);
			//m_pSkeletonRanged->flipTakingDamage();
		}
	}

	// Enemy Melee and Player collision
	for (int i = 0; i < m_pMeleeEnemy.size(); i++)
	{
		if (CollisionManager::circleAABBCheck(m_pMeleeEnemy[i], m_pPlayer))
		{
			SoundManager::Instance().playSound("skeleton_damaged");

			m_pPlayer->getHealthBar().setHealthPoints(m_pPlayer->getHealthBar().getHealthPoints() - 10);

			removeChild(m_pMeleeEnemy[i]);
			m_pMeleeEnemy[i] = nullptr;
			m_pMeleeEnemy.erase(m_pMeleeEnemy.begin() + i);
			m_pMeleeEnemy.shrink_to_fit();
		}
	}
	
	// Enemy Projectile and Player collision
	for (int i = 0; i < m_pBone.size(); i++)
	{
		if (CollisionManager::circleAABBCheck(m_pBone[i], m_pPlayer))
		{
			SoundManager::Instance().playSound("skeleton_damaged");

			m_pPlayer->getHealthBar().setHealthPoints(m_pPlayer->getHealthBar().getHealthPoints() - 10);

			removeChild(m_pBone[i]);
			m_pBone[i] = nullptr;
			m_pBone.erase(m_pBone.begin() + i);
			m_pBone.shrink_to_fit();
		}
	}

	//**************\\
	//** AI STUFF **\\
	//**************\\

	if (m_pSkeletonClose->getCurrentAction() != "Idle")
		m_pSkeletonClose->setCurrentAction(TreeMelee->MakeDecision());

	if (m_pSkeletonRanged->getCurrentAction() != "Idle")
		m_pSkeletonRanged->setCurrentAction(TreeRanged->MakeDecision());

	if (m_pSkeletonClose->getHealthBar().getHealthPoints() <= 25)
		m_pSkeletonClose->setFleeing(true);

	if (m_pSkeletonRanged->getHealthBar().getHealthPoints() <= 16)
			m_pSkeletonRanged->setFleeing(true);
	

	
	if (m_frameCounter % 5 == 0)
	{
		m_lastCloseEnemyPosition = m_pSkeletonClose->getTransform()->position;

		m_lastRangedEnemyPosition = m_pSkeletonRanged->getTransform()->position;
		/*int newHealth = (m_pShip->getHealthBar().getHealthPoints()) - 10;
		m_pShip->getHealthBar().setHealthPoints(newHealth - 10);*/
	}

	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
	updateDisplayList();

	m_CheckAgentDR(m_pPlayer, m_pSkeletonRanged);
	m_CheckAgentDR(m_pPlayer, m_pSkeletonClose);


	m_frameCounter++;
	if (m_frameCounter > 1000)
	{  
		m_frameCounter = 0;
	}

	//for each (auto & Obstacle in m_pObstacle)
	//{
	//	if (CollisionManager::lineRectCheck(m_pSkeletonClose->getTransform()->position, m_pSkeletonClose->getTransform()->position + m_pSkeletonClose->getCurrentDirection() * 25.0f, Obstacle->getTransform()->position, Obstacle->getWidth(), Obstacle->getHeight()))
	//	{
	//		m_pSkeletonClose->getRigidBody()->isColliding == true;
	//		std::cout << "Enemy collision with obstacle\n";
	//		m_pSkeletonClose->setCurrentAction("Stopped");
	//		//if (m_randomSwitch == 0)
	//		m_pSkeletonClose->turnRight();
	//		//else if (m_randomSwitch == 1)
	//			//m_pShip->turnLeft();
	//	}
	//}
	//

	std::cout << m_pSkeletonClose->getCurrentAction()<< "\n";
	std::cout << m_pSkeletonRanged->getCurrentAction() << "\n";
	

	m_CheckPathNodeLOS();
	m_CheckAgentLOS(m_pSkeletonClose, m_pPlayer);
	m_CheckAgentLOS(m_pSkeletonRanged, m_pPlayer);


	//#######
	//#######
	//Ranged Actions Start

	if (m_EnemyProjectile <= 90)
		m_EnemyProjectile++;

	if (Util::distance(m_pSkeletonRanged->getTransform()->position, m_pPlayer->getTransform()->position) < m_pSkeletonRanged->getLOSDistance() * 0.6 &&
		Util::distance(m_pSkeletonRanged->getTransform()->position, m_pPlayer->getTransform()->position) > m_pSkeletonRanged->getLOSDistance() * 0.4)
	{
		m_pSkeletonRanged->setRangedCombatRange(true);
	}
	else
		m_pSkeletonRanged->setRangedCombatRange(false);

	if (m_pSkeletonRanged->getHealthBar().getHealthPoints() <= 0)
	{
		m_pSkeletonRanged->setCurrentAction("Dying");

		//m_pShip->setEnabled(false);

		m_enemiesAlive--;
		m_enemiesDead++;

		for (int i = 0; i < m_pGameStatus.size(); i++)
		{
			removeChild(m_pGameStatus[i]);
			m_pGameStatus[i] = nullptr;
			m_pGameStatus.erase(m_pGameStatus.begin() + i);
			m_pGameStatus.shrink_to_fit();
		}

		const SDL_Color orange = { 213,110,43, 205 };
		const SDL_Color white = { 255,255,255, 205 };

		std::string enemiesA = "Enemies left: ";
		std::string enemiesD = "Enemies eliminated: ";

		m_pGameStatus.push_back(new Label(enemiesA + std::to_string(m_enemiesAlive), "Teko", 30, white, glm::vec2(178.f, 75.f)));
		m_pGameStatus.push_back(new Label(enemiesD + std::to_string(m_enemiesDead), "Teko", 30, white, glm::vec2(140.f, 50.f)));

		for (int i = 0; i < m_pGameStatus.size(); i++)
			addChild(m_pGameStatus[i]);

		skeletonRangedDead = true;
	}
	if (m_pSkeletonRanged->getCurrentAction() == "Patrol Action")
	{
		m_pSkeletonRanged->setCurrentDirection(Util::normalize(glm::vec2((m_pRangedPatrolPath[m_rangedPatrolPathPosition + 1]->
			getTransform()->position.x + offset.x - m_pSkeletonRanged->getTransform()->position.x), (m_pRangedPatrolPath[m_rangedPatrolPathPosition
				+ 1]->getTransform()->position.y + offset.y - m_pSkeletonRanged->getTransform()->position.y))));


		if (Util::distance(m_pSkeletonRanged->getTransform()->position, m_pRangedPatrolPath[m_rangedPatrolPathPosition + 1]->getTransform()->position + offset) < 5.0f)
		{
			m_rangedPatrolPathPosition++;
			std::cout << m_rangedPatrolPathPosition << std::endl;
			std::cout << m_pSkeletonRanged->getCurrentDirection().x << ", " << m_pSkeletonRanged->getCurrentDirection().y;
		}
		if (m_rangedPatrolPathPosition == 3)
		{
			m_rangedPatrolPathPosition = -1;
		}
	}
	if (m_pSkeletonRanged->getCurrentAction() == "Patrol Action" || m_pSkeletonRanged->getCurrentAction() == "Wandering" ||
		m_pSkeletonRanged->getCurrentAction() == "Taking Damage" || m_pSkeletonRanged->getCurrentAction() == "Move To Player Action" ||
		m_pSkeletonRanged->getCurrentAction() == "Flee Action" || m_pSkeletonRanged->getCurrentAction() == "Move Behind Cover Action")
	{
		m_pSkeletonRanged->moveForward();
		m_pSkeletonRanged->move();
		if (m_pSkeletonRanged->getCurrentAction() != "Taking Damage")
		{
			if ((m_pSkeletonRanged->getTransform()->position.x > m_lastRangedEnemyPosition.x) && !(m_pSkeletonRanged->getTransform()->position.y
		> m_lastRangedEnemyPosition.y + 2) && !(m_pSkeletonClose->getTransform()->position.y
			< m_lastRangedEnemyPosition.y - 2))
			{
				m_pSkeletonRanged->setAnimationState("WalkingRight");

			}
			if ((m_pSkeletonRanged->getTransform()->position.y > m_lastRangedEnemyPosition.y) && !(m_pSkeletonRanged->getTransform()->position.x
		> m_lastRangedEnemyPosition.x + 2) && !(m_pSkeletonRanged->getTransform()->position.x
			< m_lastRangedEnemyPosition.x - 2))
			{
				m_pSkeletonRanged->setAnimationState("WalkingDown");
			}
			if ((m_pSkeletonRanged->getTransform()->position.x < m_lastRangedEnemyPosition.x) && !(m_pSkeletonRanged->getTransform()->position.y
			> m_lastRangedEnemyPosition.y + 2) && !(m_pSkeletonRanged->getTransform()->position.y
				< m_lastRangedEnemyPosition.y - 2))
			{
				m_pSkeletonRanged->setAnimationState("WalkingLeft");
				m_pSkeletonRanged->setGoingRight(false);
			}
			if ((m_pSkeletonRanged->getTransform()->position.y < m_lastRangedEnemyPosition.y))
			{
				m_pSkeletonRanged->setAnimationState("WalkingUp");
			}
		}
	}
	if (m_pSkeletonRanged->getCurrentAction() == "Move To Player Action")
	{
		m_pSkeletonRanged->setCurrentDirection(Util::normalize(m_pPlayer->getTransform()->position - m_pSkeletonRanged->getTransform()->position));
		m_pSkeletonRanged->setArrivalPoint(m_pPlayer->getTransform()->position);
	}
	else
	{
		m_pSkeletonRanged->setArrivalPoint(glm::vec2(-100, 0));
		m_pSkeletonRanged->setMaxSpeed(2);
	}
	if (m_pSkeletonRanged->getCurrentAction() == "Flee Action")
	{
		m_pSkeletonRanged->setCurrentDirection(Util::normalize(m_pPlayer->getTransform()->position - m_pSkeletonRanged->getTransform()->position) * -1.f);
	}
	if(m_pSkeletonRanged->getCurrentAction() == "Wander") // no need to check collission with impassable border tiles
	{											// while on patrol path as enemy will turn on it's own
		for each (auto & Tile in m_pTileGrid)
		{
			if (Tile->getTileStatus() == IMPASSABLE)
			{
				if (CollisionManager::lineRectCheck(m_pSkeletonRanged->getTransform()->position, m_pSkeletonRanged->getTransform()->position + m_pSkeletonRanged->getCurrentDirection() * 25.0f, Tile->getTransform()->position, Tile->getWidth(), Tile->getHeight()))
				{
					m_pSkeletonRanged->getRigidBody()->isColliding == true;
					std::cout << "Enemy collision with obstacle\n";
					m_pSkeletonRanged->setCurrentAction("Stopped");
					//if (m_randomSwitch == 0)
					m_pSkeletonRanged->turnRight();
					//else if (m_randomSwitch == 1)
						//m_pShip->turnLeft();
				}
			}
		}
	}

	if (m_pSkeletonRanged->getCurrentAction() == "Move Behind Cover Action")
	{
		if (Util::distance(m_pSkeletonRanged->getTransform()->position, m_findClosestCoverPathNode(m_pSkeletonRanged)->getTransform()->position) > 10)
		{
			m_pSkeletonRanged->setCurrentDirection(Util::normalize(m_findClosestCoverPathNode(m_pSkeletonRanged)->getTransform()->position
				- m_pSkeletonRanged->getTransform()->position));
		}
	}
	if (m_pSkeletonRanged->getCurrentAction() == "Move To LOS Action")
	{
		m_pSkeletonRanged->setCurrentDirection(Util::normalize(m_findClosestLOSPathNode(m_pSkeletonRanged)->getTransform()->
			position - m_pSkeletonRanged->getTransform()->position));
		m_pSkeletonRanged->moveForward();
		m_pSkeletonRanged->move();
		if (Util::distance(m_pSkeletonRanged->getTransform()->position, m_findClosestLOSPathNode(m_pSkeletonRanged)->
			getTransform()->position) < 5)
		{
			m_pSkeletonRanged->setCurrentDirection(Util::normalize(m_pPlayer->getTransform()->position - m_pSkeletonRanged->getTransform()->position));
		}
		//m_pSkeletonClose->m_Seek(m_findClosestLOSPathNode(m_pSkeletonClose));
	}
	if (m_pSkeletonRanged->getCurrentAction() == "Taking Damage" && m_frameCounter % 20 == 0)
	{
		m_pSkeletonRanged->setCurrentAction("Patrol Action");
	}
	if (m_pSkeletonRanged->getCurrentAction() == "Attack Action" && m_EnemyProjectile >= 90)
	{
		m_pBone.push_back(new Bone(m_pSkeletonRanged->getTransform()->position, m_pPlayer->getTransform()->position));

		for (int i = 0; i < m_pBone.size(); i++)
			addChild(m_pBone[i]);

		m_EnemyProjectile = 0;
	}
	if (m_pSkeletonRanged->getCurrentAction() == "Dying" && m_frameCounter % 40 == 0)
	{
		m_pSkeletonRanged->setCurrentAction("Dead");
	}
	if (m_pSkeletonRanged->getHealthBar().getHealthPoints() == 0 && m_pSkeletonRanged->getCurrentAction() != "Dead")
	{
		m_pSkeletonRanged->setCurrentAction("Dying");
	}
	if (m_pSkeletonRanged->getCurrentAction() == "Stopped")
	{
		m_pSkeletonRanged->moveForward();
		m_pSkeletonRanged->move();
		m_pSkeletonRanged->setCurrentAction("Wandering");
	}
	if (m_pSkeletonRanged->getCurrentAction() == "Move To Range Action")
	{
		auto distance = Util::distance(m_pSkeletonRanged->getTransform()->position, m_pPlayer->getTransform()->position);

		if (distance != m_pSkeletonRanged->getLOSDistance() * 0.5f)
		{
			if (distance > m_pSkeletonRanged->getLOSDistance() * 0.5f)
			{
				m_pSkeletonRanged->setCurrentDirection(Util::normalize(m_pPlayer->getTransform()->position - m_pSkeletonRanged->getTransform()->position));
				m_pSkeletonRanged->getTransform()->position += Util::normalize(m_pPlayer->getTransform()->position - m_pSkeletonRanged->getTransform()->position);
			}
			else if (distance < m_pSkeletonRanged->getLOSDistance() * .5f)
			{
				m_pSkeletonRanged->setCurrentDirection(Util::normalize(m_pPlayer->getTransform()->position - m_pSkeletonRanged->getTransform()->position));
				m_pSkeletonRanged->getTransform()->position += -Util::normalize(m_pPlayer->getTransform()->position - m_pSkeletonRanged->getTransform()->position) * 0.5f;
			}
		}
	}
	if (m_pSkeletonRanged->getCurrentAction() == "Wait In Cover Action")
	{
		m_pSkeletonRanged->setWaitInCoverTimer(m_pSkeletonRanged->getWaintInCoverTimer() + 1);
	}
	if (m_pSkeletonRanged->getWaintInCoverTimer() > 180)
	{
		m_pSkeletonRanged->setTimerNotOut(false);
		m_pSkeletonRanged->setWaitInCoverTimer(0);
	}

	if (m_pSkeletonRanged->getCurrentAction() == "Move Behind Cover Action")
	{
		if (Util::distance(m_findClosestCoverPathNode(m_pSkeletonRanged)->getTransform()->position,
			m_pSkeletonRanged->getTransform()->position) < 5)
			m_pSkeletonRanged->setIsBehindCover(true);
	}
	if (m_pSkeletonRanged->getCurrentAction() == "Leave Cover Action")
	{
		m_pSkeletonRanged->setTimerNotOut(false);
		m_pSkeletonRanged->setIsBehindCover(false);
		m_pSkeletonRanged->setTakingDamage(false);
	}
	//Ranged End
	//#########
	





	//#######
	//#######
	//Close Actions Start

	if (m_EnemyMelee <= 60)
		m_EnemyMelee++;

	if (m_pSkeletonClose->getHealthBar().getHealthPoints() <= 0)
	{
		m_pSkeletonClose->setCurrentAction("Dying");

		//m_pShip->setEnabled(false);

		m_enemiesAlive--;
		m_enemiesDead++;

		for (int i = 0; i < m_pGameStatus.size(); i++)
		{
			removeChild(m_pGameStatus[i]);
			m_pGameStatus[i] = nullptr;
			m_pGameStatus.erase(m_pGameStatus.begin() + i);
			m_pGameStatus.shrink_to_fit();
		}

		const SDL_Color orange = { 213,110,43, 205 };
		const SDL_Color white = { 255,255,255, 205 };

		std::string enemiesA = "Enemies left: ";
		std::string enemiesD = "Enemies eliminated: ";

		m_pGameStatus.push_back(new Label(enemiesA + std::to_string(m_enemiesAlive), "Teko", 30, white, glm::vec2(178.f, 75.f)));
		m_pGameStatus.push_back(new Label(enemiesD + std::to_string(m_enemiesDead), "Teko", 30, white, glm::vec2(140.f, 50.f)));

		for (int i = 0; i < m_pGameStatus.size(); i++)
			addChild(m_pGameStatus[i]);

		skeletonCloseDead = true;
	}
	if (m_pSkeletonClose->getCurrentAction() == "Patrol Action")
	{
		m_pSkeletonClose->setCurrentDirection(Util::normalize(glm::vec2((m_pClosePatrolPath[m_closePatrolPathPosition + 1]->
			getTransform()->position.x + offset.x - m_pSkeletonClose->getTransform()->position.x), (m_pClosePatrolPath[m_closePatrolPathPosition
				+ 1]->getTransform()->position.y + offset.y - m_pSkeletonClose->getTransform()->position.y))));


		if (Util::distance(m_pSkeletonClose->getTransform()->position, m_pClosePatrolPath[m_closePatrolPathPosition + 1]->getTransform()->position + offset) < 5.0f)
		{
			m_closePatrolPathPosition++;
			std::cout << m_closePatrolPathPosition << std::endl;
			std::cout << m_pSkeletonClose->getCurrentDirection().x << ", " << m_pSkeletonClose->getCurrentDirection().y;
		}
		if (m_closePatrolPathPosition == 3)
		{
			m_closePatrolPathPosition = -1;
		}
	}
	if (m_pSkeletonClose->getCurrentAction() == "Patrol Action" || m_pSkeletonClose->getCurrentAction() == "Wandering" ||
		m_pSkeletonClose->getCurrentAction() == "Taking Damage" || m_pSkeletonClose->getCurrentAction() == "Move To Player Action" ||
		m_pSkeletonClose->getCurrentAction() == "Flee Action")
	{
		m_pSkeletonClose->moveForward();
		m_pSkeletonClose->move();
		if (m_pSkeletonClose->getCurrentAction() != "Taking Damage")
		{
			if ((m_pSkeletonClose->getTransform()->position.x > m_lastCloseEnemyPosition.x) && !(m_pSkeletonClose->getTransform()->position.y
		> m_lastCloseEnemyPosition.y + 2) && !(m_pSkeletonClose->getTransform()->position.y
			< m_lastCloseEnemyPosition.y - 2))
			{
				m_pSkeletonClose->setAnimationState("WalkingRight");

			}
			if ((m_pSkeletonClose->getTransform()->position.y > m_lastCloseEnemyPosition.y) && !(m_pSkeletonClose->getTransform()->position.x
		> m_lastCloseEnemyPosition.x + 2) && !(m_pSkeletonClose->getTransform()->position.x
			< m_lastCloseEnemyPosition.x - 2))
			{
				m_pSkeletonClose->setAnimationState("WalkingDown");
			}
			if ((m_pSkeletonClose->getTransform()->position.x < m_lastCloseEnemyPosition.x) && !(m_pSkeletonClose->getTransform()->position.y
			> m_lastCloseEnemyPosition.y + 2) && !(m_pSkeletonClose->getTransform()->position.y
				< m_lastCloseEnemyPosition.y - 2))
			{
				m_pSkeletonClose->setAnimationState("WalkingLeft");
				m_pSkeletonClose->setGoingRight(false);
			}
			if ((m_pSkeletonClose->getTransform()->position.y < m_lastCloseEnemyPosition.y))
			{
				m_pSkeletonClose->setAnimationState("WalkingUp");
			}
		}
	}
	if (m_pSkeletonClose->getCurrentAction() == "Move To Player Action")
	{
		m_pSkeletonClose->setCurrentDirection(Util::normalize(m_pPlayer->getTransform()->position - m_pSkeletonClose->getTransform()->position));
		m_pSkeletonClose->setArrivalPoint(m_pPlayer->getTransform()->position);
	}
	else
	{
		m_pSkeletonClose->setArrivalPoint(glm::vec2(-100, 0));
		m_pSkeletonClose->setMaxSpeed(2);
	}
	if (m_pSkeletonClose->getCurrentAction() == "Flee Action")
	{
		m_pSkeletonClose->setCurrentDirection(Util::normalize(m_pPlayer->getTransform()->position - m_pSkeletonClose->getTransform()->position) * -1.f);
	}
	if (m_pSkeletonClose->getCurrentAction() == "Attack Action" && m_EnemyMelee >= 60)
	{
		m_pMeleeEnemy.push_back(new Melee(m_pSkeletonClose->getTransform()->position, m_pPlayer->getTransform()->position));

		for (int i = 0; i < m_pMeleeEnemy.size(); i++)
			addChild(m_pMeleeEnemy[i]);

		m_EnemyMelee = 0;
	}
	if (m_pSkeletonClose->getCurrentAction() == "Wander") // no need to check collission with impassable border tiles
	{											// while on patrol path as enemy will turn on it's own
		for each (auto & Tile in m_pTileGrid)
		{
			if (Tile->getTileStatus() == IMPASSABLE)
			{
				if (CollisionManager::lineRectCheck(m_pSkeletonClose->getTransform()->position, m_pSkeletonClose->getTransform()->position + m_pSkeletonClose->getCurrentDirection() * 25.0f, Tile->getTransform()->position, Tile->getWidth(), Tile->getHeight()))
				{
					m_pSkeletonClose->getRigidBody()->isColliding == true;
					std::cout << "Enemy collision with obstacle\n";
					m_pSkeletonClose->setCurrentAction("Stopped");
					//if (m_randomSwitch == 0)
					m_pSkeletonClose->turnRight();
					//else if (m_randomSwitch == 1)
						//m_pShip->turnLeft();
				}
			}
		}
	}
	if (m_pSkeletonClose->getCurrentAction() == "Stopped")
	{
		m_pSkeletonClose->moveForward();
		m_pSkeletonClose->move();
		m_pSkeletonClose->setCurrentAction("Wandering");
	}
	if (m_pSkeletonClose->getHealthBar().getHealthPoints() == 0 && m_pSkeletonClose->getCurrentAction() != "Dead")
	{
		m_pSkeletonClose->setCurrentAction("Dying");
	}
	if (m_pSkeletonClose->getCurrentAction() == "Dying" && m_frameCounter % 40 == 0)
	{
		m_pSkeletonClose->setCurrentAction("Dead");
	}
	if (m_pSkeletonClose->getCurrentAction() == "Taking Damage" && m_frameCounter % 20 == 0)
	{
		m_pSkeletonClose->setCurrentAction("Patrol Action");
	}

	if (m_pSkeletonClose->getCurrentAction() == "Move To LOS Action")
	{
		m_pSkeletonClose->setCurrentDirection(Util::normalize(m_findClosestLOSPathNode(m_pSkeletonClose)->getTransform()->
			position - m_pSkeletonClose->getTransform()->position));
		m_pSkeletonClose->moveForward();
		m_pSkeletonClose->move();
		if (Util::distance(m_pSkeletonClose->getTransform()->position, m_findClosestLOSPathNode(m_pSkeletonClose)->
			getTransform()->position) < 5)
		{
			m_pSkeletonClose->setCurrentDirection(Util::normalize(m_pPlayer->getTransform()->position - m_pSkeletonClose->getTransform()->position));
		}
		//m_pSkeletonClose->m_Seek(m_findClosestLOSPathNode(m_pSkeletonClose));
	}
	if (m_pSkeletonClose->inDR() == true)
	{
		auto ShipToTargetDistance = Util::distance(m_pSkeletonClose->getTransform()->position, m_pPlayer->getTransform()->position);
		if (ShipToTargetDistance <= 30)
		{
			m_pSkeletonClose->setCloseCombatRange(true);
		}
		else
			m_pSkeletonClose->setCloseCombatRange(false);
	}
	//Close End
	//#########
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_W))
	{
		m_pPlayer->getTransform()->position.y -= m_pPlayer->getMaxSpeed();
		m_pPlayer->setAnimating(true);
	}
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
	{
		m_pPlayer->getTransform()->position.x += m_pPlayer->getMaxSpeed();
		m_pPlayer->setAnimating(true);
		m_pPlayer->setGoingRight(true);
	}
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_S))
	{
		m_pPlayer->getTransform()->position.y += m_pPlayer->getMaxSpeed();
		m_pPlayer->setAnimating(true);
	}
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A))
	{
		m_pPlayer->getTransform()->position.x -= m_pPlayer->getMaxSpeed();
		m_pPlayer->setAnimating(true);
		m_pPlayer->setGoingRight(false);
	}
	if (!EventManager::Instance().isKeyDown(SDL_SCANCODE_W) && !EventManager::Instance().isKeyDown(SDL_SCANCODE_S) && !EventManager::Instance().isKeyDown(SDL_SCANCODE_A) && !EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
	{
		m_pPlayer->setAnimating(false);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance()->changeSceneState(END_SCENE);
	}

	if(EventManager::Instance().getMouseButton(0) && m_PressCounter >= 6)
	{
		m_pBullet.push_back(new Bullet(m_pPlayer->getTransform()->position, glm::vec2(mouseX, mouseY)));

		for (int i = 0; i < m_pBullet.size(); i++)
			addChild(m_pBullet[i]);

		SoundManager::Instance().playSound("shoot_sound", 0, 1);

		m_BulletCounter = 0;
		m_PressCounter = 0;
	}
	
	if(EventManager::Instance().getMouseButton(2) && m_PressCounter >= 6 && m_MeleeCounter >= 12)
	{
		m_pMeleePlayer.push_back(new Melee(m_pPlayer->getTransform()->position + m_pPlayer->getOrientation() + 
			glm::vec2(m_pPlayer->getWidth() * 0.5f, m_pPlayer->getHeight() * 0.5f), glm::vec2(mouseX, mouseY)));

		for (int i = 0; i < m_pMeleePlayer.size(); i++)
			addChild(m_pMeleePlayer[i]);

		m_MeleeCounter = 0;
		m_PressCounter = 0;

		SoundManager::Instance().playSound("knife_sound", 0, 1);
	}

	// Toggles Debug Mode
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_H) && m_PressCounter >= 6)
	{
		m_dbgMode = !m_dbgMode;

		m_pSkeletonClose->flipDbg();
		m_pSkeletonRanged->flipDbg();
		m_pPlayer->flipDbg();

		m_gridVisible = !m_gridVisible;
		m_toggleGrid(m_gridVisible);
				
		for (int i = 0; i < m_pObstacle.size(); i++)
			m_pObstacle[i]->flipDbg();

		for (int i = 0; i < m_pObstacleDestructible.size(); i++)
			m_pObstacleDestructible[i]->flipDbg();

		m_PressCounter = 0;

	}


	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_K) && m_dbgMode && m_PressCounter >= 6 && !skeletonCloseDead)
	{
		damageSkeleton(m_pSkeletonClose);
		m_pSkeletonClose->flipTakingDamage();
		////std::cout << "Enemy damaged, new health value: " << m_pShip->getHealthBar().getHealthPoints() << "\n";

		m_PressCounter = 0;
	}


	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_P) && m_dbgMode && m_PressCounter >= 6)
	{
		if(m_pSkeletonClose->getCurrentAction() == "Patrol Action")
			m_pSkeletonClose->setCurrentAction("Idle");
		else if (m_pSkeletonClose->getCurrentAction() == "Idle")
			m_pSkeletonClose->setCurrentAction("Patrol Action");

		m_PressCounter = 0;
	}
}

void PlayScene::start()
{
	srand(time(NULL));

	TextureManager::Instance()->load("../Assets/textures/grass.png", "grass");
	TextureManager::Instance()->load("../Assets/textures/tree.png", "tree");
	TextureManager::Instance()->load("../Assets/textures/log.png", "log");

	
	const SDL_Color orange = { 213,110,43, 205 };
	const SDL_Color white = { 255,255,255, 205 };

	SoundManager::Instance().load("../Assets/audio/uhh.ogg", "skeleton_damaged", SOUND_SFX);
	
	std::string enemiesA = "Enemies left: ";
	std::string enemiesD = "Enemies eliminated: ";

	m_pGameStatus.push_back(new Label(enemiesA + std::to_string(m_enemiesAlive), "Teko", 30, white, glm::vec2(178.f, 75.f)));
	m_pGameStatus.push_back(new Label(enemiesD + std::to_string(m_enemiesDead), "Teko", 30, white, glm::vec2(140.f, 50.f)));

	for (int i = 0; i < m_pGameStatus.size(); i++)
		addChild(m_pGameStatus[i]);

	SoundManager::Instance().load("../Assets/audio/Music.mp3", "bkgMusic", SOUND_MUSIC);
	SoundManager::Instance().load("../Assets/audio/Gunshot.wav", "shoot_sound", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/Knife.wav", "knife_sound", SOUND_SFX);
	SoundManager::Instance().setMusicVolume(30);
	SoundManager::Instance().playMusic("bkgMusic", -1, 0);

	m_PressCounter = 0;
	m_EnemyMelee = 0;
	m_EnemyProjectile = 0;

	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
	//Build node grid for overlaying map

	m_enemiesAlive = 2;

	m_buildTileGrid();
	m_buildGrid();
	// Set GUI Title
	m_guiTitle = "Play Scene";

	m_pPlayer = new Gunner;
	m_pPlayer->getTransform()->position = glm::vec2(600.0f, 300.0f);
	addChild(m_pPlayer);

	// add the Close Enemy to the scene as a start point
	m_pSkeletonClose = new SkeletonClose();
	m_pSkeletonClose->getTransform()->position = m_getTile(5, 4)->getTransform()->position + offset;
	addChild(m_pSkeletonClose, 2);
	m_pSkeletonClose->setAnimationState("WalkingDown");
	m_pSkeletonClose->setMaxSpeed(2.0f);

	// add the Ranged Enemy to the scene as a start point
	m_pSkeletonRanged = new SkeletonRanged();
	m_pSkeletonRanged->getTransform()->position = m_getTile(1, 1)->getTransform()->position + offset;
	addChild(m_pSkeletonRanged, 2);
	m_pSkeletonRanged->setAnimationState("WalkingRight");
	m_pSkeletonRanged->setMaxSpeed(2.0f);

	

	for (int i = 0; i < 5; i++)
	{
		m_pObstacle.push_back(new Obstacle("Rock.png", "Rock"));
	}
	m_pObstacle[0]->getTransform()->position = m_getTile(9, 3)->getTransform()->position;
	addChild(m_pObstacle[0]);

	m_pObstacle[1]->getTransform()->position = m_getTile(9, 7)->getTransform()->position;
	addChild(m_pObstacle[1]);

	m_pObstacle[2]->getTransform()->position = m_getTile(14, 11)->getTransform()->position;
	addChild(m_pObstacle[2]);

	m_pObstacle[3]->getTransform()->position = m_getTile(12, 11)->getTransform()->position;
	addChild(m_pObstacle[3]);

	m_pObstacle[4]->getTransform()->position = m_getTile(15, 6)->getTransform()->position;
	addChild(m_pObstacle[4]);

	for (int i = 0; i < 2; i++)
	{
		m_pObstacleDestructible.push_back(new DestructibleObstacle("Rock.png", "Rock"));
	}
	m_pObstacleDestructible[0]->getTransform()->position = m_getTile(7, 3)->getTransform()->position;
	addChild(m_pObstacleDestructible[0]);

	m_pObstacleDestructible[1]->getTransform()->position = m_getTile(3, 7)->getTransform()->position;
	addChild(m_pObstacleDestructible[1]);

	// build patrol path list
	m_pRangedPatrolPath.push_back(m_getTile(1, 1));
	m_pRangedPatrolPath.push_back(m_getTile(18, 1));
	m_pRangedPatrolPath.push_back(m_getTile(18, 13));
	m_pRangedPatrolPath.push_back(m_getTile(1, 13));

	m_pClosePatrolPath.push_back(m_getTile(5, 4));
	m_pClosePatrolPath.push_back(m_getTile(13, 4));
	m_pClosePatrolPath.push_back(m_getTile(13, 9));
	m_pClosePatrolPath.push_back(m_getTile(5, 9));
	
	

	// create a CC DecisionTree
	TreeMelee = new DecisionTree(false);
	TreeMelee->setAgent(m_pSkeletonClose);
	TreeMelee->Display();

	// create a RC DecisionTree
	TreeRanged = new DecisionTree(true);
	TreeRanged->setAgent(m_pSkeletonRanged);
	TreeRanged->Display();
}

void PlayScene::GUI_Function() 
{
	//TODO: We need to deal with this
	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
	
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("GAME3001 - A3", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	// allow ship rotation
	static int angle;
	if(ImGui::SliderInt("Skeleton Direction", &angle, -360, 360))
	{
		m_pSkeletonClose->setCurrentHeading(angle);
	}
	
	ImGui::Separator();

	static int shipPosition[] = { m_pSkeletonClose->getTransform()->position.x, m_pSkeletonClose->getTransform()->position.y };
	if (ImGui::SliderInt2("Skeleton Position", shipPosition, 0, 800))
	{
		m_pSkeletonClose->getTransform()->position.x = shipPosition[0];
		m_pSkeletonClose->getTransform()->position.y = shipPosition[1];

		//std::cout << "------------------------" << std::endl;
		//std::cout << decisionTree->MakeDecision() << std::endl;
		//std::cout << "------------------------\n" << std::endl;
	}
	
	
	ImGui::Separator();


	if (ImGui::Checkbox("Grid Enabled", &m_gridVisible))
	{
		// toggle grid on/off
		m_toggleGrid(m_gridVisible);
	}

	ImGui::Separator();
	
	if (ImGui::Button("Start"))
	{

	}

	ImGui::SameLine();
	
	if (ImGui::Button("Reset"))
	{
		// reset everything back to initial values
		
	}

	ImGui::Separator();

	
	ImGui::End();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}

bool PlayScene::m_CheckAgentLOS(Agent* agent, DisplayObject* object)
{
	// initialize
	bool hasLOS = false;
	agent->setHasLOS(false);

	// if agent to object distance is less than or equal to LOS Distance
	auto AgentToObjectDistance = Util::distance(agent->getTransform()->position, object->getTransform()->position);
	if (AgentToObjectDistance <= agent->getLOSDistance())
	{
		std::vector<DisplayObject*> contactList;
		for (auto display_object : getDisplayList())
		{
			// check if obstacle is farther than than the object
			auto AgentToObstacleDistance = Util::distance(agent->getTransform()->position, display_object->getTransform()->position);

			if (AgentToObstacleDistance <= AgentToObjectDistance)
			{
				if ((display_object->getType() != AGENT) && (display_object->getType() != PATH_NODE) &&
					(display_object->getType() != object->getType()) && display_object->getType() != NONE)
				{
					if (display_object->getType() == TILE)
					{
						if (static_cast<Tile*>(display_object)->getTileStatus() != IMPASSABLE)
							contactList.push_back(display_object);
					}
					else
						contactList.push_back(display_object);
				}
			}
		}
		contactList.push_back(object); // add the target to the end of the list
		const auto agentTarget = agent->getTransform()->position + agent->getCurrentDirection() * agent->getLOSDistance();
		hasLOS = CollisionManager::LOSCheck(agent, agentTarget, contactList, object);

		agent->setHasLOS(hasLOS);
	}

	return hasLOS;
}

void PlayScene::m_CheckPathNodeLOS()
{
	for (auto path_node : m_pGrid)
	{
		auto targetDirection = m_pPlayer->getTransform()->position - path_node->getTransform()->position;
		auto normalizedDirection = Util::normalize(targetDirection);
		path_node->setCurrentDirection(normalizedDirection);
		m_CheckAgentLOS(path_node, m_pPlayer);
	}
}

void PlayScene::m_CheckAgentDR(DisplayObject* target_object, Agent* agent)
{
	//if target is within the Detection Radius
	auto ShipToTargetDistance = Util::distance(agent->getTransform()->position, target_object->getTransform()->position);
	if (ShipToTargetDistance - 20 <= agent->getDetectionRadius())
	{
		std::vector<DisplayObject*> contactList;
		contactList.push_back(target_object); // add the target to the end of the list
		auto hasDR = CollisionManager::DRCheck(agent->getDetectionRadius(), contactList, target_object);

		agent->setInDR(hasDR);
	}
	else agent->setInDR(false);
}

Tile* PlayScene::m_getTile(const int col, const int row)
{
	return m_pTileGrid[(row * Config::COL_NUM) + col];
}

Tile* PlayScene::m_getTile(const glm::vec2 grid_position)
{
	const auto col = grid_position.x;
	const auto row = grid_position.y;
	return m_pTileGrid[(row * Config::COL_NUM) + col];
}


void PlayScene::m_buildTileGrid()
{
	auto tileSize = Config::TILE_SIZE;

	// add tiles to the grid
	for (int row = 0; row < Config::ROW_NUM; ++row)
	{
		for (int col = 0; col < Config::COL_NUM; ++col)
		{
			Tile* tile = new Tile(); // create empty tile
			tile->getTransform()->position = glm::vec2(col * tileSize, row * tileSize);
			tile->setGridPosition(col, row);
			//addChild(tile);
			m_pTileGrid.push_back(tile);
		}
	}

	// create references for each tile to its neighbours
	for (int row = 0; row < Config::ROW_NUM; ++row)
	{
		for (int col = 0; col < Config::COL_NUM; ++col)
		{
			Tile* tile = m_getTile(col, row);

			// Topmost row
			if (row == 0)
			{
				tile->setNeighbourTile(TOP_TILE, nullptr);
			}
			else
			{
				tile->setNeighbourTile(TOP_TILE, m_getTile(col, row - 1));
			}

			// rightmost column
			if (col == Config::COL_NUM - 1)
			{
				tile->setNeighbourTile(RIGHT_TILE, nullptr);
			}
			else
			{
				tile->setNeighbourTile(RIGHT_TILE, m_getTile(col + 1, row));
			}

			// bottommost row
			if (row == Config::ROW_NUM - 1)
			{
				tile->setNeighbourTile(BOTTOM_TILE, nullptr);
			}
			else
			{
				tile->setNeighbourTile(BOTTOM_TILE, m_getTile(col, row + 1));
			}

			// leftmost  column
			if (col == 0)
			{
				tile->setNeighbourTile(LEFT_TILE, nullptr);
			}
			else
			{
				tile->setNeighbourTile(LEFT_TILE, m_getTile(col - 1, row));
			}
		}
	}

	//std::cout << m_pTileGrid.size() << std::endl;
}

void PlayScene::m_buildGrid()
{
	auto tileSize = Config::TILE_SIZE;

	// add path_nodes to the Grid
	for (int row = 0; row < Config::ROW_NUM; ++row)
	{
		for (int col = 0; col < Config::COL_NUM; ++col)
		{
			PathNode* path_node = new PathNode();
			path_node->getTransform()->position = glm::vec2(
				(col * tileSize) + tileSize * 0.5f, (row * tileSize) + tileSize * 0.5f);
			addChild(path_node);
			m_pGrid.push_back(path_node);

		}
	}
}

void PlayScene::m_toggleGrid(bool state)
{

	for (auto path_node : m_pGrid)
	{
		path_node->setVisible(state);
	}
}

PathNode* PlayScene::m_findClosestPathNode(Agent* agent)
{
	auto min = INFINITY;
	PathNode* closestPathNode = nullptr;
	for (auto path_node : m_pGrid)
	{
		const auto distance = Util::distance(agent->getTransform()->position, path_node->getTransform()->position);
		if (distance < min)
		{
			min = distance;
			closestPathNode = path_node;
		}
	}

	return closestPathNode;
}

PathNode* PlayScene::m_findClosestLOSPathNode(Agent* agent)
{
	auto min = INFINITY;
	PathNode* closestPathNode = nullptr;
	for (auto path_node : m_pGrid)
	{
		const auto distance = Util::distance(agent->getTransform()->position, path_node->getTransform()->position);
		if (distance < min)
		{
			if (path_node->hasLOS() == true)
			{
				closestPathNode = path_node;
				min = distance;
			}
		}
	}

	return closestPathNode;
}

PathNode* PlayScene::m_findClosestCoverPathNode(Agent* agent)
{
	auto min = INFINITY;
	PathNode* closestPathNode = nullptr;
	for (auto path_node : m_pGrid)
	{
		for (auto obstacle : m_pObstacle)
		{
			for (auto destructible_obstacle : m_pObstacleDestructible)
			{
				if (!CollisionManager::AABBCheck(path_node, obstacle) && !CollisionManager::AABBCheck(path_node, destructible_obstacle))
				{
					const auto distance = Util::distance(agent->getTransform()->position, path_node->getTransform()->position);
					if (distance < min)
					{

						if (path_node->hasLOS() == false)
						{
							closestPathNode = path_node;
							min = distance;
						}
					}
				}
			}
		}
	}

	return closestPathNode;
}


void PlayScene::damageSkeleton(Skeleton* actor)
{
	int newHealth = actor->getHealthBar().getHealthPoints() - 10;
	actor->getHealthBar().setHealthPoints(newHealth);
	if (actor->getAnimationState() == "WalkingDown")
	{
		actor->setAnimationState("FWSdamage");
	}
	else if (actor->getAnimationState() == "WalkingRight")
	{
		actor->setAnimationState("RWSdamage");
	}
	else if (actor->getAnimationState() == "WalkingLeft")
	{
		actor->setAnimationState("LWSdamage");
	}
	else if (actor->getAnimationState() == "WalkingUp")
	{
		actor->setAnimationState("BWSdamage");
	}
	actor->setCurrentAction("Taking Damage");
	
	SoundManager::Instance().playSound("skeleton_damaged", 0, -1);
}
