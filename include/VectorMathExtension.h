#pragma once
#include "GlobalHeader.h"
class VectorMathExtension
{
public:
	VectorMathExtension(void);
	~VectorMathExtension(void);

	float	Dot(sf::Vector2f &u,sf::Vector2f &v);
	float	SelfDot(sf::Vector2f &v);

	sf::Vector2f	Normalize(sf::Vector2f &v);

	float	Length(sf::Vector2f &v);
	float	Distance(sf::Vector2f &u,sf::Vector2f &v);

};

