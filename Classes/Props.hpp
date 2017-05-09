#ifndef __Props_CLASS__
#define __Props_CLASS__
#include "cocos2d.h"
USING_NS_CC;
class Props:public CCNode{
public:
	CCSprite *sprite;
	int type;
	virtual ~Props() {}
	virtual void create(CCPoint a,float scale) {}
	void remove();
	bool isdelay;
};

void Props::remove()
{
	isdelay = false;
	sprite->getParent()->removeChild(sprite);
}
#endif
