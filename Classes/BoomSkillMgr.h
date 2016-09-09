#pragma once
#include "SkillMgr.h";

class BoomSkillMgr:public SkillMgr
{
public:
	void boom(float dt,float time,float time2,float time3);
	void Boom_Do(float dt);
	void hit(float dt);
	float time;
	float time2;
	float time3;
	CREATE_FUNC(BoomSkillMgr);
private:
	
};

