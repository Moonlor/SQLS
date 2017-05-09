#ifndef __Obstacle_CLASS__
#define __Obstacle_CLASS__
#include "cocos2d.h"
#include "AniReader.hpp"
USING_NS_CC;
class Obstacle:public CCNode{
public:
	CCTexture2D* texture;
	CCAnimation* animation;
	CCAction* action;
	CCActionInterval* animate;
	CCSprite *sprite;
	int type;
	int num;
	int list[100][4];
	virtual ~Obstacle() {}
	virtual void create(CCPoint a,float scale) {}
	void moveto(CCPoint a);
	void clearMove();
	void stand();
	void destory();
};
#endif
