#include "GameOwner.h"
#include "ActorComponentInterface.h"
#include "GameLogicManager.h"
#include "PhysicalComponent.h"
#include "DisplayManager.h"
#include <sstream>
GameOwner::GameOwner(void)
{
}
GameOwner::~GameOwner(void)
{
}
void GameOwner::init(const char* actorsList)
{
	font.loadFromFile("./src/arial.ttf");

	MaxPossibleScore = 10;//hardcoded
	HasWinner = false;
	controlGame();
}
void GameOwner::update(double deltaMS)
{
	if(HasWinner)
		controlGame();
	
	updateScoreView();
}
void GameOwner::HandleEvent(sf::Event receivedEvent)
{
	if(receivedEvent.key.code == sf::Keyboard::Q)
	{
		DisplayManager::instance()->window.close();
	}
	else if(receivedEvent.key.code == sf::Keyboard::R)
	{
		RestartGame();
	}
}
void GameOwner::controlGame(void)
{
	//Hard Coded: ToDO: Make Dynamic Game Control
	if(HasWinner)
	{
		ShowWinner(winner);
		StopGame();
	}
	else
	{
		startGame();
	}
}
void GameOwner::startGame(void)
{
	
}
void GameOwner::StopGame()
{
	
}
void GameOwner::ShowWinner(GamePlayer* winner)
{
	
}
void GameOwner::updateScoreView()
{
	
}
void GameOwner::RestartGame()
{
	
	controlGame();
}
