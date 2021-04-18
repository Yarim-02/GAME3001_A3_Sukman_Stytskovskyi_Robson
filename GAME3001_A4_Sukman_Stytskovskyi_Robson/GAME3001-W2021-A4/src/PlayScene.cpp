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


	//Util::DrawLine(m_pShip->getTransform()->position, m_pShip->getTransform()->position + m_pShip->getCurrentDirection() * 25.0f, glm::vec4(1, 0, 1, 1));


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
	if (m_MeleeCounter >= 12 && !m_pMelee.empty())
	{
		for (int i = 0; i < m_pMelee.size(); i++)
		{
			removeChild(m_pMelee[i]);
			m_pMelee[i] = nullptr;
			m_pMelee.erase(m_pMelee.begin() + i);
			m_pMelee.shrink_to_fit();
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
	
	for (int i = 0; i < 5; i++)
	{
		CollisionManager::ObstacleColCheck(m_pPlayer, m_pObstacle[i]);
	}

	//Bullet and Enemy collision
	for (int i = 0; i < m_pBullet.size(); i++)
	{
		if (CollisionManager::circleAABBCheck(m_pBullet[i], m_pSkeleton) && !skeletonDead)
		{
			damageActor(m_pSkeleton);
			m_pSkeleton->flipTakingDamage();
		}
	}

	//Melee and Enemy collision
	for (int i = 0; i < m_pMelee.size(); i++)
	{
		if (CollisionManager::circleAABBCheck(m_pMelee[i], m_pSkeleton) && !skeletonDead)
		{
			damageActor(m_pSkeleton);
			m_pSkeleton->flipTakingDamage();
		}
	}

	//**************
	//** AI STUFF **
	//**************

	std::cout << "------------------------" << std::endl;
	std::cout << decisionTree->MakeDecision() << std::endl;
	std::cout << "------------------------\n" << std::endl;

	if (m_pSkeleton->hasLOS())
	{
		m_pSkeleton->setHadLOS(true);
		m_LOSCounter = 9000;
	}

	if (m_pSkeleton->getHadLOS())
	{
		m_LOSCounter--;

		if (m_LOSCounter <= 0)
		{
			m_pSkeleton->setHadLOS(false);
		}
	}

	if (m_pSkeleton->getHadLOS())
		m_pSkeleton->setCurrentAction("Move To Player Action");
	else
		m_pSkeleton->setCurrentAction(decisionTree->MakeDecision());

	

	//Enemy death
	if (m_pSkeleton->getHealthBar().getHealthPoints() <= 0)
	{
		m_pSkeleton->setCurrentAction("Dying");

		//m_pShip->setEnabled(false);

		m_enemiesAlive = 0;
		m_enemiesDead = 1;

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

		skeletonDead = true;
	}

	if (m_frameCounter % 5 == 0)
	{
		m_lastEnemyPosition = m_pSkeleton->getTransform()->position;

		/*int newHealth = (m_pShip->getHealthBar().getHealthPoints()) - 10;
		m_pShip->getHealthBar().setHealthPoints(newHealth - 10);*/
	}

	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
	updateDisplayList();

	m_CheckShipDR(m_pPlayer);

	if (m_pSkeleton->getCurrentAction() == "Patrol Action")
	{
		m_pSkeleton->setCurrentDirection(Util::normalize(glm::vec2((m_pPatrolPath[m_patrolPathPosition + 1]->
			getTransform()->position.x + offset.x - m_pSkeleton->getTransform()->position.x), (m_pPatrolPath[m_patrolPathPosition
				+ 1]->getTransform()->position.y + offset.y - m_pSkeleton->getTransform()->position.y))));


		if (Util::distance(m_pSkeleton->getTransform()->position, m_pPatrolPath[m_patrolPathPosition + 1]->getTransform()->position + offset) < 5.0f)
		{
			m_patrolPathPosition++;
			std::cout << m_patrolPathPosition << std::endl;
			std::cout << m_pSkeleton->getCurrentDirection().x << ", " << m_pSkeleton->getCurrentDirection().y;
		}
		if (m_patrolPathPosition == 3)
		{
			m_patrolPathPosition = -1;
		}
	}
	if (m_pSkeleton->getCurrentAction() == "Patrol Action" || m_pSkeleton->getCurrentAction() == "Wandering" || m_pSkeleton->getCurrentAction() == "Taking Damage" || m_pSkeleton->getCurrentAction() == "Move To Player Action")
	{
		//m_pSkeleton->moveForward();
		//m_pSkeleton->move();
		if (m_pSkeleton->getCurrentAction() != "Taking Damage")
		{
			if ((m_pSkeleton->getTransform()->position.x > m_lastEnemyPosition.x) && !(m_pSkeleton->getTransform()->position.y
		> m_lastEnemyPosition.y + 2) && !(m_pSkeleton->getTransform()->position.y
			< m_lastEnemyPosition.y - 2))
			{
				m_pSkeleton->setAnimationState("WalkingRight");
			}
			if ((m_pSkeleton->getTransform()->position.y > m_lastEnemyPosition.y) && !(m_pSkeleton->getTransform()->position.x
					> m_lastEnemyPosition.x + 2) && !(m_pSkeleton->getTransform()->position.x
						< m_lastEnemyPosition.x - 2))
			{
				m_pSkeleton->setAnimationState("WalkingDown");
			}
			if ((m_pSkeleton->getTransform()->position.x < m_lastEnemyPosition.x) && !(m_pSkeleton->getTransform()->position.y
			> m_lastEnemyPosition.y + 2) && !(m_pSkeleton->getTransform()->position.y
				< m_lastEnemyPosition.y - 2))
			{
				m_pSkeleton->setAnimationState("WalkingLeft");
			}
			if ((m_pSkeleton->getTransform()->position.y < m_lastEnemyPosition.y))
			{
				m_pSkeleton->setAnimationState("WalkingUp");
			}
		}
	}
	if (m_pSkeleton->getCurrentAction() == "Move To Player Action")
	{
		m_pSkeleton->setCurrentHeading(Util::repeat(Util::signedAngle(m_pSkeleton->getTransform()->position, m_pPlayer->getTransform()->position), 360.f));
		//m_pSkeleton->setCurrentDirection(Util::normalize(m_pPlayer->getTransform()->position));
	}

	m_frameCounter++;
	if (m_frameCounter > 1000)
	{
		m_frameCounter = 0;
	}

	for each (auto & Obstacle in m_pObstacle)
	{
		if (CollisionManager::lineRectCheck(m_pSkeleton->getTransform()->position, m_pSkeleton->getTransform()->position + m_pSkeleton->getCurrentDirection() * 25.0f, Obstacle->getTransform()->position, Obstacle->getWidth(), Obstacle->getHeight()))
		{
			m_pSkeleton->getRigidBody()->isColliding == true;
			std::cout << "Enemy collision with obstacle\n";
			m_pSkeleton->setCurrentAction("Stopped");
			//if (m_randomSwitch == 0)
			m_pSkeleton->turnRight();
			//else if (m_randomSwitch == 1)
				//m_pShip->turnLeft();
		}
	}
	if (m_pSkeleton->getCurrentAction() == "Wander") // no need to check collission with impassable border tiles
	{											// while on patrol path as enemy will turn on it's own
		for each (auto & Tile in m_pTileGrid)
		{
			if (Tile->getTileStatus() == IMPASSABLE)
			{
				if (CollisionManager::lineRectCheck(m_pSkeleton->getTransform()->position, m_pSkeleton->getTransform()->position + m_pSkeleton->getCurrentDirection() * 25.0f, Tile->getTransform()->position, Tile->getWidth(), Tile->getHeight()))
				{
					m_pSkeleton->getRigidBody()->isColliding == true;
					std::cout << "Enemy collision with obstacle\n";
					m_pSkeleton->setCurrentAction("Stopped");
					//if (m_randomSwitch == 0)
					m_pSkeleton->turnRight();
					//else if (m_randomSwitch == 1)
						//m_pShip->turnLeft();
				}
			}
		}
	}
	if (m_pSkeleton->getCurrentAction() == "Stopped")
	{
		m_pSkeleton->moveForward();
		m_pSkeleton->move();
		m_pSkeleton->setCurrentAction("Wandering");
	}

	if (m_pSkeleton->getHealthBar().getHealthPoints() == 0 && m_pSkeleton->getCurrentAction() != "Dead")
	{
		m_pSkeleton->setCurrentAction("Dying");
	}

	if (m_pSkeleton->getCurrentAction() == "Dying" && m_frameCounter % 40 == 0)
	{
		m_pSkeleton->setCurrentAction("Dead");
	}

	if (m_pSkeleton->getCurrentAction() == "Taking Damage" && m_frameCounter % 20 == 0)
	{
		m_pSkeleton->setCurrentAction("Patrol Action");
	}

	m_CheckPathNodeLOS();
	m_CheckAgentLOS(m_pSkeleton, m_pPlayer);
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
		m_pMelee.push_back(new Melee(m_pPlayer->getTransform()->position + m_pPlayer->getOrientation() + 
			glm::vec2(m_pPlayer->getWidth() * 0.5f, m_pPlayer->getHeight() * 0.5f), glm::vec2(mouseX, mouseY)));

		for (int i = 0; i < m_pMelee.size(); i++)
			addChild(m_pMelee[i]);

		m_MeleeCounter = 0;
		m_PressCounter = 0;

		SoundManager::Instance().playSound("knife_sound", 0, 1);
	}

	// Toggles Debug Mode
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_H) && m_PressCounter >= 6)
	{
		m_dbgMode = !m_dbgMode;

		m_pSkeleton->flipDbg();
		m_pPlayer->flipDbg();

		m_gridVisible = !m_gridVisible;
		m_toggleGrid(m_gridVisible);
				
		for (int i = 0; i < m_pObstacle.size(); i++)
			m_pObstacle[i]->flipDbg();

		m_PressCounter = 0;

	}


	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_K) && m_dbgMode && m_PressCounter >= 6 && !skeletonDead)
	{
		damageActor(m_pSkeleton);
		m_pSkeleton->flipTakingDamage();
		//std::cout << "Enemy damaged, new health value: " << m_pShip->getHealthBar().getHealthPoints() << "\n";

		m_PressCounter = 0;
	}


	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_P) && m_dbgMode && m_PressCounter >= 6)
	{
		if(m_pSkeleton->getCurrentAction() == "Patrol")
			m_pSkeleton->setCurrentAction("Idle");
		else if(m_pSkeleton->getCurrentAction() == "Idle")
			m_pSkeleton->setCurrentAction("Patrol");

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

	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
	//Build node grid for overlaying map

	m_enemiesAlive = 1;

	m_buildTileGrid();
	m_buildGrid();
	// Set GUI Title
	m_guiTitle = "Play Scene";

	m_pPlayer = new Gunner;
	m_pPlayer->getTransform()->position = glm::vec2(200.0f, 300.0f);
	addChild(m_pPlayer);

	// add the ship to the scene as a start point
	m_pSkeleton = new Skeleton();
	m_pSkeleton->getTransform()->position = m_getTile(1, 1)->getTransform()->position + offset;
	addChild(m_pSkeleton, 2);
	m_pSkeleton->setAnimationState("WalkingDown");
	m_pSkeleton->setMaxSpeed(2.0f);

	for (int i = 0; i < 5; i++)
	{
		m_pObstacle.push_back(new Obstacle("Rock.png", "Rock"));
	}
	m_pObstacle[0]->getTransform()->position = m_getTile(9, 2)->getTransform()->position;
	addChild(m_pObstacle[0]);

	m_pObstacle[1]->getTransform()->position = m_getTile(9, 7)->getTransform()->position;
	addChild(m_pObstacle[1]);

	m_pObstacle[2]->getTransform()->position = m_getTile(14, 11)->getTransform()->position;
	addChild(m_pObstacle[2]);

	m_pObstacle[3]->getTransform()->position = m_getTile(12, 11)->getTransform()->position;
	addChild(m_pObstacle[3]);

	m_pObstacle[4]->getTransform()->position = m_getTile(15, 6)->getTransform()->position;
	addChild(m_pObstacle[4]);

	
	
	// added the target to the scene a goal
	/*m_pTarget = new Target();
	m_pTarget->getTransform()->position = m_getTile(16, 8)->getTransform()->position + offset;
	addChild(m_pTarget);*/

	// build patrol path list
	m_pPatrolPath.push_back(m_getTile(1, 1));
	m_pPatrolPath.push_back(m_getTile(18, 1));
	m_pPatrolPath.push_back(m_getTile(18, 13));
	m_pPatrolPath.push_back(m_getTile(1, 13));

	// create a dummy DecisionTree
	decisionTree = new DecisionTree();
	decisionTree->setAgent(m_pSkeleton);
	decisionTree->Display();
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
		m_pSkeleton->setCurrentHeading(angle);
	}
	
	ImGui::Separator();

	static int shipPosition[] = { m_pSkeleton->getTransform()->position.x, m_pSkeleton->getTransform()->position.y };
	if (ImGui::SliderInt2("Skeleton Position", shipPosition, 0, 800))
	{
		m_pSkeleton->getTransform()->position.x = shipPosition[0];
		m_pSkeleton->getTransform()->position.y = shipPosition[1];

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
		auto targetDirection = -path_node->getTransform()->position;
		auto normalizedDirection = Util::normalize(targetDirection);
		path_node->setCurrentDirection(normalizedDirection);
		m_CheckAgentLOS(path_node, m_pPlayer);
	}
}

void PlayScene::m_CheckShipDR(DisplayObject* target_object)
{
	//if target is within the Detection Radius
	auto ShipToTargetDistance = Util::distance(m_pSkeleton->getTransform()->position, target_object->getTransform()->position);
	if (ShipToTargetDistance - 20 <= m_pSkeleton->getDetectionRadius())
	{
		std::vector<DisplayObject*> contactList;
		contactList.push_back(target_object); // add the target to the end of the list
		auto hasDR = CollisionManager::DRCheck(m_pSkeleton->getDetectionRadius(), contactList, target_object);

		m_pSkeleton->setInDR(hasDR);
	}
	else m_pSkeleton->setInDR(false);
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


void PlayScene::damageActor(Skeleton* actor)
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