#pragma once
#include "GlobalHeader.h"
class DisplayManager
{
public:
	DisplayManager(void);
	~DisplayManager(void);
	void InitDisplayManager();
	int IsDisplayWindowRunning();
	void getDisplayEvents();
//	int  UpdateDisplay(void);
	static DisplayManager *instance();


	static DisplayManager *s_instance;

	sf::RenderWindow window;
private:
	int width;
	int height;
	char* title;
};

