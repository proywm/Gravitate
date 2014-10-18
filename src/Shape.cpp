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
