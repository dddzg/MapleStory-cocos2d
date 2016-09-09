#include "SkillEffect.h"


void SkillEffect::SetAnimation(double loop /*= 1*/, double delay /*= 0.1*/)
{
	auto animation = Animation::create();
	for (int index = 0;;) {
		string path = this->name + StringUtils::format(".%d.png", index++);
		std::string fullpath = FileUtils::getInstance()->fullPathForFilename(path);
		if (fullpath.size() == 0)
		{
			break;
		}
		animation->addSpriteFrameWithFile(path);
	}
	animation->setLoops(loop);
	animation->setRestoreOriginalFrame(true);
	animation->setDelayPerUnit(delay);
	auto animate = Animate::create(animation);
	if (loop==1){
		auto call = CallFunc::create([&]() {
			this->removeAllChildren();
			this->removeFromParentAndCleanup(true);
		});
		auto many = Sequence::create(animate, call, NULL);
		__effect->runAction(many);
	}
	else{

		__effect->runAction(animate);
	}
}

bool SkillEffect::init()
{
	if (!Node::init()) {
		return false;
	}
	__effect = Sprite::create();
	this->addChild(__effect);
	return true;
}

void SkillEffect::SetAnimation(CallFunc call/*=NULL*/, double loop/*=1*/, double delay/*=0.1*/)
{
	auto animation = Animation::create();
	for (int index = 0;;) {
		string path = this->name + StringUtils::format(".%d.png", index++);
		std::string fullpath = FileUtils::getInstance()->fullPathForFilename(path);
		if (fullpath.size() == 0)
		{
			break;
		}
		animation->addSpriteFrameWithFile(path);
	}
	animation->setLoops(loop);
	animation->setRestoreOriginalFrame(true);
	animation->setDelayPerUnit(delay);
	auto animate = Animate::create(animation);
	auto many = Sequence::create(animate, call, NULL);
	__effect->runAction(many);
}
