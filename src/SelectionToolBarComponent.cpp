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
