#pragma once
#include "GamePlayer.h"
#include "GlobalHeader.h"
class HumanPlayer : public GamePlayer
{
public:
	HumanPlayer(void);
	~HumanPlayer(void);

	void init(const char* actorsList);
	void update(double deltaMS);
	void HandleEvent(sf::Event receivedEvent);

	char* leftKey;
	char* rightKey;
	int moveId;
};

