#include "SkillInf.h"

SkillInf::SkillInf()
{
	__level = 1;
	__damage[0] = 0;
	incd = 0;
	
}

SkillInf::~SkillInf()
{

}

bool SkillInf::init()
{
	if (!Node::init())
		return false;
	return true;
}

void SkillInf::SetInf(double num,double change, double cd, Vec2 range, string name)
{
	for (int i = 1; i <= 4; i++) {
		__damage[i] = i*num;
	}
	__cd = cd;
	__range = range;
	__name = name;
	__change = change;
}

bool SkillInf::IsInCd()
{
	return incd > 0;
}

void SkillInf::Attack()
{
	incd = __cd;
	this->scheduleUpdate();
}

void SkillInf::update(float delta)
{
	if (incd > 0) incd -= delta;
	else this->unscheduleUpdate();
	
}

