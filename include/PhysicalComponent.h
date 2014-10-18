#pragma once
#include "GlobalHeader.h"
#include "ActorComponentInterface.h"
#include "Shape.h"
class PhysicalComponent : public ActorComponentInterface
{
public:
	PhysicalComponent(void);
	~PhysicalComponent(void);

	void init(XMLElement *componentElement);
	void update(double deltaMS);

	sf::Vector2f getActorPosition();
	void ResetPosition();

	double mass;
	sf::Vector2f position;
	ActorShape::Shape* actorShape;
	int shapeID;
};

