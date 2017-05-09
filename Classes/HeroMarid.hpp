#ifndef __Hero_Marid__
#define __Hero_Marid__
#include "cocos2d.h"
#include "HeroClass.hpp"
USING_NS_CC;
class HeroMarid: public Hero{
public:
	void createhero(CCPoint a,float scale,int id,int type);
};

void HeroMarid::createhero(CCPoint a,float scale,int id,int type){
	speed = 5;
	direction = 5;
	bubble_range = 3;
	bubble_num = 3;
	sca = scale;
	islive = true;
	isfree = true;
	idx = id;
	sprite = CCSprite::create();
	sprite->setScale(scale);
	sprite->setPosition(a);
	if (type == 0)
		AniReader::read("unit_marid.ani");
	else
		AniReader::read("unit_luxmarid.ani");
	num = AniReader::num;
	for (int i = 0; i < 100; i++)
		for(int j = 0; j < 4; j++)
		{
			list[i][j] = AniReader::list[i][j];
		}
	if (type == 0)
		texture = CCTextureCache::sharedTextureCache()->addImage("image/unit/unit_marid.png");
	else
		texture = CCTextureCache::sharedTextureCache()->addImage("image/unit/unit_luxmarid.png");
	animate = CCRepeatForever::create(CCAnimate::create(moveanimation(4)));
	sprite->runAction(animate);
	action=NULL;
}

#endif
