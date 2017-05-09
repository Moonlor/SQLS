#ifndef __BUBBLE_CLASS
#define __BUBBLE_CLASS
#include "cocos2d.h"
USING_NS_CC;
using namespace CocosDenshion;
class Bubble : public CCNode{
public:
	Bubble(CCPoint a,float scale,int id,int r,int he);
	CCSprite *sprite;
	CCSprite *ups[10];
	CCSprite *downs[10];
	CCSprite *lefts[10];
	CCSprite *rights[10];
	CCTexture2D *texture;
	CCAnimation *animation;
	CCActionInterval* animate;
	CCLayer *layer;
	CCAction *action;
	CCPoint position;
	int idx;
	int num;
	int list[100][4];
	int range;
	int r[4];
	int heroid;
	bool isdelay;
	void bomb(int up,int down,int left, int right);
	CCAnimation* waitanimation();
	CCAnimation* edgeanimation(int dir);
	CCAnimation* midanimation(int dir);
	CCAnimation* centeranimation();
	void test();
	void remove();
};

CCAnimation* Bubble::edgeanimation(int dir)
{
	animation = CCAnimation::create();
	animation->setDelayPerUnit(0.1f);
	if (dir == 0)
	{
		for(int i = 8; i <= 11; i++)
			animation->addSpriteFrameWithTexture(texture,CCRectMake(list[i][0],list[i][1],list[i][2],list[i][3]));
	} else
	{
		for(int i = 16; i <= 19; i++)
			animation->addSpriteFrameWithTexture(texture,CCRectMake(list[i][0],list[i][1],list[i][2],list[i][3]));
	}
	return animation;
}

CCAnimation* Bubble::midanimation(int dir)
{
	animation = CCAnimation::create();
	animation->setDelayPerUnit(1.0f);
    if (dir == 0)
    {
    	animation->addSpriteFrameWithTexture(texture,CCRectMake(list[12][0],list[12][1],list[12][2],list[12][3]));
    } else
    {
    	animation->addSpriteFrameWithTexture(texture,CCRectMake(list[20][0],list[20][1],list[20][2],list[20][3]));
    }
	return animation;
}

CCAnimation* Bubble::centeranimation()
{
	animation = CCAnimation::create();
	animation->setDelayPerUnit(0.1f);
	for(int i = 0; i < 3; i++)
		animation->addSpriteFrameWithTexture(texture,CCRectMake(list[i+5][0],list[i+5][1],list[i+5][2],list[i+5][3]));
	animation->addSpriteFrameWithTexture(texture,CCRectMake(list[7][0],list[7][1],list[7][2],list[7][3]));
	return animation;
}
CCAnimation* Bubble::waitanimation()
{
	animation = CCAnimation::create();
	animation->setDelayPerUnit(0.2f);
	for(int i = 1; i <= 4; i++)
		animation->addSpriteFrameWithTexture(texture,CCRectMake(list[i][0],list[i][1],list[i][2],list[i][3]));
	return animation;

}
Bubble::Bubble(CCPoint a,float scale,int id,int r,int he){

	sprite=CCSprite::create();
	layer=CCLayer::create();
	sprite->setPosition(a);
	sprite->setScale(scale);
	idx = id;
	range = r;
	heroid = he;
	position = a;
	isdelay = true;
	AniReader::read("unit_bombwater.ani");
	num = AniReader::num;
	for (int i = 0; i < 100; i++)
		for(int j = 0; j < 4; j++)
		{
			list[i][j] = AniReader::list[i][j];
		}
	texture = CCTextureCache::sharedTextureCache()->addImage("image/unit/unit_bombwater.png");
	animate = CCRepeatForever::create(CCAnimate::create(waitanimation()));
	sprite->runAction(animate);
	layer->addChild(sprite,10);
	int i;
	float x,y,w,h;
	x = sprite->getPositionX();
	y = sprite->getPositionY();
	w = AniReader::mapw;
	h = AniReader::maph;
	for (i = 0; i < range; i++)
	{
		ups[i] = CCSprite::create();
		ups[i]->setPositionX(x);
		ups[i]->setPositionY(y+(i+1)*h);
		ups[i]->setScale(scale);
		layer->addChild(ups[i],10);
	}
	for (i = 0; i < range; i++)
	{
		downs[i] = CCSprite::create();
		downs[i]->setPositionX(x);
		downs[i]->setPositionY(y-(i+1)*h);
		downs[i]->setScale(scale);
		downs[i]->setFlipY(true);
		layer->addChild(downs[i],10);
	}
	for (i = 0; i < range; i++)
	{
		lefts[i] = CCSprite::create();
		lefts[i]->setPositionX(x-(i+1)*w);
		lefts[i]->setPositionY(y);
		lefts[i]->setScale(scale);
		layer->addChild(lefts[i],10);
	}
	for (i = 0; i < range; i++)
	{
		rights[i] = CCSprite::create();
		rights[i]->setPositionX(x+(i+1)*w);
		rights[i]->setPositionY(y);
		rights[i]->setScale(scale);
		rights[i]->setFlipX(true);
		layer->addChild(rights[i],10);
	}

	/*float w = texture->getContentSize().width / 2;
	float h = texture->getContentSize().height;

	animation->setDelayPerUnit(0.2f);
	for(int i = 0; i <2; i ++)
		animation->addSpriteFrameWithTexture(texture, CCRectMake(i * w, 0, w, h));
	CCAnimate *animate = CCAnimate::create(animation);
	sprite->runAction(CCRepeatForever::create(animate));*/
}
void Bubble::test()
{
	bomb(3,3,3,3);
}

void Bubble::remove()
{
	layer->getParent()->removeChild(layer);
}
void Bubble::bomb(int up,int down,int left, int right)
{
	r[0] = up;
	r[1] = down;
	r[2] = left;
	r[3] = right;
	isdelay = false;
	sprite->stopAction(animate);
	animate = CCAnimate::create(centeranimation());
	CCFiniteTimeAction* destroy = CCCallFuncN::create(this,callfuncN_selector(Bubble::remove));
	action = CCSequence::create(animate,destroy);
	sprite->runAction(action);
	int i;
	for (i = 0; i < up; i++)
	{
		CCActionInterval* a;
		if (i < up-1)
			a = CCAnimate::create(midanimation(0));
		else
			a = CCAnimate::create(edgeanimation(0));
		ups[i]->runAction(a);
	}
	for (i = 0; i < down; i++)
		{
			CCActionInterval* a;
			if (i < down-1)
				a = CCAnimate::create(midanimation(0));
			else
				a = CCAnimate::create(edgeanimation(0));
			downs[i]->runAction(a);
		}
	for (i = 0; i < left; i++)
		{
			CCActionInterval* a;
			if (i < left-1)
				a = CCAnimate::create(midanimation(1));
			else
				a = CCAnimate::create(edgeanimation(1));
			lefts[i]->runAction(a);
		}
	for (i = 0; i < right; i++)
		{
			CCActionInterval* a;
			if (i < right-1)
				a = CCAnimate::create(midanimation(1));
			else
				a = CCAnimate::create(edgeanimation(1));
			rights[i]->runAction(a);
		}
	//animat
}
#endif
