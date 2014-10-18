#include "VectorMathExtension.h"
#include "math.h"

VectorMathExtension::VectorMathExtension(void)
{
}


VectorMathExtension::~VectorMathExtension(void)
{
}

float VectorMathExtension::Dot(sf::Vector2f &u,sf::Vector2f &v)
{
	return u.x*v.x + u.y*v.y;
}

float VectorMathExtension::SelfDot(sf::Vector2f &v)
{
	return v.x*v.x + v.y*v.y;
}

sf::Vector2f VectorMathExtension::Normalize(sf::Vector2f &v)
{
	sf::Vector2f	temp;

	temp = v * (float)(1.f / (float)sqrt(SelfDot(v)));

	return temp;
}


float VectorMathExtension::Length(sf::Vector2f &v)
{
	return (float)sqrt(v.x*v.x + v.y*v.y);
}

float VectorMathExtension::Distance(sf::Vector2f &u,sf::Vector2f &v)
{
	sf::Vector2f	temp = u - v;
	return Length(temp);
}


