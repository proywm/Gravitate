#include "GameOwner.h"
#include "ActorComponentInterface.h"
#include "GameLogicManager.h"
#include "PhysicalComponent.h"
#include "DisplayManager.h"
#include "GameStateComponent.h"
#include <math.h>
#include <sstream>

#define SHIFTTIME 5000
#define GAMETIME 1000
GameOwner::GameOwner(void)
{
}
GameOwner::~GameOwner(void)
{
}
void GameOwner::init(const char* actorsList)
{
	font.loadFromFile("./src/arial.ttf");

	MaxPossibleScore = 10;//hardcoded
	direction = 0; //hardcoded
	gameTime = 0; 
	shiftTime = 0;
	HasWinner = false;
	controlGame();
}
void GameOwner::update(double deltaMS)
{	
	ImplementGravity(deltaMS);
	if(HasWinner)
		controlGame();
	
	updateScoreView();
}
void GameOwner::ImplementGravity(double deltaMS)
{
	for(actorIterType iter = actorMap.begin(); iter != actorMap.end(); ++iter)
	{
		Actor* actor = (Actor*)iter->second;
		if(actor->actorType == "Map")
		{	
			shiftTime += deltaMS;
			if (shiftTime >= SHIFTTIME)
			{
				shiftTime = 0;
				bool random = true;
				if (random)
					direction = rand();
				else
					direction ++;
					
				direction %= 4;
				printf("New gravity shift (0:SOUTH,1:EAST,2:NORTH,3:WEST): %d", direction);
			}
			
			gameTime += deltaMS;
			if (gameTime >= GAMETIME)
			{
				gameTime = 0;
				
				GameStateComponent* gameStateComponent = (GameStateComponent*)actor->GetComponent(GAMESTATE);
				//int** map = gameStateComponent->GameMap;
/* 				if (c == 1 && r == 1) printf("A");
				else if (c == 48 && r == 1) printf("B");
				else if (c == 1 && r == 48) printf("C");
				else if (c == 48 && r == 48) printf("D");
				else
				printf("%d", gameStateComponent->GameMap[r][c]); */	
				if (direction == 0)
				{
					for(int r=MAXROW-2;r>=0;r--)
					{
						for(int c=0;c<MAXCOL;c++)
						{
							if (gameStateComponent->GameMap[r][c] == NOTPLACEDBLOCK && 
								gameStateComponent->GameMap[r+1][c] == EMPTYBLOCK)
							{
								gameStateComponent->GameMap[r][c] = EMPTYBLOCK;
								gameStateComponent->GameMap[r+1][c] = NOTPLACEDBLOCK;
							}
						}
					}

				} else if (direction == 1)
				{
					for(int r=0;r<MAXROW;r++)
					{
						for(int c=MAXCOL-2;c>=0;c--)
						{
							if (gameStateComponent->GameMap[r][c] == NOTPLACEDBLOCK && 
								gameStateComponent->GameMap[r][c+1] == EMPTYBLOCK)
							{
								gameStateComponent->GameMap[r][c] = EMPTYBLOCK;
								gameStateComponent->GameMap[r][c+1] = NOTPLACEDBLOCK;
							}
						}
					}

				} else if (direction == 2)
				{
					for(int r=1;r<MAXROW;r++)
					{
						for(int c=0;c<MAXCOL;c++)
						{
							if (gameStateComponent->GameMap[r][c] == NOTPLACEDBLOCK && 
								gameStateComponent->GameMap[r-1][c] == EMPTYBLOCK)
							{
								gameStateComponent->GameMap[r][c] = EMPTYBLOCK;
								gameStateComponent->GameMap[r-1][c] = NOTPLACEDBLOCK;
							}
						}
					}

				} else //if (direction == 3)
				{
					for(int r=0;r<MAXROW;r++)
					{
						for(int c=1;c<MAXCOL;c++)
						{
							if (gameStateComponent->GameMap[r][c] == NOTPLACEDBLOCK && 
								gameStateComponent->GameMap[r][c-1] == EMPTYBLOCK)
							{
								gameStateComponent->GameMap[r][c] = EMPTYBLOCK;
								gameStateComponent->GameMap[r][c-1] = NOTPLACEDBLOCK;
							}
						}
					}
				}
			}
		}
	}
}
void GameOwner::HandleEvent(sf::Event receivedEvent)
{
	if(receivedEvent.key.code == sf::Keyboard::Q)
	{
		DisplayManager::instance()->window.close();
	}
	else if(receivedEvent.key.code == sf::Keyboard::R)
	{
		RestartGame();
	}
}
void GameOwner::ShiftLeftRequest(int shapeId)
{
	for(actorIterType iter = actorMap.begin(); iter != actorMap.end(); ++iter)
	{
		Actor* actor = (Actor*)iter->second;
		if(actor->actorType == "Map")
		{
			GameStateComponent* gameStateComponent = (GameStateComponent*)actor->GetComponent(GAMESTATE);
			//hardcoded shapes; ToDo: Make dynamic
			gameStateComponent->ShiftLeft(shapeId);
		}
	}
}
void GameOwner::ShiftRightRequest(int shapeId)
{
	for(actorIterType iter = actorMap.begin(); iter != actorMap.end(); ++iter)
	{
		Actor* actor = (Actor*)iter->second;
		if(actor->actorType == "Map")
		{
			GameStateComponent* gameStateComponent = (GameStateComponent*)actor->GetComponent(GAMESTATE);
			//hardcoded shapes; ToDo: Make dynamic
			gameStateComponent->ShiftRight(shapeId);
		}
	}
}
void GameOwner::ShiftUpRequest(int shapeId)
{
	for(actorIterType iter = actorMap.begin(); iter != actorMap.end(); ++iter)
	{
		Actor* actor = (Actor*)iter->second;
		if(actor->actorType == "Map")
		{
			GameStateComponent* gameStateComponent = (GameStateComponent*)actor->GetComponent(GAMESTATE);
			//hardcoded shapes; ToDo: Make dynamic
			gameStateComponent->ShiftUp(shapeId);
		}
	}
}
void GameOwner::ShiftDownRequest(int shapeId)
{
	for(actorIterType iter = actorMap.begin(); iter != actorMap.end(); ++iter)
	{
		Actor* actor = (Actor*)iter->second;
		if(actor->actorType == "Map")
		{
			GameStateComponent* gameStateComponent = (GameStateComponent*)actor->GetComponent(GAMESTATE);
			//hardcoded shapes; ToDo: Make dynamic
			gameStateComponent->ShiftDown(shapeId);
		}
	}
}
void GameOwner::RotateCounterClockwiseRequest(int shapeId)
{
	for(actorIterType iter = actorMap.begin(); iter != actorMap.end(); ++iter)
	{
		Actor* actor = (Actor*)iter->second;
		if(actor->actorType == "Map")
		{
			GameStateComponent* gameStateComponent = (GameStateComponent*)actor->GetComponent(GAMESTATE);
			//hardcoded shapes; ToDo: Make dynamic
			gameStateComponent->RotateCounterClockwise(shapeId);
		}
	}
}
void GameOwner::RotateClockwiseRequest(int shapeId)
{
	for(actorIterType iter = actorMap.begin(); iter != actorMap.end(); ++iter)
	{
		Actor* actor = (Actor*)iter->second;
		if(actor->actorType == "Map")
		{
			GameStateComponent* gameStateComponent = (GameStateComponent*)actor->GetComponent(GAMESTATE);
			//hardcoded shapes; ToDo: Make dynamic
			gameStateComponent->RotateClockwise(shapeId);
		}
	}
}
void GameOwner::CreateShapeRequest()
{
	for(actorIterType iter = actorMap.begin(); iter != actorMap.end(); ++iter)
	{
		Actor* actor = (Actor*)iter->second;
		if(actor->actorType == "Map")
		{
			GameStateComponent* gameStateComponent = (GameStateComponent*)actor->GetComponent(GAMESTATE);
			//hardcoded shapes; ToDo: Make dynamic

			//Get mouse position in window
			sf::Vector2i localPosition = sf::Mouse::getPosition(DisplayManager::instance() -> window);
			
			//Convert position to grid
			int xGridPosition = floor((localPosition.x - 100.0)/15.0);
			int yGridPosition = floor((localPosition.y - 100.0)/15.0);

			gameStateComponent->CreateNewShape(ZPOLYOMINO, yGridPosition, xGridPosition);
		}
	}
}
void GameOwner::controlGame(void)
{
	//Hard Coded: ToDO: Make Dynamic Game Control
	if(HasWinner)
	{
		ShowWinner(winner);
		StopGame();
	}
	else
	{
		startGame();
	}
}
void GameOwner::startGame(void)
{
	
}
void GameOwner::StopGame()
{
	
}
void GameOwner::ShowWinner(GamePlayer* winner)
{
	
}
void GameOwner::updateScoreView()
{
	
}
void GameOwner::RestartGame()
{
	
	controlGame();
}
