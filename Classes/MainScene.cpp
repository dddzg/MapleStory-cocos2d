#include "MainScene.h"
#include "MonsterController.h"
#include "BallSkillMgr.h"
#include "BoomSkillMgr.h"
#include "SimpleAudioEngine.h"
#include "PlayerInfo.h"
#include <math.h>

using namespace CocosDenshion;
/*map<int, int> MainScene::MNum = {
	{ 2,1 },
	{ 3,1 },
	{ 4,2 },
	{ 5,2 },
	{ 6,3 },
	{ 7,4 },
	{ 8,5 },
	{ 9,6 }
};*/

Scene* MainScene::CreateScene() {
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setAutoStep(false);
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vec2(0, -200));
	auto bglayer = Layer::create();
	auto bg = Sprite::create("bg.png");
	bg->setAnchorPoint(Vec2(0, 0));
	bg->setPosition(Vec2(0, 0));
	bglayer->addChild(bg);
	auto layer = MainScene::create();
	auto layer2 = PlayerInfo::create();
	scene->addChild(bglayer);
	scene->addChild(layer);
	scene->addChild(layer2);
	return scene;
}
MainScene::MainScene() {
}
bool MainScene::init() {
	if (!Layer::init()) {
		return false;
	}
	LoadMonsterList();
/*	auto sprite = Sprite::create("monkey/move.0.png");
	auto animation = Animation::create();
	for (int index = 0;;) {
		Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(string("monkey/") + StringUtils::format("%s.%d.png", string("attack1").c_str(), index++));
		if (texture == NULL) {
			break;
		}
		Rect rect = Rect::ZERO;
		rect.size = texture->getContentSize();
		SpriteFrame *frame = SpriteFrame::createWithTexture(texture, rect);
		animation->addSpriteFrame(frame);
	}
	animation->setLoops(-1);
	animation->setRestoreOriginalFrame(true);
	animation->setDelayPerUnit(0.2);
	auto animate = Animate::create(animation);
	sprite->runAction(animate);
	sprite->setPosition(Vec2(100, 400));
	this->addChild(sprite);*/
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(MainScene::die), "die", NULL);
	this->scheduleUpdate();
	this->__monstermgr = MonsterMgr::create();

	this->addChild(__monstermgr);
	Loadmap("dzg.tmx");
	auto player = Player::CreateWithSprite("monkey", 1, 2);
	SimpleAudioEngine::getInstance()->playBackgroundMusic("music/FloralLife.mp3", true);
	player->__sprite->setPosition(Vec2(100, 400));
	player->__sprite->setName("player");
	this->__player = player;
	this->addChild(player);
	this->schedule(schedule_selector(MainScene::MonsterWillLoad), 10.0);
	//加入碰撞检测
	AsForController();
	auto listener = EventListenerPhysicsContact::create();
	auto JudegAB = [](Sprite* &spriteA, Sprite* &spriteB,int& flag) { //A=entity  B=map
		flag = 1;
		auto TagA = spriteA->getTag();
		auto TagB = spriteB->getTag();
		if (TagA == OBJ::earth && TagB == OBJ::entity) {
			flag = -1;
			Sprite* spriteC = spriteA;
			spriteA = spriteB;
			spriteB = spriteC;
		}
	};
	listener->onContactBegin = [&](PhysicsContact& contact)->bool{
		auto spriteA = (Sprite*)contact.getShapeA()->getBody()->getNode();
		auto spriteB = (Sprite*)contact.getShapeB()->getBody()->getNode();
		int flag;
		if (spriteA->getName() == "entity" && spriteB->getName() == "player" || spriteB->getName() == "entity" && spriteA->getName() == "player") {
			return true;
		}
		JudegAB(spriteA, spriteB, flag);
		if (spriteA->getTag() == OBJ::entity && spriteB->getTag() == OBJ::earth) {
			//if (contact.getContactData()->normal.x*flag == 0 && contact.getContactData()->normal.y*flag == -1) {
				auto entity = (Entity*)spriteA->getParent();
				auto body = entity->__sprite->getPhysicsBody();
				body->setVelocity(Vec2(0, 0));
				if (entity->CotNum == 0 && (entity->__state=="jump"|| entity->__state=="move") )entity->SetAnimation("stand", entity->dirction);
				entity->CotNum++;
			//}
			return true;
		}
		
// 		if (TagA==OBJ::earth && TagB==OBJ::entity && contact.getContactData()->normal.x == 0 && contact.getContactData()->normal.y==1
// 			|| TagB == OBJ::earth && TagA == OBJ::entity && contact.getContactData()->normal.x == 0 && contact.getContactData()->normal.y == -1){
// 			if (spriteA && spriteA->getTag() == OBJ::earth && spriteB && spriteB->getTag() == OBJ::entity) {
// 				auto spriteC = spriteA;
// 				spriteA = spriteB;
// 				spriteB = spriteC;
// 			}
// 			//spriteA->getTag()== entity   B->getTag== earth
// 
// 			if (spriteA && spriteA->getTag() == OBJ::entity && spriteB && spriteB->getTag() == OBJ::earth) {
// 				auto entity = (Entity*)spriteA->getParent();
// 				entity->SetAnimation("stand", entity->dirction);
// 				log("yes%s", entity->__state.c_str());
// 			};
// 		}
		return false;
	};

	listener->onContactPreSolve = [&](PhysicsContact& contact,
		PhysicsContactPreSolve& solve)->bool{
		solve.setRestitution(0.0f);
		solve.setFriction(0.0f);
		int flag;
		auto spriteA = (Sprite*)contact.getShapeA()->getBody()->getNode();
		auto spriteB = (Sprite*)contact.getShapeB()->getBody()->getNode();
		JudegAB(spriteA, spriteB, flag);
		if (spriteA->getTag() == OBJ::entity && spriteB->getTag() == OBJ::earth) {
			auto entity = (Entity*)spriteA->getParent();
			if (entity->__state=="jump") {
				entity->SetAnimation("stand", entity->dirction);
			}
		}
		return true;
	};
	listener->onContactSeparate = [&](PhysicsContact& contact)->bool {

		auto spriteA = (Sprite*)contact.getShapeA()->getBody()->getNode();
		auto spriteB = (Sprite*)contact.getShapeB()->getBody()->getNode();
		int flag;
		JudegAB(spriteA, spriteB, flag);
		if (spriteA->getTag() == OBJ::entity && spriteB->getTag() == OBJ::earth) {
			//if (contact.getContactData()->normal.x*flag == 0 && contact.getContactData()->normal.y*flag == -1) {
				auto entity = (Entity*)spriteA->getParent();
				entity->CotNum--;
				if (entity->CotNum == 0) entity->Jump(0);
				
			//}
			return true;
		}
// 		auto spriteA = (Sprite*)contact.getShapeA()->getBody()->getNode();
// 		auto TagA = spriteA->getTag();
// 		auto spriteB = (Sprite*)contact.getShapeB()->getBody()->getNode();
// 		auto TagB = spriteB->getTag();
// 		if (TagA == OBJ::earth && TagB == OBJ::entity && contact.getContactData()->normal.x == 0 && contact.getContactData()->normal.y == 1
// 			|| TagB == OBJ::earth && TagA == OBJ::entity && contact.getContactData()->normal.x == 0 && contact.getContactData()->normal.y == -1) {
// 			if (spriteA && spriteA->getTag() == OBJ::earth && spriteB && spriteB->getTag() == OBJ::entity) {
// 				auto spriteC = spriteA;
// 				spriteA = spriteB;
// 				spriteB = spriteC;
// 			}
// 			//spriteA->getTag()== entity   B->getTag== earth
// 
// 			if (spriteA && spriteA->getTag() == OBJ::entity && spriteB && spriteB->getTag() == OBJ::earth) {
// 				auto entity = (Entity*)spriteA->getParent();
// 				entity->Jump();
// 				log("no%s", entity->__state.c_str());
// 			};
// 		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);


// 	auto cache = SpriteFrameCache::getInstance();
// 	cache->addSpriteFramesWithFile("jump.plist");
// 	auto sp = Sprite::createWithSpriteFrameName("move.0.png");
// 	sp->setPosition(Vec2(100, 400));
// 	this->addChild(sp);
// 	
// 	auto sfme = Vector<SpriteFrame*>::Vector();
// 	char str[20] = { 0 };
// 	for (int i = 0;; ++i) {
// 		sprintf(str, "move.%d.png", i);
// 		auto fname = cache->getSpriteFrameByName(str);
// 		if (fname)
// 		sfme.pushBack(fname);
// 		else break;
// 	}
// 	auto animation = Animation::createWithSpriteFrames(sfme);
// 	animation->setLoops(-1);
// 	animation->setRestoreOriginalFrame(true);
// 	animation->setDelayPerUnit(0.2f);
// 	sp->runAction(Animate::create(animation));
// 	sp->setAnchorPoint(Vec2(0, 0));
// 	auto body = PhysicsBody::createBox(sp->getContentSize());
// 	body->setContactTestBitmask(0xFFFFFFFF);
// 	body->setRotationEnable(false);
// 	//	body->setDynamic(false);
// 	body->setVelocity(Vec2(10, 0));
// 	body->getShape(0)->setFriction(0.0f);
// 	body->getShape(0)->setRestitution(0.0f);
// 	body->getShape(0)->setDensity(0.0f);
// 	sp->setPhysicsBody(body);
// 	sp->setFlippedX(true);
	return true;
}


void MainScene::Loadmap(string a)
{
	auto size = Director::getInstance()->getVisibleSize();
	Director::getInstance()->setProjection(Director::Projection::_2D);
	TMXTiledMap* map = TMXTiledMap::create(a);
	map->setTag(OBJ::map);
	auto mapTiledNum = map->getMapSize();
	auto titledSize = map->getTileSize();
	auto mapSize = Size(
		mapTiledNum.width*titledSize.width,
		mapTiledNum.height*titledSize.height);
	this->mapsize = mapSize;
	auto worldbody = PhysicsBody::createEdgeBox(mapSize, PHYSICSBODY_MATERIAL_DEFAULT, 5.0f);
	auto worldnode = Node::create();
	worldnode->setPosition(Vec2(mapSize.width / 2, mapSize.height / 2));
	worldnode->setPhysicsBody(worldbody);
	worldnode->setName("world");
	this->addChild(worldnode);
	this->addChild(map);
	// 	auto listener = EventListenerTouchOneByOne::create();
	// 	listener->onTouchBegan = [](Touch* touch, Event* event) {
	// 		auto touchPos = Director::getInstance()->convertToGL(touch->getStartLocationInView());
	// 		log("%f %f", touchPos.x, touchPos.y);
	// 		return true;
	// 	};
	// 	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	
	auto layer = map->getLayer("meta");
	for (int y = 0; y < mapTiledNum.height; ++y) 
		for (int x = 0; x < mapTiledNum.width; ++x) {
			auto sprite = layer->getTileAt(Vec2(x, y));
			if (sprite != NULL) {
				int length = 1;
				for (x = x + 1; x < mapTiledNum.width; ++x){
					auto sprite2 = layer->getTileAt(Vec2(x, y));
					if (sprite2 != NULL) ++length;
					else break;
				}
				auto body = PhysicsBody::createEdgeBox(Size(sprite->getContentSize().width*length, sprite->getContentSize().height),PHYSICSBODY_MATERIAL_DEFAULT,1);
				body->setPositionOffset(Vec2(sprite->getContentSize().width*(length-1)/2,0));
				body->setDynamic(false);
				body->setLinearDamping(0);
				body->getShape(0)->setFriction(0.0f);
				body->getShape(0)->setRestitution(0.0f);
				body->setMass(500);
				body->setContactTestBitmask(0xFFFFFFFF);
				sprite->setTag(OBJ::earth);
				sprite->setPhysicsBody(body);
// 				auto listener = EventListenerPhysicsContactWithBodies::create(body,this->__entity->__sprite->getPhysicsBody());
// 				listener->onContactBegin = [](PhysicsContact &contact) -> bool {
// 					auto spriteA = (Sprite*)contact.getShapeB()->getBody()->getNode();
// 					if (spriteA && spriteA->getTag() == OBJ::entity){
// 						if (contact.getContactData()->normal.x == 0 && contact.getContactData()->normal.y < 0) {
// 							return false;
// 						}
// 						else {
// 								auto entity = (Entity*)spriteA->getParent();
// 								entity->SetAnimation("stand", entity->dirction);
// 								entity->CotNum++;
// 								return true;
// 						}
// 					}
// 				};
// 				listener->onContactSeparate = [](PhysicsContact &contact) ->bool {
// 					auto spriteA = (Sprite*)contact.getShapeB()->getBody()->getNode();
// 					if (spriteA && spriteA->getTag() == OBJ::entity) {
// 						if (contact.getContactData()->normal.x == 0 && contact.getContactData()->normal.y < 0) {
// 							return false;
// 						}
// 						else {
// 							auto entity = (Entity*)spriteA->getParent();
// 							entity->CotNum--;
// 							if (entity->CotNum == 0) {
// 								entity->Jump();
// 							}
// 						}
// 					}
// 					return true;
// 				};
// 				_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
			}
		}
	for (int y = 0; y < mapTiledNum.height; ++y){
		int begin, end;
		begin = 0x7fffffff;
		end = -1;
		for (int x = 0; x < mapTiledNum.width; ++x) {
			auto sprite = layer->getTileAt(Vec2(x, y));
			int xx = x;
			int yy = (int)(mapTiledNum.height) - y - 1;
			if (sprite != NULL) {
				MapInfo[xx][yy] = 1;
				if (y>=1 && layer->getTileAt(Vec2(x, y-1))==NULL){
					begin = min(begin, xx);
					end = max(end, xx);
					if (x + 1 == mapTiledNum.width) {
						cal(begin, end,yy);
						begin = 0x7fffffff;
						end = -1;
					}
					else{
						auto sprite1 = layer->getTileAt(Vec2(x + 1, y));
						auto sprite2 = layer->getTileAt(Vec2(x + 1, y - 1));
						if (sprite1 == NULL || sprite2) {
							cal(begin, end,yy);
							begin = 0x7fffffff;
							end = -1;
						}
					}
				}
			}
			else {
				MapInfo[xx][yy] = 0;
			}
		}
	}
	__maxnum = MonsterPos.size();
}

void MainScene::update(float delta) {
	if (IsKeyPressed(EventKeyboard::KeyCode::KEY_LEFT_ARROW))  //<-
		this->__player->TurnDirction(Player::left);
	if (IsKeyPressed(EventKeyboard::KeyCode::KEY_RIGHT_ARROW))  //<-
		this->__player->TurnDirction(Player::right);
	Director::getInstance()->getRunningScene()->getPhysicsWorld()->step(1/(60.0f));
	auto size = Director::getInstance()->getVisibleSize();
	//log("node:%f %f sprite:%f %f", this->__entity->getPosition().x, this->__entity->getPosition().y, this->__entity->__sprite->getPosition().x, this->__entity->__sprite->getPosition().y);
//	this->runAction(Follow::create(this->__entity->__sprite,Rect(size.width/2, size.height / 2,mapsize.width- size.width / 2,mapsize.height-size.height / 2)));
	this->runAction(Follow::create(this->__player->__sprite, Rect(0, 0, mapsize.width, mapsize.height)));
	if (!this->__player->nodead && !this->__player->Is_Dead){
		for (int i = 0; i < this->__monstermgr->Monster.size(); ++i) {
			auto entity=this->__monstermgr->Monster[i].first;
			if (entity && !entity->Is_Dead) {
				auto PlayerPos = this->__player->getPosition();
				auto EntityPos = entity->__sprite->getPosition();
				if (IsIn(PlayerPos, PlayerPos + Vec2(this->__player->__sprite->getContentSize().width, this->__player->__sprite->getContentSize().height), EntityPos, EntityPos + Vec2(entity->__sprite->getContentSize().width, entity->__sprite->getContentSize().height))){
					
					this->__player->hit();
					this->__player->HP -= entity->__hit;
					int percent = this->__player->HP*100 / this->__player->FullHp;
					if (percent < 0) { 
						NotificationCenter::getInstance()->postNotification("die", NULL); 
						NotificationCenter::getInstance()->postNotification("blood", 0);
					}
					else NotificationCenter::getInstance()->postNotification("blood", (Ref*)percent);
					
					auto body = this->__player->__sprite->getPhysicsBody();
					auto vel=body->getVelocity();
					int x = 100;
					if (entity->dirction==Entity::right && this->__player->dirction== Entity::right || entity->dirction == Entity::left && this->__player->dirction == Entity::right) {
						x = -100;
					}
					
					body->setVelocity(Vec2(x, 40));
					break;
				}
			}
		}
	}
}

void MainScene::cal(int begin, int end,int height) {
	if (begin == 0x7fffffff && end == -1 || begin==end || begin+1==end) return;
	else {
		auto map=(TMXTiledMap*)this->getChildByTag(OBJ::map);
		auto titledSize = map->getTileSize();
		int titlednum = end - begin + 1;
		int length = titlednum*titledSize.width;
		auto calnum = [](int num)->int {
			if (num == 2 || num==3) {
				return 1;
			}
			if (num >=4 && num<=5) {
				return 2;
			}
			if (num >= 6 && num <= 7) {
				return 3;
			}
			else return num - 4;
		};
		int monsternum = calnum(titlednum);
		double piece = (double)length / (double)(monsternum+1);
		Vec2 StartPoint = Vec2(titledSize.width*begin,titledSize.height*(height+1));
		Vec2 EndPoint = StartPoint + Vec2(length, 0);
		for (int i = 1; i <= monsternum; ++i) {
			auto MonsterPos = StartPoint + Vec2(i*piece, 0);
			MainScene::MonsterPos.push_back(MonsterPos);
			MonsterBegin.push_back(StartPoint);
			MonsterEnd.push_back(EndPoint);
			int r = random() % MonsterList.size();
			LoadMonster(MonsterPos,StartPoint,EndPoint, MonsterList[r].c_str());
		}
	}
}



void MainScene::LoadMonster(Vec2 MonsterPos, Vec2 StartPoint,Vec2 EndPoint,const char *filename,int state) {
	auto entity = Entity::CreateWithSprite(filename, 1, 2);
	entity->__sprite->setPosition(MonsterPos);
	entity->__sprite->setName("entity");
	auto MonsterCon = MonsterController::create();
	MonsterCon->Set(StartPoint, EndPoint, entity);
	this->__monstermgr->push(entity, MonsterCon);
}

void MainScene::AsForController()
{
	keylistener = EventListenerKeyboard::create();
	keylistener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event) {
		if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW) {
			this->__player->Jump(200,2);
			return;
		}
		else if (keyCode == EventKeyboard::KeyCode::KEY_Q) {
			Attack('Q');
			return;
		}
		else if (keyCode == EventKeyboard::KeyCode::KEY_W) {
			Attack('W');
			return;
		}
		else if (keyCode == EventKeyboard::KeyCode::KEY_E) {
			Attack('E');
			return;
		}
		keys[keyCode] = true;
	};
	keylistener->onKeyReleased = [&](EventKeyboard::KeyCode keyCode, Event* event) {
		keys[keyCode] = false;
		if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW || keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
			this->__player->stop();
			if (!this->__player->judge("stand") && this->__player->__state != "move") return;
			else this->__player->SetAnimation("stand", keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW ? Player::left : Player::right);
			return;
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keylistener, this);
	this->scheduleUpdate();
}

bool MainScene::IsKeyPressed(EventKeyboard::KeyCode keycode)
{
		return keys[keycode];
}

void MainScene::Attack(char S)
{
	switch (S)
	{
	case 'Q':
		if (!__player->Q->IsInCd() && __player->judge("attack")) {
			__player->Attack(S);
			auto pos = Vec2(__player->getPosition().x + __player->__sprite->getContentSize().width / 2, __player->getPosition().y);
			//	log("!!!%f %f", pos.x, pos.y);
			vector<int> num;
			if (__player->dirction == Entity::right)
				num = FindEntity(pos, Vec2(pos.x + __player->Q->__range.x, pos.y + __player->Q->__range.y), 2);
			else
				num = FindEntity(Vec2(pos.x - __player->Q->__range.x, pos.y), Vec2(pos.x, pos.y + __player->Q->__range.y), 2);
			for (int i = 0; i < num.size(); ++i) {
				auto skill = BallSkillMgr::create();
				this->addChild(skill);
				skill->SetInf(__player, this->__player->plist_name + this->__player->Q->__name,this->__player->GetDamage(S));
				skill->use_effect = __player->Q->use_effect;
				skill->hit_effect = __player->Q->hit_effect;
				skill->PushMonster(this->__monstermgr->Monster[num[i]]);
				skill->ball(0.5);
			}
		}
		break;
	case 'W':
		if (!__player->W->IsInCd() && __player->judge("attack")) {
			__player->Attack(S);
			auto pos = Vec2(__player->getPosition().x /*+ __player->__sprite->getContentSize().width / 2*/, __player->getPosition().y);
			//	log("!!!%f %f", pos.x, pos.y);
			vector<int> num;
			if (__player->dirction == Entity::right)
				num = FindEntity(pos, Vec2(pos.x + __player->W->__range.x, pos.y + __player->W->__range.y), 10);
			else
				num = FindEntity(Vec2(pos.x - __player->W->__range.x, pos.y), Vec2(pos.x, pos.y + __player->W->__range.y), 10);
			auto skill = BoomSkillMgr::create();
			this->addChild(skill);
			skill->SetInf(__player, this->__player->plist_name + this->__player->W->__name, this->__player->GetDamage(S));
			skill->use_effect = __player->W->use_effect;
			skill->hit_effect = __player->W->hit_effect;
			for (auto tmp:num)
				skill->PushMonster(this->__monstermgr->Monster[tmp]);
			skill->boom(0.5,0.3,0.4,0.2);
		}
	case 'E':
		if (!__player->E->IsInCd() && __player->judge("attack")) {
			__player->Attack(S);
			auto pos = Vec2(__player->getPosition().x /*+ __player->__sprite->getContentSize().width / 2*/, __player->getPosition().y);
			//	log("!!!%f %f", pos.x, pos.y);
			vector<int> num;
			if (__player->dirction == Entity::right)
				num = FindEntity(pos, Vec2(pos.x + __player->E->__range.x, pos.y + __player->E->__range.y), 10);
			else
				num = FindEntity(Vec2(pos.x - __player->E->__range.x, pos.y), Vec2(pos.x, pos.y + __player->E->__range.y), 10);
			auto skill = BoomSkillMgr::create();
			this->addChild(skill);
			skill->SetInf(__player, this->__player->plist_name + this->__player->E->__name, this->__player->GetDamage(S));
			skill->use_effect = __player->E->use_effect;
			skill->hit_effect = __player->E->hit_effect;
			for (auto tmp : num)
				skill->PushMonster(this->__monstermgr->Monster[tmp]);
			skill->boom(0.5,0.15,1,0.1);
		}
	default:
		break;
	}
}

int MainScene::FindEntity(Vec2 begin, Vec2 end)
{
//DrawNode(begin, end);
	int ans=-1;
	double dis;
	for (int i = 0; i < this->__monstermgr->Monster.size(); ++i) {
		auto mon = this->__monstermgr->Monster[i];
		auto entity = mon.first;
		if (entity->__sprite == nullptr) continue;
		auto pos = entity->getPosition();
		//log("%d %f %f", i, pos.x, pos.y);
		//DrawNode(pos, Vec2(pos.x+ entity->__sprite->getContentSize().width,pos.y+ entity->__sprite->getContentSize().height));
		double width = entity->__sprite->getContentSize().width ;
		double height = entity->__sprite->getContentSize().height;
		if (IsIn(begin, end, pos, Vec2(pos.x + width, pos.y + height))) {
			if (ans == -1) {
				ans = i;
				dis = fabs(pos.x - begin.x);
			}
			else {
				auto dis2= fabs(pos.x - begin.x);
				if (dis2 < dis) {
					dis2 = dis;
					ans = i;
				}
			}
		}
	}
	return ans;
}

vector<int> MainScene::FindEntity(Vec2 begin, Vec2 end, int num)
{
//	DrawNode(begin, end);
	priority_queue< pair<double, int> > ans;
	for (int i = 0; i < this->__monstermgr->Monster.size(); ++i) {
		auto mon = this->__monstermgr->Monster[i];
		auto entity = mon.first;
		if (entity && !entity->Is_Dead) {
			log("%d", entity->Is_Dead);
			auto pos = entity->__sprite->getPosition();
			//log("%d %f %f", i, pos.x, pos.y);
			//DrawNode(pos, Vec2(pos.x+ entity->__sprite->getContentSize().width,pos.y+ entity->__sprite->getContentSize().height));
			double width = entity->__sprite->getContentSize().width;
			double height = entity->__sprite->getContentSize().height;
			if (IsIn(begin,end,pos, Vec2(pos.x + width, pos.y + height))){
				double tmp = fabs(pos.x - begin.x);
				if (ans.size() < num) {
					ans.push(make_pair(tmp, i));
				}
				else {
					if (tmp < ans.top().first) {
						ans.pop();
						ans.push(make_pair(tmp, i));
					}
				}
			}
		}
	}
	vector <int> tmp;
	int all = ans.size();
	tmp.resize(all);
	for (all; all; --all) {
		tmp[all - 1] = ans.top().second;
		ans.pop();
	}
	return tmp;
}

void MainScene::DrawNode(Vec2 begin, Vec2 end)
{
	auto drawNode = DrawNode::create();
	addChild(drawNode);
	drawNode->drawRect(begin, end, Color4F::WHITE);
}

void MainScene::MonsterWillLoad(float dt)
{
	for (int i = 0; i < __monstermgr->Monster.size(); ++i) {
		auto mon = __monstermgr->Monster[i].first;
		if (mon->Is_Dead) {
			mon->removeFromParent();
			
		}
	}
	vector < pair<Entity*, MonsterController*> >::iterator itr = __monstermgr->Monster.begin();
	while (itr != __monstermgr->Monster.end()) {
		auto mon = (*itr).first;
		if (mon->Is_Dead) {
			itr = __monstermgr->Monster.erase(itr);
		}
		else {
			itr++;
		}
	}
	int num = __maxnum*2 - __monstermgr->Monster.size();
	for (int i = 0; i < num; ++i) {
		int ran = random() % __maxnum;
		int r = random() % MonsterList.size();
		LoadMonster(MonsterPos[ran], MonsterBegin[ran],MonsterEnd[ran], MonsterList[r].c_str());
	}
}

bool MainScene::IsIn(Vec2 s1, Vec2 e1, Vec2 s2, Vec2 e2)
{
	float x1 = (s1.x + e1.x) /2;
	float y1 = (s1.y + e1.y) / 2;
	float w1 = e1.x-s1.x;
	float h1 = e1.y - s1.y;
	float x2 = (s2.x + e2.x) / 2;
	float y2 = (s2.y + e2.y) / 2;
	float w2 = e2.x - s2.x;
	float h2 = e2.y - s2.y;

	if (x1 + w1*0.5<x2 - w2*0.5)
		return false;
	else if (x1 - w1*0.5>x2 + w2*0.5)
		return false;
	else if (y1 + h1*0.5<y2 - h2*0.5)
		return false;
	else if (y1 - h1*0.5>y2 + h2*0.5)
		return false;

	return true;
}

void MainScene::LoadMonsterList()
{
	Json::Reader reader;
	Json::Value root;
	string data = FileUtils::getInstance()->getStringFromFile(string("json/monsterlist.json"));
	if (reader.parse(data, root, false)) {
		auto list = root["name"];
		for (int i = 0; i < list.size(); ++i) {
			MonsterList.push_back(list[i].asString());
		}
	}
}

void MainScene::LoadUI()
{
	auto UI = GUIReader::getInstance()->
		widgetFromJsonFile("UI/NewUi_1.ExportJson");
	UIPos = UI->getPosition();
	this->addChild(UI, 4);
	HpBar = (LoadingBar*)Helper::seekWidgetByName(UI, "hpbar");
	Name = (Text*)Helper::seekWidgetByName(UI, "name");
	My_Level = (Text*)Helper::seekWidgetByName(UI, "level");
	auto name = string("戴志港");
	Name->setText(gb23122utf8(name.c_str()));
	Name->setColor(Color3B::WHITE);
	auto level = string("黄金会员");
	My_Level->setText(gb23122utf8(level.c_str()));
	My_Level->setColor(Color3B::WHITE);
}
const char * MainScene::gb23122utf8(const char * gb2312)
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
void MainScene::die(Ref * sender)
{
	_eventDispatcher->removeEventListener(this->keylistener);
}

//void MainScene::LoadBlood()
//{
/*	auto JumpBtn = (Button*)Helper::seekWidgetByName(UI, "JumpBtn");
	JumpBtn->addTouchEventListener(this, toucheventselector(TollgateScene::jumpEvent));
	UI = GUIReader::getInstance()->
		widgetFromJsonFile("b_1.ExportJson");
	this->addChild(UI, 1);
	m_score = (Text*)Helper::seekWidgetByName(UI, "Score");
	m_hpBar = (LoadingBar*)Helper::seekWidgetByName(UI, "Blood");*/
//}

