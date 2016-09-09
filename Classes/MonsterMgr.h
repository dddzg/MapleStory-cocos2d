#pragma once
#include "Entity.h"
#include "MonsterController.h"

class MonsterMgr:public Node
{
public:
	vector < pair<Entity*, MonsterController*> > Monster;
	CREATE_FUNC(MonsterMgr);
	void push(Entity*, MonsterController*);
protected:
private:

};