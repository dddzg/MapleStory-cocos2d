#pragma once
#include "cocos2d.h"
#include "Entity.h"
#include "Player.h"
using namespace std;
USING_NS_CC;
//FSM
enum EnumState{
	stand,
	work,
	angry
};

enum Range {
	InRange,
	TooLeft,
	TooRight,
	Die,
};
class MonsterController:public Node
{
public:
	MonsterController();
	~MonsterController();
	CREATE_FUNC(MonsterController);
	virtual bool init();
	void Set(Vec2 StartPoint, Vec2 EndPoint,Entity* entity);
	void BindEntity(Entity* Entity);
	EnumState CurState;
	void MyUpdate(float dt);
	bool Half();
	void hit(Player* player,int damage);
	void die();
	virtual void update(float delta);
private:
	Vec2 StartPoint;
	Vec2 EndPoint;
	Entity* __Entity;
	Player* __player;
	bool IsTimeToWork();
	void ChangeState(EnumState param1);
	Range InTheRange();
	void Stop();
};

