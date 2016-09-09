#pragma once
#include "cocos2d.h"
#include <iostream>
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
using namespace std;
USING_NS_CC;
class SkillInf:public Node
{
public:
	SkillInf();
	~SkillInf();
	CREATE_FUNC(SkillInf);
	virtual bool init();
	void SetInf(double num, double change,double cd, Vec2 range,string name);
	int __level;
	double __damage[5];
	Vec2 __range;
	double __cd;
	string __name;
	string use_effect;
	string hit_effect;
	double incd;
	int __change;
	bool IsInCd();
	void Attack();
	virtual void update(float delta);
protected:
private:

};