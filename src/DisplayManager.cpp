#include "DisplayManager.h"


DisplayManager::DisplayManager(void)
{
	width = 800;
	height = 600;
	title = "Pong";
}
DisplayManager::~DisplayManager(void)
{
	window.close();
}
DisplayManager* DisplayManager::instance()
 {
	if (!s_instance)
		s_instance = new DisplayManager;
	return s_instance;
 }
void DisplayManager::InitDisplayManager(void)
{
	 window.create(sf::VideoMode(width, height), title,   sf::Style::Titlebar |   sf::Style::Close );
}
int  DisplayManager::IsDisplayWindowRunning(void)
{
	return window.isOpen();
}
