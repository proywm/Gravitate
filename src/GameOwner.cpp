#include "GameOwner.h"
#include "ActorComponentInterface.h"
#include "GameLogicManager.h"
#include "PhysicalComponent.h"
#include "VisualComponent.h"
#include "DisplayManager.h"
#include "GameStateComponent.h"
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

	sf::Music music;
	if (!music.openFromFile("./resources/sounds/BGMusic.wav"))
    	exit (EXIT_FAILURE);
    music.setLoop(true);
    music.setVolume(100);
	music.play();

	font.loadFromFile("./resources/fonts/arial.ttf");
	maxShift = 15;

	MaxPossibleScore = 10;//hardcoded
	srand(1);
	direction[0]= randomGravity();
	direction[1]= randomGravity();
	direction[2]= randomGravity();
	direction[3]= randomGravity();

	gameTime = 0; 
	shiftTime = 0;
	score = 0;
	HasWinner = false;
	controlGame();
}
void GameOwner::update(double deltaMS)
{	
	ImplementGravity(deltaMS);
	
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

					sf::SoundBuffer buffer;
	    			if (!buffer.loadFromFile("./resources/sounds/GravityChange.wav"))
	        			exit (EXIT_FAILURE);
	    			sf::Sound sound;
					sound.setBuffer(buffer);
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
				sf::SoundBuffer buffer2;
	    		if (!buffer2.loadFromFile("./resources/sounds/ForcedDeletion.wav"))
	        		exit (EXIT_FAILURE);
	    		sf::Sound sound2;
				sound2.setBuffer(buffer2);
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
		    if(gameStateComponent->GameMap[r][0] < 1)
		    {
		      topColFlag = true;
		    }
		  }
		  
		  if(topColFlag == false)
		  {
		    std::cout<<"LINEDELETION";
		    
		    // YOU SCORED!
		    score = score + 1000;

		    sf::SoundBuffer buffer3;
    		if (!buffer3.loadFromFile("./resources/sounds/LineDeletion.wav"))
        		exit (EXIT_FAILURE);
    		sf::Sound sound3;
			sound3.setBuffer(buffer3);
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
		      gameStateComponent->GameMap[i][gameStateComponent->CurrentGameCol] = 0;
		    }
		  }
			
			
		  
		  //Check bottom col. If full: delete bottom col. RIGHT.
		  for (int r=0; r < gameStateComponent->CurrentGameRow;r++)
		  {
		    if(gameStateComponent->GameMap[r][gameStateComponent->CurrentGameCol - 1] < 1)
		    {
		      botColFlag = true;
		    }
		  }
		  
		  if(botColFlag == false)
		  {
		    std::cout<<"LINEDELETION";
		    
		    // YOU SCORED!
		    score = score + 1000;

		    sf::SoundBuffer buffer4;
    		if (!buffer4.loadFromFile("./resources/sounds/LineDeletion.wav"))
        		exit (EXIT_FAILURE);
    		sf::Sound sound4;
			sound4.setBuffer(buffer4);
			sound4.play();
		    
		    for (int c=gameStateComponent->CurrentGameRow - 2; c > 0;c--)
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
		  
		  //Check top row. If full: delete top row TOP.
		  for (int r=0; r < gameStateComponent->CurrentGameCol;r++)
		  {
		   // std::cout<<"checcord"<<gameStateComponent->GameMap[0][r];

		    if(gameStateComponent->GameMap[0][r] < 1)
		    {
		      topRowFlag = true;
		    }
		  }
		  
		  if(topRowFlag == false)
		  {
		    std::cout<<"LINEDELETION";
		    
		    // YOU SCORED!
		    score = score + 1000;

		    sf::SoundBuffer buffer5;
    		if (!buffer5.loadFromFile("./resources/sounds/LineDeletion.wav"))
        		exit (EXIT_FAILURE);
    		sf::Sound sound5;
			sound5.setBuffer(buffer5);
			sound5.play();

		    
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
		  
		    //Check bot row. If full: delete bot row. BOTTOM.
		  for (int r=0; r < gameStateComponent->CurrentGameCol;r++)
		  {
		    if(gameStateComponent->GameMap[gameStateComponent->CurrentGameCol -1][r] < 1)
		    {
		      botRowFlag = true;
		    }
		  }
		  
		  if(botRowFlag == false)
		  {
		    std::cout<<"LINEDELETION";
		    
		     // YOU SCORED!
		    score = score + 1000;

		    sf::SoundBuffer buffer6;
    		if (!buffer6.loadFromFile("./resources/sounds/PiecePlacement.wav"))
        		exit (EXIT_FAILURE);
    		sf::Sound sound6;
			sound6.setBuffer(buffer6);
			sound6.play();
		    
		    for (int c=0; c < gameStateComponent->CurrentGameRow;c++)
		    {
		      for (int r=gameStateComponent->CurrentGameCol - 2; r >= 0;r--)
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

 //If Red button is pushed, bottom row or column relative to gravity is deleted. Points deducted based on how many spaces are filled in row/col. 25 points for each
 //space not filled in on row/col to be deleted.
void GameOwner::ForcedDeletion()
{
	std::cout<<"Forced Deletion";
	int pointCounter = 0;
	sf::SoundBuffer buffer7;
	sf::SoundBuffer buffer8;
	sf::SoundBuffer buffer9;
	sf::SoundBuffer buffer10;

	sf::Sound sound7;
	sf::Sound sound8;
	sf::Sound sound9;
	sf::Sound sound10;


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
					if(gameStateComponent->GameMap[gameStateComponent->CurrentGameCol - 1][k] > 0)
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
		     		gameStateComponent->GameMap[0][i] = 0;
		    	}

		    	score = score - ((gameStateComponent->CurrentGameRow - pointCounter) * 25);

    			if (!buffer7.loadFromFile("./resources/sounds/ForcedDeletion.wav"))
        			exit (EXIT_FAILURE);
				sound7.setBuffer(buffer7);
				sound7.play();

				break;

				case EAST:
					for(int k=0; k < gameStateComponent->CurrentGameCol; k++)
					{
						if(gameStateComponent->GameMap[k][gameStateComponent->CurrentGameRow - 1] > 0)
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
			      		gameStateComponent->GameMap[i][0] = 0;
			    	}

		    		score = score - ((gameStateComponent->CurrentGameCol - pointCounter) * 25);

	    			if (!buffer8.loadFromFile("./resources/sounds/ForcedDeletion.wav"))
	        			exit (EXIT_FAILURE);
					sound8.setBuffer(buffer8);
					sound8.play();

					break;

				case NORTH:

					for(int k=0; k < gameStateComponent->CurrentGameRow; k++)
					{
						if(gameStateComponent->GameMap[0][k] > 0)
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
			      		gameStateComponent->GameMap[gameStateComponent->CurrentGameRow][i] = 0;
			    	}

		    		score = score - ((gameStateComponent->CurrentGameRow - pointCounter) * 25);

	    			if (!buffer9.loadFromFile("./resources/sounds/ForcedDeletion.wav"))
	        			exit (EXIT_FAILURE);
					sound9.setBuffer(buffer9);
					sound9.play();

					break;

				case WEST:

					for(int k=0; k < gameStateComponent->CurrentGameCol; k++)
					{
						if(gameStateComponent->GameMap[k][0] > 0)
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
			      		gameStateComponent->GameMap[i][gameStateComponent->CurrentGameCol] = 0;
			   		}

		    		score = score - ((gameStateComponent->CurrentGameCol - pointCounter) * 25);

	    			if (!buffer10.loadFromFile("./resources/sounds/ForcedDeletion.wav"))
	        			exit (EXIT_FAILURE);
					sound10.setBuffer(buffer10);
					sound10.play();

					break;

				default:
				 break;
			}
		}
	}
}