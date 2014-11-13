#include "GameOwner.h"
#include "ActorComponentInterface.h"
#include "GameLogicManager.h"
#include "PhysicalComponent.h"
#include "VisualComponent.h"
#include "DisplayManager.h"
#include "GameStateComponent.h"
#include "SelectionToolBarComponent.h"
#include <math.h>
#include <sstream>

GameOwner::GameOwner(void)
{
}
GameOwner::~GameOwner(void)
{
}
void GameOwner::init(const char* actorsList)
{
	XMLDocument* doc = new XMLDocument();
    	doc->LoadFile(actorsList);
	XMLElement *playerFiles = doc->FirstChildElement();
	ConfiguredSHIFTTIME = playerFiles->IntAttribute("shiftTime");
	ConfiguredGAMETIME = playerFiles->IntAttribute("gameTime");

	font.loadFromFile("./src/arial.ttf");
	maxShift = 15;

	MaxPossibleScore = 10;//hardcoded
	srand(1);
	direction[0]= randomGravity();
	direction[1]= randomGravity();
	direction[2]= randomGravity();
	direction[3]= randomGravity();

	gameTime = 0; 
	shiftTime = 0;
	HasWinner = false;
	controlGame();
}
void GameOwner::update(double deltaMS)
{	
	ImplementGravity(deltaMS);
	
	//Check for Line Deletion
	LineDeletion();
	
	if(HasWinner)
		controlGame();
	
	updateScoreView();
}
bool GameOwner::ismoveableBlock(int blockId)
{
	if (blockId != BOUNDARYIDENTIFIER && blockId != EMPTYBLOCK && blockId != CONCRETEBLOCK)
		return true;
	else
		return false;
}
void GameOwner::ImplementGravity(double deltaMS)
{
	for(actorIterType iter = actorMap.begin(); iter != actorMap.end(); ++iter)
	{
		Actor* actor = (Actor*)iter->second;
		if(actor->actorType == "Map")
		{	

			gameTime += deltaMS;
			if (gameTime >= ConfiguredGAMETIME)
			{
				gameTime = 0;
				shiftTime += 1000;
				if (shiftTime >= ConfiguredSHIFTTIME)
				{
					shiftTime = 0;
					for (int i = 0; i < 3; i++)
					{
						direction[i] = direction[i+1];
					}
					direction[3] = randomGravity();
				}
				GameStateComponent* gameStateComponent = (GameStateComponent*)actor->GetComponent(GAMESTATE);	

				 switch(direction[0])
				 {
				   case SOUTH:
					for(int r=MAXROW-2;r>=0;r--)
					{
						for(int c=0;c<MAXCOL;c++)
						{
							//calling proper function to shift non-empty block
							if (ismoveableBlock(gameStateComponent->GameMap[r][c]))
								ShiftDownRequest(gameStateComponent->GameMap[r][c]);

						}
					}
					break;
					
				   case EAST: 
					for(int r=0;r<MAXROW;r++)
					{
						for(int c=MAXCOL-2;c>=0;c--)
						{
							//calling proper function to shift non-empty block
							if (ismoveableBlock(gameStateComponent->GameMap[r][c]))
								ShiftRightRequest(gameStateComponent->GameMap[r][c]);
						}
					}
					break;
				   case NORTH:
					for(int r=1;r<MAXROW;r++)
					{
						for(int c=0;c<MAXCOL;c++)
						{
							//calling proper function to shift non-empty block
							if (ismoveableBlock(gameStateComponent->GameMap[r][c]))
								ShiftUpRequest(gameStateComponent->GameMap[r][c]);
						}
					}
					break;
				   case WEST:
					for(int r=0;r<MAXROW;r++)
					{
						for(int c=1;c<MAXCOL;c++)
						{
							//calling proper function to shift non-empty block
							if (ismoveableBlock(gameStateComponent->GameMap[r][c]))
								ShiftLeftRequest(gameStateComponent->GameMap[r][c]);
						}
					}
					break;
				   default:
				     break;
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
int GameOwner::randomShapeSelection()
{
	TetrominoShape tetrominoShape = (TetrominoShape)(rand()% ((int)TOTALSHAPES-1)+1);
	return tetrominoShape;
}
int GameOwner::SelectedShape()
{
//	return randomShapeSelection();
	for(actorIterType iter = actorMap.begin(); iter != actorMap.end(); ++iter)
	{
		Actor* actor = (Actor*)iter->second;
		if(actor->actorType == "ShapeSelectionToolBar")
		{
			SelectionToolBarComponent* selectionToolBarComponent = (SelectionToolBarComponent*)actor->GetComponent(SELECTIONTOOLBAR);
			return selectionToolBarComponent->GetSelectedShape();
		}
	}
	return -1;
}
void GameOwner::CreateShapeRequest()
{
	for(actorIterType iter = actorMap.begin(); iter != actorMap.end(); ++iter)
	{
		Actor* actor = (Actor*)iter->second;
		if(actor->actorType == "Map")
		{
			GameStateComponent* gameStateComponent = (GameStateComponent*)actor->GetComponent(GAMESTATE);
			PhysicalComponent* physicalComponent = (PhysicalComponent*)actor->GetComponent(PHYSICAL);
			int blockSize = ((ActorShape::GridMap*)physicalComponent->actorShape)->blockSize;
			//Get mouse position in window
			sf::Vector2i localPosition = sf::Mouse::getPosition(DisplayManager::instance() -> window);
			
			//Convert position to grid
			int xGridPosition = floor((localPosition.x - physicalComponent->getActorPosition().x)/blockSize);
			int yGridPosition = floor((localPosition.y - physicalComponent->getActorPosition().x)/blockSize);
			
			int shapeId = SelectedShape();
			if(shapeId!=-1)//something seleceted check
				gameStateComponent->CreateNewShape((TetrominoShape)shapeId, yGridPosition, xGridPosition);
			else
				printf("Nothing Selected");
		}
	}
}
void GameOwner::SelectShapeRequest()
{
	for(actorIterType iter = actorMap.begin(); iter != actorMap.end(); ++iter)
	{
		Actor* actor = (Actor*)iter->second;
		if(actor->actorType == "ShapeSelectionToolBar")
		{
			SelectionToolBarComponent* selectionToolBarComponent = (SelectionToolBarComponent*)actor->GetComponent(SELECTIONTOOLBAR);
			PhysicalComponent* physicalComponent = (PhysicalComponent*)actor->GetComponent(PHYSICAL);
			int blockSize = ((ActorShape::GridMap*)physicalComponent->actorShape)->blockSize;
			//Get mouse position in window
			sf::Vector2i localPosition = sf::Mouse::getPosition(DisplayManager::instance() -> window);
		
			//Convert position to grid
			int xGridPosition = floor((localPosition.x - physicalComponent->getActorPosition().x)/blockSize);
			int yGridPosition = floor((localPosition.y - physicalComponent->getActorPosition().y)/blockSize);
			
			selectionToolBarComponent->SelectShape(yGridPosition, xGridPosition);
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

Direction GameOwner::randomGravity()
{
	int dirr = rand() % 4;
	if (dirr == 0)
		return (SOUTH);
	else if (dirr == 1)
		return (EAST);
	else if (dirr == 2)
		return (NORTH);
	else
		return (WEST);
}
Direction GameOwner::getDirection()
{
  return (direction[0]);
}

void GameOwner::LineDeletion()
{  
  //actorIterType iter = actorMap.begin();
  //Actor* actor = (Actor*)iter->second;

  bool topRowFlag = false;
  bool botRowFlag = false;
  bool topColFlag = false;
  bool botColFlag = false;

  for(actorIterType iter = actorMap.begin(); iter != actorMap.end(); ++iter)
	{
		Actor* actor = (Actor*)iter->second;
		if(actor->actorType == "Map")
		{
			GameStateComponent* gameStateComponent = (GameStateComponent*)actor->GetComponent(GAMESTATE);	
 
		  //Check top col. If full: delete top col
		  for (int r=0; r < gameStateComponent->CurrentGameRow;r++)
		  {
		    if(gameStateComponent->GameMap[r][0] == 0)
		    {
		      topColFlag = true;
		    }
		  }
		  
		  if(topColFlag == false)
		  {
		    for (int c=1; c < gameStateComponent->CurrentGameRow;c++)
		    {
		      for (int r=0; r < gameStateComponent->CurrentGameCol;r++)
		      {
				gameStateComponent->GameMap[r][c-1] = gameStateComponent->GameMap[r][c];
		      }
		    }
		    
		    for (int i=0; i < gameStateComponent->CurrentGameCol; i++)
		    {
		      gameStateComponent->GameMap[i][gameStateComponent->CurrentGameCol] = 0;
		    }
		  }
		  
		  //Check bottom col. If full: delete bottom col
		  for (int r=0; r < gameStateComponent->CurrentGameRow;r++)
		  {
		    if(gameStateComponent->GameMap[r][gameStateComponent->CurrentGameCol] == 0)
		    {
		      botColFlag = true;
		    }
		  }
		  
		  if(botColFlag == false)
		  {
		    for (int c=gameStateComponent->CurrentGameRow - 1; c > 0;c--)
		    {
		      for (int r=0; r < gameStateComponent->CurrentGameCol;r++)
		      {
				gameStateComponent->GameMap[r][c + 1] = gameStateComponent->GameMap[r][c];
		      }
		    }
		    
		    for (int i=0; i < gameStateComponent->CurrentGameCol; i++)
		    {
		      gameStateComponent->GameMap[i][0] = 0;
		    }
		  }
		  
		  //Check top row. If full: delete top row
		  for (int r=0; r < gameStateComponent->CurrentGameCol;r++)
		  {
		    if(gameStateComponent->GameMap[0][r] == 0)
		    {
		      topRowFlag = true;
		    }
		  }
		  
		  if(topRowFlag == false)
		  {
		    for (int c=0; c < gameStateComponent->CurrentGameRow;c++)
		    {
		      for (int r=1; r < gameStateComponent->CurrentGameCol;r++)
		      {
				gameStateComponent->GameMap[r-1][c] = gameStateComponent->GameMap[r][c];
		      }
		    }
		    
		    for (int i=0; i < gameStateComponent->CurrentGameRow; i++)
		    {
		      gameStateComponent->GameMap[gameStateComponent->CurrentGameRow][i] = 0;
		    }
		  }
		  
		    //Check bot row. If full: delete bot row
		  for (int r=0; r < gameStateComponent->CurrentGameCol;r++)
		  {
		    if(gameStateComponent->GameMap[gameStateComponent->CurrentGameCol][r] == 0)
		    {
		      botRowFlag = true;
		    }
		  }
		  
		  if(botRowFlag == false)
		  {
		    for (int c=0; c < gameStateComponent->CurrentGameRow;c++)
		    {
		      for (int r=gameStateComponent->CurrentGameCol; r > 0;r--)
		      {
				gameStateComponent->GameMap[r+1][c] = gameStateComponent->GameMap[r][c];
		      }
		    }
		    
		    for (int i=0; i < gameStateComponent->CurrentGameRow; i++)
		    {
		      gameStateComponent->GameMap[0][i] = 0;
		    }
		  }
		}
	}
}



