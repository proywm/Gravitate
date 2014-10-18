#pragma once
#include "GlobalHeader.h"
namespace ActorShape
{
	class Shape
	{
	public:
		Shape(void);
		~Shape(void);

	};
	class Rectangle : public Shape
	{
	public:
		Rectangle(void);
		~Rectangle(void);
		void setSize(double Height, double Width);

		sf::RectangleShape actorShape;
		double width;
		double height;
	};
	class Circle : public Shape
	{
		public:
			Circle(void);
			~Circle(void);
			void setSize(double Radious);

			sf::CircleShape actorShape;
			double radious;
	};
}