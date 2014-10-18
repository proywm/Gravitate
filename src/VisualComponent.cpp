#include "VisualComponent.h"
#include "DisplayManager.h"
#include "PhysicalComponent.h"
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
		default:
			actorShape = new ActorShape::Circle();
			((ActorShape::Circle*)actorShape)->setSize(10);
			((ActorShape::Circle*)actorShape)->actorShape.setFillColor((const sf::Color &)actorColor);
			break;
	}
	
}

void VisualComponent::update(double deltaMS)
{
	PhysicalComponent* physicalComponent = (PhysicalComponent*)owner->GetComponent(PHYSICAL);
	/*sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
	DisplayManager::instance()->window.draw(shape);*/
	if(physicalComponent->shapeID == 1)
	{
		((ActorShape::Rectangle*)actorShape)->actorShape.setPosition(physicalComponent->getActorPosition());
		((ActorShape::Rectangle*)actorShape)->actorShape.setFillColor(sf::Color::Green);
		DisplayManager::instance()->window.draw(((ActorShape::Rectangle*)actorShape)->actorShape);
	}
	else
	{
		((ActorShape::Circle*)actorShape)->actorShape.setPosition(physicalComponent->getActorPosition());
		((ActorShape::Circle*)actorShape)->actorShape.setFillColor(sf::Color::Green);
		DisplayManager::instance()->window.draw(((ActorShape::Circle*)actorShape)->actorShape);
	}
}