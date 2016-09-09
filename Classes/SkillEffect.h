#pragma once
#include "cocos2d.h"
#include <iostream>
#include <string>
using namespace std;
USING_NS_CC;

class SkillEffect:public Node
{
public:
	Sprite* __effect;
	string name;
	void SetAnimation(CallFunc call, double loop = 1, double delay = 0.1);
	void SetAnimation(double loop = 1, double delay = 0.1);
	CREATE_FUNC(SkillEffect);
	virtual bool init();
protected:
private:
};