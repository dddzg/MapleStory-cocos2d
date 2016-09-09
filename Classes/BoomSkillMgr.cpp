#include "BoomSkillMgr.h"

void BoomSkillMgr::boom(float dt,float time,float time2,float time3)
{
	this->scheduleOnce(schedule_selector(BoomSkillMgr::Boom_Do), dt);
	this->time = time;
	this->time2 = time2;
	this->time3 = time3;
}

void BoomSkillMgr::Boom_Do(float dt)
{
	effect = SkillEffect::create();
	SimpleAudioEngine::getInstance()->playEffect(this->use_effect.c_str());
	effect->name = this->__name + string("boom");
	auto pos = this->__player->getPosition();
	pos.x += (this->__player->dirction == Entity::left) ? this->__player->__sprite->getContentSize().width : 0;
	effect->__effect->setPosition(pos);
	if (this->__player->dirction == Entity::left) {
		effect->__effect->setAnchorPoint(Vec2(1, 0));
	}
	else {
		effect->__effect->setAnchorPoint(Vec2(0, 0));
	}
	effect->SetAnimation(1,this->time);
	this->addChild(effect);
	this->scheduleOnce(schedule_selector(BoomSkillMgr::hit),time2);
}

void BoomSkillMgr::hit(float dt) {
	SimpleAudioEngine::getInstance()->playEffect(this->hit_effect.c_str());
	for (auto mon : this->__monster) {
		auto entity = mon.first;
		if (entity == nullptr || entity->Is_Dead) {
			continue;
		}
		auto goal = Vec2(entity->getPosition().x + entity->__sprite->getContentSize().width / 2, entity->getPosition().y + entity->__sprite->getContentSize().height / 2);
		auto effect2 = SkillEffect::create();
		this->addChild(effect2);
		effect2->name = this->__name + string("hit");
		effect2->__effect->setPosition(goal);
		effect2->__effect->setAnchorPoint(Vec2(0.5, 0.5));
		effect2->SetAnimation(1,this->time3);
		mon.second->hit(this->__player,this->__damage);
	}
}
