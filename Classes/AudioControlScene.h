

#ifndef _AudioControlScene_h_
#define _AudioControlScene_h_

#include "SimpleAudioEngine.h"
#include "cocos2d.h"
#include "MenuScene.h"

class AudioControl : public cocos2d::Layer
{
public:
	//��������
	static cocos2d::Scene* createScene();
	//�����Զ����init���������ɹ�����������ʱ�򷵻�true
	virtual bool init();

	CREATE_FUNC(AudioControl);
};


#endif /* _AudioControlScene_h_ */
