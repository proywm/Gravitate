#pragma once
#include "GlobalHeader.h"

class Actor;
#include "Actor.h"

enum ComponentType 
{
	VISUAL = 1, 
	PHYSICAL = 2,
	GAMESTATE = 3,
	SELECTIONTOOLBAR = 4,
	TEXTAREA = 5
};

class ActorComponentInterface
{
	
public:
	virtual void init(XMLElement *componentElement);
	virtual void update(double deltaMS);

	int getComponentID();
	void SetOwner(Actor* o);

	int ComponentID;
	Actor* owner;
};

