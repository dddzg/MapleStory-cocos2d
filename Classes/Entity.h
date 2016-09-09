#pragma once
#include "cocos2d.h"
#include <iostream>
#include "JSON\writer.h"
#include "JSON\reader.h"
#include "JSON\value.h"
#include "SimpleAudioEngine.h"
using namespace std;
using namespace CocosDenshion;
USING_NS_CC;
class MonsterController;
class Entity:public Node
{
public:
	enum DIR {
		left,
		right,
	};
	Entity();
	~Entity();
	static Entity* CreateWithSprite(const char* plist_name,unsigned int bit1=0xffffffff, unsigned int bit2 = 0xffffffff, char* Entity_name="stand.0.png");
	void SetAnimation(string state,DIR, double delay = 0.2f,int loop=-1);
	void TurnDirction(DIR,float speed = 10,int state=1);
	void Jump(float speed = 200.0f,int state=1);
	void stop();
	DIR dirction;
	string __state;
	int HP;
	int FullHp;
	int __hit;
	string jump_effect;
	string hit_effect;
	string die_effect;
	Sprite* __sprite;
	Sprite* __blood;
	Sprite* __bloodbase;
	bool haveblood;
	virtual const Vec2 & getPosition();
	static map<string, int> rank;
	bool judge(string state);
	int CotNum;
	void Attack(char S);
	void hit();
	void die();
	bool InitWithSprite(const char* plist_name, unsigned int, unsigned int, char* Entity_name = "stand.0.png");
	int speed;
	string plist_name;
	bool Is_Dead;
	string run_state;
	void LoadInfo(string name);
	void SetBlood();
	virtual void update(float dt);
	float init_height;
	float init_width;
protected:
private:
	void StopAnimation();
	void CheckDir(DIR);
	
};