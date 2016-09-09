#include "SceneManager.h"
#include "MainScene.h"
SceneManager* SceneManager::mSceneManager = NULL;

SceneManager* SceneManager::getInstance() {
	if (mSceneManager == NULL) {
		mSceneManager = new SceneManager();
		if (mSceneManager && mSceneManager->init()) {
			mSceneManager->autorelease();
			mSceneManager->retain();
		}
		else {
			CC_SAFE_DELETE(mSceneManager);
		}
	}
	return mSceneManager;
}

bool SceneManager::init() {
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(SceneManager::game), "game", NULL);
	return true;
}

void SceneManager::game(Ref* sender) {
	Director::getInstance()->getRunningScene()->cleanup();
	NotificationCenter::getInstance()->destroyInstance();
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(SceneManager::game), "game", NULL);
	changeScene(MainGame);
}
void SceneManager::changeScene(EnumSceneType enSceneType) {
	Scene* pScene = NULL;
	switch (enSceneType)
	{
	case SceneManager::Login:
		pScene = Login::CreateScene();
		break;
	case SceneManager::MainGame:
		pScene = MainScene::CreateScene();
		break;
	default:
		break;
	}

	if (pScene == NULL) {
		return;
	}
	auto pDirector = Director::getInstance();
	auto curScene = pDirector->getRunningScene();
	if (curScene == NULL) {
		pDirector->runWithScene(pScene);
	}
	else {
		pDirector->replaceScene(pScene);
	}
}