#ifndef __Props_Silver__
#define __Props_Silver__
#include "cocos2d.h"
#include "Props.hpp"
USING_NS_CC;
class PropsSilver:public Props{
public:
	void create(CCPoint a,float scale);
};

void PropsSilver::create(CCPoint a,float scale)
{
	isdelay = true;
	sprite = CCSprite::create("image/unit/item_Silver.png");
	sprite->setScale(scale);
	sprite->setPosition(a);
	type = 5;
	CCMoveBy *action1 = CCMoveBy::create(0.4,ccp(0,sprite->boundingBox().size.height/5));
	CCMoveBy *action2 = CCMoveBy::create(0.4,ccp(0,-sprite->boundingBox().size.height/5));
	sprite->runAction(CCRepeatForever::create(CCSequence::create(action1,action2,NULL)));
}
#endif
