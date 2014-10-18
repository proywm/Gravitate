#pragma once
#include "GamePlayer.h"
#include "GlobalHeader.h"
class GameOwner :
	public GamePlayer
{
public:
	GameOwner(void);
	~GameOwner(void);

	void init(const char* actorsList);
	void update(double deltaMS);
	void HandleEvent(sf::Event receivedEvent);

	void controlGame(void);
	void startGame(void);
	void StopGame();
	void ShowWinner(GamePlayer* winner);
	void updateScoreView();
	void notifyAboutBallToAI(sf::Vector2f position);
	void RestartGame();

	int MaxPossibleScore;
	GamePlayer* winner;
	bool HasWinner;

	sf::Font font;
};

