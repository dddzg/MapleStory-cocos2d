#pragma once
#include "cocos2d.h"
#include "Player.h"
#include <iostream>
USING_NS_CC;

class Controller:public Node
{
public:
	Controller();
	~Controller();
	bool init();
	CREATE_FUNC(Controller);
	void BindPlayer(Player* player);
	virtual void update(float delta);
	std::map<EventKeyboard::KeyCode, bool> keys;
	bool IsKeyPressed(EventKeyboard::KeyCode keycode);
	Player* __Player;
protected:
private:
	
};