#include "HumanPlayer.h"
#include "GameLogicManager.h"
#include "GamePlayer.h"
#include "GameOwner.h"
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
		printf("call move Left\n");
	else if(receivedEvent.key.code == rightKey)//D
		printf("call move Right\n");
	else if(receivedEvent.key.code == upKey)//W
		printf("call move Up\n");
	else if(receivedEvent.key.code == downKey)//S
	{
		printf("call move Down\n");
		((GameOwner*)gamePlayer)->ShiftDownRequest();
	}
	else if(receivedEvent.key.code == createKey)//C
	{
		printf("call Create\n");
		((GameOwner*)gamePlayer)->CreateShapeRequest();
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

	createKey = (sf::Keyboard::Key)playerFiles->IntAttribute("CreateKeyID");//C
	moveId = playerFiles->IntAttribute("MoveId");
}
void HumanPlayer::update(double deltaMS)
{}
