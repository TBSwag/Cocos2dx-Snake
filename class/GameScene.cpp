#include "GameScene.h"
#include "cocos2d.h"
#include "ui\CocosGUI.h"

USING_NS_CC;

Scene* GameScene::createscene(){
	auto scene = Scene::create();
	auto layer = GameScene::create();

	scene->addChild(layer);

	return scene;

}

bool GameScene::init(){
	if (!Layer::init()){
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();

	head = new SnakeNode();
	head->col = rand() % 32;
	head->row = rand() % 24;

	food = new SnakeNode();
	food->col = rand() % 32;
	food->row = rand() % 24;

	head->nodeSprite = Sprite::create("head_d.png");
	head->nodeSprite->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
	head->nodeSprite->setPosition(Vec2(head->col * 32, head->row * 32));
	this->addChild(head->nodeSprite,0);

	food->nodeSprite = Sprite::create("apple.png");
	food->nodeSprite->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
	food->nodeSprite->setPosition(Vec2(food->col * 32, food->row * 32));
	this->addChild(food->nodeSprite, 0);

	auto touchlistener = EventListenerTouchOneByOne::create();
	touchlistener->setSwallowTouches(true);
	touchlistener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchlistener, this);

	this->schedule(schedule_selector(GameScene::update), 0.2);

	return true;
}

bool GameScene::onTouchBegan(Touch* touch, Event* event){
	Point touchPoint = touch->getLocation();

	int touchCol = (int)touchPoint.x / 32;
	int touchRow = (int)touchPoint.y / 32;
    //
	if (abs(touchCol - head->col) > abs(touchRow - head->row)){
		if (touchCol > head->col)
		{
			if (head->dir != RIGHT && head->dir != LEFT)
			     head->dir = DIR_DEF::RIGHT;
		}
		else
		{
			if (head->dir != RIGHT && head->dir != LEFT)
			     head->dir = DIR_DEF::LEFT;
		}
	}
	else
	{
		if (touchRow > head->row){
			if (head->dir != UP && head->dir != DOWN)
			    head->dir = DIR_DEF::UP;
		}
		else{
			if (head->dir != UP && head->dir != DOWN)
			    head->dir = DIR_DEF::DOWN;
		}
	}


	return true;
}

void GameScene::update(float dt){
	for (int i = body.size() - 1; i >= 0; i--){
		auto sn = body.at(i);
		if (body.empty()){
			sn->col = head->col;
			sn->row = head->row;
			sn->dir = head->dir;
		}
		else{
			sn->col = sn->Prenode->col;
			sn->row = sn->Prenode->row;
			sn->dir = sn->Prenode->dir;
		}
		sn->nodeSprite->setPosition(Point(sn->col * 32, sn->row * 32));
	}

	switch (head->dir){
	case DIR_DEF::UP:
		head->row++;
		head->nodeSprite->setAnchorPoint(Point::ANCHOR_TOP_RIGHT);
		head->nodeSprite->setRotation(-180);
		if (head->row >= 24){
			head->row = 0;
		}
		break;
	case DIR_DEF::DOWN :
		head->row--;
		head->nodeSprite->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
		head->nodeSprite->setRotation(0);
		if (head->row <= 0){
			head->row = 24;
		}
		break;
	case DIR_DEF::LEFT:
		head->col--;
		head->nodeSprite->setAnchorPoint(Point::ANCHOR_BOTTOM_RIGHT);
		head->nodeSprite->setRotation(90);
		if (head->col <= 0){
			head->col = 32;
		}
		break;
	case DIR_DEF::RIGHT:
		head->col++;
		head->nodeSprite->setRotation(-90);
		head->nodeSprite->setAnchorPoint(Point::ANCHOR_TOP_LEFT);
		if (head->col >= 32){
			head->col = 0;
		}
		break;
	}

	head->nodeSprite->setPosition(Point(head->col * 32, head->row * 32));

	if (head->col == food->col && head->row == food->row){
		food->col = rand() % 32;
		food->row = rand() % 24;
		food->nodeSprite->setPosition(Point(food->col * 32, food->row * 32));

		auto sn = new SnakeNode();
		if (body.size() > 0){
			sn->Prenode = body.back();
		}
		else{
			sn->Prenode = head;
		}

		switch (sn->Prenode->dir){
		case UP:
			sn->row = sn->Prenode->row - 1;
			sn->col = sn->Prenode->col;
			break;
		case DOWN:
			sn->row = sn->Prenode->row + 1;
			sn->col = sn->Prenode->col;
			break;
		case LEFT:
			sn->row = sn->Prenode->row;
			sn->col = sn->Prenode->col + 1;
			break;
		case RIGHT:
			sn->row = sn->Prenode->row;
			sn->col = sn->Prenode->col - 1;
			break;
		}
		sn->nodeSprite = Sprite::create("body.png");
		sn->nodeSprite->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
		sn->nodeSprite->setPosition(Point(sn->col * 32, sn->row * 32));
		this->addChild(sn->nodeSprite);

		body.pushBack(sn);


	}
	
	for (auto i = body.size() - 1; i >= 0; i--){
		auto sn = body.at(i);
		if (head->col == sn->col && head->row == sn->row){
			Director::getInstance()->pause();
			Size visiblesize = Director::getInstance()->getVisibleSize();
			auto gameover = Sprite::create("gameover.png");
			gameover->setPosition(visiblesize.width / 2, visiblesize.height / 2);
			this->addChild(gameover);
		}
	}

	  

	
}
