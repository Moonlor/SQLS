#ifndef __TITLE_SCENE__
#define __TITLE_SCENE__
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
class TitleScene : public CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static CCScene* scene();
    // a selector callback

    // implement the "static node()" method manually
    CREATE_FUNC(TitleScene);
    void showBackground();
    void showTitle();
    void showLoginLayer();
	CCSize size;
	CCSprite *bomb,*title,*touch;
	static CCScene *titleScene;
	void enableTouch();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    virtual void ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent);
    virtual void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);
    virtual void ccTouchCancelled(CCTouch* pTouch, CCEvent* pEvent);
    virtual void registerWithTouchDispatcher();
};

#include "TitleSceneLoginLayer.hpp"
CCScene *TitleScene::titleScene;
CCScene* TitleScene::scene()
{
	// 'scene' is an autorelease object
	titleScene = CCScene::create();

	// 'layer' is an autorelease object
	TitleScene *layer = TitleScene::create();
	// add layer as a child to scene
	titleScene->addChild(layer,0);

	// return the scene
	return titleScene;
}

// on "init" you need to initialize your instance
bool TitleScene::init()
{
	if ( !CCLayer::init() )
	{
		return false;
	}
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("audio/bg_0.ogg");
	SimpleAudioEngine::sharedEngine()->preloadEffect("audio/ef_11.ogg");
	size = CCDirector::sharedDirector()->getWinSize();
	bomb=CCSprite::create("image/ui/bomb.png");
	bomb->setScale(size.height/4/bomb->getContentSize().height);
	CCFiniteTimeAction* place=CCPlace::create(ccp(size.width/2,size.height+bomb->getContentSize().height));
	CCAction *movein=CCEaseExponentialOut::create(CCMoveTo::create(2,ccp(size.width/2,size.height/4)));
	CCAction *moveback=CCEaseExponentialOut::create(CCMoveTo::create(2,ccp(size.width/2,size.height/2)));
	CCFiniteTimeAction *showbackground=CCCallFuncN::create(this,callfuncN_selector(TitleScene::showBackground));
	CCFiniteTimeAction *showtitle=CCCallFuncN::create(this,callfuncN_selector(TitleScene::showTitle));
	CCFiniteTimeAction *enabletouch=CCCallFuncN::create(this,callfuncN_selector(TitleScene::enableTouch));
	CCAction *action=CCSequence::create(place,movein,showbackground,moveback,showtitle,enabletouch,NULL);
	this->addChild(bomb,3);
	bomb->runAction(action);
	return true;
}
void TitleScene::showBackground(){
	SimpleAudioEngine::sharedEngine()->playEffect("audio/ef_11.ogg");
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic("audio/bg_0.ogg",true);
	CCSprite *background=CCSprite::create("image/ui/bg.png");
	background->setPosition( ccp(size.width/2, size.height/2) );
	background->setScaleX(size.width/background->getContentSize().width);
	background->setScaleY(size.height/background->getContentSize().height);
	CCAction *fadein=CCFadeIn::create(0.5);
	background->runAction(fadein);
	this->addChild(background,0);
	CCSprite *title_left=CCSprite::create("image/ui/title_left.png");
	CCSprite *title_right=CCSprite::create("image/ui/title_right.png");
	title_left->setPosition(ccp(size.width/2,size.height/2));
	title_right->setPosition(ccp(size.width/2,size.height/2));
	title_left->setScale(0);
	title_right->setScale(0);
	CCScaleTo *scale_left=CCScaleTo::create(1,size.height/title_left->getContentSize().height);
	CCScaleTo *scale_right=CCScaleTo::create(1,size.height/title_right->getContentSize().height);
	CCMoveTo *move_left=CCMoveTo::create(1,ccp(title_left->getContentSize().width*size.height/title_left->getContentSize().height/2,size.height/2));
	CCMoveTo *move_right=CCMoveTo::create(1,ccp(size.width-title_right->getContentSize().width*size.height/title_right->getContentSize().height/2,size.height/2));
	title_left->runAction(CCEaseExponentialOut::create(move_left));
	title_left->runAction(CCEaseExponentialOut::create(scale_left));
	title_right->runAction(CCEaseExponentialOut::create(move_right));
	title_right->runAction(CCEaseExponentialOut::create(scale_right));
	this->addChild(title_left,1);
	this->addChild(title_right,1);
	CCSprite *lefttop=CCSprite::create("image/ui/title_lefttop.png");
	CCSprite *leftbottom=CCSprite::create("image/ui/title_leftbottom.png");
	CCSprite *righttop=CCSprite::create("image/ui/title_righttop.png");
	CCSprite *rightbottom=CCSprite::create("image/ui/title_rightbottom.png");
//	leftbottom->setScale(0);lefttop->setScale(0);rightbottom->setScale(0);righttop->setScale(0);
	leftbottom->setPosition(ccp(size.width/2,size.height/2));
	lefttop->setPosition(ccp(size.width/2,size.height/2));
	rightbottom->setPosition(ccp(size.width/2,size.height/2));
	righttop->setPosition(ccp(size.width/2,size.height/2));
	CCMoveTo *move_lefttop=CCMoveTo::create(1,ccp(size.width/3,size.height/3*2));
	CCMoveTo *move_leftbottom=CCMoveTo::create(1,ccp(size.width/3,size.height/3));
	CCMoveTo *move_righttop=CCMoveTo::create(1,ccp(size.width/3*2,size.height/3*2));
	CCMoveTo *move_rightbottom=CCMoveTo::create(1,ccp(size.width/3*2,size.height/3));
	lefttop->runAction(move_lefttop);
	leftbottom->runAction(move_leftbottom);
	righttop->runAction(move_righttop);
	rightbottom->runAction(move_rightbottom);
	this->addChild(lefttop,2);
	this->addChild(leftbottom,2);
	this->addChild(rightbottom,2);
	this->addChild(righttop,2);
}
void TitleScene::showTitle(){
	title=CCSprite::create("image/ui/003c.png");
	CCAnimation *animation = CCAnimation::create();
	animation->setDelayPerUnit(0.25f);
	animation->addSpriteFrameWithFileName("image/ui/003c-r.png");
	animation->addSpriteFrameWithFileName("image/ui/003c-g.png");
	animation->addSpriteFrameWithFileName("image/ui/003c-b.png");
	animation->addSpriteFrameWithFileName("image/ui/003c.png");
	CCAnimate *animate = CCAnimate::create(animation);
	title->runAction(CCRepeatForever::create(animate));
	title->setScale(size.width/3/title->getContentSize().width);
	title->setPosition(ccp(size.width / 2, size.height/2 - title->boundingBox().size.height));
	CCAction *fadein=CCFadeIn::create(0.5);
	title->runAction(fadein);
	// add the label as a child to this layer
	this->addChild(title, 4);
	touch=CCSprite::create("image/ui/touch.png");
	touch->setPosition(ccp	(size.width/2,size.height/4));
	CCAction *delay2=CCDelayTime::create(1);
	CCAction *delay1=CCDelayTime::create(0.5);
	CCActionInterval *blink=CCSequence::create(CCShow::create(),delay2,CCHide::create(),delay1,NULL);
	touch->runAction(fadein);
	touch->runAction(CCRepeatForever::create(blink));
	this->addChild(touch,4);
}
void TitleScene::enableTouch(){
	this->setTouchEnabled(true);
}
void TitleScene::showLoginLayer(){
	CCLayer *loginLayer=TitleSceneLoginLayer::create();
	titleScene->addChild(loginLayer,1);
}
bool TitleScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
	CCFiniteTimeAction *showmenu=CCCallFuncN::create(this,callfuncN_selector(TitleScene::showLoginLayer));
	CCFiniteTimeAction* moveup=CCEaseExponentialOut::create(CCMoveBy::create(0.5,ccp(0, size.height/2 - bomb->boundingBox().size.height/2)));
	CCAction *action=CCSequence::create(moveup,showmenu,NULL);
	bomb->runAction(action);
	moveup=CCEaseExponentialOut::create(CCMoveBy::create(0.5,ccp(0, size.height/2 - bomb->boundingBox().size.height/2)));
	title->runAction(moveup);
	touch->removeFromParentAndCleanup(true);
	this->setTouchEnabled(false);
	return true;
}
void TitleScene::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){
}
void TitleScene::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
}
void TitleScene::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
}
void TitleScene::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    CCLayer::registerWithTouchDispatcher();
}
#endif // __TitleScene_SCENE_H__
