#include "HumanPlayer.h"
#include "GameLogicManager.h"
#include "GamePlayer.h"
#include "GameOwner.h"
#include "GameStateComponent.h"

HumanPlayer::HumanPlayer(void)
{
}
HumanPlayer::~HumanPlayer(void)
{
}
void HumanPlayer::HandleEvent(sf::Event receivedEvent)
{
	GamePlayer* gamePlayer;
	for(playerIterType iter = GameLogicManager::instance()->playerMap.begin(); iter != GameLogicManager::instance()->playerMap.end(); ++iter)
	{
		gamePlayer = ((GamePlayer*)iter->second);
		if(gamePlayer != NULL)
		{
			if(gamePlayer->gamePlayerTypeId == 2)
				break;
		}
	}
	if (receivedEvent.type == sf::Event::MouseButtonPressed)
	{
	    if (receivedEvent.mouseButton.button == sf::Mouse::Left)
	    {
	        std::cout << "the right button was pressed" << std::endl;
	        std::cout << "mouse x: " << receivedEvent.mouseButton.x << std::endl;
	        std::cout << "mouse y: " << receivedEvent.mouseButton.y << std::endl;
		printf("call Create\n");
		((GameOwner*)gamePlayer)->SelectShapeRequest();
		((GameOwner*)gamePlayer)->CreateShapeRequest();
		return;
	    }
	    else
		return;
	}	

	if(receivedEvent.key.code == leftKey)//A
	{

	  switch(((GameOwner*)gamePlayer)->getDirection())
	  {
	    case EAST :
	      break;
	    default :
	      printf("call move Left\n");
	      ((GameOwner*)gamePlayer)->ShiftLeftRequest(ACTIVEBLOCK);
	      break;
	  }
		
	}	
	else if(receivedEvent.key.code == rightKey)//D
	{
	  switch(((GameOwner*)gamePlayer)->getDirection())
	  {
	    case WEST :
	      break;
	     default:
	      printf("call move Right\n");
	      ((GameOwner*)gamePlayer)->ShiftRightRequest(ACTIVEBLOCK);
	       break;
	  }
	}	
	else if(receivedEvent.key.code == upKey)//W
	{
	  switch(((GameOwner*)gamePlayer)->getDirection())
	  {
	    case SOUTH :
	      break;
	    default:
	      printf("call move Up\n");
	      ((GameOwner*)gamePlayer)->ShiftUpRequest(ACTIVEBLOCK);
	      break;
	  }
	}	
	else if(receivedEvent.key.code == downKey)//S
	{
	  switch(((GameOwner*)gamePlayer)->getDirection())
	  {
	    case NORTH :
	      break;
	    default :
	      	printf("call move Down\n");
		((GameOwner*)gamePlayer)->ShiftDownRequest(ACTIVEBLOCK);
	      break;
	  }
	}
	else if(receivedEvent.key.code == createKey)//C
	{
		printf("call Create\n");
		((GameOwner*)gamePlayer)->CreateShapeRequest();
	}
	else if(receivedEvent.key.code == rotateCounterClockwise)//R
	{
		printf("call Rotate Counterclockwise\n");
		((GameOwner*)gamePlayer)->RotateCounterClockwiseRequest(ACTIVEBLOCK);
	}
	else if(receivedEvent.key.code == rotateClockwise)//T
	{
		printf("call Rotate Clockwise\n");
		((GameOwner*)gamePlayer)->RotateClockwiseRequest(ACTIVEBLOCK);
	}
}
void HumanPlayer::init(const char* actorsList)
{
	
	XMLDocument* doc = new XMLDocument();
    	doc->LoadFile(actorsList);
	XMLElement *playerFiles = doc->FirstChildElement();
	playerFiles = playerFiles->FirstChildElement("Control");
	leftKey = (sf::Keyboard::Key)playerFiles->IntAttribute("LeftKeyID");//A
	rightKey = (sf::Keyboard::Key)playerFiles->IntAttribute("RightKeyID");//D
	upKey = (sf::Keyboard::Key)playerFiles->IntAttribute("UpKeyID");//W
	downKey = (sf::Keyboard::Key)playerFiles->IntAttribute("DownKeyID");//S
	rotateCounterClockwise = (sf::Keyboard::Key)playerFiles->IntAttribute("RotateCounterClockwiseID");//R
	rotateClockwise = (sf::Keyboard::Key)playerFiles->IntAttribute("RotateClockwiseID");//T

	createKey = (sf::Keyboard::Key)playerFiles->IntAttribute("CreateKeyID");//C
	moveId = playerFiles->IntAttribute("MoveId");
}
void HumanPlayer::update(double deltaMS)
{}
