#pragma once

#include "SkillMgr.h";
class BallSkillMgr:public SkillMgr
{
public:
	BallSkillMgr();
	~BallSkillMgr();
	CREATE_FUNC(BallSkillMgr);
	virtual bool init();

	void ball(double dt);
	void Ball_Do(float dt);
	Vec2 CalV(Vec2 pos);
	virtual void update(float dt);
	void hit(Vec2 goal);
private:

};
