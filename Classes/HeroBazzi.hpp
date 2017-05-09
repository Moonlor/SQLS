#ifndef __Hero_Bazzi__
#define __Hero_Bazzi__
#include "cocos2d.h"
#include "HeroClass.hpp"
USING_NS_CC;
class HeroBazzi: public Hero{
public:
	void createhero(CCPoint a,float scale,int id,int type);
};

void HeroBazzi::createhero(CCPoint a,float scale,int id,int type)
{
	speed = 5;
	direction = 5;
	bubble_range = 3;
	bubble_num = 3;
	sca = scale;
	idx = id;
	islive = true;
	isfree = true;
	sprite = CCSprite::create();
	sprite->setScale(scale);
	sprite->setPosition(a);
	if (type == 0)
		AniReader::read("unit_bazzi.ani");
	else
		AniReader::read("unit_luxbazzi.ani");
	num = AniReader::num;
	for (int i = 0; i < 100; i++)
		for(int j = 0; j < 4; j++)
		{
			list[i][j] = AniReader::list[i][j];
		}
	if  (type == 0)
		texture = CCTextureCache::sharedTextureCache()->addImage("image/unit/unit_bazzi.png");
	else
		texture = CCTextureCache::sharedTextureCache()->addImage("image/unit/unit_luxbazzi.png");
	animate = CCRepeatForever::create(CCAnimate::create(moveanimation(4)));
	sprite->runAction(animate);
	action=NULL;
}


#endif
