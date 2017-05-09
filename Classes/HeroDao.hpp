#ifndef __Hero_Dao__
#define __Hero_Dao__
#include "cocos2d.h"
#include "HeroClass.hpp"
USING_NS_CC;
class HeroDao: public Hero{
public:
	void createhero(CCPoint a,float scale,int id,int type);
};

void HeroDao::createhero(CCPoint a,float scale,int id,int type){
	speed = 5;
	direction = 5;
	bubble_range = 3;
	sca = scale;
	bubble_num = 3;
	islive = true;
	isfree = true;
	idx = id;
	sprite = CCSprite::create();
	sprite->setScale(scale);
	sprite->setPosition(a);
	if (type == 0)
		AniReader::read("unit_dao.ani");
	else
		AniReader::read("unit_luxdao.ani");
	num = AniReader::num;
	for (int i = 0; i < 100; i++)
		for(int j = 0; j < 4; j++)
		{
			list[i][j] = AniReader::list[i][j];
		}
	if (type == 0)
		texture = CCTextureCache::sharedTextureCache()->addImage("image/unit/unit_dao.png");
	else
		texture = CCTextureCache::sharedTextureCache()->addImage("image/unit/unit_luxdao.png");
	animate = CCRepeatForever::create(CCAnimate::create(moveanimation(4)));
	sprite->runAction(animate);
	action=NULL;
}


#endif
