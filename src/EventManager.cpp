#include "EventManager.h"
#include "GameLogicManager.h"
EventManager::EventManager(void)
{
}

EventManager::~EventManager(void)
{
}
EventManager* EventManager::instance()
 {
	if (!s_instance)
		s_instance = new EventManager;
	return s_instance;
 }

void  EventManager::getEvents(void)
{
	sf::Event event;
    while (DisplayManager::instance()->window.pollEvent(event))
    {
		switch (event.type)
		{
			case sf::Event::Closed:
				DisplayManager::instance()->window.close();
			break;
			// key pressed 
			case sf::Event::KeyPressed:
			case sf::Event::MouseButtonPressed://// Mouse Button Pressed
					GameLogicManager::instance()->SendKeyEventToPlayer(event);
            		break;
		}
	}
}
