#ifndef __Props_Gold__
#define __Props_Gold__
#include "cocos2d.h"
#include "Props.hpp"
USING_NS_CC;
class PropsGold:public Props{
public:
	void create(CCPoint a,float scale);
};

void PropsGold::create(CCPoint a,float scale)
{
	isdelay = true;
	sprite = CCSprite::create("image/unit/item_Gold.png");
	sprite->setScale(scale);
	sprite->setPosition(a);
	type = 4;
	CCMoveBy *action1 = CCMoveBy::create(0.4,ccp(0,sprite->boundingBox().size.height/5));
	CCMoveBy *action2 = CCMoveBy::create(0.4,ccp(0,-sprite->boundingBox().size.height/5));
	sprite->runAction(CCRepeatForever::create(CCSequence::create(action1,action2,NULL)));
}
#endif
