#pragma once
#include "cocos2d.h"
#include "ui\CocosGUI.h"
#include "cocostudio\CCSGUIReader.h"
#include "extensions/cocos-ext.h"
#include "network/HttpClient.h"
#include "network/HttpRequest.h"
#include "network/HttpResponse.h"
#include "JSON\writer.h"
#include "JSON\reader.h"
#include "JSON\value.h"
#include "infomation.h"
#include <string.h>
#include <string>
USING_NS_CC;
using namespace ui;
using namespace cocostudio;
using namespace std;
using namespace network;
class Login :public Layer {
public:
	static Scene* CreateScene();
	CREATE_FUNC(Login);
	virtual bool init();
	Button* Start;
	Button* Register;
	TextField* user;
	TextField* password;
	void Click_Start(Ref*, TouchEventType type);
	void Click_Register(Ref*, TouchEventType type);
	void Post();
	void onHttpRequestCompleted(HttpClient *sender, HttpResponse *response);
protected:
private:
};