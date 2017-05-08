//
//  MenuScene.h
//  Airfight
//
//  Created by 李坤 on 2017/4/15.
//
//

#ifndef _MenuScene_h_
#define _MenuScene_h_

#include "cocos2d.h"
#include "GameScene.h"
#include "AudioControlScene.h"

class GameMenu : public cocos2d::Layer
{
public:
	//创建场景
	static cocos2d::Scene* createScene();
	//创建自定义的init函数，当成功创建场景的时候返回true
	virtual bool init();

	CREATE_FUNC(GameMenu);
};


#endif /* _MenuScene_h_ */
