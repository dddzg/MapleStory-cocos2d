#pragma once
#include "cocos2d.h"
#include "ui\CocosGUI.h"
#include "cocostudio\CCSGUIReader.h"
#include <string.h>
#include <string>
USING_NS_CC;
using namespace ui;
using namespace cocostudio;
using namespace std;
#include "extensions/cocos-ext.h"
#include "network/HttpClient.h"
#include "network/HttpRequest.h"
#include "network/HttpResponse.h"
#include "JSON\writer.h"
#include "JSON\reader.h"
#include "JSON\value.h"
using namespace network;
class PlayerInfo:public Layer{
public:
	PlayerInfo();
	~PlayerInfo();
	CREATE_FUNC(PlayerInfo);
	void SetBlood();
	void ChangeBlood(Ref* sender);
	void die(Ref* sender);
	void AddDieNum(Ref* sender);
	int MonsterDieNum;
	void Click_Ok(Ref*,TouchEventType type);
	void Click_Cancel(Ref*, TouchEventType type);
private:
	LoadingBar* HpBar;
	Text* Name;
	Text* My_Level;
	Text* My_Kill;
	TextBMFont* Kill;
	Button* ok;
	Button* cancel;
	const char* gb23122utf8(const char* gb2312);
	void onHttpRequestCompleted(HttpClient *sender, HttpResponse *response);
};

