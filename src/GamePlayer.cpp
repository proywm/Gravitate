#include "GamePlayer.h"
#include "ActorComponentInterface.h"
#include "VisualComponent.h"
#include "PhysicalComponent.h"

GamePlayer::GamePlayer(void)
{
	
}


GamePlayer::~GamePlayer(void)
{
}
void GamePlayer::init(const char* actorsList)
{
}
void GamePlayer::update(double deltaMS)
{
}
void GamePlayer::HandleEvent(sf::Event receivedEvent)
{
}
void GamePlayer::initGamePlayer(const char* actorsList, int PlayerId)
{
	playerFile = actorsList;
	XMLDocument* doc = new XMLDocument();
    doc->LoadFile(actorsList);
	XMLElement *playerElements = doc->FirstChildElement();
	gamePlayerID = PlayerId;//playerFiles->IntAttribute("PlayerId");

	initGamePlayerActors(playerElements);
	score = 0;
}
void GamePlayer::initGamePlayerActors(XMLElement *actorFiles)
{

	for(actorFiles = actorFiles->FirstChildElement(); actorFiles != NULL; actorFiles = actorFiles->NextSiblingElement())
	{
		const char* fileLocation = actorFiles->Attribute("ActorFileName");
		for(int totalActors = actorFiles->IntAttribute("totalActors"); totalActors !=0 ; --totalActors)
		{
			Actor* actor = ActorFactory::instance()->CreateActor(fileLocation,actorFiles->IntAttribute("actorId"),actorFiles->FloatAttribute("spawnPositionX"),actorFiles->FloatAttribute("spawnPositionY"));
			actorMap.insert(std::make_pair(actor->actorId,actor));
			ActorFactory::instance()->actorMapALL.insert(std::make_pair(actor->actorId,actor));
		}
	}
}
void GamePlayer::updateActors(double deltaMS)
{
	for(actorIterType iter = actorMap.begin(); iter != actorMap.end(); ++iter)
	{
		Actor* actor = (Actor*)iter->second;

		if(actor->actorTypeId == 2)
		{
			
		}
		
		
		PhysicalComponent* physicalComponent = (PhysicalComponent*)(actor->GetComponent(PHYSICAL));
		if(physicalComponent != NULL)
		{
			physicalComponent->update(deltaMS);
		}
		VisualComponent* visualComponent = (VisualComponent*)(actor->GetComponent(VISUAL));
		if(visualComponent != NULL)
		{
			visualComponent->update(deltaMS);
		}

	}
	
}
void GamePlayer::deleteGamePlayerActors()
{
	for(actorIterType iter = actorMap.begin(); iter != actorMap.end(); ++iter)
	{
		Actor* actor = (Actor*)iter->second;
		ActorFactory::instance()->actorMapALL.erase(actor->actorId);
	}
	actorMap.clear();
}
