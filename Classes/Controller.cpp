#include "Controller.h"

Controller::Controller() {
}
Controller::~Controller() {
}

bool Controller::init() {
	if (!Node::init()) {
		return false;
	}
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event) {
		if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW) {
			this->__Player->Jump();
			return;
		}
		else if (keyCode == EventKeyboard::KeyCode::KEY_Q) {
			this->__Player->Attack('Q');
			return;
		}
		keys[keyCode] = true;
	};
	listener->onKeyReleased = [&](EventKeyboard::KeyCode keyCode, Event* event) {
		keys[keyCode] = false;
		if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW || keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
			this->__Player->stop();
			if (!this->__Player->judge("stand")&& this->__Player->__state!="move") return;
			else this->__Player->SetAnimation("stand", keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW?Player::left:Player::right);
			return;
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	this->scheduleUpdate();
	return true;
}
void Controller::BindPlayer(Player* player)
{
	this->__Player = player;
}
void Controller::update(float delta) {
	if (IsKeyPressed(EventKeyboard::KeyCode::KEY_LEFT_ARROW))  //<-
		this->__Player->TurnDirction(Player::left);
	if (IsKeyPressed(EventKeyboard::KeyCode::KEY_RIGHT_ARROW))  //<-
		this->__Player->TurnDirction(Player::right);
/*	if (IsKeyPressed(EventKeyboard::KeyCode::KEY_UP_ARROW))  //<-
		this->__Entity->Jump();
	if (IsKeyPressed(EventKeyboard::KeyCode::KEY_DOWN_ARROW))  //<-
		this->__Entity->Jump(-10.0f);*/
}

bool Controller::IsKeyPressed(EventKeyboard::KeyCode keycode) {
	return keys[keycode];
}