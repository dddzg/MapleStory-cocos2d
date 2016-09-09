#include "Player.h"


Player::Player()
{
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(Player::die), "die", NULL);
}

Player::~Player() {
}

Player * Player::CreateWithSprite(char * plist_name, unsigned int bit1, unsigned int bit2, char * Entity_name)
{
	auto player = new Player();
	player->LoadInfo("json/skill.json");
	Json::Reader reader;
	Json::Value root;
	auto SetSkill = [&](SkillInf* &sk, Json::Value value) {
		sk = SkillInf::create();
		sk->SetInf(value["damage"]["base"].asInt(), value["damage"]["change"].asInt(), value["cd"].asDouble(), Vec2(value["range"]["x"].asDouble(), value["range"]["y"].asDouble()), value["file"].asString());
		sk->use_effect = value["use_effect"].asString();
		sk->hit_effect = value["hit_effect"].asString();
		player->addChild(sk);
	};
	string data = FileUtils::getInstance()->getStringFromFile("json/skill.json");
	if (reader.parse(data, root, false)) {
		player->jump_effect = root["jump_effect"].asString();
		player->die_effect = root["die_effect"].asString();
		player->hit_effect = root["hit_effect"].asString();
		SetSkill(player->Q, root["Q"]);
		SetSkill(player->W, root["W"]);
		SetSkill(player->E, root["E"]);
	}
	player->FullHp=player->HP = 500;
	player->level = 1;
	player->nodead = false;
	if (player && player->InitWithSprite(plist_name, bit1, bit2, Entity_name)) {
		player->autorelease();
	}
	else {
		CC_SAFE_DELETE(player);
	}
	return player;
}

void Player::Attack(char S) {
	switch (S)
	{
	case 'Q':
		Q->Attack();
		Entity::Attack(S);
		break;
	case 'W':
		W->Attack();
		Entity::Attack(S);
		break;
	case 'E':
		E->Attack();
		Entity::Attack(S);
		break;

	default:
		break;
	}

}
int Player::GetDamage(char S)
{
	auto skill = GetSkill(S);
	return skill->__damage[skill->__level]+random()%skill->__change;
}
void Player::hit() {
	this->nodead = true;
	SimpleAudioEngine::getInstance()->playEffect(this->hit_effect.c_str());
	this->scheduleOnce(schedule_selector(Player::changedead), 1.5);
	if (!judge("hit1")) return;
	SetAnimation("hit1", dirction, 0.5, 1);
}
void Player::die(Ref* psender)
{
	if (!judge("die1")) return;
	SetAnimation("die1", dirction, 0.5, -1);
	SimpleAudioEngine::getInstance()->playEffect(this->die_effect.c_str());
	Is_Dead = true;
}
void Player::changedead(float dt)
{
	this->nodead = false;
}

const Vec2 & Player::getPosition()
{
	// TODO: 在此处插入 return 语句
	return this->__sprite->getPosition();
}



SkillInf * Player::GetSkill(char S)
{
	log("%c", S);
	switch (S)
	{
	case 'Q':
		return this->Q;
		break;
	case 'W':
		return this->W;
		break;
	case 'E':
		return this->E;
		break;
	default:
		break;
	}
	return nullptr;
}
/*void Player::Attack(char S)
{
	if (S == 'Q' && !Q->IsInCd()){
		Q->Attack();
		Entity::Attack();
		auto pos = getPosition();
	//	log("!!!%f %f", pos.x, pos.y);
		int num;
		if (dirction==right)
			num=FindEntity(pos,Vec2(pos.x+Q->__range.x,pos.y+ Q->__range.y));
		else 
			num= FindEntity(Vec2(pos.x+__sprite->getContentSize().width- Q->__range.x,pos.y), Vec2(pos.x+ __sprite->getContentSize().width, pos.y + Q->__range.y));
	//	log("%d", num);
		if (num != -1) {
			((*Monster)[num].first)->hit();
		}
	}
}

int Player::FindEntity(Vec2 begin,Vec2 end)
{
	int i;
	//DrawRect(begin, end);
//	log("begin:%f %f end:%f %f",begin.x,begin.y,end.x,end.y);
	for (i = 0; i < (*Monster).size();++i) {
		auto mon = (*Monster)[i];
		auto entity = mon.first;
		auto pos = entity->getPosition();
		//log("%d %f %f", i, pos.x, pos.y);
		auto IsIn = [&](Vec2 pos)->bool {
			if (pos.x >= begin.x && pos.y >= begin.y && pos.x <= end.x && pos.y <= end.y) {
				return true;
			}
			else return false;
		};
		//DrawRect(pos, Vec2(pos.x+ entity->__sprite->getContentSize().width,pos.y+ entity->__sprite->getContentSize().height));
		double width = entity->__sprite->getContentSize().width / 2;
		double height = entity->__sprite->getContentSize().height;
		if (IsIn(pos)|| IsIn(Vec2(pos.x+ width,pos.y+ height)) || IsIn(Vec2(pos.x + width, pos.y )) || IsIn(Vec2(pos.x, pos.y +height))) {
			break;
		}
	}
	if (i < (*Monster).size()) {
		return i;
	}
	else {
		return -1;
	}
}

void Player::DrawRect(Vec2 begin, Vec2 end, double time/*=1)
{
	auto drawNode = DrawNode::create();
	addChild(drawNode);
	drawNode->drawRect(begin, end, Color4F::WHITE);
}*/


