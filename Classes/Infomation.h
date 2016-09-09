#pragma once
#include "cocos2d.h"
#include <iostream>
#include <string>
#include <cstring>
using namespace cocos2d;
using namespace std;
class Infomation: public Ref {
public:
	static Infomation* getInstance();
	string name;
	string id;
	int score;
	virtual bool init();
private:
	static Infomation* info;
};