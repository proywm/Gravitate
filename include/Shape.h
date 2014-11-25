#pragma once
#include "GlobalHeader.h"
#define MAXROW 50
#define MAXCOL 50

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
	class GridMap : public Shape
	{
		public:
			GridMap(void);
			~GridMap(void);
			void setBlockSize(int BlockHeight,int BlockWidth);
			void LoadTexture(const char* fileLocation);
			bool hasTexture;
			void LoadFont(const char* fileLocation);
			void SetText(int size, int color,sf::Text::Style t);
			void setTexture();
			void SetTextInBox(std::ostringstream& string, int PosX, int PosY);

			int blockHeight;
			int blockWidth;
			sf::RectangleShape gridMap[MAXROW][MAXCOL];
			sf::Texture texture;
			sf::Sprite gridMapSprite[MAXROW][MAXCOL];
			sf::Font font;
			sf::Text gridMaptext[MAXROW][MAXCOL];
	};
}
