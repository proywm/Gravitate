#include "HumanPlayer.h"

HumanPlayer::HumanPlayer(void)
{
}
HumanPlayer::~HumanPlayer(void)
{
}
void HumanPlayer::HandleEvent(sf::Event receivedEvent)
{
	if(receivedEvent.key.code == sf::Keyboard::Left)
		printf("call gameOwner Functionality to move Left");
	else if(receivedEvent.key.code == sf::Keyboard::Right)
		printf("call gameOwner Functionality to move Right");
}
void HumanPlayer::init(const char* actorsList)
{
	
	XMLDocument* doc = new XMLDocument();
    	doc->LoadFile(actorsList);
	XMLElement *playerFiles = doc->FirstChildElement();
	playerFiles = playerFiles->FirstChildElement("Control");
	leftKey = (char*)playerFiles->Attribute("LeftKey");
	rightKey = (char*)playerFiles->Attribute("RightKey");
	moveId = playerFiles->IntAttribute("MoveId");
}
void HumanPlayer::update(double deltaMS)
{}
