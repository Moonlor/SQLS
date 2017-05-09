#ifndef __GAME_SCENE_MESSAGE_LAYER__
#define __GAME_SCENE_MESSAGE_LAYER__
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "MainUIScene.hpp"
USING_NS_CC;
using namespace CocosDenshion;
class GameSceneMessageLayer : public CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    bool init();
    // a selector callback
    // implement the "static node()" method manually
    CREATE_FUNC(GameSceneMessageLayer);
	CCSize size;
	void setmessage(int type);
    bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    void ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent);
    void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);
    void ccTouchCancelled(CCTouch* pTouch, CCEvent* pEvent);
    void registerWithTouchDispatcher();
    void enableTouch();
    void showResult();
};
// onserverField "init" you need to initialize your instance
bool GameSceneMessageLayer::init()
{
	if (!CCLayer::init()){
		return false;
	}
	size = CCDirector::sharedDirector()->getWinSize();

	CCSprite *background=CCSprite::create("image/ui/black_blank.png");
	background->setScaleX(size.width/background->getContentSize().width);
	background->setScaleY(size.height/background->getContentSize().height);
	background->setPosition(ccp(size.width/2,size.height/2));
	this->addChild(background,0);
	CCFiniteTimeAction *delay=CCDelayTime::create(2);
	CCFiniteTimeAction *enableTouch=CCCallFuncN::create(this,callfuncN_selector(GameSceneMessageLayer::enableTouch));
	CCAction *action=CCSequence::create(delay,enableTouch,NULL);
	this->runAction(action);
	return true;
}

void GameSceneMessageLayer::setmessage(int type)
{
	if (type == 0)
	{
		CCLabelTTF *implentmenting=CCLabelTTF::create("You Lose!","fonts/FZKaTong-M19T.ttf",25);
		implentmenting->setPosition(ccp(size.width/2,size.height/2));
		this->addChild(implentmenting);
	} else
	{
		CCLabelTTF *implentmenting=CCLabelTTF::create("You Win!","fonts/FZKaTong-M19T.ttf",25);
		implentmenting->setPosition(ccp(size.width/2,size.height/2));
		this->addChild(implentmenting);
	}
}
void GameSceneMessageLayer::enableTouch(){
	this->setTouchEnabled(true);
}
bool GameSceneMessageLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
	UserData::current->clearRoom();
	MainUIScene::ready=false;
	this->setTouchEnabled(false);
	CCScene *pScene = MainUIScene::scene();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFlipY::create(0.5f, pScene));
	return true;
}
void GameSceneMessageLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){
}
void GameSceneMessageLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
}
void GameSceneMessageLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
}
void GameSceneMessageLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    CCLayer::registerWithTouchDispatcher();
}
void GameSceneMessageLayer::showResult(){

}
#endif
