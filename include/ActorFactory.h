#pragma once
#include "GlobalHeader.h"
#include "Actor.h"
#include "ActorComponentInterface.h"
typedef std::map<int,ActorComponentInterface*>::iterator componentTypeIter;
class ActorFactory
{
public:
	ActorFactory(void);
	~ActorFactory(void);

	void init();
	Actor* CreateActor(const char* resource, double SpawnPositionX,double SpawnPositionY);
	ActorComponentInterface* CreateComponent(  XMLElement *componentElement, Actor* owner);
	ActorComponentInterface* CreateComponentOf( int componentType);
	int NextId();
	
	static ActorFactory *instance();


	static ActorFactory *s_instance;
	int currentId;
	std::map<int, ActorComponentInterface*> componentCreatorsMap;//int : component type 

	//mapping actor list for quick access
	std::map<int,Actor*> actorMapALL;//playerID int
};

