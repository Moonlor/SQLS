#ifndef __GAME_SCENE_STATUS_LAYER__
#define __GAME_SCENE_STATUS_LAYER__
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace CocosDenshion;
class GameSceneStatusLayer : public CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    virtual void ccTouchesBegan(CCSet* touches, CCEvent* pEvent);
    virtual void ccTouchesMoved(CCSet* touches, CCEvent* pEvent);
    virtual void ccTouchesEnded(CCSet* touches, CCEvent* pEvent);
    virtual void ccTouchesCancelled(CCSet* touches, CCEvent* pEvent);
    virtual void registerWithTouchDispatcher();
    void showAllStatus();
    // a selector callback
    // implement the "static node()" method manually
    CREATE_FUNC(GameSceneStatusLayer);
	CCSize size;
};
// onserverField "init" you need to initialize your instance
bool GameSceneStatusLayer::init()
{
	if (!CCLayer::init()){
		return false;
	}
	//目标：三指上划移回此层
	//显示所有人的状态
	size = CCDirector::sharedDirector()->getWinSize();
	CCLabelTTF *implentmenting=CCLabelTTF::create("Status施工中","fonts/FZKaTong-M19T.ttf",25);
	implentmenting->setPosition(ccp(size.width/2,size.height/2));
	this->addChild(implentmenting);
	CCSprite *background=CCSprite::create("image/ui/black_blank.png");
	background->setScaleX(size.width/background->getContentSize().width);
	background->setScaleY(size.height/background->getContentSize().height);
	background->setPosition(ccp(size.width/2,size.height/2));
	this->addChild(background,0);
	return true;
}
void GameSceneStatusLayer::ccTouchesBegan(CCSet* touches, CCEvent* pEvent){

}
void GameSceneStatusLayer::ccTouchesMoved(CCSet* touches, CCEvent* pEvent){

}
void GameSceneStatusLayer::ccTouchesEnded(CCSet* touches, CCEvent* pEvent){

}
void GameSceneStatusLayer::ccTouchesCancelled(CCSet* touches, CCEvent* pEvent){

}
void GameSceneStatusLayer::registerWithTouchDispatcher(){

}
void GameSceneStatusLayer::showAllStatus(){

}
#endif
