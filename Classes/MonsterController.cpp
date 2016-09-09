#include "MonsterController.h"
MonsterController::MonsterController()
{
	CurState = work;
}

MonsterController::~MonsterController()
{
}

bool MonsterController::init()
{
	this->scheduleUpdate();
	this->schedule(schedule_selector(MonsterController::MyUpdate), 2.0f);
	return true;
}

void MonsterController::Set(Vec2 StartPoint, Vec2 EndPoint, Entity* entity)
{
	this->StartPoint = StartPoint;
	this->EndPoint = EndPoint;
	BindEntity(entity);
	if (Half())
		this->__Entity->dirction = Entity::left;
	else
		this->__Entity->dirction = Entity::right;
}

void MonsterController::BindEntity(Entity* Entity)
{
	this->__Entity = Entity;
}

void MonsterController::MyUpdate(float dt)
{
	switch (CurState) {
	case stand:
		if (IsTimeToWork()) {
			ChangeState(work);
		}
		else {
			Stop();
		}
		break;
	case work:
		do 
		{
			float ran = CCRANDOM_0_1();
			Entity::DIR dir;
			if (ran <= 0.4) {
				dir = Entity::left;
				this->__Entity->TurnDirction(dir,  this->__Entity->speed, 0);
 				if (ran <= 0.02 && this->__Entity->run_state=="jump") {
 					this->__Entity->Jump(200.0f,0);
 				}
			}
			else if (ran>0.4 && ran<=0.8){
				dir = Entity::right;
				this->__Entity->TurnDirction(dir, this->__Entity->speed, 0);
				if (ran <= 0.42 && this->__Entity->run_state == "jump") {
					this->__Entity->Jump(200.0f,0);
				}
			}
			else {
				Stop();
				ChangeState(stand);
			}
		} while (0);
		break;
	case angry:
		do {
			auto GoalPos = this->__player->__sprite->getPosition().x;
			auto MyPos = this->__Entity->__sprite->getPosition().x;
			if (MyPos < GoalPos) {
				this->__Entity->TurnDirction(Entity::right, this->__Entity->speed, 0);
			}
			else if (MyPos > GoalPos) {
				this->__Entity->TurnDirction(Entity::left, this->__Entity->speed, 0);
			}
			else {
				Stop();
			}
		} while (0);
		break;
	default:
		break;
	}
}


bool MonsterController::Half()
{
	float ran = CCRANDOM_0_1();
	if (ran <= 0.5) {
		return true;
	}
	else {
		return false;
	}
}

void MonsterController::hit(Player * player,int damage)
{
	this->__player = player;
	this->__Entity->hit();
	this->__Entity->HP -= damage;
	if (this->__Entity->HP <= 0) {
		die();
	}
	this->CurState = angry;
}
void MonsterController::die() {
	this->__Entity->Is_Dead = true;
	this->__Entity->die();
	this->removeFromParent();
//	
}
void MonsterController::update(float delta)
{
	Range inTheRange = InTheRange();
	if (inTheRange == Die) {
		this->unscheduleUpdate();
		this->unschedule(schedule_selector(MonsterController::MyUpdate));
		this->removeAllChildrenWithCleanup(true);
		return;
	}
	if (inTheRange == InRange) {
		return;
	}
	else {
		if (CurState == angry) {
			this->__Entity->TurnDirction(this->__Entity->dirction, 0, 0);
		}
		else {
			if (inTheRange == TooRight) {
				this->__Entity->TurnDirction(Entity::left, this->__Entity->speed, 0);
			}
			else {
				this->__Entity->TurnDirction(Entity::right, this->__Entity->speed, 0);
			}
			ChangeState(work);
		}
	}
}

bool MonsterController::IsTimeToWork()
{
	return Half();
}

void MonsterController::ChangeState(EnumState param1)
{
	this->CurState = param1;
}

Range MonsterController::InTheRange()
{
	if (this->__Entity->__sprite == nullptr) {
		return Die;
	}
	auto left = this->__Entity->getPosition().x;
	auto right = this->__Entity->getPosition().x + this->__Entity->__sprite->getContentSize().width;
	if (left > StartPoint.x && right < EndPoint.x) {
		return InRange;
	}
	else if (left<= StartPoint.x) {
		return TooLeft;
	}
	else {
		return TooRight;
	}
}

void MonsterController::Stop()
{
	this->__Entity->stop();
	if (!this->__Entity->judge("stand") && this->__Entity->__state != "move") return;
	else this->__Entity->SetAnimation("stand", this->__Entity->dirction);
}
