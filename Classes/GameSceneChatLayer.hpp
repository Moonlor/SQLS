#ifndef __GAME_SCENE_CHAT_LAYER__
#define __GAME_SCENE_CHAT_LAYER__
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace CocosDenshion;
class GameSceneChatLayer : public CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    virtual void ccTouchesBegan(CCSet* touches, CCEvent* pEvent);
    virtual void ccTouchesMoved(CCSet* touches, CCEvent* pEvent);
    virtual void ccTouchesEnded(CCSet* touches, CCEvent* pEvent);
    virtual void ccTouchesCancelled(CCSet* touches, CCEvent* pEvent);
    virtual void registerWithTouchDispatcher();
    void showChat(char* chatContent);
    void showInfo();
    CREATE_FUNC(GameSceneChatLayer);
	CCSize size;
};
// onserverField "init" you need to initialize your instance
bool GameSceneChatLayer::init()
{
	if (!CCLayer::init()){
		return false;
	}
	//三指下滑移回此层
	//提供对话支持
	size = CCDirector::sharedDirector()->getWinSize();
	CCLabelTTF *implentmenting=CCLabelTTF::create("Chat施工中","fonts/FZKaTong-M19T.ttf",25);
	implentmenting->setPosition(ccp(size.width/2,size.height/2));
	this->addChild(implentmenting);
	CCSprite *background=CCSprite::create("image/ui/black_blank.png");
	background->setScaleX(size.width/background->getContentSize().width);
	background->setScaleY(size.height/background->getContentSize().height);
	background->setPosition(ccp(size.width/2,size.height/2));
	this->addChild(background,0);
	return true;
}
void GameSceneChatLayer::ccTouchesBegan(CCSet* touches, CCEvent* pEvent){

}
void GameSceneChatLayer::ccTouchesMoved(CCSet* touches, CCEvent* pEvent){

}
void GameSceneChatLayer::ccTouchesEnded(CCSet* touches, CCEvent* pEvent){

}
void GameSceneChatLayer::ccTouchesCancelled(CCSet* touches, CCEvent* pEvent){

}
void GameSceneChatLayer::registerWithTouchDispatcher(){

}
void GameSceneChatLayer::showChat(char* chatContent){

}
void GameSceneChatLayer::showInfo(){

}
#endif
