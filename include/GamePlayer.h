#pragma once
#include "GlobalHeader.h"
#include "Actor.h"
#include "ActorFactory.h"

typedef std::map<int,Actor*>::iterator actorIterType;
class GamePlayer
{
public:
	GamePlayer(void);
	~GamePlayer(void);

	void initGamePlayer(const char* actorsList, int PlayerId);
	void initGamePlayerActors(XMLElement *actorFiles);
	void updateActors(double deltaMS);
	void deleteGamePlayerActors();
	
	virtual void init(const char* actorsList);
	virtual void update(double deltaMS);
	virtual void HandleEvent(sf::Event receivedEvent);

	int gamePlayerID;
	int gamePlayerTypeId;
	int score;

	const char* playerFile;
	std::map<int,Actor*> actorMap;//actorId int
};

