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
	}
}
void ActorShape::GridMap::LoadFont(const char* fileLocation)
{
	if (!font.loadFromFile(fileLocation))
	{
		printf("Error loading Font file \n");
	}
}
void ActorShape::GridMap::SetText()
{
	for(int r=0;r<MAXROW;r++)
	{
		for(int c=0;c<MAXCOL;c++)
		{
			gridMaptext[r][c].setFont(font); 
			gridMaptext[r][c].setCharacterSize(15); // in pixels, not points!
			gridMaptext[r][c].setColor(sf::Color::Red);
			gridMaptext[r][c].setStyle(sf::Text::Bold | sf::Text::Underlined);
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
			gridMapSprite[r][c].setTexture(texture);
			gridMapSprite[r][c].setTextureRect(sf::IntRect(0, 0,blockWidth, blockHeight));
		}
	}
	SetText();
}
void ActorShape::GridMap::setTexture()
{
	for(int r=0;r<MAXROW;r++)
	{
		for(int c=0;c<MAXCOL;c++)
		{
			gridMapSprite[r][c].setTexture(texture);
		}
	}
}
