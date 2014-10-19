#include "Shape.h"
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
void ActorShape::GridMap::setBlockSize(double BlockSize)
{
	blockSize = BlockSize;
	for(int r=0;r<MAXROW;r++)
	{
		for(int c=0;c<MAXCOL;c++)
		{
			gridMap[r][c].setSize(sf::Vector2f(blockSize, blockSize));
		}
	}
}
