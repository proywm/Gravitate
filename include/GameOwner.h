#pragma once
#include "GamePlayer.h"
#include "GlobalHeader.h"
#include "Actor.h"
#include "GameStateComponent.h"

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
	void resetGame();
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

	void MouseClicked(int posX, int posY);
	void CreateShapeRequest();
	int SelectedShape();
	int randomShapeSelection();
	void SelectShapeRequest(int selectedBox);
	void updateDirectionImage();
	void SelectGivenShapeRequest(int tetrominoId);
	void SelectGameLevel(Actor* actor,int posX, int posY);
	void ShowButtonTexts();

	void ImplementGravity(double deltaMS);
	bool ismoveableBlock(int blockId);
	bool isEmptyBlock(int blockId);

	int MaxPossibleScore;
	int maxShift;
	Direction direction[4];
	GamePlayer* winner;
	bool HasWinner;
	bool ShapeSelected;
	int CurrentTetrominoShapeID;
	int visualDirection;
	const char* actorsList1;
	
	Direction randomGravity();
	Direction getDirection();
	void ShowCursor();
	void LineDeletion();
	void ForcedDeletion();
	void showTitleView();
	void ShowResultPage();
	
	int shiftTime;
	int gameTime;
	int levelTime;
	
	sf::Font font;
	
	int ConfiguredSHIFTTIME;
	int ConfiguredGAMETIME;
	int ConfiguredLEVELTIME;
	
	int score;

	sf::SoundBuffer buffer;
	sf::SoundBuffer buffer2;
	sf::SoundBuffer buffer3;
	sf::SoundBuffer buffer4;
	sf::Sound sound;
	sf::Sound sound2;
	sf::Sound sound3;
	sf::Sound sound4;

};
