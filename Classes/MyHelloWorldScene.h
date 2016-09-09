#ifndef __MYHELLOWORLD_SCENE_H__
#define __MYHELLOWORLD_SCENE_H__

#include "cocos2d.h"
using namespace cocos2d;

class MyHelloWorldScene :public Layer {
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(MyHelloWorldScene);
};

#endif