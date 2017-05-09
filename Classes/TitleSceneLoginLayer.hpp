#ifndef __TITLE_SCENE_LOGIN_LAYER__
#define __TITLE_SCENE_LOGIN_LAYER__
typedef unsigned int size_t;
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
class TitleSceneLoginLayer : public CCLayer ,public CCTextFieldDelegate
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    // a selector callback
    void loginButtonClicked(CCObject* pSender);
    void registButtonClicked(CCObject* pSender);
    void usernameFieldPressed(CCObject *sender);
    void passwdFieldPressed(CCObject *sender);
    void serverFieldPressed(CCObject *sender);
    // implement the "static node()" method manually
    CREATE_FUNC(TitleSceneLoginLayer);
    void login();
    CCTextFieldTTF *usernameField,*passwdField,*serverField;
    CCMenuItemFont* passwdTapItem;
    CCLabelTTF * message;
	CCSize size;
	bool onTextFieldAttachWithIME(CCTextFieldTTF *sender);
	bool onTextFieldDetachWithIME(CCTextFieldTTF *sender);
	void menuCloseCallback(CCObject* pSender);
	void showMessage(const char* st);
};
#include "MainUIScene.hpp"
#include "UserData.hpp"
#include "TitleSceneMessageLayer.hpp"
bool TitleSceneLoginLayer::init()
{
	if (!CCLayer::init())
	{
		return false;
	}
	size = CCDirector::sharedDirector()->getWinSize();
	SimpleAudioEngine::sharedEngine()->preloadEffect("audio/ef_0.ogg");
	CCMenuItemImage *pCloseItem = CCMenuItemImage::create("image/ui/CloseNormal.png","image/ui/CloseSelected.png",this,menu_selector(TitleSceneLoginLayer::menuCloseCallback));
    pCloseItem->setPosition(ccp(size.width - pCloseItem->getContentSize().width/2 ,pCloseItem->getContentSize().height/2));
	CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
	pMenu->setPosition( CCPointZero );
	this->addChild(pMenu,1);

	CCSprite* menuBackground=CCSprite::create("image/ui/mainmenu.png");
	menuBackground->setScale(size.height/24*14/menuBackground->getContentSize().height);
	menuBackground->setPosition(ccp(size.width/2,menuBackground->boundingBox().size.height/2+size.height/64*7));
	this->addChild(menuBackground,0);

	CCLabelTTF *serverLabel=CCLabelTTF::create("请输入服务器地址","fonts/FZKaTong-M19T.ttf",25);
	serverLabel->setPosition(ccp(size.width/2,menuBackground->boundingBox().size.height/32*27+size.height/64*7));
	this->addChild(serverLabel,1);
	serverField = CCTextFieldTTF::textFieldWithPlaceHolder("", "fonts/FZKaTong-M19T.ttf", 25);
	serverField->setString("192.168.1.102");
	serverField->setPosition(ccp(size.width/2, menuBackground->boundingBox().size.height/8*6+size.height/64*7));
	serverField->setDelegate(this);
	this->addChild(serverField,1);
	CCMenuItem* serverTapItem = CCMenuItemFont::create("              ",this,menu_selector(TitleSceneLoginLayer::serverFieldPressed));
	serverTapItem->setPosition(ccp(size.width/2, menuBackground->boundingBox().size.height/8*6+size.height/64*7));
	pMenu->addChild(serverTapItem, 1);

	CCLabelTTF* usernameLabel=CCLabelTTF::create("请输入用户名","fonts/FZKaTong-M19T.ttf",25);
	usernameLabel->setPosition(ccp(size.width/2, menuBackground->boundingBox().size.height/16*9+size.height/64*7));
	this->addChild(usernameLabel,1);
	usernameField=CCTextFieldTTF::textFieldWithPlaceHolder("", "fonts/FZKaTong-M19T.ttf", 25);
	usernameField->setPosition(ccp(size.width/2, menuBackground->boundingBox().size.height/16*7+size.height/64*7));
	usernameField->setDelegate(this);
	this->addChild(usernameField,1);
	CCMenuItem* usernameTapItem = CCMenuItemFont::create("              ",this,menu_selector(TitleSceneLoginLayer::usernameFieldPressed));
	usernameTapItem->setPosition(ccp(size.width/2, menuBackground->boundingBox().size.height/16*7+size.height/64*7));
	pMenu->addChild(usernameTapItem, 1);

	CCLabelTTF *passwdLabel=CCLabelTTF::create("请输入密码","fonts/FZKaTong-M19T.ttf",25);
	passwdLabel->setPosition(ccp(size.width/2, menuBackground->boundingBox().size.height/8*2+size.height/64*7));
	this->addChild(passwdLabel,1);
	passwdField=CCTextFieldTTF::textFieldWithPlaceHolder("", "fonts/FZKaTong-M19T.ttf", 25);
	passwdField->setPosition(ccp(size.width/2, menuBackground->boundingBox().size.height/32*5+size.height/64*7));
	passwdField->setDelegate(this);
	this->addChild(passwdField,1);
	passwdTapItem = CCMenuItemFont::create("              ",this,menu_selector(TitleSceneLoginLayer::passwdFieldPressed));
	passwdTapItem->setPosition(ccp(size.width/2, menuBackground->boundingBox().size.height/32*5+size.height/64*7));
	pMenu->addChild(passwdTapItem, 1);

	CCMenuItemImage *loginButton = CCMenuItemImage::create("image/ui/c8.png","image/ui/c3.png",this,menu_selector(TitleSceneLoginLayer::loginButtonClicked));
	loginButton->setScaleY(size.height/64*7/loginButton->getContentSize().height);
	loginButton->setScaleX(menuBackground->boundingBox().size.width/2/loginButton->getContentSize().width);
	loginButton->setPosition( ccp(size.width/2-menuBackground->boundingBox().size.width/4,loginButton->boundingBox().size.height/2));
	pMenu->addChild(loginButton,2);
	CCLabelTTF *loginLabel=CCLabelTTF::create("登录","fonts/FZKaTong-M19T.ttf",25);
	loginLabel->setPosition( ccp(size.width/2-menuBackground->boundingBox().size.width/4,loginButton->boundingBox().size.height/2));
	loginLabel->setColor(ccYELLOW);
	this->addChild(loginLabel,3);

	CCMenuItemImage *registButton = CCMenuItemImage::create("image/ui/c8.png","image/ui/c3.png",this,menu_selector(TitleSceneLoginLayer::registButtonClicked));
	registButton->setScaleY(size.height/64*7/registButton->getContentSize().height);
	registButton->setScaleX(menuBackground->boundingBox().size.width/2/registButton->getContentSize().width);
	registButton->setPosition( ccp(size.width/2+menuBackground->boundingBox().size.width/4,registButton->boundingBox().size.height/2));
	pMenu->addChild(registButton,2);
	CCLabelTTF *registLabel=CCLabelTTF::create("注册","fonts/FZKaTong-M19T.ttf",25);
	registLabel->setPosition( ccp(size.width/2+menuBackground->boundingBox().size.width/4,registButton->boundingBox().size.height/2));
	registLabel->setColor(ccYELLOW);
	this->addChild(registLabel,3);

    return true;
}
void TitleSceneLoginLayer::usernameFieldPressed(CCObject *sender)
{
	usernameField->attachWithIME();
}
void TitleSceneLoginLayer::serverFieldPressed(CCObject *sender)
{
	serverField->attachWithIME();
}
void TitleSceneLoginLayer::passwdFieldPressed(CCObject *sender)
{
	passwdField->attachWithIME();
	passwdField->setVisible(false);
	passwdTapItem->setString("********");
}
bool TitleSceneLoginLayer::onTextFieldAttachWithIME(CCTextFieldTTF *sender)
{
	SimpleAudioEngine::sharedEngine()->playEffect("audio/ef_0.ogg");
	return false;
}
bool TitleSceneLoginLayer::onTextFieldDetachWithIME(CCTextFieldTTF *sender)
{
	SimpleAudioEngine::sharedEngine()->playEffect("audio/ef_0.ogg");
	return false;
}
void TitleSceneLoginLayer::login(){
	UserData::current->login();
    if(UserData::httpAns==-1 or UserData::httpAns==-404){
    	showMessage("密码错误");
    }
    else{
    	CCScene *pScene = MainUIScene::scene();
    	CCDirector::sharedDirector()->replaceScene(CCTransitionFlipY::create(0.5f, pScene));
    }
}
void TitleSceneLoginLayer::loginButtonClicked(CCObject* pSender)
{
	SimpleAudioEngine::sharedEngine()->playEffect("audio/ef_0.ogg");
	sprintf(UserData::current->server,"%s",serverField->getString());
	sprintf(UserData::current->username,"%s",usernameField->getString());
	sprintf(UserData::current->passwd,"%s",passwdField->getString());
	UserData::current->checkName();
	if(UserData::httpAns!=-404){
		if(UserData::current->userid==-1)
			showMessage("用户名不存在");
		else
			login();
	}
	else
		showMessage("连接失败");
}
void TitleSceneLoginLayer::registButtonClicked(CCObject* pSender)
{
	SimpleAudioEngine::sharedEngine()->playEffect("audio/ef_0.ogg");
	sprintf(UserData::current->server,"%s",serverField->getString());
	sprintf(UserData::current->username,"%s",usernameField->getString());
	sprintf(UserData::current->passwd,"%s",passwdField->getString());
	UserData::current->checkName();
	if(UserData::httpAns!=-404){
		if(UserData::current->userid!=-1)
			showMessage("该用户名已存在");
		else{
			UserData::current->regist();
			login();
		}
	}
	else
		showMessage("连接失败");
}
void TitleSceneLoginLayer::showMessage(const char* st){
	CCLayer* messageLayer=TitleSceneMessageLayer::create();
	CCLabelTTF *message=CCLabelTTF::create(st,"fonts/FZKaTong-M19T.ttf",60);
	message->setPosition(ccp(size.width/2,size.height/2));
	messageLayer->addChild(message,1);
	this->getParent()->addChild(messageLayer,20);
}
void TitleSceneLoginLayer::menuCloseCallback(CCObject* pSender)
{
	SimpleAudioEngine::sharedEngine()->playEffect("audio/ef_0.ogg");
	CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
#endif // __TitleSceneLoginLayer_LAYER__
