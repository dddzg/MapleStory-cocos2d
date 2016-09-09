#pragma once
#include "cocos2d.h"
#include "login.h"
using namespace cocos2d;
class SceneManager :public Ref {
public:
	enum EnumSceneType {
		Login,
		MainGame
	};
	static SceneManager* getInstance();
	virtual bool init();
	void changeScene(EnumSceneType enSceneType);
	void game(Ref* sender);
private:
	static SceneManager* mSceneManager;
};