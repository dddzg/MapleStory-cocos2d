#include "Entity.h"
map<string, int> Entity::rank = {
	{"stand",0},
	{"move",5},
	{"jump",10},
	{"hit1",15},
	{"attack",20},
	{"die1",25}
};

Entity::Entity() {
	dirction = left;
	__state = "";
	CotNum = 0;
}
Entity::~Entity() {
}
Entity* Entity::CreateWithSprite(const char* plist_name,unsigned int bit1,unsigned int bit2,char* Entity_name/* ="stand.0.png" */) {
	auto entity = new Entity();
	if (entity && entity->InitWithSprite(plist_name,bit1,bit2,Entity_name)) {
		entity->autorelease();
	}
	else {
		CC_SAFE_DELETE(entity);
	}
	return entity;
}

bool Entity::InitWithSprite(const char* plist_name, unsigned int bit1, unsigned int bit2, char* Entity_name/* ="stand.0.png" */) {
	if (!Node::init()) {
		return false;
	}
	this->LoadInfo(string(plist_name));
	this->Is_Dead = false;
	this->haveblood = false;
	this->plist_name = string(plist_name)+ string("/");
	this->__sprite = Sprite::create(string(this->plist_name)+string(Entity_name));
	__sprite->setAnchorPoint(Vec2(0, 0));
	init_height = this->__sprite->getContentSize().height + 8;
	this->addChild(__sprite);
	auto body = PhysicsBody::createEdgeBox(__sprite->getContentSize(), PHYSICSSHAPE_MATERIAL_DEFAULT,1.0f);
	body->setCategoryBitmask(bit1);
	body->setCollisionBitmask(bit2);
	body->setContactTestBitmask(0xFFFFFFFF);
	body->getShape(0)->setFriction(0.0f);
	body->getShape(0)->setRestitution(0.0f);
	body->setMass(500);
	body->setDynamic(true);
	__sprite->setTag(1);
	__sprite->setPhysicsBody(body);

	SetAnimation("stand",this->dirction);
	return true;
}
void Entity::CheckDir(DIR dir)
{
	if (dirction != dir) {
		dirction = dir;
	}
	__sprite->setFlippedX(dirction == right ? true : false);
}

const Vec2 & Entity::getPosition()
{
	return this->__sprite->getPosition();
}

void Entity::LoadInfo(string name)
{
	Json::Reader reader;
	Json::Value root;
	string data = FileUtils::getInstance()->getStringFromFile(string("json/") + name+string(".json"));
	if (reader.parse(data, root, false)) {
		this->FullHp=this->HP = root["HP"].asInt();
		this->__hit = root["damage"].asInt();
		this->speed = root["speed"].asInt();
		this->run_state = root["state"].asString();
		this->hit_effect = root["hit_effect"].asString();
		this->die_effect = root["die_effect"].asString();
	}
}

void Entity::SetBlood()
{
	this->__bloodbase = Sprite::create("bloodbase.png");
	this->__blood = Sprite::create("blood.png");
	this->addChild(this->__bloodbase);
	this->addChild(this->__blood);
	init_width = this->__blood->getContentSize().width;
	this->__bloodbase->setPosition(Vec2(this->__sprite->getPosition().x + this->__sprite->getContentSize().width / 2- this->__bloodbase->getContentSize().width/2, this->__sprite->getPosition().y + this->__sprite->getContentSize().height+8));
	this->__blood->setPosition(this->__bloodbase->getPosition()+Vec2(3,3));
	this->__blood->setAnchorPoint(Vec2(0, 0));
	this->__bloodbase->setAnchorPoint(Vec2(0, 0));
	this->scheduleUpdate();
}

void Entity::update(float dt)
{
	this->__bloodbase->setPosition(Vec2(this->__sprite->getPosition().x + this->__sprite->getContentSize().width / 2- this->__bloodbase->getContentSize().width/2, this->__sprite->getPosition().y +init_height));
	this->__blood->setPosition(this->__bloodbase->getPosition()+Vec2(3,3));
	double percent = double(this->HP) / double(this->FullHp);
	if (percent < 0) percent = 0;
//	this->__blood->setContentSize(Size(this->__blood->getContentSize().width*percent, this->__blood->getContentSize().height));
	this->__blood->setScaleX(percent);
}

void Entity::StopAnimation() {
	__sprite->stopAllActions();
}

void Entity::SetAnimation(string state, DIR dir, double delay, int loop) {
	CheckDir(dir);
	if (state == "jump" && __state == "move") {
		__state = "jump";
		return;
	}
	if (__state == state) return;
	else{
		StopAnimation();
		__state = state;
	}
	if (state == "jump") state = "move";
	auto animation = Animation::create();
	for (int index = 0;;) {
		string path = this->plist_name + StringUtils::format("%s.%d.png", state.c_str(), index++);
		std::string fullpath = FileUtils::getInstance()->fullPathForFilename(path);
		if (fullpath.size() == 0)
		{
			break;
		}
		animation->addSpriteFrameWithFile(path);
	}
	animation->setLoops(loop);
	animation->setRestoreOriginalFrame(true);
	animation->setDelayPerUnit(delay);
	auto animate = Animate::create(animation);
	if (__state == "die1" && delay==0.2) {
		auto call = CallFunc::create([this]() {
			/*this->removeAllChildrenWithCleanup(true);*/
			this->setVisible(false);
			//this->__sprite = NULL;
		});
		auto many = Sequence::create(animate, call, NULL);
		__sprite->runAction(many);
	}
	if (__state!="move" && __state!="jump" && __state!="stand"){
		auto call = CallFunc::create([this]() {
			this->__state = "jump";
		});
		auto many = Sequence::create(animate, call, NULL);
		__sprite->runAction(many);
	}
	else
	{
		__sprite->runAction(animate);
	}
}

void Entity::TurnDirction(DIR dir, float speed,int state) {
	if (!judge("move")) return;
	CheckDir(dir);
	if (__state!="jump")
	SetAnimation("move",dir);
	auto body = __sprite->getPhysicsBody();
	if (body->getVelocity().x >= 100 && dir==DIR::right || body->getVelocity().x <= -100 && dir==DIR::left) return;
	if (state) body->setVelocity(body->getVelocity() + Vec2(speed*(dir==DIR::right?1:-1), 0));
	else {
		body->setVelocity(/*body->getVelocity() +*/ Vec2(speed*(dir == DIR::right ? 1 : -1), 0));
	}
}
bool Entity::judge(string state) {
	if (state == __state && state == "move") return true;
	if (state == "move" && __state == "jump") return true;
	if (rank[__state] >= rank[state]) return false;
	else return true;
}

void Entity::Attack(char S)
{
	if (!judge("attack")) return;
	SetAnimation("attack", dirction,0.1,1);
}
void Entity::hit() {
	if (!judge("hit1")) return;
	if (!haveblood) {
		this->SetBlood();
		haveblood = true;
	}
	SimpleAudioEngine::getInstance()->playEffect(this->hit_effect.c_str());
	SetAnimation("hit1", dirction,0.6,1);
}
void Entity::die()
{
	if (!judge("die1")) return;
	NotificationCenter::getInstance()->postNotification("DieMonster", NULL);
	SimpleAudioEngine::getInstance()->playEffect(this->die_effect.c_str());
	auto body = __sprite->getPhysicsBody();
	body->setVelocity(Vec2(0, 0));
	SetAnimation("die1", dirction,0.2,1);
}
void Entity::Jump(float speed, int state)
{
	if (!judge("jump")) return;
	SetAnimation("jump",dirction);
	if (state == 2) {
		SimpleAudioEngine::getInstance()->playEffect(this->jump_effect.c_str());
	}
	auto body = __sprite->getPhysicsBody();
	if (state>=1){
		body->setVelocity(body->getVelocity()+Vec2(0, speed));
	}
	else {
		body->setVelocity(Vec2(body->getVelocity().x * CCRANDOM_0_1()*5, body->getVelocity().y));
		body->setVelocity(body->getVelocity() + Vec2(0, speed));
	}
}

void Entity::stop() {
	auto body = __sprite->getPhysicsBody();
	body->setVelocity(Vec2(0, body->getVelocity().y));
}


