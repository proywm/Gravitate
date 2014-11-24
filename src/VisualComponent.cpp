#include "VisualComponent.h"
#include "DisplayManager.h"
#include "PhysicalComponent.h"
#include "GameStateComponent.h"
#include "TextAreaComponent.h"
#include <sstream>
#include <stdlib.h>
VisualComponent::VisualComponent(void)
{
}


VisualComponent::~VisualComponent(void)
{
}

void VisualComponent::init(XMLElement *componentElement)
{
	XMLElement* textureElement;
	const char* textureFileLocation;
	const char* fontFileLocation;
	std::ostringstream LabelString; 

	ComponentID = componentElement->IntAttribute("ComponentType");
	viewType = (ViewType)componentElement->IntAttribute("GameViewId");
	XMLElement* colorElement = componentElement->FirstChildElement("Color");
	//actorColor = new sf::Color(colorElement->IntAttribute("r"),colorElement->IntAttribute("g"),colorElement->IntAttribute("b"),colorElement->IntAttribute("a"));
	XMLElement* shapeElement = colorElement->NextSiblingElement("Structure");
	shapeID = shapeElement->IntAttribute("ShapeID");
	switch(shapeElement->IntAttribute("ShapeID"))
	{
		case 1://"Rectangle":
			actorShape = new ActorShape::Rectangle();
			((ActorShape::Rectangle*)actorShape)->setSize(shapeElement->FloatAttribute("Height"), shapeElement->FloatAttribute("Width"));
			((ActorShape::Rectangle*)actorShape)->actorShape.setFillColor((const sf::Color &)actorColor);//(const sf::Color &)actorColor
			break;
		case 2://"Circle":
			actorShape = new ActorShape::Circle();
			((ActorShape::Circle*)actorShape)->setSize(shapeElement->FloatAttribute("Radious"));
			((ActorShape::Circle*)actorShape)->actorShape.setFillColor((const sf::Color &)actorColor);
			break;
		case 3://"GRID MAP":
			actorShape = new ActorShape::GridMap();
			((ActorShape::GridMap*)actorShape)->setBlockSize(shapeElement->IntAttribute("BlockHeight"),shapeElement->IntAttribute("BlockWidth"));
			
			break;
		case 5://TextArea
			textureElement = shapeElement->NextSiblingElement("Texture");
			textureFileLocation = textureElement->Attribute("TextureFileName");
			actorShape = new ActorShape::GridMap();
			((ActorShape::GridMap*)actorShape)->LoadTexture(textureFileLocation);
			((ActorShape::GridMap*)actorShape)->setBlockSize(shapeElement->IntAttribute("BlockHeight"),shapeElement->IntAttribute("BlockWidth"));
			fontFileLocation = textureElement->Attribute("FontFileName");
			((ActorShape::GridMap*)actorShape)->LoadFont(fontFileLocation);
			LabelString << textureElement->Attribute("Text");// put float into string buffer
			((ActorShape::GridMap*)actorShape)->SetTextInBox(LabelString, 0, 0);
			break;
		default:
			actorShape = new ActorShape::Circle();
			((ActorShape::Circle*)actorShape)->setSize(10);
			((ActorShape::Circle*)actorShape)->actorShape.setFillColor((const sf::Color &)actorColor);
			break;
	}
	isVisible = true;
}

void VisualComponent::update(double deltaMS)
{
	/*sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
	DisplayManager::instance()->window.draw(shape);*/
	if(!isVisible)
		return;
	PhysicalComponent* physicalComponent = (PhysicalComponent*)owner->GetComponent(PHYSICAL);
	if(shapeID == 1)
	{
		((ActorShape::Rectangle*)actorShape)->actorShape.setPosition(physicalComponent->getActorPosition());
		((ActorShape::Rectangle*)actorShape)->actorShape.setFillColor(sf::Color::Green);
		DisplayManager::instance()->window.draw(((ActorShape::Rectangle*)actorShape)->actorShape);
	}
	else if(shapeID == 2)
	{
		((ActorShape::Circle*)actorShape)->actorShape.setPosition(physicalComponent->getActorPosition());
		((ActorShape::Circle*)actorShape)->actorShape.setFillColor(sf::Color::Green);
		DisplayManager::instance()->window.draw(((ActorShape::Circle*)actorShape)->actorShape);
	}
	else if(shapeID == 3)//Grid Map
	{
		GameStateComponent* gameStateComponent = (GameStateComponent*)owner->GetComponent(GAMESTATE);
		for(int r=0;r<gameStateComponent->CurrentGameRow;r++)
		{
			for(int c=0;c<gameStateComponent->CurrentGameCol;c++)
			{
				int posX = physicalComponent->getActorPosition().x + (c * ((ActorShape::GridMap*)actorShape)->blockWidth)+ c;
				int posY = physicalComponent->getActorPosition().y + 
									(r * ((ActorShape::GridMap*)actorShape)->blockHeight) + r;
				((ActorShape::GridMap*)actorShape)->gridMap[r][c].setPosition(sf::Vector2f(posX, posY));
				if(gameStateComponent->GameMap[r][c]>ACTIVEBLOCK)//placed
				{
					((ActorShape::GridMap*)actorShape)->gridMap[r][c].setFillColor(sf::Color::Green);
				}
				else if(gameStateComponent->GameMap[r][c]==ACTIVEBLOCK)
				{
					((ActorShape::GridMap*)actorShape)->gridMap[r][c].setFillColor(sf::Color::Red);			
				}
				else if(gameStateComponent->GameMap[r][c]==HOVERBLOCK)
				{
					((ActorShape::GridMap*)actorShape)->gridMap[r][c].setFillColor(sf::Color::Red);			
				}
				else if(gameStateComponent->GameMap[r][c]==EMPTYBLOCK)
				{
					((ActorShape::GridMap*)actorShape)->gridMap[r][c].setFillColor(sf::Color::Blue);			
				}
				else if(gameStateComponent->GameMap[r][c]==CONCRETEBLOCK)
				{
					((ActorShape::GridMap*)actorShape)->gridMap[r][c].setFillColor(sf::Color::Black);			
				}			
				DisplayManager::instance()->window.draw(((ActorShape::GridMap*)actorShape)->gridMap[r][c]);
				
			}
		}
	}
	else if(shapeID == 5)//"TextAreaComponent"
	{
		TextAreaComponent* textAreaComponent = (TextAreaComponent*)owner->GetComponent(TEXTAREA);
		for(int r=0;r<textAreaComponent->CurrentTextAreaRow;r++)
		{
			for(int c=0;c<textAreaComponent->CurrentTextAreaCol;c++)
			{
				int posX = physicalComponent->getActorPosition().x + (c * ((ActorShape::GridMap*)actorShape)->blockWidth)+ c;
				int posY = physicalComponent->getActorPosition().y + 
									(r * ((ActorShape::GridMap*)actorShape)->blockHeight) + r;
				((ActorShape::GridMap*)actorShape)->gridMapSprite[r][c].setPosition(sf::Vector2f(posX, posY));
				if(textAreaComponent->TextArea[r][c]==1)
				{
					((ActorShape::GridMap*)actorShape)->gridMap[r][c].setFillColor(sf::Color::Green);
				}				
				else
					((ActorShape::GridMap*)actorShape)->gridMap[r][c].setFillColor(sf::Color::Black);
				DisplayManager::instance()->window.draw(((ActorShape::GridMap*)actorShape)->gridMapSprite[r][c]);
			}
		}
		for(int r=0;r<textAreaComponent->CurrentTextAreaRow;r++)
		{
			for(int c=0;c<textAreaComponent->CurrentTextAreaCol;c++)
			{
				int posX = physicalComponent->getActorPosition().x + (c * ((ActorShape::GridMap*)actorShape)->blockWidth)+ c;
				int posY = physicalComponent->getActorPosition().y + 
									(r * ((ActorShape::GridMap*)actorShape)->blockHeight) + r;
				((ActorShape::GridMap*)actorShape)->gridMapSprite[r][c].setPosition(sf::Vector2f(posX, posY));
				((ActorShape::GridMap*)actorShape)->gridMaptext[r][c].setPosition(posX,posY);
				DisplayManager::instance()->window.draw(((ActorShape::GridMap*)actorShape)->gridMaptext[r][c]);
			}
		}
	}
}
void VisualComponent::setVisibility(bool visible)
{
	isVisible = visible;
}
bool VisualComponent::IsAnInclusivePoint(int posX, int posY)
{
	int Top, Bottom, Left, Right;
	PhysicalComponent* physicalComponent = (PhysicalComponent*)owner->GetComponent(PHYSICAL);
	if(shapeID == 1)
	{
		Right = 15;
		Bottom = 15;
		Left = 0;
		Top = 0;
	}
	else if(shapeID == 2)
	{
		Right = 15;
		Bottom = 15;
		Left = 0;
		Top = 0;
	}
	else if(shapeID == 3)//Grid Map
	{
		GameStateComponent* gameStateComponent = (GameStateComponent*)owner->GetComponent(GAMESTATE);
		Right = physicalComponent->getActorPosition().x + (gameStateComponent->CurrentGameCol *
						((ActorShape::GridMap*)actorShape)->blockWidth);
		Bottom = physicalComponent->getActorPosition().y + (gameStateComponent->CurrentGameRow * 
						((ActorShape::GridMap*)actorShape)->blockHeight);
		Left = physicalComponent->getActorPosition().x;
		Top = physicalComponent->getActorPosition().y;
	}
	else if(shapeID == 5)//"TextAreaComponent"
	{
		TextAreaComponent* textAreaComponent = (TextAreaComponent*)owner->GetComponent(TEXTAREA);
		Right = physicalComponent->getActorPosition().x + (textAreaComponent->CurrentTextAreaCol *
						((ActorShape::GridMap*)actorShape)->blockWidth);
		Bottom = physicalComponent->getActorPosition().y + (textAreaComponent->CurrentTextAreaRow * 
						((ActorShape::GridMap*)actorShape)->blockHeight);
		Left = physicalComponent->getActorPosition().x;
		Top = physicalComponent->getActorPosition().y;
	}
	if(Left<=posX && Right>=posX && Top <= posY && Bottom >=posY)
		return true;
	else
		return false;
}
