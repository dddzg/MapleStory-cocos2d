#include "PlayerInfo.h"
#include "infomation.h"
PlayerInfo::PlayerInfo()
{
	auto UI = GUIReader::getInstance()->
		widgetFromJsonFile("UI/NewUi_1.ExportJson");
	this->addChild(UI);


	HpBar = (LoadingBar*)Helper::seekWidgetByName(UI, "hpbar");
	Name = (Text*)Helper::seekWidgetByName(UI, "name");
	My_Level = (Text*)Helper::seekWidgetByName(UI, "level");
	auto name = Infomation::getInstance()->name;
	Name->setText(name);
	Name->setFontName("Î¢ÈíÑÅºÚ");
	auto level = string("LV1");
	My_Level->setText(level);
	My_Level->setFontName("Î¢ÈíÑÅºÚ");
	My_Kill= (Text*)Helper::seekWidgetByName(UI, "killnum");
	MonsterDieNum = 0;
	auto TTT= (Text*)Helper::seekWidgetByName(UI, "Label_5");
	TTT->setFontName("Î¢ÈíÑÅºÚ");
	TTT->setColor(Color3B::BLACK);
	My_Kill->setText("0");
	My_Kill->setFontName("Î¢ÈíÑÅºÚ");
	My_Kill->setColor(Color3B::BLACK);
	NotificationCenter::getInstance()->addObserver(
		this,
		callfuncO_selector(PlayerInfo::ChangeBlood),
		"blood",
		NULL
		);
	NotificationCenter::getInstance()->addObserver(
		this,
		callfuncO_selector(PlayerInfo::die),
		"die",
		NULL
		);
	NotificationCenter::getInstance()->addObserver(
		this, 
		callfuncO_selector(PlayerInfo::AddDieNum),
		"DieMonster", 
		NULL
		);
}



PlayerInfo::~PlayerInfo()
{
}

void PlayerInfo::ChangeBlood(Ref * sender)
{
	int data = (int)sender;
	this->HpBar->setPercent(data);
}
void PlayerInfo::die(Ref * sender) {
	Name->setColor(Color3B::RED);
	My_Level->setColor(Color3B::RED);
	auto UI = GUIReader::getInstance()->
		widgetFromJsonFile("DIE/NewUi12_1.ExportJson");
	this->addChild(UI);
	Kill = (TextBMFont*)Helper::seekWidgetByName(UI, "KILL");
	//string tt = string("±¾´ÎÉ±µÐ") +  + string("Ö»");
	Kill->setText(string("KILL ")+StringUtils::toString(MonsterDieNum));
	ok=(Button*)Helper::seekWidgetByName(UI, "ok");
	ok->addTouchEventListener(this, toucheventselector(PlayerInfo::Click_Ok));
	cancel= (Button*)Helper::seekWidgetByName(UI, "cancel");
	cancel->addTouchEventListener(this, toucheventselector(PlayerInfo::Click_Cancel));
}
const char * PlayerInfo::gb23122utf8(const char * gb2312)
{
	int len = MultiByteToWideChar(0, 0, gb2312, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(0, 0, gb2312, -1, wstr, len);
	len = WideCharToMultiByte(65001, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(65001, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
}

void PlayerInfo::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response)
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
	log("%s", res);
	return;
		//log("%s %s", status.c_str(), root["name"].asCString());
}

void PlayerInfo::AddDieNum(Ref * sender)
{
	MonsterDieNum++;
	My_Kill->setText(StringUtils::toString(MonsterDieNum));
}

void PlayerInfo::Click_Ok(Ref*, TouchEventType type)
{
	switch (type) {
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	case TouchEventType::TOUCH_EVENT_ENDED:
		do 
		{
			if (MonsterDieNum > Infomation::getInstance()->score) {
				Infomation::getInstance()->score = MonsterDieNum;
				HttpRequest* request = new (std::nothrow) HttpRequest();
				request->setUrl("http://www.dzglalala.cn:1024/update");
				request->setRequestType(HttpRequest::Type::POST);
				request->setResponseCallback(CC_CALLBACK_2(PlayerInfo::onHttpRequestCompleted, this));

				// write the post data
				//const char* postData = "visitor=cocos2d&TestSuite=Extensions Test/NetworkTest";
				string postData = StringUtils::format("id=%s&score=%d", Infomation::getInstance()->id.c_str(), MonsterDieNum);
				log("%s", postData);
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
			}
			NotificationCenter::getInstance()->postNotification("game");
		} while (0);
		break;
	case TouchEventType::TOUCH_EVENT_CANCELED:
		break;
	}
}

void PlayerInfo::Click_Cancel(Ref *, TouchEventType type)
{
	switch (type) {
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	case TouchEventType::TOUCH_EVENT_ENDED:
		Director::getInstance()->end();
		break;
	case TouchEventType::TOUCH_EVENT_CANCELED:
		break;
	}
}
