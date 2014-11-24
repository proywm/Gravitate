#include "GameViewManager.h"
#include "GamePlayer.h"
#include "VisualComponent.h"

GameViewManager::GameViewManager(void)
{
	
}
GameViewManager::~GameViewManager(void)
{
}
GameViewManager* GameViewManager::instance()
{
	if (!s_instance)
		s_instance = new GameViewManager;
	return s_instance;
}
void GameViewManager::InitGameViewManager()
{
}
Actor* GameViewManager::GetTopVisibleActorAtPoint(int posX, int posY)
{
	Actor* retActor;
	bool gotActor = false;
	for(actorIterType iter = ActorFactory::instance()->actorMapALL.begin(); iter != ActorFactory::instance()->actorMapALL.end(); ++iter)
	{
		Actor* actor = (Actor*)iter->second;
		
		VisualComponent* visualComponent = (VisualComponent*)actor->GetComponent(VISUAL);
		if(visualComponent->viewType==currentGameView)
		{	
			if(visualComponent->IsAnInclusivePoint(posX, posY))
			{
				retActor = actor;
				gotActor = true;
			}
		}
	}
	if(gotActor == true)
		return retActor;
	else
		return NULL;
}
void GameViewManager::setCurrentView(ViewType type)
{
	for(actorIterType iter = ActorFactory::instance()->actorMapALL.begin(); iter != ActorFactory::instance()->actorMapALL.end(); ++iter)
	{
		Actor* actor = (Actor*)iter->second;
		VisualComponent* visualComponent = (VisualComponent*)actor->GetComponent(VISUAL);
		if(visualComponent->viewType==type)
			visualComponent->isVisible = true;
		else
			visualComponent->isVisible = false;
	}
	currentGameView = type;
}
/*
void GameViewManager::goToNextView()
{
	switch(currentGameView)
	{
		case TITLEVIEW:
			setCurrentView(GAMEVIEW);
		break;
		case GAMEVIEW:
			setCurrentView(RESULTVIEW);
		break;
		case RESULTVIEW:
			setCurrentView(TITLEVIEW);
		break;
	}
}
*/
