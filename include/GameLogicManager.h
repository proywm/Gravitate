#pragma once
#include "ActorFactory.h"
#include "GlobalHeader.h"
#include "GamePlayer.h"
#include "EventManager.h"
typedef std::map<int,GamePlayer*>::iterator playerIterType;
class GameLogicManager
{
public:
	GameLogicManager(void);
	~GameLogicManager(void);
	void InitGameLogicManager();
	static GameLogicManager *instance();
	void updateGameLogic(double deltaMS);

	GamePlayer* CreateGamePlayer(int playerTypeId);
	void SendKeyEventToPlayer(sf::Event receivedEvent);
	//int initSound();
	//void PlaySound(int eventType);

	static GameLogicManager *s_instance;
	std::map<int,GamePlayer*> playerMap;//playerID int

	void HandleEvents(void);
	int ArePlayersRunning(void);

	//sf::SoundBuffer bufferCollide;//sound
};
