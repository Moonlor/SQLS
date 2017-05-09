#ifndef __Props_Range__
#define __Props_Range__
#include "cocos2d.h"
#include "Props.hpp"
USING_NS_CC;
class PropsRange:public Props{
public:
	void create(CCPoint a,float scale);
};

void PropsRange::create(CCPoint a,float scale)
{
	isdelay = true;
	sprite = CCSprite::create("image/unit/item_Range.png");
	sprite->setScale(scale);
	sprite->setPosition(a);
	type = 2;
	CCMoveBy *action1 = CCMoveBy::create(0.4,ccp(0,sprite->boundingBox().size.height/5));
	CCMoveBy *action2 = CCMoveBy::create(0.4,ccp(0,-sprite->boundingBox().size.height/5));
	sprite->runAction(CCRepeatForever::create(CCSequence::create(action1,action2,NULL)));
}

#endif
