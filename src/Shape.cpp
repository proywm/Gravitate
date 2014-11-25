#include "Shape.h"
#include <sstream>
#include <stdlib.h>
ActorShape::Shape::Shape(void)
{
}
ActorShape::Shape::~Shape(void)
{
}

ActorShape::Rectangle::Rectangle(void)
{}
ActorShape::Rectangle::~Rectangle(void)
{}
void ActorShape::Rectangle::setSize(double Height, double Width)
{
	height = Height;
	width = Width;
	actorShape.setSize(sf::Vector2f(height, width));
}
ActorShape::Circle::Circle(void)
{}
ActorShape::Circle::~Circle(void)
{}
void ActorShape::Circle::setSize(double Radious)
{
	radious = Radious;
	actorShape.setRadius(radious);
}
ActorShape::GridMap::GridMap(void)
{}
ActorShape::GridMap::~GridMap(void)
{}
void ActorShape::GridMap::LoadTexture(const char* fileLocation)
{
	if (!texture.loadFromFile(fileLocation))
	{
		printf("Error loading Texture file \n");
		hasTexture = false;
	}
	else
		hasTexture = true;
}
void ActorShape::GridMap::LoadFont(const char* fileLocation)
{
	if (!font.loadFromFile(fileLocation))
	{
		printf("Error loading Font file \n");
	}
}
void ActorShape::GridMap::SetText(int size, int color,sf::Text::Style t)
{
	
		
	for(int r=0;r<MAXROW;r++)
	{
		for(int c=0;c<MAXCOL;c++)
		{
			gridMaptext[r][c].setFont(font); 
			gridMaptext[r][c].setCharacterSize(size); // in pixels, not points!
			if(color == 1)
				gridMaptext[r][c].setColor(sf::Color(50, 205, 50));
			if(color == 2)
				gridMaptext[r][c].setColor(sf::Color(178, 34, 34));
			if(color == 3)
				gridMaptext[r][c].setColor(sf::Color::Yellow);
			gridMaptext[r][c].setStyle(t);
		}
	}
}
void ActorShape::GridMap::SetTextInBox(std::ostringstream& string, int PosX, int PosY)
{
	gridMaptext[PosY][PosX].setString(string.str());
}
void ActorShape::GridMap::setBlockSize(int BlockHeight,int BlockWidth)
{
	blockHeight = BlockHeight;
	blockWidth = BlockWidth;
	for(int r=0;r<MAXROW;r++)
	{
		for(int c=0;c<MAXCOL;c++)
		{
			gridMap[r][c].setSize(sf::Vector2f(blockWidth,blockHeight));
			if(hasTexture)
			{
				gridMapSprite[r][c].setTexture(texture);
				gridMapSprite[r][c].setTextureRect(sf::IntRect(0, 0,blockWidth, blockHeight));
			}
		}
	}
	SetText(15,1,sf::Text::Bold);
}
void ActorShape::GridMap::setTexture()
{
	if(hasTexture)
	{
		for(int r=0;r<MAXROW;r++)
		{
			for(int c=0;c<MAXCOL;c++)
			{
				gridMapSprite[r][c].setTexture(texture);
			}
		}
	}
}
