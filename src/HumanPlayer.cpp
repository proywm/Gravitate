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
	else if(receivedEvent.key.code == key1)//1
	{
	    printf("call Create piece 1\n");
	    ((GameOwner*)gamePlayer)->SelectGivenShapeRequest(1);
	}
	else if(receivedEvent.key.code == key2)//2
	{
	    printf("call Create piece 2\n");
	    ((GameOwner*)gamePlayer)->SelectGivenShapeRequest(2);
	}
	else if(receivedEvent.key.code == key3)//3
	{
	    printf("call Create piece 3\n");
	    ((GameOwner*)gamePlayer)->SelectGivenShapeRequest(3);
	}
	else if(receivedEvent.key.code == key4)//4
	{
	    printf("call Create piece 4\n");
	    ((GameOwner*)gamePlayer)->SelectGivenShapeRequest(4);
	}
	else if(receivedEvent.key.code == key5)//5
	{
	    printf("call Create piece 5\n");
	    ((GameOwner*)gamePlayer)->SelectGivenShapeRequest(5);
	}
	else if(receivedEvent.key.code == key6)//6
	{
	    printf("call Create piece 6\n");
	    ((GameOwner*)gamePlayer)->SelectGivenShapeRequest(6);
	}
	else if(receivedEvent.key.code == key7)//7
	{
	    printf("call Create piece 7\n");
	    ((GameOwner*)gamePlayer)->SelectGivenShapeRequest(7);
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
	key1 = (sf::Keyboard::Key)playerFiles->IntAttribute("key1");
	key2 = (sf::Keyboard::Key)playerFiles->IntAttribute("key2");
	key3 = (sf::Keyboard::Key)playerFiles->IntAttribute("key3");
	key4 = (sf::Keyboard::Key)playerFiles->IntAttribute("key4");
	key5 = (sf::Keyboard::Key)playerFiles->IntAttribute("key5");
	key6 = (sf::Keyboard::Key)playerFiles->IntAttribute("key6");
	key7 = (sf::Keyboard::Key)playerFiles->IntAttribute("key7");
	
	
	moveId = playerFiles->IntAttribute("MoveId");
}
void HumanPlayer::update(double deltaMS)
{}
