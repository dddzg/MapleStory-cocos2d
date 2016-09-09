#pragma once
#include"SkillInf.h"
#include"cocos2d.h"
USING_NS_CC;
class MonsterController;
class Ball:public SkillInf
{
public:
	Ball();
	~Ball();
	CREATE_FUNC(Ball);
	void attack();
	virtual bool init();
private:
	Sprite* __sprite;
	void SetAnimation(string name="ball",double delay=-1);
};

