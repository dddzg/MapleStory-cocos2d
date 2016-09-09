#pragma once
#include "Entity.h"
#include "SkillInf.h"
#include "ui\CocosGUI.h"
#include "cocostudio\CCSGUIReader.h"
using namespace ui;
using namespace cocostudio;
class Player:public Entity
{
public:
	Player();
	~Player();
//	vector < pair<Entity*, MonsterController*> > *Monster;
	static Player* CreateWithSprite(char* plist_name, unsigned int bit1 = 0xffffffff, unsigned int bit2 = 0xffffffff, char* Entity_name = "stand.0.png");
	void Attack(char S);
//	int FindEntity(Vec2 begin, Vec2 end);
	SkillInf* Q;
	SkillInf* W;
	SkillInf* E;
	int GetDamage(char S);
	SkillInf* GetSkill(char S);
	int level;
	int HP;
	bool nodead;
	void hit();
	void die(Ref* psender);
	void changedead(float dt);
	LoadingBar* HpBar;
	Text* Name;
	Text* My_Level;
	virtual const Vec2 & getPosition();
//	void DrawRect(Vec2 begin,Vec2 end,double time=1);
protected:
private:
	
};