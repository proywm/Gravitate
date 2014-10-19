#pragma once
#include "GlobalHeader.h"
#include "ActorComponentInterface.h"
#include "Shape.h"

#define BOUNDARYIDENTIFIER	-2
#define EMPTYBLOCK	0
#define CONCRETEBLOCK	-1
#define NOTPLACEDBLOCK	1

enum TetrominoShape 
{
	STRAIGHTPOLYOMINO = 1, // ----
	SQUAREPOLYOMINO = 2,// ::
	TPOLYOMINO = 3,// T
	JPOLYOMINO = 4,// J
	LPOLYOMINO = 5,// L
	SPOLYOMINO = 6,// S
	ZPOLYOMINO = 7 // Z
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

	int CurrentTetrominoShapeID;
	int getTetrominoShapeID();

	void PlaceNewShape();
	void CreateNewShape(TetrominoShape tetrominoShape, int xCorr, int yCorr);
	void CreateMap(XMLElement *componentElement);
	
	void ShiftRight();
	void ShiftLeft();
	void ShiftUp();
	void ShiftDown();
};

