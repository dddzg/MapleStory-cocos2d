#include "Infomation.h"
Infomation* Infomation::info = NULL;
Infomation * Infomation::getInstance()
{
	if (info == NULL) {
		info = new Infomation();
		if (info && info->init()) {
			info->autorelease();
			info->retain();
		}
		else {
			CC_SAFE_DELETE(info);
		}
	}
	return info;
}

bool Infomation::init()
{
	this->name = "Ä¬ÈÏÃû³Æ";
	this->score = 0;
	return true;
}

