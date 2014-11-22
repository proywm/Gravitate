#include "SelectionToolBarComponent.h"
#include "DisplayManager.h"
#include "VisualComponent.h"
#include <iostream>
#include <algorithm>

SelectionToolBarComponent::SelectionToolBarComponent(void)
{
}
SelectionToolBarComponent::~SelectionToolBarComponent(void)
{
}

void SelectionToolBarComponent::init(XMLElement *componentElement)
{

	ComponentID = componentElement->IntAttribute("ComponentType");
	XMLElement* shapeElement = componentElement->FirstChildElement("Structure");
 
	CurrentToolBarRow = shapeElement->IntAttribute("NoOfRows");
	CurrentToolBarCol = shapeElement->IntAttribute("NoOfCols");
	
	CreateMap(shapeElement);
	CreateShapes();
	CurrentTetrominoShapeID = -1;
	Selected = false;
}
void SelectionToolBarComponent::CreateMap(XMLElement *componentElement)
{
	//ToDO: read new custom shapes from componentElement
	//simplified implementation for now
	for(int r=0;r<MAXROW;r++)
	{
		for(int c=0;c<MAXCOL;c++)
		{
			if(r<CurrentToolBarRow && c<CurrentToolBarCol)
				GameMap[r][c] = EMPTYBLOCK;
			else
				GameMap[r][c] = BOUNDARYIDENTIFIER;
		}
	}
}

void SelectionToolBarComponent::update(double deltaMS)
{

}
void SelectionToolBarComponent::CreateShapes()
{
	int shape = 1;
	int xCorr = 2, yCorr = 2;
	while(shape!=((int)TOTALSHAPES-1))
	{
		printf("Shape-------------->x %d\n",xCorr);
		CreateNewShape((TetrominoShape)shape,xCorr,yCorr);
		xCorr = xCorr + 6;
		shape++;
	}
}
void SelectionToolBarComponent::CreateNewShape(TetrominoShape tetrominoShape, int xCorr, int yCorr)
{
	switch(tetrominoShape)
	{
		case STRAIGHTPOLYOMINO:
			if(isEmptyBlock(GameMap[xCorr][yCorr]) && isEmptyBlock(GameMap[xCorr+1][yCorr]) 
				&& isEmptyBlock(GameMap[xCorr+2][yCorr]) && isEmptyBlock(GameMap[xCorr+3][yCorr]))
			{
				GameMap[xCorr][yCorr] = STRAIGHTPOLYOMINO;//getTetrominoShapeID();
				GameMap[xCorr+1][yCorr] = GameMap[xCorr][yCorr];
				GameMap[xCorr+2][yCorr] = GameMap[xCorr][yCorr];
				GameMap[xCorr+3][yCorr] = GameMap[xCorr][yCorr];
			} 

			break; 
		case SQUAREPOLYOMINO:
			if(isEmptyBlock(GameMap[xCorr][yCorr]) && isEmptyBlock(GameMap[xCorr+1][yCorr]) 
				&& isEmptyBlock(GameMap[xCorr][yCorr+1]) && isEmptyBlock(GameMap[xCorr+1][yCorr+1]))
			{
				GameMap[xCorr][yCorr] = SQUAREPOLYOMINO;//getTetrominoShapeID();
				GameMap[xCorr+1][yCorr] = GameMap[xCorr][yCorr];
				GameMap[xCorr][yCorr+1] = GameMap[xCorr][yCorr];
				GameMap[xCorr+1][yCorr+1] = GameMap[xCorr][yCorr];
			} 

			break;
		case TPOLYOMINO:
			if(isEmptyBlock(GameMap[xCorr][yCorr]) && isEmptyBlock(GameMap[xCorr+1][yCorr]) 
				&& isEmptyBlock(GameMap[xCorr+2][yCorr]) && isEmptyBlock(GameMap[xCorr+1][yCorr+1]))
			{
				GameMap[xCorr][yCorr] = TPOLYOMINO;//getTetrominoShapeID();
				GameMap[xCorr+1][yCorr] = GameMap[xCorr][yCorr];
				GameMap[xCorr+2][yCorr] = GameMap[xCorr][yCorr];
				GameMap[xCorr+1][yCorr+1] = GameMap[xCorr][yCorr];
			}

			break;
		case JPOLYOMINO:
			if(isEmptyBlock(GameMap[xCorr][yCorr]) && isEmptyBlock(GameMap[xCorr][yCorr+1]) 
				&& isEmptyBlock(GameMap[xCorr][yCorr+2]) && isEmptyBlock(GameMap[xCorr-1][yCorr+2]))
			{
				GameMap[xCorr][yCorr] = JPOLYOMINO;//getTetrominoShapeID();
				GameMap[xCorr][yCorr+1] = GameMap[xCorr][yCorr];
				GameMap[xCorr][yCorr+2] = GameMap[xCorr][yCorr];
				GameMap[xCorr-1][yCorr+2] = GameMap[xCorr][yCorr];
			}

			break;
		case LPOLYOMINO:
			if(isEmptyBlock(GameMap[xCorr][yCorr]) && isEmptyBlock(GameMap[xCorr][yCorr+1]) 
				&& isEmptyBlock(GameMap[xCorr][yCorr+2]) && isEmptyBlock(GameMap[xCorr+1][yCorr+2]))
			{
				GameMap[xCorr][yCorr] = LPOLYOMINO;//getTetrominoShapeID();
				GameMap[xCorr][yCorr+1] = GameMap[xCorr][yCorr];
				GameMap[xCorr][yCorr+2] = GameMap[xCorr][yCorr];
				GameMap[xCorr+1][yCorr+2] = GameMap[xCorr][yCorr];
			}

			break;
		case SPOLYOMINO:
			if(isEmptyBlock(GameMap[xCorr][yCorr]) && isEmptyBlock(GameMap[xCorr+1][yCorr]) 
				&& isEmptyBlock(GameMap[xCorr][yCorr+1]) && isEmptyBlock(GameMap[xCorr-1][yCorr+1]))
			{
				GameMap[xCorr][yCorr] = SPOLYOMINO;//getTetrominoShapeID();
				GameMap[xCorr+1][yCorr] = GameMap[xCorr][yCorr];
				GameMap[xCorr][yCorr+1] = GameMap[xCorr][yCorr];
				GameMap[xCorr-1][yCorr+1] = GameMap[xCorr][yCorr];
			}

			break;
		case ZPOLYOMINO:
			if(isEmptyBlock(GameMap[xCorr][yCorr]) && isEmptyBlock(GameMap[xCorr+1][yCorr]) 
				&& isEmptyBlock(GameMap[xCorr+1][yCorr+1]) && isEmptyBlock(GameMap[xCorr+2][yCorr+1]))
			{
				GameMap[xCorr][yCorr] = ZPOLYOMINO;//getTetrominoShapeID();
				GameMap[xCorr+1][yCorr] = GameMap[xCorr][yCorr];
				GameMap[xCorr+1][yCorr+1] = GameMap[xCorr][yCorr];
				GameMap[xCorr+2][yCorr+1] = GameMap[xCorr][yCorr];
			}
			break;
		default:
			break;
	}
}
int SelectionToolBarComponent::findShape(int xGridPosition, int yGridPosition)
{
	for(int i = -2; i<3;i++)
		for(int j = -2 ; j<3;j++)
			if(GameMap[xGridPosition+i][yGridPosition+j] > 0)
				return GameMap[xGridPosition+i][yGridPosition+j];
	return 0;
}
void SelectionToolBarComponent::SelectShape(int xGridPosition, int yGridPosition)
{
	if(yGridPosition<CurrentToolBarCol && yGridPosition>=0 && xGridPosition<CurrentToolBarRow && xGridPosition>=0 )
	{	
		CurrentTetrominoShapeID = findShape(xGridPosition, yGridPosition);
		if(CurrentTetrominoShapeID>0)
		{
			Selected = true;
		}
			
	}
}
void SelectionToolBarComponent::SelectGivenShape(int tetrominoId)
{
    CurrentTetrominoShapeID = tetrominoId;
    Selected = true;
}
int SelectionToolBarComponent::GetSelectedShape()
{
	if(Selected)
		return CurrentTetrominoShapeID;
	return -1;
}
bool SelectionToolBarComponent::isEmptyBlock(int blockId)
{
	return blockId == EMPTYBLOCK || blockId == HOVERBLOCK;
}
