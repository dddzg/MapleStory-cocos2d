#include "MyHelloWorldScene.h"
Scene* MyHelloWorldScene::createScene() {
	auto scene = Scene::create();
	auto layer = MyHelloWorldScene::create();
	scene->addChild(layer);
	return scene;
}

bool MyHelloWorldScene::init(){
	auto sprite = Sprite::create("HelloWorld.png");
	sprite->setPosition(ccp(300, 300));
	this->addChild(sprite);
	return true;
}
