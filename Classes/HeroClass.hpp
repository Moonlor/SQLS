#ifndef __HERO_CLASS__
#define __HERO_CLASS__
#include "cocos2d.h"
#include "AniReader.hpp"
USING_NS_CC;
class Hero:public CCNode{
public:
	CCTexture2D* texture;
	CCAnimation* animation;
	CCAction* action;
	CCActionInterval* animate;
	CCSprite *sprite;
	bool isfree;
	bool islive;
	float speed;
	int bubble_num;
	int bubble_range;
	int teamid;
	int num;
	float sca;
	int idx;
	int direction;
	int list[100][4];
	virtual ~Hero() {}
	virtual void createhero(CCPoint a,float scale,int id,int type) {}
	CCAnimation* moveanimation(int);
	CCAnimation* liveanimation();
	CCAnimation* dieanimation();
	CCAnimation* encaseanimation();
	void moveto(CCPoint a);
	void clearMove();
	void stand();
	void remove();
	void encase();	//包住动画
	void die();		//挂掉动画
	void live();  //解救动画
};

CCAnimation* Hero::moveanimation(int dir)
{
	animation = CCAnimation::create();
	if (dir < 3)
	{
		animation->setDelayPerUnit(1.0/speed);
		for(int i = dir * 4; i < dir * 4 + 4; i ++)
			animation->addSpriteFrameWithTexture(texture, CCRectMake(list[i+9][0],list[i+9][1],list[i+9][2],list[i+9][3]));
	} else {
		animation->setDelayPerUnit(3.0);
		animation->addSpriteFrameWithTexture(texture, CCRectMake(list[36+(dir-3)*4][0],list[36+(dir-3)*4][1],list[36+(dir-3)*4][2],list[36+(dir-3)*4][3]));
	}
	return animation;
}

CCAnimation* Hero::encaseanimation()
{
	animation = CCAnimation::create();
	animation->setDelayPerUnit(0.5f);
	animation->addSpriteFrameWithTexture(texture, CCRectMake(list[21][0],list[21][1],list[21][2],list[21][3]));
	animation->addSpriteFrameWithTexture(texture, CCRectMake(list[22][0],list[22][1],list[22][2],list[22][3]));
	animation->addSpriteFrameWithTexture(texture, CCRectMake(list[21][0],list[21][1],list[21][2],list[21][3]));
	animation->addSpriteFrameWithTexture(texture, CCRectMake(list[22][0],list[22][1],list[22][2],list[22][3]));
	animation->addSpriteFrameWithTexture(texture, CCRectMake(list[25][0],list[25][1],list[25][2],list[25][3]));
	animation->addSpriteFrameWithTexture(texture, CCRectMake(list[26][0],list[26][1],list[26][2],list[26][3]));
	return animation;
}

CCAnimation* Hero::dieanimation()
{
	animation = CCAnimation::create();
	animation->setDelayPerUnit(0.4f);
	animation->addSpriteFrameWithTexture(texture, CCRectMake(list[31][0],list[31][1],list[31][2],list[31][3]));
	animation->addSpriteFrameWithTexture(texture, CCRectMake(list[34][0],list[34][1],list[34][2],list[34][3]));
	animation->addSpriteFrameWithTexture(texture, CCRectMake(list[35][0],list[35][1],list[35][2],list[35][3]));
	return animation;

}

CCAnimation* Hero::liveanimation()
{
	animation = CCAnimation::create();
	animation->setDelayPerUnit(0.5f);
	animation->addSpriteFrameWithTexture(texture, CCRectMake(list[27][0],list[27][1],list[27][2],list[27][3]));
	animation->addSpriteFrameWithTexture(texture, CCRectMake(list[40][0],list[40][1],list[40][2],list[40][3]));
	return animation;

}

void Hero::moveto(CCPoint ptNode){
	if(action!=NULL)return;
	CCMoveTo *move = CCMoveTo::create(ccpDistance(sprite->getPosition(),ptNode)/(speed*40)/sca, ptNode);
	CCFiniteTimeAction *clearmove=CCCallFuncN::create(this,callfuncN_selector(Hero::clearMove));
	action=CCSequence::create(move,clearmove,NULL);
	sprite->runAction(action);
	if(ptNode.x==sprite->getPosition().x and ptNode.y == sprite->getPosition().y)return;
	sprite->setFlipX(ptNode.x<sprite->getPosition().x);
	bool t = true;
	if (ptNode.y < sprite->getPosition().y)
	{
		if (direction == 1)
			t = false;
		direction = 1;
	} else if (ptNode.y > sprite->getPosition().y)
	{
		if (direction == 0)
			t = false;
		direction = 0;
	} else
	{
		if (direction == 2)
			t = false;
		direction = 2;
	}
	if (t)
	{
		sprite->stopAction(animate);
		animate = CCRepeatForever::create(CCAnimate::create(moveanimation(direction)));
		sprite->runAction(animate);
	}

}

void Hero::clearMove(){
	sprite->stopAction(action);
	action=NULL;
}

void Hero::stand() {
	if (direction == 5) return;
	sprite->stopAction(animate);
	animate = CCRepeatForever::create(CCAnimate::create(moveanimation(direction+3)));
	sprite->runAction(animate);
	direction = 5;
}

void Hero::encase() {
	isfree = false;
	sprite->stopAction(animate);
	animate = CCAnimate::create(encaseanimation());
	sprite->runAction(animate);

}

void Hero::die() {
	islive = false;
	sprite->stopAction(animate);
	animate = CCAnimate::create(dieanimation());
	CCFiniteTimeAction* destroy = CCCallFuncN::create(this,callfuncN_selector(Hero::remove));
	CCAction * a = CCSequence::create(animate,destroy);
	sprite->runAction(a);
	//sprite->runAction(animate);
}
void Hero::remove() {
	sprite->getParent()->removeChild(sprite);
}
void Hero::live() {
	isfree = true;
	sprite->stopAction(animate);
	animate = CCAnimate::create(liveanimation());
	sprite->runAction(animate);
}
#endif
