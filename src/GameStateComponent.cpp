#include "GameStateComponent.h"
#include "DisplayManager.h"
#include "VisualComponent.h"
#include <iostream>
#include <algorithm>
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
	CurrentTetrominoShapeID = 3;
	Hover = false;
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
bool GameStateComponent::isEmptyBlock(int blockId)
{
	return blockId == EMPTYBLOCK || blockId == HOVERBLOCK;
}
void GameStateComponent::CreateNewShape()
{
	if(Hover)
	{
		printf("can't create now\n");
		return;
	}
	else if (!canCreate)
	{
	    printf("Piece cannot be created here\n");
	    return;
	}

	for(int r = 0; r<CurrentGameRow;r++)
	{
	    for(int c = 0;c<CurrentGameCol;c++)
	    {
		if (GameMap[r][c] == HOVERBLOCK)
		    GameMap[r][c] = ACTIVEBLOCK;
	    }
	}
	
	Hover = true;

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
			if(GameMap[r][c] == ACTIVEBLOCK)
			{
				GameMap[r][c] = setID;
			}
		}
	}
	Hover = false;
}

//This method moves piece East on the grid regardless of gravity.
void GameStateComponent::ShiftRight(int id)
{
	int arrayOfPoints[8] = {-2};
	int arrayTicker = 0;
	bool obstacleFlag = false;

	for(int r=0;r<CurrentGameRow;r++)
	{
		for(int c=0;c<CurrentGameCol;c++)
		{
			if(GameMap[r][c] == id)
			{
				arrayOfPoints[arrayTicker] = r;
				arrayTicker++;
				arrayOfPoints[arrayTicker] = c;
				arrayTicker++;
			}
		}
	}

	if (arrayOfPoints[0] == -2)
	{
		return;
	}



	//Check right of each point. y+1
	for (int i=0; i < (sizeof(arrayOfPoints) / sizeof(arrayOfPoints[0])); i = i + 2)
	{
		if (arrayOfPoints[i] != -2)
		{
			if (isEmptyBlock(GameMap[arrayOfPoints[i]][arrayOfPoints[i+1] + 1]) or GameMap[arrayOfPoints[i]][arrayOfPoints[i+1] + 1] == id)
			{
			}
			else
			{
				obstacleFlag = true;
			}
		}
	}

	 // Set old points to 0 first, then write new points as id. This avoids 0ing out points moved.
	if (not obstacleFlag)
	{
		for (int i=0; i < (sizeof(arrayOfPoints) / sizeof(arrayOfPoints[0])); i = i + 2)
		{
			if (arrayOfPoints[i] != -2)
			{
				GameMap[arrayOfPoints[i]][arrayOfPoints[i+1]] = EMPTYBLOCK;
			}
		}
		for (int i=0; i < (sizeof(arrayOfPoints) / sizeof(arrayOfPoints[0])); i = i + 2)
		{
			if (arrayOfPoints[i] != -2)
			{
				GameMap[arrayOfPoints[i]][arrayOfPoints[i+1] + 1] = id;
			}
		}	
	}else
	{
		if(id==ACTIVEBLOCK)
			PlaceNewShape();
	}

}

//This method moves piece West on the grid regardless of gravity.
void GameStateComponent::ShiftLeft(int id)
{
	int arrayOfPoints[8] = {-2};
	int arrayTicker = 0;
	bool obstacleFlag = false;

	for(int r=0;r<CurrentGameRow;r++)
	{
		for(int c=0;c<CurrentGameCol;c++)
		{
			if(GameMap[r][c] == id)
			{
				arrayOfPoints[arrayTicker] = r;
				arrayTicker++;
				arrayOfPoints[arrayTicker] = c;
				arrayTicker++;
			}
		}
	}

	if (arrayOfPoints[0] == -2)
	{
		return;
	}

	//Check right of each point. y-1
	for (int i=0; i < (sizeof(arrayOfPoints) / sizeof(arrayOfPoints[0])); i = i + 2)
	{
		if (arrayOfPoints[i] != -2)
		{
			if (isEmptyBlock(GameMap[arrayOfPoints[i]][arrayOfPoints[i+1] - 1]) or GameMap[arrayOfPoints[i]][arrayOfPoints[i+1] - 1] == id)
			{
				//printf("isEmptyBlock %d \n",isEmptyBlock(GameMap[arrayOfPoints[i]][arrayOfPoints[i+1] - 1]));
				printf("arrayOfPoints[i]: %d ,arrayOfPoints[i+1] - 1: %d \n",arrayOfPoints[i], arrayOfPoints[i+1] - 1);
				//printf("GameMap[arrayOfPoints[i]][arrayOfPoints[i+1] - 1] %d, %d \n",GameMap[arrayOfPoints[i]][arrayOfPoints[i+1] - 1], id);
			}
			else
			{
				obstacleFlag = true;
			}
		}
	}

	 // Set old points to 0 first, then write new points as id. This avoids 0ing out points moved.
	if (not obstacleFlag)
	{
		for (int i=0; i < (sizeof(arrayOfPoints) / sizeof(arrayOfPoints[0])); i = i + 2)
		{
			if (arrayOfPoints[i] != -2)
			{
				GameMap[arrayOfPoints[i]][arrayOfPoints[i+1]] = EMPTYBLOCK;
			}
		}
		for (int i=0; i < (sizeof(arrayOfPoints) / sizeof(arrayOfPoints[0])); i = i + 2)
		{
			if (arrayOfPoints[i] != -2)
			{
				GameMap[arrayOfPoints[i]][arrayOfPoints[i+1] - 1] = id;
			}
		}
	}
	else
	{
		if(id==ACTIVEBLOCK)
			PlaceNewShape();
	}

}

//This method moves piece North on the grid regardless of gravity.
void GameStateComponent::ShiftUp(int id)
{
	int arrayOfPoints[8] = {-2};
	int arrayTicker = 0;
	bool obstacleFlag = false;

	for(int r=0;r<CurrentGameRow;r++)
	{
		for(int c=0;c<CurrentGameCol;c++)
		{
			if(GameMap[r][c] == id)
			{
				arrayOfPoints[arrayTicker] = r;
				arrayTicker++;
				arrayOfPoints[arrayTicker] = c;
				arrayTicker++;
			}
		}
	}

	if (arrayOfPoints[0] == -2)
	{
		return;
	}

	//Check below each point. x-1
	for (int i=0; i < (sizeof(arrayOfPoints) / sizeof(arrayOfPoints[0])); i = i + 2)
	{
		if (arrayOfPoints[i] != -2)
		{
			if ((isEmptyBlock(GameMap[arrayOfPoints[i] - 1][arrayOfPoints[i+1]]) or GameMap[arrayOfPoints[i] - 1][arrayOfPoints[i+1]] == id) 
				and not(arrayOfPoints[i] - 1 < 0))
			{
			}
			else
			{
				obstacleFlag = true;
			}
		}
	}

	 // Set old points to 0 first, then write new points as id. This avoids 0ing out points moved.
	if (not obstacleFlag)
	{
		for (int i=0; i < (sizeof(arrayOfPoints) / sizeof(arrayOfPoints[0])); i = i + 2)
		{
			if (arrayOfPoints[i] != -2)
			{
				GameMap[arrayOfPoints[i]][arrayOfPoints[i+1]] = EMPTYBLOCK;
			}
		}
		for (int i=0; i < (sizeof(arrayOfPoints) / sizeof(arrayOfPoints[0])); i = i + 2)
		{
			if (arrayOfPoints[i] != -2)
			{
				GameMap[arrayOfPoints[i] - 1][arrayOfPoints[i+1]] = id;
			}
		}
	}else
	{
		if(id==ACTIVEBLOCK)
			PlaceNewShape();
	}
}

//This method moves piece South on the grid regardless of gravity.
void GameStateComponent::ShiftDown(int id)
{
	int arrayOfPoints[8] = {-2};	
	int arrayTicker = 0;
	bool obstacleFlag = false;

	for(int r=0;r<CurrentGameRow;r++)
	{
		for(int c=0;c<CurrentGameCol;c++)
		{
			if(GameMap[r][c] == id)
			{
				arrayOfPoints[arrayTicker] = r;
				arrayTicker++;
				arrayOfPoints[arrayTicker] = c;
				arrayTicker++;
			}
		}
	}
	if (arrayOfPoints[0] == -2)
	{
		return;
	}

	//Check below each point. x+1
	for (int i=0; i < (sizeof(arrayOfPoints) / sizeof(arrayOfPoints[0])); i = i + 2)
	{
		if (arrayOfPoints[i] != -2)
		{
			if (isEmptyBlock(GameMap[arrayOfPoints[i] + 1][arrayOfPoints[i+1]]) or GameMap[arrayOfPoints[i] + 1][arrayOfPoints[i+1]] == id)
			{
			}
			else
			{
				obstacleFlag = true;
			}
		}
	}
	 // Set old points to 0 first, then write new points as id. This avoids 0ing out points moved.
	if (not obstacleFlag)
	{
		for (int i=0; i < (sizeof(arrayOfPoints) / sizeof(arrayOfPoints[0])); i = i + 2)
		{
			if (arrayOfPoints[i] != -2)
			{
				GameMap[arrayOfPoints[i]][arrayOfPoints[i+1]] = EMPTYBLOCK;
			}
		}
		for (int i=0; i < (sizeof(arrayOfPoints) / sizeof(arrayOfPoints[0])); i = i + 2)
		{
			if (arrayOfPoints[i] != -2)
			{
				GameMap[arrayOfPoints[i] + 1][arrayOfPoints[i+1]] = id;
			}
		}
	}else
	{
		if(id==ACTIVEBLOCK)
			PlaceNewShape();
	}
}

//This method rotates piece clockwise relative to a "normal" gravity.
void GameStateComponent::RotateClockwise(int id)
{
	//get (x,y) of points. also get x and y values in seperate array
	int arrayOfPoints[8] = {-2};
	int arrayOfPointsOLD[8];
	int arrayTicker = 0;
	int arrayX[4];
	int arrayXYcounter = 0;
	int arrayY[4];
	int minX;
	int minY;
	
	for(int r=0;r<CurrentGameRow;r++)
	{
		for(int c=0;c<CurrentGameCol;c++)
		{
			if(GameMap[r][c] == id)
			{
				arrayOfPoints[arrayTicker] = r;
				arrayOfPointsOLD[arrayTicker] = r;
				arrayX[arrayXYcounter] = r;
				arrayTicker++;
				arrayOfPoints[arrayTicker] = c;
 				arrayOfPointsOLD[arrayTicker] = c;
				arrayY[arrayXYcounter] = c;
				arrayTicker++;
				arrayXYcounter++;
			}
		}

	}

	if (arrayOfPoints[0] == -2)
	{
		return;
	}
	


	//Sort
	std::sort(arrayX, arrayX + 4);
	std::sort(arrayY, arrayY + 4);
	minX = arrayX[0];
	minY = arrayY[0];
	


	//"Normalize" each point to the min
	for (int i=0; i < (sizeof(arrayOfPoints) / sizeof(arrayOfPoints[0])); i = i + 2)
	{
		if (arrayOfPoints[i] != -2)
		{

			arrayOfPoints[i] = arrayOfPoints[i] - minX;
			arrayOfPoints[i+1] = arrayOfPoints[i+1] - minY;
		}
	}
	
	//Place normalized points into a 4x4 2D array
	int rotaterArray[4][4];

	for(int a=0;a<4;a++)
	{
		for(int b=0;b<4;b++)
		{
			rotaterArray[a][b] = 0;
		}
	}
	for (int i=0; i < (sizeof(arrayOfPoints) / sizeof(arrayOfPoints[0])); i = i + 2)
	{
		if (arrayOfPoints[i] != -2)
		{
			rotaterArray[arrayOfPoints[i]][arrayOfPoints[i+1]] = 1;
		}
	}

	int rotatedArray[4][4];
	for(int a=0;a<4;a++)
	{
		for(int b=0;b<4;b++)
		{
			rotatedArray[a][b] = 0;
		}
	}
	//Rotate
	for(int a=0;a<4;a++)
	{
		for(int b=0;b<4;b++)
		{
			rotatedArray[a][b] = rotaterArray[4-b-1][a];
		}
	}

	int arrayOfRotatedPoints[8];
	int newCounter = 0;
	for(int a=0;a<4;a++)
	{
		for(int b=0;b<4;b++)
		{
			if (rotatedArray[a][b] > 0)
			{
				arrayOfRotatedPoints[newCounter] = a;
				newCounter++;
				arrayOfRotatedPoints[newCounter] = b;
				newCounter++;
			}
		}
	}

	int row=0;
	int col=0;
	bool obstacleFlag = false;

	for(int z=0; z<(sizeof(arrayOfRotatedPoints) / sizeof(arrayOfRotatedPoints[0])); z = z+2)
	{
		if (arrayOfRotatedPoints[z] != -2)
		{
			row = arrayOfRotatedPoints[z] + minX;
			col = arrayOfRotatedPoints[z+1] + minY;
			if ((isEmptyBlock(GameMap[row][col]) or GameMap[row][col] == id) 
				and not(row < 0) and not(row > CurrentGameRow) and not(col > CurrentGameCol) and not(col < 0))
			{
			}
			else
			{
				obstacleFlag = true;
			}
		}
	}

	if (not obstacleFlag){
	  	//Zero-out previous shape place
		for(int z=0; z< (sizeof(arrayOfPointsOLD) / sizeof(arrayOfPointsOLD[0])); z = z+2)
		{
			if (arrayOfPointsOLD[z] != -2)
			{
				GameMap[arrayOfPointsOLD[z]][arrayOfPointsOLD[z+1]]= EMPTYBLOCK;
			}
		}

		for(int z=0; z<(sizeof(arrayOfRotatedPoints) / sizeof(arrayOfRotatedPoints[0])); z = z+2)
		{
			if (arrayOfRotatedPoints[z] != -2)
			{
				row = arrayOfRotatedPoints[z] + minX;
				col = arrayOfRotatedPoints[z+1] + minY;
				GameMap[row][col] = id;
			}
		}
	}else
	{
		if(id==ACTIVEBLOCK)
			PlaceNewShape();
	}
}

//This method rotates piece counter-clockwise relative to a "normal" gravity.
void GameStateComponent::RotateCounterClockwise(int id)
{
  	//get (x,y) of points. also get x and y values in seperate array
	int arrayOfPoints[8] = {-2};
	int arrayOfPointsOLD[8];
	int arrayTicker = 0;
	int arrayX[4];
	int arrayXYcounter = 0;
	int arrayY[4];
	int minX;
	int minY;
	
	for(int r=0;r<CurrentGameRow;r++)
	{
		for(int c=0;c<CurrentGameCol;c++)
		{
			if(GameMap[r][c] == id)
			{
				arrayOfPoints[arrayTicker] = r;
				arrayOfPointsOLD[arrayTicker] = r;
				arrayX[arrayXYcounter] = r;
				arrayTicker++;
				arrayOfPoints[arrayTicker] = c;
 				arrayOfPointsOLD[arrayTicker] = c;
				arrayY[arrayXYcounter] = c;
				arrayTicker++;
				arrayXYcounter++;
			}
		}

	}

	if (arrayOfPoints[0] == -2)
	{
		return;
	}
	


	//Sort
	std::sort(arrayX, arrayX + 4);
	std::sort(arrayY, arrayY + 4);
	minX = arrayX[0];
	minY = arrayY[0];
	


	//"Normalize" each point to the min
	for (int i=0; i < (sizeof(arrayOfPoints) / sizeof(arrayOfPoints[0])); i = i + 2)
	{
		if (arrayOfPoints[i] != -2)
		{
			arrayOfPoints[i] = arrayOfPoints[i] - minX;
			arrayOfPoints[i+1] = arrayOfPoints[i+1] - minY;
		}
	}
	
	//Place normalized points into a 4x4 2D array
	int rotaterArray[4][4];

	for(int a=0;a<4;a++)
	{
		for(int b=0;b<4;b++)
		{
			rotaterArray[a][b] = 0;
		}
	}
	for (int i=0; i < (sizeof(arrayOfPoints) / sizeof(arrayOfPoints[0])); i = i + 2)
	{
		if (arrayOfPoints[i] != -2)
		{
			rotaterArray[arrayOfPoints[i]][arrayOfPoints[i+1]] = 1;
		}
	}

	int rotatedArray[4][4];
	for(int a=0;a<4;a++)
	{
		for(int b=0;b<4;b++)
		{
			rotatedArray[a][b] = 0;
		}
	}
	//Rotate
	for(int a=0;a<4;a++)
	{
		for(int b=0;b<4;b++)
		{
			rotatedArray[a][b] = rotaterArray[b][4-a-1];
		}
	}

	int arrayOfRotatedPoints[8];
	int newCounter = 0;
	for(int a=0;a<4;a++)
	{
		for(int b=0;b<4;b++)
		{
			if (rotatedArray[a][b] > 0)
			{
				arrayOfRotatedPoints[newCounter] = a;
				newCounter++;
				arrayOfRotatedPoints[newCounter] = b;
				newCounter++;
			}
		}
	}

	int row=0;
	int col=0;
	bool obstacleFlag = false;

	for(int z=0; z<(sizeof(arrayOfRotatedPoints) / sizeof(arrayOfRotatedPoints[0])); z = z+2)
	{
		if (arrayOfRotatedPoints[z] != -2)
		{
			row = arrayOfRotatedPoints[z] + minX -1;
			col = arrayOfRotatedPoints[z+1] + minY;
			if ((isEmptyBlock(GameMap[row][col]) or GameMap[row][col] == id) 
				and not(row < 0) and not(row > CurrentGameRow) and not(col > CurrentGameCol) and not(col < 0))
			{
			}
			else
			{
				obstacleFlag = true;
			}
		}
	}

	if (not obstacleFlag){
	  	//Zero-out previous shape place
		for(int z=0; z< (sizeof(arrayOfPointsOLD) / sizeof(arrayOfPointsOLD[0])); z = z+2)
		{
			if (arrayOfPointsOLD[z] != -2)
			{
				GameMap[arrayOfPointsOLD[z]][arrayOfPointsOLD[z+1]]= EMPTYBLOCK;
			}
		}

		for(int z=0; z<(sizeof(arrayOfRotatedPoints) / sizeof(arrayOfRotatedPoints[0])); z = z+2)
		{
			if (arrayOfRotatedPoints[z] != -2)
			{
				row = arrayOfRotatedPoints[z] + minX -1;
				col = arrayOfRotatedPoints[z+1] + minY;
				GameMap[row][col] = id;
			}
		}
	}else
	{
		if(id==ACTIVEBLOCK)
			PlaceNewShape();
	}

}
