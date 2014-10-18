
#pragma once
#include "GlobalHeader.h"
#include "DisplayManager.h"

class EventManager
{
public:
	EventManager(void);
	EventManager(DisplayManager displayManager);
	~EventManager(void);
	void  getEvents(void);
	static EventManager *instance();

	static EventManager *s_instance;
	DisplayManager displayManager;

};

