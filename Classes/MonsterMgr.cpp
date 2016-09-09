#include "MonsterMgr.h";

void MonsterMgr::push(Entity* entity, MonsterController* monstercontroller)
{
	this->Monster.push_back(make_pair(entity, monstercontroller));
	this->addChild(entity);
	this->addChild(monstercontroller);
}

