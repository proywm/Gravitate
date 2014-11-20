#pragma once
#include "GlobalHeader.h"
#include "ActorComponentInterface.h"
#include "Shape.h"
#include "GameStateComponent.h"

class SelectionToolBarComponent : public ActorComponentInterface
{
public:
	SelectionToolBarComponent(void);
	~SelectionToolBarComponent(void);

	void init(XMLElement *componentElement);
	void update(double deltaMS);

	int GameMap[MAXROW][MAXCOL];
	int CurrentToolBarRow;
	int CurrentToolBarCol;

	bool Selected;
	//TetrominoShape CurrentTetrominoShapeID;
	int CurrentTetrominoShapeID;

	void PlaceNewShape();
	void CreateShapes();
	void CreateNewShape(TetrominoShape tetrominoShape, int xCorr, int yCorr);
	void CreateMap(XMLElement *componentElement);
	void SelectShape(int xGridPosition, int yGridPosition);
	int GetSelectedShape();
	bool isEmptyBlock(int blockId);
	int findShape(int xGridPosition, int yGridPosition);
};
