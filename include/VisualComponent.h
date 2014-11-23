#pragma once
#include "GlobalHeader.h"
#include "ActorComponentInterface.h"
#include "Shape.h"
#include "GameViewManager.h"
class VisualComponent : public ActorComponentInterface
{
public:
	VisualComponent(void);
	~VisualComponent(void);

	void init(XMLElement *componentElement);
	void update(double deltaMS);
	void setVisibility(bool visible);
	bool IsAnInclusivePoint(int posX, int posY);

	sf::Color* actorColor;
	//ShapeInterface* actorShape;
	//sf::Shape actorShape;
	ActorShape::Shape* actorShape;
	int shapeID;
	bool isVisible;
	ViewType viewType;
};

