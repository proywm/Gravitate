#include "GameApplication.h"
#include "DisplayManager.h"
#include "EventManager.h"
#include "GameLogicManager.h"
#include "GameViewManager.h"
#include <SFML/Graphics.hpp>

DisplayManager *DisplayManager::s_instance = 0;
EventManager *EventManager::s_instance = 0;
GameLogicManager *GameLogicManager::s_instance = 0;
GameViewManager *GameViewManager::s_instance = 0;
GameApplication::GameApplication(void)
{
	
}
GameApplication::~GameApplication(void)
{
}

void GameApplication::Run(void)
{
	DisplayManager::instance()->InitDisplayManager();
	//EventManager::instance()->InitEventManager();
	GameViewManager::instance()->InitGameViewManager();//instatiate
	GameLogicManager::instance()->InitGameLogicManager();
//	GameViewManager::instance()->InitGameViewManager();

	sf::Music music;
	if (!music.openFromFile("./resources/sounds/BGMusic.wav"))
    	exit (EXIT_FAILURE);
    music.setLoop(true);
    music.setVolume(40);
	music.play();

	GameLoop();
}
void GameApplication::GameLoop(void)
{
	sf::Clock clock;
    while (GameLogicManager::instance()->ArePlayersRunning())//dispManager.window.isOpen()
    {
		sf::Time elapsed = clock.restart();
		double elapsedMiliSecond = elapsed.asMilliseconds();
       		 GameLogicManager::instance()->HandleEvents();
		//gameLogic();
		

		GameLogicManager::instance()->updateGameLogic(elapsedMiliSecond);
        
    }
}
