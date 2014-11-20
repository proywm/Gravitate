#include "VisualComponent.h"
#include "DisplayManager.h"
#include "PhysicalComponent.h"
#include "GameStateComponent.h"
#include "SelectionToolBarComponent.h"

VisualComponent::VisualComponent(void)
{
}


VisualComponent::~VisualComponent(void)
{
}

void VisualComponent::init(XMLElement *componentElement)
{
	ComponentID = componentElement->IntAttribute("ComponentType");
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
		case 4://"SelectionToolBarMAP"-> "GRID MAP":
			actorShape = new ActorShape::GridMap();
			((ActorShape::GridMap*)actorShape)->setBlockSize(shapeElement->FloatAttribute("BlockWidth"));
			
			break;
		default:
			actorShape = new ActorShape::Circle();
			((ActorShape::Circle*)actorShape)->setSize(10);
			((ActorShape::Circle*)actorShape)->actorShape.setFillColor((const sf::Color &)actorColor);
			break;
	}
	
}

void VisualComponent::update(double deltaMS)
{
	/*sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
	DisplayManager::instance()->window.draw(shape);*/
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
				int posX = physicalComponent->getActorPosition().x + (c * ((ActorShape::GridMap*)actorShape)->blockSize)+ c;
				int posY = physicalComponent->getActorPosition().y + 
									(r * ((ActorShape::GridMap*)actorShape)->blockSize) + r;
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
	else if(shapeID == 4)//"SelectionToolBarMAP"
	{
		SelectionToolBarComponent* selectionToolBarComponent = (SelectionToolBarComponent*)owner->GetComponent(SELECTIONTOOLBAR);
		for(int r=0;r<selectionToolBarComponent->CurrentToolBarRow;r++)
		{
			for(int c=0;c<selectionToolBarComponent->CurrentToolBarCol;c++)
			{
				int posX = physicalComponent->getActorPosition().x + (c * ((ActorShape::GridMap*)actorShape)->blockSize)+ c;
				int posY = physicalComponent->getActorPosition().y + 
									(r * ((ActorShape::GridMap*)actorShape)->blockSize) + r;
				
				((ActorShape::GridMap*)actorShape)->gridMap[r][c].setPosition(sf::Vector2f(posX, posY));
				if(selectionToolBarComponent->GameMap[r][c]==STRAIGHTPOLYOMINO)
				{
					((ActorShape::GridMap*)actorShape)->gridMap[r][c].setFillColor(sf::Color::Green);
				}
				else if(selectionToolBarComponent->GameMap[r][c]==SQUAREPOLYOMINO)
				{
					((ActorShape::GridMap*)actorShape)->gridMap[r][c].setFillColor(sf::Color::Red);			
				}
				else if(selectionToolBarComponent->GameMap[r][c]==TPOLYOMINO)
				{
					((ActorShape::GridMap*)actorShape)->gridMap[r][c].setFillColor(sf::Color::Yellow);
				}
				else if(selectionToolBarComponent->GameMap[r][c]==JPOLYOMINO)
				{
					((ActorShape::GridMap*)actorShape)->gridMap[r][c].setFillColor(sf::Color::Magenta);			
				}
				else if(selectionToolBarComponent->GameMap[r][c]==LPOLYOMINO)
				{
					((ActorShape::GridMap*)actorShape)->gridMap[r][c].setFillColor(sf::Color::White);
				}
				else if(selectionToolBarComponent->GameMap[r][c]==SPOLYOMINO)
				{
					((ActorShape::GridMap*)actorShape)->gridMap[r][c].setFillColor(sf::Color::Cyan);			
				}
				else if(selectionToolBarComponent->GameMap[r][c]==ZPOLYOMINO)
				{
					((ActorShape::GridMap*)actorShape)->gridMap[r][c].setFillColor(sf::Color::Black);			
				}
				else if(selectionToolBarComponent->GameMap[r][c]==EMPTYBLOCK)
				{
					((ActorShape::GridMap*)actorShape)->gridMap[r][c].setFillColor(sf::Color::Blue);			
				}
				else if(selectionToolBarComponent->GameMap[r][c]==CONCRETEBLOCK)
				{
					((ActorShape::GridMap*)actorShape)->gridMap[r][c].setFillColor(sf::Color::Black);			
				}			
				DisplayManager::instance()->window.draw(((ActorShape::GridMap*)actorShape)->gridMap[r][c]);
				
			}
		}
	}
}
