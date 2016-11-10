#include "cocos2d.h"

enum DIR_DEF{
	UP = 1,
	DOWN,
	LEFT,
	RIGHT
};

class SnakeNode : public cocos2d::Ref{
public:
	int col, row;
	int dir;
	SnakeNode* Prenode;
	cocos2d::Sprite* nodeSprite;
};

class GameScene :public cocos2d::Layer{
public:
	static cocos2d::Scene* createscene();
	bool init();
	void update(float dt);
	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);

	CREATE_FUNC(GameScene);
protected:
	SnakeNode* head;
	SnakeNode* food;
	cocos2d::Vector<SnakeNode*> body;
};

