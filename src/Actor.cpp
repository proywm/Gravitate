#include "Actor.h"
#include "ActorFactory.h"

Actor::Actor(ActorType aType,int aTypeId)
{
	actorType = aType;
	actorTypeId = aTypeId;
}


Actor::~Actor(void)
{
}
void Init()
{
}
void Update(double deltaMS)
{
}
void Destroy()
{
}
ActorComponentInterface* Actor::GetComponent(int cId)
{
	componentTypeIter cTIter =  componentList.find(cId);
	ActorComponentInterface* componentInterface = (ActorComponentInterface*)cTIter->second;
	//return componentList[cId];
	return componentInterface;
}

void Actor::addComponent(ActorComponentInterface* actorComponent)
{
	componentList.insert(std::make_pair(actorComponent->ComponentID,actorComponent));
}

sf::Vector2f Actor::GetSpawnPosition()
{
	return spawnPosition;
}