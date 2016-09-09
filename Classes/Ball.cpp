#include "Ball.h"
Ball::Ball()
{
}

Ball::~Ball()
{
}

void Ball::attack()
{
	auto scene = Director::getInstance()->getRunningScene();
	log("%d", scene->getChildrenCount());
}

bool Ball::init()
{
	if (!SkillInf::init()) {
		return false;
	}
	return true;
}

void Ball::SetAnimation(string name/*="ball"*/, double delay/*=-1*/)
{
	auto tSprite = Sprite::create();
}