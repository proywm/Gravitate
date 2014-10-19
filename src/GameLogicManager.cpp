#include "GameLogicManager.h"
#include "GamePlayer.h"
#include "ActorFactory.h"
#include "DisplayManager.h"
#include "HumanPlayer.h"
#include "GameOwner.h"

ActorFactory *ActorFactory::s_instance = 0;
GameLogicManager::GameLogicManager(void)
{
	
}
GameLogicManager::~GameLogicManager(void)
{
}
GameLogicManager* GameLogicManager::instance()
{
	if (!s_instance)
		s_instance = new GameLogicManager;
	return s_instance;
}
void GameLogicManager::InitGameLogicManager(void)
{
	ActorFactory::instance()->init();
	
	char* playerList = "./src/PlayerList.xml";
	XMLDocument* doc = new XMLDocument();
    doc->LoadFile(playerList);

	XMLElement *playerFiles = doc->FirstChildElement();

	for(playerFiles = playerFiles->FirstChildElement(); playerFiles != NULL; playerFiles = playerFiles->NextSiblingElement())
	{
		const char* fileLocation = playerFiles->Attribute("PlayerFileName");
		int playerId = playerFiles->IntAttribute("PlayerId");
		int playerTypeId = playerFiles->IntAttribute("PlayerTypeId");
		
		GamePlayer* gamePlayer = CreateGamePlayer(playerTypeId);//new GamePlayer();
		
		gamePlayer->initGamePlayer(fileLocation,playerId);
		
		//gameplayer init
		gamePlayer->init(fileLocation);

		playerMap.insert(std::make_pair(gamePlayer->gamePlayerID,gamePlayer));
	}
	
	//initSound();//hardcoded
}
GamePlayer* GameLogicManager::CreateGamePlayer(int playerTypeId)
{
	GamePlayer* gPlayer;
		
	switch(playerTypeId)
	{
		case 1:
			gPlayer = new HumanPlayer();
			break;
		case 2:
			gPlayer = new GameOwner();
			break;
		default :
			gPlayer = new GameOwner();
		break;
	}
	gPlayer->gamePlayerTypeId = playerTypeId;
	return gPlayer;
}
void GameLogicManager::updateGameLogic(double deltaMS)
{
	DisplayManager::instance()->window.clear();
	for(playerIterType iter = playerMap.begin(); iter != playerMap.end(); ++iter)
	{
		GamePlayer* gamePlayer = ((GamePlayer*)iter->second);
		if(gamePlayer != NULL)
		{
			gamePlayer->update( deltaMS);
			gamePlayer->updateActors( deltaMS);
		}
	}
	DisplayManager::instance()->window.display();
}
//Hardcoded. ToDo: replace with EventManager
void GameLogicManager::SendKeyEventToPlayer(sf::Event receivedEvent)
{
	for(playerIterType iter = playerMap.begin(); iter != playerMap.end(); ++iter)
	{
		GamePlayer* gamePlayer = ((GamePlayer*)iter->second);
		if(gamePlayer != NULL)
		{
			if(gamePlayer->gamePlayerTypeId == 1)
			{
				gamePlayer->HandleEvent(receivedEvent);
			}
			else if(gamePlayer->gamePlayerTypeId == 2)
			{
				gamePlayer->HandleEvent(receivedEvent);
			}
		}
	}
}

void GameLogicManager::HandleEvents(void)
{
	EventManager::instance()->getEvents();
}

int GameLogicManager::ArePlayersRunning(void)
{
	//simplified
	return DisplayManager::instance()->IsDisplayWindowRunning();
}

//int GameLogicManager::initSound()
//{
//    if (!bufferCollide.loadFromFile("sound.wav"))
//        return -1;
//	return 0;
//}
//void GameLogicManager::PlaySound(int eventType)
//{
//	sf::Sound sound;
//	if(eventType == 0)//collide
//	{
//		sound.setBuffer(bufferCollide);
//		sound.play();
//	}
//}
