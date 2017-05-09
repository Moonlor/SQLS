#ifndef __TITLE_SCENE_MESSAGE_LAYER__
#define __TITLE_SCENE_MESSAGE_LAYER__
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace CocosDenshion;
class TitleSceneMessageLayer : public CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    // a selector callback
    // implement the "static node()" method manually
    CREATE_FUNC(TitleSceneMessageLayer);
	CCSize size;
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    virtual void ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent);
    virtual void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);
    virtual void ccTouchCancelled(CCTouch* pTouch, CCEvent* pEvent);
    virtual void registerWithTouchDispatcher();
};
// onserverField "init" you need to initialize your instance
bool TitleSceneMessageLayer::init()
{
	if (!CCLayer::init()){
		return false;
	}
	size = CCDirector::sharedDirector()->getWinSize();
	SimpleAudioEngine::sharedEngine()->preloadEffect("audio/ef_13.ogg");
	SimpleAudioEngine::sharedEngine()->playEffect("audio/ef_13.ogg");
	CCSprite *background=CCSprite::create("image/ui/black_blank.png");
	background->setScaleX(size.width/background->getContentSize().width);
	background->setScaleY(size.height/background->getContentSize().height);
	background->setPosition(ccp(size.width/2,size.height/2));
	this->addChild(background,0);
	this->setTouchEnabled(true);
	CCFadeIn *fadein=CCFadeIn::create(1);
	this->runAction(fadein);
	return true;
}
bool TitleSceneMessageLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
	this->setTouchEnabled(false);
	this->removeFromParentAndCleanup(true);
	return true;
}
void TitleSceneMessageLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){
}
void TitleSceneMessageLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
}
void TitleSceneMessageLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
}
void TitleSceneMessageLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    CCLayer::registerWithTouchDispatcher();
}
#endif
