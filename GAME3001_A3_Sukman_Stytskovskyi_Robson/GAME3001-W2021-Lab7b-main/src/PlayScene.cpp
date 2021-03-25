#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

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
			}
			if( m_getTile(col, row)->getNeighbourTile(RIGHT_TILE) == nullptr)
			{
				TextureManager::Instance()->draw("tree", m_getTile(col, row)->getTransform()->position.x + offset.x, m_getTile(col, row)->getTransform()->position.y + offset.y, 0, 255, true);
				m_getTile(col, row)->setTileStatus(IMPASSABLE);
			}
			if (m_getTile(col, row)->getNeighbourTile(LEFT_TILE) == nullptr)
			{
				TextureManager::Instance()->draw("tree", m_getTile(col, row)->getTransform()->position.x + offset.x, m_getTile(col, row)->getTransform()->position.y + offset.y, 0, 255, true);
				m_getTile(col, row)->setTileStatus(IMPASSABLE);
			}
			if (m_getTile(col, row)->getNeighbourTile(BOTTOM_TILE) == nullptr)
			{
				TextureManager::Instance()->draw("log", m_getTile(col, row)->getTransform()->position.x + offset.x, m_getTile(col, row)->getTransform()->position.y + offset.y, 0, 255, true);
				m_getTile(col, row)->setTileStatus(IMPASSABLE);
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
	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
	updateDisplayList();

	m_CheckShipLOS(m_pTarget);
	m_CheckShipDR(m_pTarget);


	//if (currentEnemyAction == "Performing Patrol Action")
	//{
		/*m_pShip->setMaxSpeed(1.5f);
		m_pShip->moveForward();
		m_pShip->move();*/
	

	if (m_frameCounter % 20 == 0)
	{
		patrolSize--;
		m_pShip->getTransform()->position = m_pPatrolPath[patrolSize]->getTransform()->position + offset;
	}
	if (patrolSize == 0)
	{
		patrolSize = m_pPatrolPath.size();
	}
	if (m_frameCounter > 1000)
		m_frameCounter = 0;
	m_frameCounter++;
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

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance()->changeSceneState(END_SCENE);
	}

	if(EventManager::Instance().isKeyDown(SDL_SCANCODE_F))
	{

	}
	
	if(EventManager::Instance().isKeyDown(SDL_SCANCODE_M))
	{
	}

	// Toggles Debug Mode
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_H))
	{
		m_pShip->flipDbg();
	}
	
}

void PlayScene::start()
{
	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
	//Build node grid for overlaying map
	m_buildGrid();
	// Set GUI Title
	m_guiTitle = "Play Scene";

	// add the ship to the scene as a start point
	m_pShip = new Ship();
	m_pShip->getTransform()->position = m_getTile(4, 8)->getTransform()->position + offset;
	addChild(m_pShip, 2);
	
	// add the Obstacle to the scene as a start point
	m_pObstacle1 = new Obstacle("Rock.png", "Rock");
	m_pObstacle1->getTransform()->position = m_getTile(9,2)->getTransform()->position;
	addChild(m_pObstacle1);

	// add the Obstacle to the scene as a start point
	m_pObstacle2 = new Obstacle("Rock.png", "Rock");
	m_pObstacle2->getTransform()->position = m_getTile(9, 7)->getTransform()->position;
	addChild(m_pObstacle2);

	// add the Obstacle to the scene as a start point
	m_pObstacle3 = new Obstacle("Rock.png", "Rock");
	m_pObstacle3->getTransform()->position = m_getTile(14, 11)->getTransform()->position;
	addChild(m_pObstacle3);
	
	// added the target to the scene a goal
	m_pTarget = new Target();
	m_pTarget->getTransform()->position = m_getTile(16, 8)->getTransform()->position + offset;
	addChild(m_pTarget);

	// build patrol path list
	for (int i = 8; i <= 12; i++)
	{
		m_pPatrolPath.push_back(m_getTile(4, i));
	}
	for (int i = 5; i <= 13; i++)
	{
		m_pPatrolPath.push_back(m_getTile(i, 12));
	}
	m_pPatrolPath.push_back(m_getTile(13, 11));
	for (int i = 13; i <= 17; i++)
	{
		m_pPatrolPath.push_back(m_getTile(i, 10));
	}
	for (int i = 9; i >= 1; i--)
	{
		m_pPatrolPath.push_back(m_getTile(17, i));
	}
	for (int i = 16; i >= 1; i--)
	{
		m_pPatrolPath.push_back(m_getTile(i, 1));
	}
	for (int i = 1; i <= 8; i++)
	{
		m_pPatrolPath.push_back(m_getTile(1, i));
	}
	for (int i = 1; i < 4; i++)
	{
		m_pPatrolPath.push_back(m_getTile(i, 8));
	}

	for (Tile* element : m_pPatrolPath)
	{
		//std::cout << element->getGridPosition().x << element->getGridPosition().y << std::endl;
		m_pShip->getTransform()->position = element->getTransform()->position;
	}
	
	
	// create a dummy DecisionTree
	decisionTree = new DecisionTree();
	decisionTree->setAgent(m_pShip);
	decisionTree->Display();

	std::cout << "------------------------" << std::endl;
	std::cout << decisionTree->MakeDecision() << std::endl;
	std::cout << "------------------------\n" << std::endl;
	

	patrolSize = m_pPatrolPath.size();
	

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
				if ((object->getType() != m_pShip->getType()) && (object->getType() != target_object->getType()))
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
