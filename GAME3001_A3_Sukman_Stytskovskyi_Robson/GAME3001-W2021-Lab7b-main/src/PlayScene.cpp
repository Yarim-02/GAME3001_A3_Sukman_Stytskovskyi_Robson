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
			TextureManager::Instance()->load("../Assets/textures/grass.png", "grass");
			TextureManager::Instance()->load("../Assets/textures/tree.png", "tree");
			TextureManager::Instance()->load("../Assets/textures/log.png", "log");
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
	
	srand(time(NULL));
	m_randomSwitch = 0 + rand() % 2;

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

	if (m_frameCounter % 5 == 0)
	{
		m_lastEnemyPosition = m_pShip->getTransform()->position;

		/*int newHealth = (m_pShip->getHealthBar().getHealthPoints()) - 10;
		m_pShip->getHealthBar().setHealthPoints(newHealth - 10);*/
	}

	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
	updateDisplayList();

	SDL_GetMouseState(&mouseX, &mouseY);

	m_pPlayer->setDestination(glm::vec2(mouseX, mouseY));

	m_CheckShipLOS(m_pPlayer);
	m_CheckShipDR(m_pPlayer);

	if (m_pShip->getCurrentAction() == "Patrol")
	{
		m_pShip->setCurrentDirection(Util::normalize(glm::vec2((m_pPatrolPath[m_patrolPathPosition + 1]->
			getTransform()->position.x + offset.x - m_pShip->getTransform()->position.x), (m_pPatrolPath[m_patrolPathPosition
				+ 1]->getTransform()->position.y + offset.y - m_pShip->getTransform()->position.y))));


		if (Util::distance(m_pShip->getTransform()->position, m_pPatrolPath[m_patrolPathPosition + 1]->getTransform()->position + offset) < 5.0f)
		{
			m_patrolPathPosition++;
			std::cout << m_patrolPathPosition << std::endl;
			std::cout << m_pShip->getCurrentDirection().x << ", " << m_pShip->getCurrentDirection().y;
		}
		if (m_patrolPathPosition == 3)
		{
			m_patrolPathPosition = -1;
		}
	}
	if (m_pShip->getCurrentAction() == "Patrol" || m_pShip->getCurrentAction() == "Wandering" || m_pShip->getCurrentAction() == "Taking Damage")
	{
		m_pShip->moveForward();
		m_pShip->move();
		if (m_pShip->getCurrentAction() != "Taking Damage")
		{
			if ((m_pShip->getTransform()->position.x > m_lastEnemyPosition.x) && !(m_pShip->getTransform()->position.y
		> m_lastEnemyPosition.y + 2) && !(m_pShip->getTransform()->position.y
			< m_lastEnemyPosition.y - 2))
			{
				m_pShip->setAnimationState("WalkingRight");
			}
			if ((m_pShip->getTransform()->position.y > m_lastEnemyPosition.y) && !(m_pShip->getTransform()->position.x
	> m_lastEnemyPosition.x + 2) && !(m_pShip->getTransform()->position.x
		< m_lastEnemyPosition.x - 2))
			{
				m_pShip->setAnimationState("WalkingDown");
			}
			if ((m_pShip->getTransform()->position.x < m_lastEnemyPosition.x) && !(m_pShip->getTransform()->position.y
			> m_lastEnemyPosition.y + 2) && !(m_pShip->getTransform()->position.y
				< m_lastEnemyPosition.y - 2))
			{
				m_pShip->setAnimationState("WalkingLeft");
			}
			if ((m_pShip->getTransform()->position.y < m_lastEnemyPosition.y))
			{
				m_pShip->setAnimationState("WalkingUp");
			}
		}
	}

	std::cout << "------------------------" << std::endl;
	std::cout << decisionTree->MakeDecision() << std::endl;
	std::cout << "------------------------\n" << std::endl;

	m_frameCounter++;
	if (m_frameCounter > 1000)
	{
		m_frameCounter = 0;
	}

	for each (auto & Obstacle in m_pObstacle)
	{
		if (CollisionManager::lineRectCheck(m_pShip->getTransform()->position, m_pShip->getTransform()->position + m_pShip->getCurrentDirection() * 25.0f, Obstacle->getTransform()->position, Obstacle->getWidth(), Obstacle->getHeight()))
		{
			m_pShip->getRigidBody()->isColliding == true;
			std::cout << "Enemy collision with obstacle\n";
			m_pShip->setCurrentAction("Stopped");
			//if (m_randomSwitch == 0)
			m_pShip->turnRight();
			//else if (m_randomSwitch == 1)
				//m_pShip->turnLeft();
		}
	}
	if (m_pShip->getCurrentAction() == "Wander") // no need to check collission with impassable border tiles
	{											// while on patrol path as enemy will turn on it's own
		for each (auto & Tile in m_pGrid)
		{
			if (Tile->getTileStatus() == IMPASSABLE)
			{
				if (CollisionManager::lineRectCheck(m_pShip->getTransform()->position, m_pShip->getTransform()->position + m_pShip->getCurrentDirection() * 25.0f, Tile->getTransform()->position, Tile->getWidth(), Tile->getHeight()))
				{
					m_pShip->getRigidBody()->isColliding == true;
					std::cout << "Enemy collision with obstacle\n";
					m_pShip->setCurrentAction("Stopped");
					//if (m_randomSwitch == 0)
					m_pShip->turnRight();
					//else if (m_randomSwitch == 1)
						//m_pShip->turnLeft();
				}
			}
		}
	}
	if (m_pShip->getCurrentAction() == "Stopped")
	{
		m_pShip->moveForward();
		m_pShip->move();
		m_pShip->setCurrentAction("Wandering");
	}

	if (m_pShip->getHealthBar().getHealthPoints() == 0 && m_pShip->getCurrentAction() != "Dead")
	{
		m_pShip->setCurrentAction("Dying");
	}

	if (m_pShip->getCurrentAction() == "Dying" && m_frameCounter % 40 == 0)
	{
		m_pShip->setCurrentAction("Dead");
	}

	if (m_pShip->getCurrentAction() == "Taking Damage" && m_frameCounter % 20 == 0)
	{
		m_pShip->setCurrentAction("Patrol");
	}

	
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
	}

	// Toggles Debug Mode
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_H) && m_PressCounter >= 6)
	{
		m_dbgMode = !m_dbgMode;

		m_pShip->flipDbg();
		
		for (int i = 0; i < m_pObstacle.size(); i++)
			m_pObstacle[i]->flipDbg();

		m_PressCounter = 0;

		//m_setGridEnabled(!m_getGridEnabled());
	}


	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_K) && m_pShip->getTakingDamage() == false && m_dbgMode && m_PressCounter >= 6)
	{
		damageActor(m_pShip);
		m_pShip->flipTakingDamage();
		//std::cout << "Enemy damaged, new health value: " << m_pShip->getHealthBar().getHealthPoints() << "\n";

		m_PressCounter = 0;
	}


	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_P) && m_dbgMode && m_PressCounter >= 6)
	{
		if(m_pShip->getCurrentAction() == "Patrol")
			m_pShip->setCurrentAction("Idle");
		else if(m_pShip->getCurrentAction() == "Idle")
			m_pShip->setCurrentAction("Patrol");

		m_PressCounter = 0;
	}
}

void PlayScene::start()
{
	m_PressCounter = 0;

	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
	//Build node grid for overlaying map
	const SDL_Color orange = { 213,110,43, 205 };
	const SDL_Color white = { 255,255,255, 205 };

	std::string enemiesA = "Enemies left: ";
	std::string enemiesD = "Enemies eliminated: ";

	m_pGameStatus = new Label(enemiesA + std::to_string(m_enemiesAlive), "Teko", 30, white, glm::vec2(178.f, 75.f));
	m_pGameStatus->setParent(this);
	addChild(m_pGameStatus, 0);

	m_pGameStatus = new Label(enemiesD + std::to_string(m_enemiesDead), "Teko", 30, white, glm::vec2(140.f, 50.f));
	m_pGameStatus->setParent(this);
	addChild(m_pGameStatus, 0);
	

	m_buildGrid();
	// Set GUI Title
	m_guiTitle = "Play Scene";

	m_pPlayer = new Gunner;
	m_pPlayer->getTransform()->position = glm::vec2(200.0f, 300.0f);
	addChild(m_pPlayer);

	// add the ship to the scene as a start point
	m_pShip = new Ship();
	m_pShip->getTransform()->position = m_getTile(1, 1)->getTransform()->position + offset;
	addChild(m_pShip, 2);
	m_pShip->setAnimationState("WalkingDown");
	m_pShip->setMaxSpeed(2.0f);

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
	m_pTarget = new Target();
	m_pTarget->getTransform()->position = m_getTile(16, 8)->getTransform()->position + offset;
	addChild(m_pTarget);

	// build patrol path list
	m_pPatrolPath.push_back(m_getTile(1, 1));
	m_pPatrolPath.push_back(m_getTile(18, 1));
	m_pPatrolPath.push_back(m_getTile(18, 13));
	m_pPatrolPath.push_back(m_getTile(1, 13));

	// create a dummy DecisionTree
	decisionTree = new DecisionTree();
	decisionTree->setAgent(m_pShip);
	decisionTree->Display();

	std::cout << "------------------------" << std::endl;
	std::cout << decisionTree->MakeDecision() << std::endl;
	std::cout << "------------------------\n" << std::endl;

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
	if(ImGui::SliderInt("Ship Direction", &angle, -360, 360))
	{
		m_pShip->setCurrentHeading(angle);
	}
	
	ImGui::Separator();

	static int shipPosition[] = { m_pShip->getTransform()->position.x, m_pShip->getTransform()->position.y };
	if (ImGui::SliderInt2("Ship Position", shipPosition, 0, 800))
	{
		m_pShip->getTransform()->position.x = shipPosition[0];
		m_pShip->getTransform()->position.y = shipPosition[1];

		std::cout << "------------------------" << std::endl;
		std::cout << decisionTree->MakeDecision() << std::endl;
		std::cout << "------------------------\n" << std::endl;
	}
	
	static int targetPosition[] = { m_pTarget->getTransform()->position.x, m_pTarget->getTransform()->position.y };
	if(ImGui::SliderInt2("Target Position", targetPosition, 0, 800))
	{
		m_pTarget->getTransform()->position.x = targetPosition[0];
		m_pTarget->getTransform()->position.y = targetPosition[1];
	}
	
	ImGui::Separator();

	static bool isGridEnabled = false;
	if (ImGui::Checkbox("Grid Enabled", &isGridEnabled))
	{
		// toggle grid on/off
		m_setGridEnabled(isGridEnabled);
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

void PlayScene::m_CheckShipLOS(DisplayObject* target_object)
{
	// if ship to target distance is less than or equal to LOS Distance
	auto ShipToTargetDistance = Util::distance(m_pShip->getTransform()->position, target_object->getTransform()->position);
	if (ShipToTargetDistance <= m_pShip->getLOSDistance())
	{
		std::vector<DisplayObject*> contactList;
		for (auto object : getDisplayList())
		{
			// check if object is farther than than the target
			auto ShipToObjectDistance = Util::distance(m_pShip->getTransform()->position, object->getTransform()->position);

			if (ShipToObjectDistance <= ShipToTargetDistance)
			{
				if (object->getType() == OBSTACLE_TILE || object->getType() == OBSTACLE) //(object->getType() != TILE) && (object->getType() != m_pShip->getType()) && (object->getType() != target_object->getType()))
				{
					contactList.push_back(object);
				}
			}
		}
		contactList.push_back(target_object); // add the target to the end of the list
		auto hasLOS = CollisionManager::LOSCheck(m_pShip->getTransform()->position,
			m_pShip->getTransform()->position + m_pShip->getCurrentDirection() * m_pShip->getLOSDistance(), contactList, target_object);

		m_pShip->setHasLOS(hasLOS);
	}
}

void PlayScene::m_CheckShipDR(DisplayObject* target_object)
{
	//if target is within the Detection Radius
	auto ShipToTargetDistance = Util::distance(m_pShip->getTransform()->position, target_object->getTransform()->position);
	if (ShipToTargetDistance - 20 <= m_pShip->getDetectionRadius())
	{
		std::vector<DisplayObject*> contactList;
		for (auto object : getDisplayList())
		{
			// check if object is farther than than the target
			auto ShipToObjectDistance = Util::distance(m_pShip->getTransform()->position, object->getTransform()->position);

			if (ShipToObjectDistance <= ShipToTargetDistance)
			{
				if ((object->getType() != m_pShip->getType()) && (object->getType() != target_object->getType()))
				{
					contactList.push_back(object);
				}
			}
		}
		contactList.push_back(target_object); // add the target to the end of the list
		auto hasDR = CollisionManager::DRCheck(m_pShip->getDetectionRadius(), contactList, target_object);

		m_pShip->setInDR(hasDR);
	}
	else m_pShip->setInDR(false);
}


void PlayScene::m_setGridEnabled(bool state)
{
	for (auto tile : m_pGrid)
	{
		tile->setEnabled(state);
		tile->setLabelsEnabled(state);
	}

	if (state == false)
	{
		SDL_RenderClear(Renderer::Instance()->getRenderer());
	}

	m_isGridEnabled = state;
}

bool PlayScene::m_getGridEnabled() const
{
	return m_isGridEnabled;
}

Tile* PlayScene::m_getTile(const int col, const int row)
{
	return m_pGrid[(row * Config::COL_NUM) + col];
}

Tile* PlayScene::m_getTile(const glm::vec2 grid_position)
{
	const auto col = grid_position.x;
	const auto row = grid_position.y;
	return m_pGrid[(row * Config::COL_NUM) + col];
}


void PlayScene::m_buildGrid()
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
			addChild(tile);
			tile->addLabels();
			tile->setEnabled(false);
			m_pGrid.push_back(tile);
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

	std::cout << m_pGrid.size() << std::endl;
}

void PlayScene::damageActor(Ship* actor)
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
}