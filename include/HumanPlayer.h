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

	sf::Keyboard::Key leftKey;
	sf::Keyboard::Key rightKey;
	sf::Keyboard::Key upKey;
	sf::Keyboard::Key downKey;
	
	sf::Keyboard::Key createKey;
	int moveId;
};

