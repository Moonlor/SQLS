

#ifndef _AudioControlScene_h_
#define _AudioControlScene_h_

#include "SimpleAudioEngine.h"
#include "cocos2d.h"
#include "MenuScene.h"

class AudioControl : public cocos2d::Layer
{
public:
    //创建场景
    static cocos2d::Scene* createScene();
    //创建自定义的init函数，当成功创建场景的时候返回true
    virtual bool init();
    
    CREATE_FUNC(AudioControl);
};


#endif /* _AudioControlScene_h_ */
