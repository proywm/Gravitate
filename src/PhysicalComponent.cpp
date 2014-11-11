#include "PhysicalComponent.h"
#include "GameLogicManager.h"

PhysicalComponent::PhysicalComponent(void)
{
}


PhysicalComponent::~PhysicalComponent(void)
{
}
void PhysicalComponent::init(XMLElement *componentElement)
{
	ComponentID = componentElement->IntAttribute("ComponentType");
	XMLElement* shapeElement = componentElement->FirstChildElement("Structure");
	shapeID = shapeElement->IntAttribute("ShapeID");
	switch(shapeElement->IntAttribute("ShapeID"))
	{
		case 1://"Rectangle":
			actorShape = new ActorShape::Rectangle();
			((ActorShape::Rectangle*)actorShape)->setSize(shapeElement->FloatAttribute("Height"), shapeElement->FloatAttribute("Width"));
			break;
		case 2://"Circle":
			actorShape = new ActorShape::Circle();
			((ActorShape::Circle*)actorShape)->setSize(shapeElement->FloatAttribute("Radious"));
			//actorShape = new sf::CircleShape(shapeElement->IntAttribute("Radious"));
			break;
		case 3://"GridMap":
		case 4:
			actorShape = new ActorShape::GridMap();
			((ActorShape::GridMap*)actorShape)->setBlockSize(shapeElement->FloatAttribute("BlockWidth"));
			//actorShape = new sf::CircleShape(shapeElement->IntAttribute("Radious"));
			break;
		default:
			actorShape = new ActorShape::Circle();
			((ActorShape::Circle*)actorShape)->setSize(10);
			break;
	}
	mass = shapeElement->FloatAttribute("Mass");

	//get position
	position = owner->GetSpawnPosition();
}
void PhysicalComponent::update(double deltaMS)
{
	
}
sf::Vector2f PhysicalComponent::getActorPosition()
{
	return position;
}
void PhysicalComponent::ResetPosition()
{
	position = owner->GetSpawnPosition();
}
