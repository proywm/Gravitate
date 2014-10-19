#include "GameStateComponent.h"
#include "DisplayManager.h"
#include "VisualComponent.h"
GameStateComponent::GameStateComponent(void)
{
}
GameStateComponent::~GameStateComponent(void)
{
}

void GameStateComponent::init(XMLElement *componentElement)
{
	ComponentID = componentElement->IntAttribute("ComponentType");
	XMLElement* shapeElement = componentElement->FirstChildElement("Structure");
 
	CurrentGameRow = shapeElement->IntAttribute("NoOfRows");
	CurrentGameCol = shapeElement->IntAttribute("NoOfCols");
	
	CreateMap(shapeElement);
	CurrentTetrominoShapeID = 2;
}
void GameStateComponent::CreateMap(XMLElement *componentElement)
{
	//ToDO: read new custom shapes from componentElement
	//simplified implementation for now
	for(int r=0;r<MAXROW;r++)
	{
		for(int c=0;c<MAXCOL;c++)
		{
			if(r<CurrentGameRow && c<CurrentGameCol)
				GameMap[r][c] = EMPTYBLOCK;
			else
				GameMap[r][c] = BOUNDARYIDENTIFIER;
		}
	}
}

void GameStateComponent::update(double deltaMS)
{

}

void GameStateComponent::CreateNewShape(TetrominoShape tetrominoShape, int xCorr, int yCorr)
{
	switch(tetrominoShape)
	{
		case STRAIGHTPOLYOMINO:
			if((GameMap[xCorr][yCorr]==0) && (GameMap[xCorr+1][yCorr]==0) 
&& (GameMap[xCorr+2][yCorr]==0) && (GameMap[xCorr+3][yCorr]==0))
			{
				GameMap[xCorr][yCorr] = NOTPLACEDBLOCK;//getTetrominoShapeID();
				GameMap[xCorr+1][yCorr] = GameMap[xCorr][yCorr];
				GameMap[xCorr+2][yCorr] = GameMap[xCorr][yCorr];
				GameMap[xCorr+3][yCorr] = GameMap[xCorr][yCorr];
			} 
			break; 
		case SQUAREPOLYOMINO:
			if((GameMap[xCorr][yCorr]==0) && (GameMap[xCorr+1][yCorr]==0) 
				&& (GameMap[xCorr][yCorr+1]==0) && (GameMap[xCorr+1][yCorr+1]==0))
			{
				GameMap[xCorr][yCorr] = NOTPLACEDBLOCK;//getTetrominoShapeID();
				GameMap[xCorr+1][yCorr] = GameMap[xCorr][yCorr];
				GameMap[xCorr][yCorr+1] = GameMap[xCorr][yCorr];
				GameMap[xCorr+1][yCorr+1] = GameMap[xCorr][yCorr];
			} 
			break;
		case TPOLYOMINO:
			if((GameMap[xCorr][yCorr]==0) && (GameMap[xCorr+1][yCorr]==0) 
				&& (GameMap[xCorr+2][yCorr]==0) && (GameMap[xCorr+1][yCorr+1]==0))
			{
				GameMap[xCorr][yCorr] = NOTPLACEDBLOCK;//getTetrominoShapeID();
				GameMap[xCorr+1][yCorr] = GameMap[xCorr][yCorr];
				GameMap[xCorr+2][yCorr] = GameMap[xCorr][yCorr];
				GameMap[xCorr+1][yCorr+1] = GameMap[xCorr][yCorr];
			}
			break;
		case JPOLYOMINO:
			if((GameMap[xCorr][yCorr]==0) && (GameMap[xCorr][yCorr+1]==0) 
				&& (GameMap[xCorr][yCorr+2]==0) && (GameMap[xCorr-1][yCorr+2]==0))
			{
				GameMap[xCorr][yCorr] = NOTPLACEDBLOCK;//getTetrominoShapeID();
				GameMap[xCorr][yCorr+1] = GameMap[xCorr][yCorr];
				GameMap[xCorr][yCorr+2] = GameMap[xCorr][yCorr];
				GameMap[xCorr-1][yCorr+2] = GameMap[xCorr][yCorr];
			}
			break;
		case LPOLYOMINO:
			if((GameMap[xCorr][yCorr]==0) && (GameMap[xCorr][yCorr+1]==0) 
				&& (GameMap[xCorr][yCorr+2]==0) && (GameMap[xCorr+1][yCorr+2]==0))
			{
				GameMap[xCorr][yCorr] = NOTPLACEDBLOCK;//getTetrominoShapeID();
				GameMap[xCorr][yCorr+1] = GameMap[xCorr][yCorr];
				GameMap[xCorr][yCorr+2] = GameMap[xCorr][yCorr];
				GameMap[xCorr+1][yCorr+2] = GameMap[xCorr][yCorr];
			}
			break;
		case SPOLYOMINO:
			if((GameMap[xCorr][yCorr]==0) && (GameMap[xCorr+1][yCorr]==0) 
				&& (GameMap[xCorr][yCorr+1]==0) && (GameMap[xCorr-1][yCorr+1]==0))
			{
				GameMap[xCorr][yCorr] = NOTPLACEDBLOCK;//getTetrominoShapeID();
				GameMap[xCorr+1][yCorr] = GameMap[xCorr][yCorr];
				GameMap[xCorr][yCorr+1] = GameMap[xCorr][yCorr];
				GameMap[xCorr-1][yCorr+1] = GameMap[xCorr][yCorr];
			}
			break;
		case ZPOLYOMINO:
			if((GameMap[xCorr][yCorr]==0) && (GameMap[xCorr+1][yCorr]==0) 
				&& (GameMap[xCorr+1][yCorr+1]==0) && (GameMap[xCorr+2][yCorr+1]==0))
			{
				GameMap[xCorr][yCorr] = NOTPLACEDBLOCK;//getTetrominoShapeID();
				GameMap[xCorr+1][yCorr] = GameMap[xCorr][yCorr];
				GameMap[xCorr+1][yCorr+1] = GameMap[xCorr][yCorr];
				GameMap[xCorr+2][yCorr+1] = GameMap[xCorr][yCorr];
			}
			break;
		default:
			break;
	}
}
int GameStateComponent::getTetrominoShapeID()
{
	//ToDo: Make a list to optimize
	//Now simply return the next ID
	return CurrentTetrominoShapeID++;
}
void GameStateComponent::PlaceNewShape()
{
	int setID  = getTetrominoShapeID();
	for(int r = 0; r<CurrentGameRow;r++)
	{
		for(int c = 0;c<CurrentGameCol;c++)
		{
			if(GameMap[r][c] = NOTPLACEDBLOCK)
			{
				GameMap[r][c] = setID;
			}
		}
	}	
}

void GameStateComponent::ShiftRight()
{

}
void GameStateComponent::ShiftLeft()
{

}
void GameStateComponent::ShiftUp()
{

}
void GameStateComponent::ShiftDown()
{

}
