#pragma once
#include "SkillEffect.h"
#include "Player.h"
#include "Entity.h"
#include "MonsterController.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
class SkillMgr :public Node {
public:
	string __name;
	Player* __player;
	SkillEffect* effect;
	int __damage;
	string use_effect;
	string hit_effect;
	vector< pair<Entity*, MonsterController*> > __monster;
	void SetInf(Player* player, string name,int damage);
	void PushMonster(pair<Entity*, MonsterController*> p);
private:
};