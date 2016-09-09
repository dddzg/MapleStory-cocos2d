#pragma once
#include "cocos2d.h"
#include <iostream>
#include <cstring>
#include "Player.h"
#include "MonsterController.h"
#include "MonsterMgr.h"
#include "JSON\writer.h"
#include "JSON\reader.h"
#include "JSON\value.h"
#include "ui\CocosGUI.h"
#include "cocostudio\CCSGUIReader.h"
using namespace ui;
using namespace cocostudio;
using namespace std;
USING_NS_CC;
class Player;
class MainScene :public Layer
{
public:
//	static std::map<int, int>MNum;
	enum OBJ
	{
		earth,
		entity,
		map,
	};
	static Scene* CreateScene();
	CREATE_FUNC(MainScene);
	virtual bool init();
	MainScene();
protected:
private:
	void Loadmap(string);
	Player* __player;
	virtual void update(float delta);
	Size mapsize;
	PhysicsWorld* __World;
	int __maxnum;
	int MapInfo[15][20];
	vector < Vec2 >MonsterPos;
	vector < Vec2 >MonsterBegin;
	vector < Vec2 >MonsterEnd;
	MonsterMgr* __monstermgr;
	void cal(int begin, int end,int height);
	void LoadMonster(Vec2,Vec2,Vec2,const char* filename,int state=-1);
	void AsForController();
	std::map<EventKeyboard::KeyCode, bool> keys;
	bool IsKeyPressed(EventKeyboard::KeyCode keycode);
	void Attack(char S);
	int FindEntity(Vec2 begin, Vec2 end);
	vector<int> FindEntity(Vec2 begin, Vec2 end, int num);
	void DrawNode(Vec2 begin, Vec2 end);
	void MonsterWillLoad(float dt);
	vector<string> MonsterList;
	bool IsIn(Vec2 s1,Vec2 e1,Vec2 s2,Vec2 e2);
	void LoadMonsterList();
	LoadingBar* HpBar;
	Text* Name;
	Text* My_Level;
	void LoadUI();
	EventListenerKeyboard* keylistener;
	int MonsterDieNum;
	const char* gb23122utf8(const char* gb2312);
	Vec2 UIPos;
	void die(Ref* sender);
	
};