#include "SkillMgr.h"

void SkillMgr::SetInf(Player * player, string name,int damage)
{
	__player = player;
	__name = name;
	__damage = damage;
}

void SkillMgr::PushMonster(pair<Entity*, MonsterController*> p)
{
	__monster.push_back(p);
}