#pragma once
#include "GlobalHeader.h"
#include "ActorComponentInterface.h"
#include "Shape.h"
class VisualComponent : public ActorComponentInterface
{
public:
	VisualComponent(void);
	~VisualComponent(void);

	void init(XMLElement *componentElement);
	void update(double deltaMS);


	sf::Color* actorColor;
	//ShapeInterface* actorShape;
	//sf::Shape actorShape;
	ActorShape::Shape* actorShape;
};

