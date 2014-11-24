#pragma once
#include "ActorFactory.h"
enum ViewType
{
	TITLEVIEW=1, GAMEVIEW, RESULTVIEW, RULEVIEW, TOTALVIEWS
};
class GameViewManager
{
public:
	GameViewManager(void);
	~GameViewManager(void);

	void InitGameViewManager();
	static GameViewManager* instance();

	void setCurrentView(ViewType type);
	//void goToNextView();
	Actor* GetTopVisibleActorAtPoint(int posX, int posY);

	ViewType currentGameView;
	static GameViewManager *s_instance;
};
