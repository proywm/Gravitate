#pragma once
#include "GamePlayer.h"
#include "GlobalHeader.h"
#include "Actor.h"

enum Direction 
{
	SOUTH = 0,
	EAST = 1,
	NORTH = 2,
	WEST = 3,
};


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
	void RotateCounterClockwiseRequest(int shapeId);
	void RotateClockwiseRequest(int shapeId);

	void CreateShapeRequest();
	int SelectedShape();
	int randomShapeSelection();
	void SelectShapeRequest();

	void ImplementGravity(double deltaMS);
	bool ismoveableBlock(int blockId);

	int MaxPossibleScore;
	int maxShift;
	Direction direction[4];
	GamePlayer* winner;
	bool HasWinner;
	
	Direction randomGravity();
	Direction getDirection();
	void LineDeletion();
	
	int shiftTime;
	int gameTime;

	sf::Font font;
	
	int ConfiguredSHIFTTIME;
	int ConfiguredGAMETIME;
};
