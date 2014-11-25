#include "GameOwner.h"
#include "ActorComponentInterface.h"
#include "GameLogicManager.h"
#include "GameViewManager.h"
#include "PhysicalComponent.h"
#include "VisualComponent.h"
#include "DisplayManager.h"
#include "GameStateComponent.h"
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
	actorsList1 = actorsList;
	XMLDocument* doc = new XMLDocument();
	doc->LoadFile(actorsList);
	XMLElement *playerFiles = doc->FirstChildElement();
	ConfiguredSHIFTTIME = playerFiles->IntAttribute("shiftTime");
	ConfiguredGAMETIME = playerFiles->IntAttribute("gameTime");
	ConfiguredLEVELTIME = playerFiles->IntAttribute("levelTime");
	
	
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
	
	
	showTitleView();
	controlGame();
}

void GameOwner::resetGame()
{
	XMLDocument* doc = new XMLDocument();
	doc->LoadFile(actorsList1);
	XMLElement *playerFiles = doc->FirstChildElement();
	ConfiguredSHIFTTIME = playerFiles->IntAttribute("shiftTime");
	ConfiguredGAMETIME = playerFiles->IntAttribute("gameTime");
	ConfiguredLEVELTIME = playerFiles->IntAttribute("levelTime");
	maxShift = 15;
	
	MaxPossibleScore = 10;//hardcoded
	srand(1);
	direction[0]= randomGravity();
	direction[1]= randomGravity();
	direction[2]= randomGravity();
	direction[3]= randomGravity();
	gameTime = 0; 
	shiftTime = 0;
	levelTime = ConfiguredLEVELTIME;
	score = 0;
	HasWinner = false;
	ShapeSelected = false;
	CurrentTetrominoShapeID = -1;
	visualDirection = -1;
}
void GameOwner::showTitleView()
{
	int i = 0;
	XMLDocument* doc = new XMLDocument();
    	doc->LoadFile(playerFile);
	XMLElement *playerElements ;
	for(actorIterType iter = ActorFactory::instance()->actorMapALL.begin(); iter != ActorFactory::instance()->actorMapALL.end(); ++iter)
	{
		Actor* actor = (Actor*)iter->second;
		if(actor->actorId == 4)//show level
		{
			for(playerElements = doc->FirstChildElement(); playerElements != NULL; playerElements = playerElements->NextSiblingElement())
			{
				std::ostringstream LevelString;			
				VisualComponent* visualComponent = (VisualComponent*)actor->GetComponent(VISUAL);
				LevelString << playerElements->Attribute("Difficulty");// put float into string buffer
				((ActorShape::GridMap*)visualComponent->actorShape)->SetTextInBox(LevelString, 0, i);
				((ActorShape::GridMap*)visualComponent->actorShape)->SetText(15,2,sf::Text::Bold);
				printf("value of i---------------------->%s\n",playerElements->Attribute("Difficulty"));
				i++;
			}
		}
	}
	printf("value of i---------------------->%d",i);
	
	//setting the directions
	srand(1);
	direction[0]= randomGravity();
	direction[1]= randomGravity();
	direction[2]= randomGravity();
	direction[3]= randomGravity();

	gameTime = 0; 
	shiftTime = 0;
	levelTime = ConfiguredLEVELTIME;
	viewTime = 0;
	score = 0;
	HasWinner = false;
	ShapeSelected = false;
	CurrentTetrominoShapeID = -1;
	visualDirection = -1;
	initFrame = 0;
	GameViewManager::instance()->setCurrentView(TITLEVIEW);
}
void GameOwner::update(double deltaMS)
{	
	ShowButtonTexts();
	switch(GameViewManager::instance()->currentGameView)//Title screen
	{
		case TITLEVIEW:
		{
			resetGame();
			break;
		}
		case GAMEVIEW:
		{
			ImplementGravity(deltaMS);
			updateDirectionImage();
			viewTime += deltaMS;
			if (viewTime >= 15)
			{
				viewTime = 0;
				ShowGameBackground();
				//printf("%d, %d\n",DisplayManager::instance()->window.getSize().x, DisplayManager::instance()->window.getSize().y);
			}
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
		break;
		case RESULTVIEW:
		//	ShowResultPage();
		{
			resetGame();
			break;
		}
	}
}
void GameOwner::ShowGameBackground()
{
	for(actorIterType iter = ActorFactory::instance()->actorMapALL.begin(); iter != ActorFactory::instance()->actorMapALL.end(); ++iter)
	{
		Actor* actor = (Actor*)iter->second;
		if(actor->actorId == 1)//directional Image
		{
			VisualComponent* visualComponent = (VisualComponent*)actor->GetComponent(VISUAL);
			initFrame++;
			initFrame %= 40;
			int xpos = initFrame % 5;
			int ypos = initFrame / 5;
			
			((ActorShape::GridMap*)visualComponent->actorShape)->setSprite(0, 0, xpos * (287 + 2), ypos * (249 + 2), 287, 249, getDirection());
		}
	}
}
void GameOwner::ShowResultPage()
{
	GameViewManager::instance()->setCurrentView(RESULTVIEW);
	std::ostringstream ResultString; 
	for(actorIterType iter = ActorFactory::instance()->actorMapALL.begin(); iter != ActorFactory::instance()->actorMapALL.end(); ++iter)
	{
		Actor* actor = (Actor*)iter->second;
		if(actor->actorId == 26)//directional Image
		{
			VisualComponent* visualComponent = (VisualComponent*)actor->GetComponent(VISUAL);
			ResultString << "Score: " << score;// put float into string buffer
			((ActorShape::GridMap*)visualComponent->actorShape)->SetTextInBox(ResultString, 0, 0);
			((ActorShape::GridMap*)visualComponent->actorShape)->SetText(60,3,sf::Text::Bold);
		}
	}
}
void GameOwner::ShowButtonTexts()
{
	std::ostringstream HelpString; 
	std::ostringstream QuitString; 
	std::ostringstream ExitString; 
	std::ostringstream MenuString; 
	std::ostringstream MenuString1; 
	for(actorIterType iter = ActorFactory::instance()->actorMapALL.begin(); iter != ActorFactory::instance()->actorMapALL.end(); ++iter)
	{
		Actor* actor = (Actor*)iter->second;
		if(actor->actorId == 18)//directional Image
		{
			VisualComponent* visualComponent = (VisualComponent*)actor->GetComponent(VISUAL);
			HelpString << "Help";// put float into string buffer
			((ActorShape::GridMap*)visualComponent->actorShape)->SetTextInBox(HelpString, 0, 0);
		}
		else if(actor->actorId == 19)//directional Image
		{
			VisualComponent* visualComponent = (VisualComponent*)actor->GetComponent(VISUAL);
			QuitString << "Quit";// put float into string buffer
			((ActorShape::GridMap*)visualComponent->actorShape)->SetTextInBox(QuitString, 0, 0);
			((ActorShape::GridMap*)visualComponent->actorShape)->SetText(15,2,sf::Text::Bold);
		}
		else if(actor->actorId == 20)//directional Image
		{
			VisualComponent* visualComponent = (VisualComponent*)actor->GetComponent(VISUAL);
			ExitString << "Exit";// put float into string buffer
			((ActorShape::GridMap*)visualComponent->actorShape)->SetTextInBox(ExitString, 0, 0);
			((ActorShape::GridMap*)visualComponent->actorShape)->SetText(15,2,sf::Text::Bold);
		}
		else if(actor->actorId == 21)//directional Image
		{
			VisualComponent* visualComponent = (VisualComponent*)actor->GetComponent(VISUAL);
			MenuString << "Menu";// put float into string buffer
			((ActorShape::GridMap*)visualComponent->actorShape)->SetTextInBox(MenuString, 0, 0);
		}
		else if(actor->actorId == 22)//directional Image
		{
			VisualComponent* visualComponent = (VisualComponent*)actor->GetComponent(VISUAL);
			MenuString1 << "Menu";// put float into string buffer
			((ActorShape::GridMap*)visualComponent->actorShape)->SetTextInBox(MenuString1, 0, 0);
		}
		
	}
}
void GameOwner::updateDirectionImage()
{
	int nextDirection;
	bool directionChanged = false;
	for(actorIterType iter = ActorFactory::instance()->actorMapALL.begin(); iter != ActorFactory::instance()->actorMapALL.end(); ++iter)
	{
		Actor* actor = (Actor*)iter->second;
		switch(actor->actorId)//directional Image
		{
			case 14:
			{
				if(visualDirection!=direction[0])
				{
					directionChanged = true;
					VisualComponent* visualComponent = (VisualComponent*)actor->GetComponent(VISUAL);
					switch(direction[0])
					{
						case 0:
							((ActorShape::GridMap*)visualComponent->actorShape)->LoadTexture("./resources/images/south.png");
							((ActorShape::GridMap*)visualComponent->actorShape)->setTexture();
						break;
						case 1:
							((ActorShape::GridMap*)visualComponent->actorShape)->LoadTexture("./resources/images/east.png");
							((ActorShape::GridMap*)visualComponent->actorShape)->setTexture();
						break;
						case 2:
							((ActorShape::GridMap*)visualComponent->actorShape)->LoadTexture("./resources/images/north.png");
							((ActorShape::GridMap*)visualComponent->actorShape)->setTexture();
						break;
						case 3:
							((ActorShape::GridMap*)visualComponent->actorShape)->LoadTexture("./resources/images/west.png");
							((ActorShape::GridMap*)visualComponent->actorShape)->setTexture();
						break;				
					}
					visualDirection = direction[0];
				}
			}
			break;
			case 23:
			{
				if(!directionChanged)
				{
					continue;
				}
				nextDirection = direction[1];
				break;
			}
			case 24:
			{
				if(!directionChanged)
				{
					continue;
				}
				nextDirection = direction[2];
				break;
			}
			case 25:
			{
				if(!directionChanged)
				{
					continue;
				}
				nextDirection = direction[3];
				break;
			}
		}
		switch(actor->actorId)//directional Image
		{
			case 23:
			case 24:
			case 25:
				VisualComponent* visualComponent = (VisualComponent*)actor->GetComponent(VISUAL);
				switch(nextDirection)
				{
					case 0:
						((ActorShape::GridMap*)visualComponent->actorShape)->LoadTexture("./resources/images/southS.png");
						((ActorShape::GridMap*)visualComponent->actorShape)->setTexture();
					break;
					case 1:
						((ActorShape::GridMap*)visualComponent->actorShape)->LoadTexture("./resources/images/eastS.png");
						((ActorShape::GridMap*)visualComponent->actorShape)->setTexture();
					break;
					case 2:
						((ActorShape::GridMap*)visualComponent->actorShape)->LoadTexture("./resources/images/northS.png");
						((ActorShape::GridMap*)visualComponent->actorShape)->setTexture();
					break;
					case 3:
						((ActorShape::GridMap*)visualComponent->actorShape)->LoadTexture("./resources/images/westS.png");
						((ActorShape::GridMap*)visualComponent->actorShape)->setTexture();
					break;				
				}
			break;
		}
	}
}
void GameOwner::MouseClicked(int posX, int posY)
{
	Actor* actor = GameViewManager::instance()->GetTopVisibleActorAtPoint(posX, posY);
	if(actor!=NULL)
	{
		printf("actor id ------------>%d\n", actor->actorId);
		switch(actor->actorId)
		{
			case 1://background
			break;
			case 2://Game Map
				CreateShapeRequest();
			break;
			case 3://ShapeSelectionToolBar
			break;
			case 4://GameLevelSelector
				SelectGameLevel(actor, posX, posY);
			break;
			case 5://GameScore
			break;
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
			case 11:
			case 12:
				SelectShapeRequest(actor->actorId);
			break;
			case 15://score, time
			break;
			case 16://detetion
				ForcedDeletion();
				break;			
			case 18:
				//Go to Help Page
				GameViewManager::instance()->setCurrentView(RULEVIEW);
			break;
			case 19:
				//quit game
				ShowResultPage();
			break;
			case 20:
				//End game
				DisplayManager::instance()->window.close();
			break;
			case 21:
			case 22:
				//Go to Title Page
				showTitleView();
			break;
			
		}
	}
	else
		printf("actor id ------------None\n");
}
void GameOwner::SelectGameLevel(Actor* actor,int posX, int posY)
{
	int level = 0;
	PhysicalComponent* physicalComponent = (PhysicalComponent*)actor->GetComponent(PHYSICAL);
	int blockHeight = ((ActorShape::GridMap*)physicalComponent->actorShape)->blockHeight;
	int blockWidth = ((ActorShape::GridMap*)physicalComponent->actorShape)->blockWidth;
	sf::Vector2i localPosition = sf::Mouse::getPosition(DisplayManager::instance() -> window);
	int xGridPosition = floor((posX - physicalComponent->getActorPosition().x)/blockWidth);
	int yGridPosition = floor((posY - physicalComponent->getActorPosition().y)/blockHeight);
	printf("Grid Selected ------------>%d , %d\n", xGridPosition,yGridPosition);
	XMLDocument* doc = new XMLDocument();
    	doc->LoadFile(playerFile);
	XMLElement *playerElements = doc->FirstChildElement();
	while(yGridPosition!=level)
	{
		playerElements = playerElements->NextSiblingElement("Player");
		level++;
	}
	deleteGamePlayerActors();
	initGamePlayerActors(playerElements);
	ConfiguredSHIFTTIME = playerElements->IntAttribute("shiftTime");
	ConfiguredGAMETIME = playerElements->IntAttribute("gameTime");
	ConfiguredLEVELTIME = playerElements->IntAttribute("levelTime");
	GameViewManager::instance()->setCurrentView(GAMEVIEW);
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
			levelTime -= deltaMS;
			shiftTime += deltaMS;
			if(levelTime <= 0)
			{
				//finish
				ShowResultPage();
			}
			if (gameTime >= ConfiguredGAMETIME)
			{
				gameTime = 0;
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

				for(int r=0;r<gameStateComponent->CurrentGameRow;r++)
				{
					for(int c=0;c<gameStateComponent->CurrentGameCol;c++)
					printf("%d ",gameStateComponent->GameMap[r][c]);
					
					printf("\n");
				}
				printf("\n");

				switch(direction[0])
				{
				case SOUTH:
					for(int r=gameStateComponent->CurrentGameRow-1;r>=0;r--)
					{
						for(int c=0;c<gameStateComponent->CurrentGameCol;c++)
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
					for(int r=0;r<gameStateComponent->CurrentGameRow;r++)
					{
						for(int c=gameStateComponent->CurrentGameCol-1;c>=0;c--)
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
					for(int r=0;r<gameStateComponent->CurrentGameRow;r++)
					{
						for(int c=0;c<gameStateComponent->CurrentGameCol;c++)
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
					for(int r=0;r<gameStateComponent->CurrentGameRow;r++)
					{
						for(int c=0;c<gameStateComponent->CurrentGameCol;c++)
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
			int blockHeight = ((ActorShape::GridMap*)physicalComponent->actorShape)->blockHeight;
			int blockWidth = ((ActorShape::GridMap*)physicalComponent->actorShape)->blockWidth;
			//Get mouse position in window
			sf::Vector2i localPosition = sf::Mouse::getPosition(DisplayManager::instance() -> window);
			
			//Convert position to grid
			int yCorr = floor((localPosition.x - physicalComponent->getActorPosition().x)/blockWidth);
			int xCorr = floor((localPosition.y - physicalComponent->getActorPosition().y)/blockHeight);
		
			int tetrominoShape = SelectedShape();
			if(tetrominoShape == -1)//something selected check
				continue;
			
			for(int r=0;r<gameStateComponent->CurrentGameRow;r++)
			{
				for(int c=0;c<gameStateComponent->CurrentGameCol;c++)
				{
					if (gameStateComponent->GameMap[r][c] == HOVERBLOCK)
						gameStateComponent->GameMap[r][c] = EMPTYBLOCK;
				}
			}
			
			int count = 0;
			switch(tetrominoShape)
			{
			case STRAIGHTPOLYOMINO:
				if(gameStateComponent->GameMap[xCorr][yCorr] == EMPTYBLOCK)
				{
					gameStateComponent->GameMap[xCorr][yCorr] = HOVERBLOCK;
					count++;
				}
				if(gameStateComponent->GameMap[xCorr+1][yCorr] == EMPTYBLOCK)
				{
					gameStateComponent->GameMap[xCorr+1][yCorr] = HOVERBLOCK;
					count++;
				}
				if(gameStateComponent->GameMap[xCorr+2][yCorr] == EMPTYBLOCK)
				{
					gameStateComponent->GameMap[xCorr+2][yCorr] = HOVERBLOCK;
					count++;
				}
				if(gameStateComponent->GameMap[xCorr+3][yCorr] == EMPTYBLOCK)
				{
					gameStateComponent->GameMap[xCorr+3][yCorr] = HOVERBLOCK;
					count++;
				}
				break; 
			case SQUAREPOLYOMINO:
				if(gameStateComponent->GameMap[xCorr][yCorr] == EMPTYBLOCK)
				{
					gameStateComponent->GameMap[xCorr][yCorr] = HOVERBLOCK;
					count++;
				}
				if(gameStateComponent->GameMap[xCorr+1][yCorr] == EMPTYBLOCK)
				{
					gameStateComponent->GameMap[xCorr+1][yCorr] = HOVERBLOCK;
					count++;
				}
				if(gameStateComponent->GameMap[xCorr][yCorr+1] == EMPTYBLOCK)
				{
					gameStateComponent->GameMap[xCorr][yCorr+1] = HOVERBLOCK;
					count++;
				}
				if(gameStateComponent->GameMap[xCorr+1][yCorr+1] == EMPTYBLOCK)
				{
					gameStateComponent->GameMap[xCorr+1][yCorr+1] = HOVERBLOCK;
					count++;
				}
				break;
			case TPOLYOMINO:
				if(gameStateComponent->GameMap[xCorr][yCorr] == EMPTYBLOCK)
				{
					gameStateComponent->GameMap[xCorr][yCorr] = HOVERBLOCK;
					count++;
				}
				if(gameStateComponent->GameMap[xCorr+1][yCorr] == EMPTYBLOCK)
				{
					gameStateComponent->GameMap[xCorr+1][yCorr] = HOVERBLOCK;
					count++;
				}
				if(gameStateComponent->GameMap[xCorr+2][yCorr] == EMPTYBLOCK)
				{
					gameStateComponent->GameMap[xCorr+2][yCorr] = HOVERBLOCK;
					count++;
				}
				if(gameStateComponent->GameMap[xCorr+1][yCorr+1] == EMPTYBLOCK)
				{
					gameStateComponent->GameMap[xCorr+1][yCorr+1] = HOVERBLOCK;
					count++;
				}
				break;
			case JPOLYOMINO:
				if(gameStateComponent->GameMap[xCorr][yCorr] == EMPTYBLOCK)
				{
					gameStateComponent->GameMap[xCorr][yCorr] = HOVERBLOCK;
					count++;
				}
				if(gameStateComponent->GameMap[xCorr][yCorr+1] == EMPTYBLOCK)
				{
					gameStateComponent->GameMap[xCorr][yCorr+1] = HOVERBLOCK;
					count++;
				}
				if(gameStateComponent->GameMap[xCorr][yCorr+2] == EMPTYBLOCK)
				{
					gameStateComponent->GameMap[xCorr][yCorr+2] = HOVERBLOCK;
					count++;
				}
				if(gameStateComponent->GameMap[xCorr-1][yCorr+2] == EMPTYBLOCK)
				{
					gameStateComponent->GameMap[xCorr-1][yCorr+2] = HOVERBLOCK;
					count++;
				}
				break;
			case LPOLYOMINO:
				if(gameStateComponent->GameMap[xCorr][yCorr] == EMPTYBLOCK)
				{
					gameStateComponent->GameMap[xCorr][yCorr] = HOVERBLOCK;
					count++;
				}
				if(gameStateComponent->GameMap[xCorr][yCorr+1] == EMPTYBLOCK)
				{
					gameStateComponent->GameMap[xCorr][yCorr+1] = HOVERBLOCK;
					count++;
				}
				if(gameStateComponent->GameMap[xCorr][yCorr+2] == EMPTYBLOCK)
				{
					gameStateComponent->GameMap[xCorr][yCorr+2] = HOVERBLOCK;
					count++;
				}
				if(gameStateComponent->GameMap[xCorr+1][yCorr+2] == EMPTYBLOCK)
				{
					gameStateComponent->GameMap[xCorr+1][yCorr+2] = HOVERBLOCK;
					count++;
				}
				break;
			case SPOLYOMINO:
				if(gameStateComponent->GameMap[xCorr][yCorr] == EMPTYBLOCK)
				{
					gameStateComponent->GameMap[xCorr][yCorr] = HOVERBLOCK;
					count++;
				}
				if(gameStateComponent->GameMap[xCorr+1][yCorr] == EMPTYBLOCK)
				{
					gameStateComponent->GameMap[xCorr+1][yCorr] = HOVERBLOCK;
					count++;
				}
				if(gameStateComponent->GameMap[xCorr][yCorr+1] == EMPTYBLOCK)
				{
					gameStateComponent->GameMap[xCorr][yCorr+1] = HOVERBLOCK;
					count++;
				}
				if(gameStateComponent->GameMap[xCorr-1][yCorr+1] == EMPTYBLOCK)
				{
					gameStateComponent->GameMap[xCorr-1][yCorr+1] = HOVERBLOCK;
					count++;
				}
				break;
			case ZPOLYOMINO:
				if(gameStateComponent->GameMap[xCorr][yCorr] == EMPTYBLOCK)
				{
					gameStateComponent->GameMap[xCorr][yCorr] = HOVERBLOCK;
					count++;
				}
				if(gameStateComponent->GameMap[xCorr+1][yCorr] == EMPTYBLOCK)
				{
					gameStateComponent->GameMap[xCorr+1][yCorr] = HOVERBLOCK;
					count++;
				}
				if(gameStateComponent->GameMap[xCorr+1][yCorr+1] == EMPTYBLOCK)
				{
					gameStateComponent->GameMap[xCorr+1][yCorr+1] = HOVERBLOCK;
					count++;
				}
				if(gameStateComponent->GameMap[xCorr+2][yCorr+1] == EMPTYBLOCK)
				{
					gameStateComponent->GameMap[xCorr+2][yCorr+1] = HOVERBLOCK;
					count++;
				}
				break;
			default:
				break;
			}
			gameStateComponent->canCreate = (count == 4); //True if all 4 pieces appeared, so a shape can be created.
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
	if(ShapeSelected)
	{
		//printf("shape has been Selected--->%d\n",CurrentTetrominoShapeID);
		//printf("shape has been Selected--->%d\n",CurrentTetrominoShapeID);
		return CurrentTetrominoShapeID;
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
			if(SelectedShape()!=-1)//something selected check
			{
				gameStateComponent->CreateNewShape();
				sound2.play();
				
			}
			else
			printf("Nothing Selected");
		}
	}
}
void GameOwner::SelectShapeRequest(int selectedBox)
{
	sound2.play();
	switch(selectedBox)
	{
		case 6:
			CurrentTetrominoShapeID = STRAIGHTPOLYOMINO;
			break;
		case 7:
			CurrentTetrominoShapeID = JPOLYOMINO;
			break;
		case 8:
			CurrentTetrominoShapeID = LPOLYOMINO;
			break;
		case 9:
			CurrentTetrominoShapeID = SPOLYOMINO;
			break;
		case 10:
			CurrentTetrominoShapeID = ZPOLYOMINO;
			break;
		case 11:
			CurrentTetrominoShapeID = TPOLYOMINO;
			break;
		case 12:
			CurrentTetrominoShapeID = SQUAREPOLYOMINO;
			break;
		default:
			CurrentTetrominoShapeID = -1;
			break;
	}
	if(CurrentTetrominoShapeID>0)
	{
			ShapeSelected = true;
	}
}

void GameOwner::SelectGivenShapeRequest(int tetrominoId)
{
	sound2.play();
	CurrentTetrominoShapeID = tetrominoId;
	if(CurrentTetrominoShapeID>0)
	{
			ShapeSelected = true;
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
	std::ostringstream GravityShiftString; 
	std::ostringstream ScoreString; 
	std::ostringstream LevelTimeString; 
	for(actorIterType iter = ActorFactory::instance()->actorMapALL.begin(); iter != ActorFactory::instance()->actorMapALL.end(); ++iter)
	{
		Actor* actor = (Actor*)iter->second;
		if(actor->actorId == 15)//directional Image
		{
			VisualComponent* visualComponent = (VisualComponent*)actor->GetComponent(VISUAL);
			GravityShiftString << "Next Gravity Shift: " << (ConfiguredSHIFTTIME-shiftTime) / 1000;// put float into string buffer
			((ActorShape::GridMap*)visualComponent->actorShape)->SetTextInBox(GravityShiftString, 0, 0);
			
			ScoreString << "Score " << score;// put float into string buffer
			((ActorShape::GridMap*)visualComponent->actorShape)->SetTextInBox(ScoreString, 1, 0);
					
			LevelTimeString << "Time Left: " << levelTime / 1000;// put float into string buffer
			((ActorShape::GridMap*)visualComponent->actorShape)->SetTextInBox(LevelTimeString, 2, 0);
			((ActorShape::GridMap*)visualComponent->actorShape)->SetText(15,2,sf::Text::Bold);
		}
	}
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
				gameStateComponent->PlaceNewShape();
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
					gameStateComponent->GameMap[i][gameStateComponent->CurrentGameCol - 1] = EMPTYBLOCK;
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
				gameStateComponent->PlaceNewShape();
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
				gameStateComponent->PlaceNewShape();
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
					gameStateComponent->GameMap[gameStateComponent->CurrentGameRow - 1][i] = EMPTYBLOCK;
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
				gameStateComponent->PlaceNewShape();
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
