//
//  MenuScene.h
//  Airfight
//
//  Created by ���� on 2017/4/15.
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
	//��������
	static cocos2d::Scene* createScene();
	//�����Զ����init���������ɹ�����������ʱ�򷵻�true
	virtual bool init();

	CREATE_FUNC(GameMenu);
};


#endif /* _MenuScene_h_ */
