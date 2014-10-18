#pragma once
#include "GlobalHeader.h"
class ActorComponentInterface;
#include "ActorComponentInterface.h"
//using namespace std;

typedef std::string ActorType;
class Actor
{
public:
	Actor(ActorType aType,int aTypeId);
	~Actor(void);

	void Init();
	void Update(double deltaMS);
	void Destroy();
	ActorComponentInterface* GetComponent(int cId);
	void addComponent(ActorComponentInterface* actorComponent);
	sf::Vector2f GetSpawnPosition();

	int actorId;
	ActorType actorType;
	int actorTypeId;//as its not possible to switch case on string
	std::map<int, ActorComponentInterface*> componentList;//int: componentID
	sf::Vector2f spawnPosition;
};

