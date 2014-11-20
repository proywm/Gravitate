#pragma once
#include "GlobalHeader.h"
#include "ActorComponentInterface.h"
#include "Shape.h"

#define BOUNDARYIDENTIFIER	-2
#define CONCRETEBLOCK		-1
#define EMPTYBLOCK			 0
#define HOVERBLOCK		 	 1
#define ACTIVEBLOCK		 	 2

enum TetrominoShape 
{
	STRAIGHTPOLYOMINO = 1, // ----
	SQUAREPOLYOMINO = 2,// ::
	TPOLYOMINO = 3,// T
	JPOLYOMINO = 4,// J
	LPOLYOMINO = 5,// L
	SPOLYOMINO = 6,// S
	ZPOLYOMINO = 7, // Z
	TOTALSHAPES
};

class GameStateComponent : public ActorComponentInterface
{
public:
	GameStateComponent(void);
	~GameStateComponent(void);

	void init(XMLElement *componentElement);
	void update(double deltaMS);

	int GameMap[MAXROW][MAXCOL];
	int CurrentGameRow;
	int CurrentGameCol;

	bool Hover;

	int CurrentTetrominoShapeID;
	int getTetrominoShapeID();
	bool isEmptyBlock(int blockId);

	void PlaceNewShape();
	void CreateNewShape(TetrominoShape tetrominoShape, int xCorr, int yCorr);
	void CreateMap(XMLElement *componentElement);
	
	void ShiftRight(int id);
	void ShiftLeft(int id);
	void ShiftUp(int id);
	void ShiftDown(int id);

	void RotateClockwise(int id);
	void RotateCounterClockwise(int id);
};
