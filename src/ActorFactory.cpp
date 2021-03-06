#include "ActorFactory.h"
#include "PhysicalComponent.h"
#include "VisualComponent.h"
#include "GameStateComponent.h"
#include "TextAreaComponent.h"

ActorFactory::ActorFactory(void)
{
	currentId = 0;
}


ActorFactory::~ActorFactory(void)
{
}

void ActorFactory::init()
{
	componentCreatorsMap.insert(std::make_pair(PHYSICAL,new PhysicalComponent()));
	componentCreatorsMap.insert(std::make_pair(VISUAL,new VisualComponent()));
}
Actor* ActorFactory::CreateActor(const char* actorFile, int actorId, double SpawnPositionX,double SpawnPositionY)
{

	XMLDocument* doc = new XMLDocument();
    	doc->LoadFile(actorFile);
	
	XMLElement *rootActorElement = doc->FirstChildElement();

	Actor* actor = new Actor(rootActorElement->Attribute("ActorType"),rootActorElement->IntAttribute("ActorTypeID"));
	actor->actorId = actorId;
	actor->spawnPosition.x = SpawnPositionX;
	actor->spawnPosition.y = SpawnPositionY;
	
	for(XMLElement *componentElement = rootActorElement->FirstChildElement(); componentElement != NULL; componentElement = componentElement->NextSiblingElement())
	{
		ActorComponentInterface* actorComponent = CreateComponent(componentElement,actor);
		actor->addComponent(actorComponent);
	}
	return actor;
}
ActorComponentInterface* ActorFactory::CreateComponentOf( int componentType)
{
	switch(componentType)
	{
	case PHYSICAL:
		return new PhysicalComponent();
		break;
	case VISUAL:
		return new VisualComponent();
		break;
	case GAMESTATE:
		return new GameStateComponent();
		break;
	case TEXTAREA:
		return new TextAreaComponent();
		break;
	default:
		return NULL;
	}
}
ActorComponentInterface* ActorFactory::CreateComponent( XMLElement *componentElement, Actor* owner)
{
	//componentTypeIter cTIter = componentCreatorsMap.find(componentElement->IntAttribute("ComponentType"));
	ActorComponentInterface* actorComponentInterface = CreateComponentOf(componentElement->IntAttribute("ComponentType"));//cTIter->second;
	actorComponentInterface->owner = owner;
	actorComponentInterface->init(componentElement);
	return actorComponentInterface;
}
ActorFactory* ActorFactory::instance()
 {
	if (!s_instance)
		s_instance = new ActorFactory;
	return s_instance;
}

int ActorFactory::NextId()
{
	return currentId++;
}
