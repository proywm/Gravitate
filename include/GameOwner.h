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
	void RestartGame();

	void ShiftLeftRequest(int shapeId);
	void ShiftRightRequest(int shapeId);
	void ShiftUpRequest(int shapeId);
	void ShiftDownRequest(int shapeId);
	void CreateShapeRequest();

	void ImplementGravity(double deltaMS);

	int MaxPossibleScore;
	GamePlayer* winner;
	bool HasWinner;

	sf::Font font;
};

