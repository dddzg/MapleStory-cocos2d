#include "BallSkillMgr.h"


BallSkillMgr::BallSkillMgr()
{
}

BallSkillMgr::~BallSkillMgr()
{
}

bool BallSkillMgr::init()
{
	if (!Node::init()) {
		return false;
	}
	return true;
}


void BallSkillMgr::ball(double dt) {
	this->scheduleOnce(schedule_selector(BallSkillMgr::Ball_Do), dt);
}
void BallSkillMgr::Ball_Do(float dt) {
	effect = SkillEffect::create();
	SimpleAudioEngine::getInstance()->playEffect(this->use_effect.c_str());
	effect->name = this->__name + string("ball");
	auto pos = this->__player->getPosition();
	pos.x += (this->__player->dirction == Entity::left) ? this->__player->__sprite->getContentSize().width : 0;
	effect->__effect->setPosition(pos);
	auto body = PhysicsBody::createEdgeBox(effect->__effect->getContentSize());
	body->setCategoryBitmask(0);
	body->setCollisionBitmask(0);
	body->setContactTestBitmask(0xffffffff);
	body->getShape(0)->setFriction(0.0f);
	body->getShape(0)->setRestitution(0.0f);
	body->setDynamic(true);
	body->setVelocity(Vec2(150 * (this->__player->dirction == Entity::right ? 1 : -1), 100));
	body->setMass(500);
	effect->__effect->setPhysicsBody(body);
	effect->__effect->setAnchorPoint(Vec2(0.5, 0));
	effect->SetAnimation(-1, 0.15);
	this->addChild(effect);
	this->scheduleUpdate();
}
Vec2 BallSkillMgr::CalV(Vec2 pos) {
	double dis = this->__monster[0].first->getPosition().x - pos.x;
	return Vec2(100, dis * 3);
}

void BallSkillMgr::update(float dt)
{
	if (this->__monster[0].first==nullptr ||this->__monster[0].first->Is_Dead) {
		this->unscheduleUpdate();
		effect->removeFromParentAndCleanup(true);
		return;
	}
	Vec2 goal = Vec2(this->__monster[0].first->getPosition().x + this->__monster[0].first->__sprite->getContentSize().width/2, this->__monster[0].first->getPosition().y + this->__monster[0].first->__sprite->getContentSize().height / 2);

	auto IsIn = [&]()->bool {
		auto begin = effect->__effect->getPosition();
		auto end = begin + effect->__effect->getContentSize();
		if (goal.x >= begin.x && goal.x <= end.x && goal.y >= begin.y && goal.y <= end.y) {
			return true;
		}
		else {
			return false;
		}
	}();
	if (IsIn == true) {
		this->unscheduleUpdate();
		effect->removeFromParentAndCleanup(true);
		hit(goal);
		return;
	}
	if (effect->__effect->getPhysicsBody()->getVelocity().y < 0 && effect->__effect->getPosition().y+ effect->__effect->getContentSize().height < goal.y) {
		auto v = effect->__effect->getPhysicsBody()->getVelocity();
		effect->__effect->getPhysicsBody()->setVelocity(Vec2(v.x,100));
	}
	if (effect->__effect->getPhysicsBody()->getVelocity().x < 0 && effect->__effect->getPosition().x < goal.x || effect->__effect->getPhysicsBody()->getVelocity().x > 0 && effect->__effect->getPosition().x > goal.x) {
		auto v = effect->__effect->getPhysicsBody()->getVelocity();
		effect->__effect->getPhysicsBody()->setVelocity(Vec2(-v.x, v.y));
	}
}

void BallSkillMgr::hit(Vec2 goal)
{
	auto effect2 = SkillEffect::create();
	this->addChild(effect2);
	SimpleAudioEngine::getInstance()->playEffect(this->hit_effect.c_str());
	effect2->__effect->setPosition(goal);
	effect2->name= this->__name + string("hit");
	effect2->__effect->setAnchorPoint(Vec2(0.5, 0.5));
	effect2->SetAnimation(1,0.2);
	this->__monster[0].second->hit(this->__player,this->__damage);
}
