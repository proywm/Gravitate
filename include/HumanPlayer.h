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
	sf::Keyboard::Key key1;
	sf::Keyboard::Key key2;
	sf::Keyboard::Key key3;
	sf::Keyboard::Key key4;
	sf::Keyboard::Key key5;
	sf::Keyboard::Key key6;
	sf::Keyboard::Key key7;

	sf::Keyboard::Key rotateCounterClockwise;
	sf::Keyboard::Key rotateClockwise;
	int moveId;
};

