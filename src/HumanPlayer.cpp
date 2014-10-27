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
	
	if(receivedEvent.key.code == leftKey)//A
	{
		printf("call move Left\n");
		((GameOwner*)gamePlayer)->ShiftLeftRequest(NOTPLACEDBLOCK);
	}	
	else if(receivedEvent.key.code == rightKey)//D
	{
		printf("call move Right\n");
		((GameOwner*)gamePlayer)->ShiftRightRequest(NOTPLACEDBLOCK);	
	}	
	else if(receivedEvent.key.code == upKey)//W
	{
		printf("call move Up\n");
		((GameOwner*)gamePlayer)->ShiftUpRequest(NOTPLACEDBLOCK);	
	}	
	else if(receivedEvent.key.code == downKey)//S
	{
		printf("call move Down\n");
		((GameOwner*)gamePlayer)->ShiftDownRequest(NOTPLACEDBLOCK);
	}
	else if(receivedEvent.key.code == createKey)//C
	{
		printf("call Create\n");
		((GameOwner*)gamePlayer)->CreateShapeRequest();
	}
		else if(receivedEvent.key.code == rotateCounterClockwise)//R
	{
		printf("call Rotate Counterclockwise\n");
		((GameOwner*)gamePlayer)->RotateCounterClockwiseRequest(NOTPLACEDBLOCK);
	}
		else if(receivedEvent.key.code == rotateClockwise)//T
	{
		printf("call Rotate Clockwise\n");
		((GameOwner*)gamePlayer)->RotateClockwiseRequest(NOTPLACEDBLOCK);
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
