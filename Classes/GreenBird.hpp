#ifndef __GREEN_BIRD__
#define __GREEN_BIRD__
#include "cocos2d.h"
#include "HeroClass.hpp"
USING_NS_CC;
class GreenBird:public CCNode,public Hero{
public:
	CCAnimation* moveanimation(int);
	GreenBird(CCPoint a,float);
	void moveto(CCPoint a);
	void clearMove();
	void stand();
	CCSprite *sprite;
	CCAction* action;
};
GreenBird::GreenBird(CCPoint a,float scale){
	speed = 5;
	direction = 5;
	sprite = CCSprite::create();
	sprite->setScale(scale);
	sprite->setPosition(a);
	AniReader::read("unit_bazzi.ani");
	num = AniReader::num;
	for (int i = 0; i < 100; i++)
		for(int j = 0; j < 4; j++)
		{
			list[i][j] = AniReader::list[i][j];
		}
	//sprite->setScale(scale);
	//CCLog("%d/n",num);
	/*texture = CCTextureCache::sharedTextureCache()->addImage("image/unit/green_bird.png");
	float w = texture->getContentSize().width / 2;
	float h = texture->getContentSize().height;
	animation = CCAnimation::create();
	animation->setDelayPerUnit(0.2f);
<<<<<<< HEAD
	for(int i = 0; i <2; i ++)
		animation->addSpriteFrameWithTexture(texture, CCRectMake(i * w, 0, w, h));
	animate = CCAnimate::create(animation);
	sprite->runAction(CCRepeatForever::create(animate));
	action=NULL;
=======
	animation->addSpriteFrameWithTexture(texture,CCRectMake(0,0,w,h));*/
	texture = CCTextureCache::sharedTextureCache()->addImage("image/unit/unit_bazzi.png");
	/*animation[0] = CCAnimation::create();           //up
	animation[0]->setDelayPerUnit(1.0/speed);
	for(int i = 0; i < 4; i ++)
		animation[0]->addSpriteFrameWithTexture(texture, CCRectMake(list[i+9][0],list[i+9][1],list[i+9][2],list[i+9][3]));

    animation[1] = CCAnimation::create();         //down
	animation[1]->setDelayPerUnit(1.0/speed);
	for(int i = 1; i < 4; i ++)
		animation[1]->addSpriteFrameWithTexture(texture, CCRectMake(list[i+13][0],list[i+13][1],list[i+13][2],list[i+13][3]));

	animation[2] = CCAnimation::create();          //left&right
		animation[2]->setDelayPerUnit(1.0/speed);
		for(int i = 2; i < 4; i ++)
			animation[2]->addSpriteFrameWithTexture(texture, CCRectMake(list[i+17][0],list[i+17][1],list[i+17][2],list[i+17][3]));

	animation[3] = CCAnimation::create();           //stop-up
	animation[3]->setDelayPerUnit(1.0);
	animation[3]->addSpriteFrameWithTexture(texture, CCRectMake(list[36][0],list[36][1],list[36][2],list[36][3]));

	animation[4] = CCAnimation::create();           //stop-down
	animation[4]->setDelayPerUnit(1.0);
	animation[4]->addSpriteFrameWithTexture(texture, CCRectMake(list[40][0],list[40][1],list[40][2],list[40][3]));

	animation[5] = CCAnimation::create();           //stop-left&right
	animation[5]->setDelayPerUnit(1.0);
	animation[5]->addSpriteFrameWithTexture(texture, CCRectMake(list[44][0],list[44][1],list[44][2],list[44][3]));*/

	animate = CCRepeatForever::create(CCAnimate::create(moveanimation(4)));
	sprite->runAction(animate);
	action=NULL;
}
CCAnimation* GreenBird::moveanimation(int dir)
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
void GreenBird::moveto(CCPoint ptNode){
	if(action!=NULL)return;
	//sprite->setFlipX(ptNode.x>sprite->getPosition().x);
	CCMoveTo *move = CCMoveTo::create(ccpDistance(sprite->getPosition(),ptNode)/200, ptNode);
	CCFiniteTimeAction *clearmove=CCCallFuncN::create(this,callfuncN_selector(GreenBird::clearMove));
	action=CCSequence::create(move,clearmove,NULL);
	sprite->runAction(action);
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
	//animate->release();
	//animate = CCAnimate::create(animation[direction]);
	//animate->setAnimation(animation[direction]);
	/*animate->stop();
	animate->setAnimation(animation[direction]);
	sprite->stopAction(animate);
	animate = CCAnimate::create(animation[direction]);
	sprite->runAction(animate);*/
}
void GreenBird::clearMove(){
	sprite->stopAction(action);
	action=NULL;
	/*sprite->stopAction(animate);
	animate = CCRepeatForever::create(CCAnimate::create(moveanimation(direction+3)));
	sprite->runAction(animate);*/

}
void GreenBird::stand() {
	if (direction == 5) return;
	sprite->stopAction(animate);
	animate = CCRepeatForever::create(CCAnimate::create(moveanimation(direction+3)));
	sprite->runAction(animate);
	direction = 5;
}

#endif
