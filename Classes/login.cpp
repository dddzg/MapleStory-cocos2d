#include "login.h"
#include "SimpleAudioEngine.h"
Scene * Login::CreateScene()
{
	auto scene = Scene::create();
	auto layer = Login::create();
	scene->addChild(layer);
	return scene;
}

bool Login::init()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/Title.mp3", true);
	auto UI = GUIReader::getInstance()->
		widgetFromJsonFile("login/ffff_1.ExportJson");
	auto bg = Sprite::create("bg.png");
	bg->setAnchorPoint(Vec2(0, 0));
	bg->setPosition(Vec2(0, 0));
	this->addChild(bg);
	this->addChild(UI);
	user = (TextField*)Helper::seekWidgetByName(UI, "user");
	password = (TextField*)Helper::seekWidgetByName(UI, "password");
	Start = (Button*)Helper::seekWidgetByName(UI, "start");
	Start->addTouchEventListener(this, toucheventselector(Login::Click_Start));
	Register = (Button*)Helper::seekWidgetByName(UI, "register");
	Register->addTouchEventListener(this, toucheventselector(Login::Click_Register));
	if (!Layer::init()) return false;
	return true;
}

void Login::Click_Start(Ref *, TouchEventType type)
{
	switch (type) {
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	case TouchEventType::TOUCH_EVENT_ENDED:
		do {
			log("%s %s", user->getString().c_str(), password->getString().c_str());
			HttpRequest* request = new (std::nothrow) HttpRequest();
			request->setUrl("http://www.dzglalala.cn:1024/login");
			request->setRequestType(HttpRequest::Type::POST);
			request->setResponseCallback(CC_CALLBACK_2(Login::onHttpRequestCompleted, this));

			// write the post data
			//const char* postData = "visitor=cocos2d&TestSuite=Extensions Test/NetworkTest";
			string postData = StringUtils::format("id=%s&password=%s", user->getString().c_str(),password->getString().c_str());
			request->setRequestData(postData.c_str(), postData.size());
			/*if (isImmediate)
			{
				request->setTag("POST immediate test1");
				HttpClient::getInstance()->sendImmediate(request);
			}
			else
			{*/
				request->setTag("POST test1");
				HttpClient::getInstance()->sendImmediate(request);
			//}
			request->release();
			
		} while (0);
		
		break;
	case TouchEventType::TOUCH_EVENT_CANCELED:
		break;
	}
}

void Login::Click_Register(Ref *, TouchEventType type)
{
	switch (type) {
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	case TouchEventType::TOUCH_EVENT_ENDED:
		Application::getInstance()->openURL("http://www.dzglalala.cn:1024/C++");  //µ¯¿ò×¢²á
		break;
	case TouchEventType::TOUCH_EVENT_CANCELED:
		break;
	}
}





void Login::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response)
{
	if (!response)
	{
		return;
	}

	// You can get original request type from: response->request->reqType
	if (0 != strlen(response->getHttpRequest()->getTag()))
	{
		log("%s completed", response->getHttpRequest()->getTag());
	}

	long statusCode = response->getResponseCode();
	char statusString[64] = {};
	sprintf(statusString, "HTTP Status Code: %ld, tag = %s", statusCode, response->getHttpRequest()->getTag());
	//_labelStatusCode->setString(statusString);
	log("response code: %ld", statusCode);

	if (!response->isSucceed())
	{
		log("response failed");
		log("error buffer: %s", response->getErrorBuffer());
		return;
	}

	// dump data
	std::vector<char> *buffer = response->getResponseData();
	string res;
	log("Http Test, dump data: ");
	for (unsigned int i = 0; i < buffer->size(); i++)
	{
		res += (*buffer)[i];
		//log("%c", (*buffer)[i]);
	}
	Json::Reader reader;
	Json::Value root;
	string status;
	string name;
	if (reader.parse(res, root, false)) {
		status = root["ok"].asString();
		//name = root["name"].asString();
		if (status == "ok") {
			Infomation::getInstance()->name = root["name"].asString();
			Infomation::getInstance()->score = root["score"].asInt();
			Infomation::getInstance()->id = root["id"].asString();
			NotificationCenter::getInstance()->postNotification("game");  //µÇÂ½
		}
		//log("%s %s", status.c_str(), root["name"].asCString());
	}
// 	else {
// 		log("error");
// 	}
// 	log("%s\n",res.c_str());
	//if (response->getHttpRequest()->getReferenceCount() != 2)
	//{
	//	log("request ref count not 2, is %d", response->getHttpRequest()->getReferenceCount());
	//}
}
