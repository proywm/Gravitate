#include "GameOwner.h"
#include "ActorComponentInterface.h"
#include "GameLogicManager.h"
#include "PhysicalComponent.h"
#include "VisualComponent.h"
#include "DisplayManager.h"
#include "SelectionToolBarComponent.h"
#include <set>
#include <math.h>
#include <sstream>
#include <stdlib.h>

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

	font.loadFromFile("./resources/fonts/arial.ttf");
	maxShift = 15;

	MaxPossibleScore = 10;//hardcoded
	srand(1);
	direction[0]= randomGravity();
	direction[1]= randomGravity();
	direction[2]= randomGravity();
	direction[3]= randomGravity();

	//Init sounds
	if (!buffer.loadFromFile("./resources/sounds/GravityChange.wav"))
	    exit (EXIT_FAILURE);
	sound.setBuffer(buffer);
	sound.setVolume(65);
	if (!buffer2.loadFromFile("./resources/sounds/PiecePlacement.wav"))
	    exit (EXIT_FAILURE);
	sound2.setBuffer(buffer2);
	sound2.setVolume(18);
	//sound2.setPitch(1.2);
	if (!buffer3.loadFromFile("./resources/sounds/LineDeletion.wav"))
	    exit (EXIT_FAILURE);
	sound3.setBuffer(buffer3);
	if (!buffer4.loadFromFile("./resources/sounds/ForcedDeletion.wav"))
	    exit (EXIT_FAILURE);
	sound4.setBuffer(buffer4);

	gameTime = 0; 
	shiftTime = 0;
	score = 0;
	HasWinner = false;
	controlGame();
}
void GameOwner::update(double deltaMS)
{	
	ImplementGravity(deltaMS);
	
	ShowCursor();
	
	//Check for Line Deletion
	LineDeletion();

	 // No negative scores!!
	if (score < 0){
		score = 0;
	}
	
	if(HasWinner)
		controlGame();
	
	updateScoreView();
}
bool GameOwner::ismoveableBlock(int blockId)
{
	if (blockId != BOUNDARYIDENTIFIER && blockId != EMPTYBLOCK && blockId != HOVERBLOCK && blockId != CONCRETEBLOCK)
		return true;
	else
		return false;
}
bool GameOwner::isEmptyBlock(int blockId)
{
	return blockId == EMPTYBLOCK || blockId == HOVERBLOCK;
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
					sound.play();
				}
				GameStateComponent* gameStateComponent = (GameStateComponent*)actor->GetComponent(GAMESTATE);	
				std::set<int> pieces; 

				switch(direction[0])
				{
				    case SOUTH:
					for(int r=MAXROW-2;r>=0;r--)
					{
						for(int c=0;c<MAXCOL;c++)
						{
							int id = gameStateComponent->GameMap[r][c];
							//calling proper function to shift non-empty block
							if (ismoveableBlock(id) && (pieces.find(id) == pieces.end()) )
							{
								pieces.insert(id);
								ShiftDownRequest(id);
							}
						}
					}
					break;
					
				   case EAST: 
					for(int r=0;r<MAXROW;r++)
					{
						for(int c=MAXCOL-2;c>=0;c--)
						{
							int id = gameStateComponent->GameMap[r][c];
							//calling proper function to shift non-empty block
							if (ismoveableBlock(id) && (pieces.find(id) == pieces.end()) )
							{
								pieces.insert(id);
								ShiftRightRequest(id);
							}
						}
					}
					break;
				   case NORTH:
					for(int r=1;r<MAXROW;r++)
					{
						for(int c=0;c<MAXCOL;c++)
						{
							int id = gameStateComponent->GameMap[r][c];
							//calling proper function to shift non-empty block
							if (ismoveableBlock(id) && (pieces.find(id) == pieces.end()) )
							{
								pieces.insert(id);
								ShiftUpRequest(id);
							}
						}
					}
					break;
				   case WEST:
					for(int r=0;r<MAXROW;r++)
					{
						for(int c=1;c<MAXCOL;c++)
						{
							int id = gameStateComponent->GameMap[r][c];
							//calling proper function to shift non-empty block
							if (ismoveableBlock(id) && (pieces.find(id) == pieces.end()) )
							{
								pieces.insert(id);
								ShiftLeftRequest(id);
							}
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
void GameOwner::ShowCursor()
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
			if(shapeId!=-1)//something selected check
				CreateNewShape(gameStateComponent->GameMap, (TetrominoShape)shapeId, yGridPosition, xGridPosition);
			else
			//	printf("Nothing Selected\n");
			;
		}
	}
}
void GameOwner::CreateNewShape(int (*GameMap)[MAXCOL], TetrominoShape tetrominoShape, int xCorr, int yCorr)
{
	for(int r=0;r<MAXROW;r++)
	{
		for(int c=0;c<MAXCOL;c++)
		{
			if (GameMap[r][c] == HOVERBLOCK)
				GameMap[r][c] = EMPTYBLOCK;
		}
	}
	switch(tetrominoShape)
	{
		case STRAIGHTPOLYOMINO:
			if(GameMap[xCorr][yCorr] == EMPTYBLOCK)
				GameMap[xCorr][yCorr] = HOVERBLOCK;
			if(GameMap[xCorr+1][yCorr] == EMPTYBLOCK)
				GameMap[xCorr+1][yCorr] = HOVERBLOCK;
			if(GameMap[xCorr+2][yCorr] == EMPTYBLOCK)
				GameMap[xCorr+2][yCorr] = HOVERBLOCK;
			if(GameMap[xCorr+3][yCorr] == EMPTYBLOCK)
				GameMap[xCorr+3][yCorr] = HOVERBLOCK;
			break; 
		case SQUAREPOLYOMINO:
			if(GameMap[xCorr][yCorr] == EMPTYBLOCK)
				GameMap[xCorr][yCorr] = HOVERBLOCK;
			if(GameMap[xCorr+1][yCorr] == EMPTYBLOCK)
				GameMap[xCorr+1][yCorr] = HOVERBLOCK;
			if(GameMap[xCorr][yCorr+1] == EMPTYBLOCK)
				GameMap[xCorr][yCorr+1] = HOVERBLOCK;
			if(GameMap[xCorr+1][yCorr+1] == EMPTYBLOCK)
				GameMap[xCorr+1][yCorr+1] = HOVERBLOCK;
			break;
		case TPOLYOMINO:
			if(GameMap[xCorr][yCorr] == EMPTYBLOCK)
				GameMap[xCorr][yCorr] = HOVERBLOCK;
			if(GameMap[xCorr+1][yCorr] == EMPTYBLOCK)
				GameMap[xCorr+1][yCorr] = HOVERBLOCK;
			if(GameMap[xCorr+2][yCorr] == EMPTYBLOCK)
				GameMap[xCorr+2][yCorr] = HOVERBLOCK;
			if(GameMap[xCorr+1][yCorr+1] == EMPTYBLOCK)
				GameMap[xCorr+1][yCorr+1] = HOVERBLOCK;
			break;
		case JPOLYOMINO:
			if(GameMap[xCorr][yCorr] == EMPTYBLOCK)
				GameMap[xCorr][yCorr] = HOVERBLOCK;
			if(GameMap[xCorr][yCorr+1] == EMPTYBLOCK)
				GameMap[xCorr][yCorr+1] = HOVERBLOCK;
			if(GameMap[xCorr][yCorr+2] == EMPTYBLOCK)
				GameMap[xCorr][yCorr+2] = HOVERBLOCK;
			if(GameMap[xCorr-1][yCorr+2] == EMPTYBLOCK)
				GameMap[xCorr-1][yCorr+2] = HOVERBLOCK;
			break;
		case LPOLYOMINO:
			if(GameMap[xCorr][yCorr] == EMPTYBLOCK)
				GameMap[xCorr][yCorr] = HOVERBLOCK;
			if(GameMap[xCorr][yCorr+1] == EMPTYBLOCK)
				GameMap[xCorr][yCorr+1] = HOVERBLOCK;
			if(GameMap[xCorr][yCorr+2] == EMPTYBLOCK)
				GameMap[xCorr][yCorr+2] = HOVERBLOCK;
			if(GameMap[xCorr+1][yCorr+2] == EMPTYBLOCK)
				GameMap[xCorr+1][yCorr+2] = HOVERBLOCK;
			break;
		case SPOLYOMINO:
			if(GameMap[xCorr][yCorr] == EMPTYBLOCK)
				GameMap[xCorr][yCorr] = HOVERBLOCK;
			if(GameMap[xCorr+1][yCorr] == EMPTYBLOCK)
				GameMap[xCorr+1][yCorr] = HOVERBLOCK;
			if(GameMap[xCorr][yCorr+1] == EMPTYBLOCK)
				GameMap[xCorr][yCorr+1] = HOVERBLOCK;
			if(GameMap[xCorr-1][yCorr+1] == EMPTYBLOCK)
				GameMap[xCorr-1][yCorr+1] = HOVERBLOCK;
			break;
		case ZPOLYOMINO:
			if(GameMap[xCorr][yCorr] == EMPTYBLOCK)
				GameMap[xCorr][yCorr] = HOVERBLOCK;
			if(GameMap[xCorr+1][yCorr] == EMPTYBLOCK)
				GameMap[xCorr+1][yCorr] = HOVERBLOCK;
			if(GameMap[xCorr+1][yCorr+1] == EMPTYBLOCK)
				GameMap[xCorr+1][yCorr+1] = HOVERBLOCK;
			if(GameMap[xCorr+2][yCorr+1] == EMPTYBLOCK)
				GameMap[xCorr+2][yCorr+1] = HOVERBLOCK;
			break;
		default:
			break;
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
			{
				gameStateComponent->CreateNewShape((TetrominoShape)shapeId, yGridPosition, xGridPosition);
				sound2.play();
				
			}
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
		return (SOUTH); //SOUTH
	else if (dirr == 1)
		return (EAST);//EAST
	else if (dirr == 2)
		return (NORTH); //NORTH
	else
		return (WEST); //WEST
}
Direction GameOwner::getDirection()
{
  return (direction[0]);
}

void GameOwner::LineDeletion()
{  
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
 
		  //Check top col. If full: delete top col. LEFT.
		  for (int r=0; r < gameStateComponent->CurrentGameRow;r++)
		  {
		    if(gameStateComponent->GameMap[r][0] < ACTIVEBLOCK)
		    {
		      topColFlag = true;
		    }
		  }
		  
		  if(topColFlag == false)
		  {
		    std::cout<<"LINEDELETION";
		    
		    // YOU SCORED!
		    score = score + 1000;
			sound3.play();
		    
		    for (int c=1; c < gameStateComponent->CurrentGameRow;c++)
		    {
		      for (int r=0; r < gameStateComponent->CurrentGameCol;r++)
		      {
				gameStateComponent->GameMap[r][c-1] = gameStateComponent->GameMap[r][c];
		      }
		    }
		    
		    for (int i=0; i < gameStateComponent->CurrentGameCol; i++)
		    {
		      gameStateComponent->GameMap[i][gameStateComponent->CurrentGameCol] = EMPTYBLOCK;
		    }
		  }
			
			
		  
		  //Check bottom col. If full: delete bottom col. RIGHT.
		  for (int r=0; r < gameStateComponent->CurrentGameRow;r++)
		  {
		    if(gameStateComponent->GameMap[r][gameStateComponent->CurrentGameCol - 1] < ACTIVEBLOCK)
		    {
		      botColFlag = true;
		    }
		  }
		  
		  if(botColFlag == false)
		  {
		    std::cout<<"LINEDELETION";
		    
		    // YOU SCORED!
		    score = score + 1000;
			sound3.play();
		    
		    for (int c=gameStateComponent->CurrentGameRow - 2; c > 0;c--)
		    {
		      for (int r=0; r < gameStateComponent->CurrentGameCol;r++)
		      {
				gameStateComponent->GameMap[r][c + 1] = gameStateComponent->GameMap[r][c];
		      }
		    }
		    
		    for (int i=0; i < gameStateComponent->CurrentGameCol; i++)
		    {
		      gameStateComponent->GameMap[i][0] = EMPTYBLOCK;
		    }
		  }
		  
		  //Check top row. If full: delete top row TOP.
		  for (int r=0; r < gameStateComponent->CurrentGameCol;r++)
		  {
		   // std::cout<<"checcord"<<gameStateComponent->GameMap[0][r];

		    if(gameStateComponent->GameMap[0][r] < ACTIVEBLOCK)
		    {
		      topRowFlag = true;
		    }
		  }
		  
		  if(topRowFlag == false)
		  {
		    std::cout<<"LINEDELETION";
		    
		    // YOU SCORED!
		    score = score + 1000;
			sound3.play();

		    for (int c=0; c < gameStateComponent->CurrentGameRow;c++)
		    {
		      for (int r=1; r < gameStateComponent->CurrentGameCol;r++)
		      {
				gameStateComponent->GameMap[r-1][c] = gameStateComponent->GameMap[r][c];
		      }
		    }
		    
		    for (int i=0; i < gameStateComponent->CurrentGameRow; i++)
		    {
		      gameStateComponent->GameMap[gameStateComponent->CurrentGameRow][i] = EMPTYBLOCK;
		    }
		  }
		  
		    //Check bot row. If full: delete bot row. BOTTOM.
		  for (int r=0; r < gameStateComponent->CurrentGameCol;r++)
		  {
		    if(gameStateComponent->GameMap[gameStateComponent->CurrentGameCol -1][r] < ACTIVEBLOCK)
		    {
		      botRowFlag = true;
		    }
		  }
		  
		  if(botRowFlag == false)
		  {
		    std::cout<<"LINEDELETION";
		    
		     // YOU SCORED!
		    score = score + 1000;
			sound3.play();
		    
		    for (int c=0; c < gameStateComponent->CurrentGameRow;c++)
		    {
		      for (int r=gameStateComponent->CurrentGameCol - 2; r >= 0;r--)
		      {
				gameStateComponent->GameMap[r+1][c] = gameStateComponent->GameMap[r][c];
		      }
		    }
		    
		    for (int i=0; i < gameStateComponent->CurrentGameRow; i++)
		    {
		      gameStateComponent->GameMap[0][i] = EMPTYBLOCK;
		    }
		  }
		  
		  
		}	
	}
}

 //If Red button is pushed, bottom row or column relative to gravity is deleted. Points deducted based on how many spaces are filled in row/col. 25 points for each
 //space not filled in on row/col to be deleted.
void GameOwner::ForcedDeletion()
{
	std::cout<<"Forced Deletion";
	int pointCounter = 0;

	for(actorIterType iter = actorMap.begin(); iter != actorMap.end(); ++iter)
	{
		Actor* actor = (Actor*)iter->second;
		if(actor->actorType == "Map")
		{
			GameStateComponent* gameStateComponent = (GameStateComponent*)actor->GetComponent(GAMESTATE);	
 
			switch(direction[0])
			{
				case SOUTH:

				for(int k=0; k < gameStateComponent->CurrentGameRow; k++)
				{
					if(gameStateComponent->GameMap[gameStateComponent->CurrentGameCol - 1][k] >  HOVERBLOCK)
					{
						pointCounter++;
					}
				}

				for (int c=0; c < gameStateComponent->CurrentGameRow;c++)
		    	{
		      		for (int r=gameStateComponent->CurrentGameCol - 2; r >= 0;r--)
		      		{
						gameStateComponent->GameMap[r+1][c] = gameStateComponent->GameMap[r][c];
		      		}
		    	}
		    
		    	for (int i=0; i < gameStateComponent->CurrentGameRow; i++)
		    	{
		     		gameStateComponent->GameMap[0][i] = EMPTYBLOCK;
		    	}

		    	score = score - ((gameStateComponent->CurrentGameRow - pointCounter) * 25);
				sound4.play();

				break;

				case EAST:
					for(int k=0; k < gameStateComponent->CurrentGameCol; k++)
					{
						if(gameStateComponent->GameMap[k][gameStateComponent->CurrentGameRow - 1] > HOVERBLOCK)
						{
							pointCounter++;
						}
					}

					for (int c=gameStateComponent->CurrentGameRow - 2; c > 0;c--)
			    	{
			      		for (int r=0; r < gameStateComponent->CurrentGameCol;r++)
			      		{
							gameStateComponent->GameMap[r][c + 1] = gameStateComponent->GameMap[r][c];
			      		}
			   		}
			    
			  		for (int i=0; i < gameStateComponent->CurrentGameCol; i++)
			    	{
			      		gameStateComponent->GameMap[i][0] = EMPTYBLOCK;
			    	}

		    		score = score - ((gameStateComponent->CurrentGameCol - pointCounter) * 25);
					sound4.play();

					break;

				case NORTH:

					for(int k=0; k < gameStateComponent->CurrentGameRow; k++)
					{
						if(gameStateComponent->GameMap[0][k] > HOVERBLOCK)
						{
							pointCounter++;
						}
					}
					for (int c=0; c < gameStateComponent->CurrentGameRow;c++)
			    	{
			      		for (int r=1; r < gameStateComponent->CurrentGameCol;r++)
			     		{
							gameStateComponent->GameMap[r-1][c] = gameStateComponent->GameMap[r][c];
			      		}
			   		}
			    
			    	for (int i=0; i < gameStateComponent->CurrentGameRow; i++)
			    	{
			      		gameStateComponent->GameMap[gameStateComponent->CurrentGameRow][i] = EMPTYBLOCK;
			    	}

		    		score = score - ((gameStateComponent->CurrentGameRow - pointCounter) * 25);
					sound4.play();

					break;

				case WEST:

					for(int k=0; k < gameStateComponent->CurrentGameCol; k++)
					{
						if(gameStateComponent->GameMap[k][0] > HOVERBLOCK)
						{
							pointCounter++;
						}
					}
					
					for (int c=1; c < gameStateComponent->CurrentGameRow;c++)
			    	{
			      		for (int r=0; r < gameStateComponent->CurrentGameCol;r++)
			      		{
							gameStateComponent->GameMap[r][c-1] = gameStateComponent->GameMap[r][c];
			      		}
			    	}
			    	for (int i=0; i < gameStateComponent->CurrentGameCol; i++)
			    	{
			      		gameStateComponent->GameMap[i][gameStateComponent->CurrentGameCol] = EMPTYBLOCK;
			   		}

		    		score = score - ((gameStateComponent->CurrentGameCol - pointCounter) * 25);
					sound4.play();

					break;

				default:
				 break;
			}
		}
	}
}