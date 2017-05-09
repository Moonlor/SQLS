#ifndef __MAINUI_SCENE__
#define __MAINUI_SCENE__
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace CocosDenshion;
/**
@brief 主界面显示玩家刚刚登录游戏后的界面，在该界面上有登出按钮、游戏开始按钮、以及玩家的头像，显示玩家等级和金币数，在主界面上还有最大的四个板块，分别是“多人模式”、“我的好友”、“我的物品”、“道具商店”四个标签选项。玩家可主界面进入这四个界面，查看详细信息。而游戏大厅设置在多人模式界面下。
该类继承自CCLayer层类，所以主界面其实是一个层。
*/
class MainUIScene : public CCLayer
{
	public:
	/**
	@brief
	*/
		// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
		virtual bool init();
		// there's no 'id' in cpp, so we recommand to return the exactly class pointer
		/**
		@brief
		*/
		static CCScene* scene();
		// a selector callback
		/**
		@brief 处理登出事件的函数
		*/
		void logout();   //登出
		/**
		@brief “多人模式”标签触发
		*/
		void multiplayerSelected();//多人模式
		/**
		@brief “我的好友”标签触发
		*/
		void buddylistSelected(); //好友列表
		/**
		@brief “我的物品”标签触发
		*/
		void inventorySelected();//我的物品
		/**
		@brief “道具商店”标签触发
		*/
		void storeSelected();//道具商店
		/**
		@brief 处理游戏开始事件
		*/
		void okButtonClicked();//游戏开始按钮
		/**
		@brief 处理显示人物表情
		*/
		void showEmotion (int num); //显示表情
		/**
		@brief 显示玩家游戏角色
		*/
		void showAvatar (int num); //显示角色
		/**
		@brief 展示金币数量
		*/
		void showCoinNum (int num); //显示金币
		void okButton();
		/**
		@brief
		*/
		// implement the "static node()" method manually
		CREATE_FUNC (MainUIScene);
		/**
		@brief
		*/
		CCSize size;
		/**
		@brief 创建按钮选择框精灵表情精灵、和玩家角色精灵
		*/
		CCSprite *buttonSurround, *emotion, *face;
		/**
		@brief
		*/
		CCMenuItemImage *multiplayerItem, *buddylistItem, *inventoryItem, *storeItem, *okButtonItem;
		/**
		@brief
		*/
		CCLabelTTF *coinLabel;
		/**
		@brief
		*/
		static CCScene *mainUIScene;
		static bool ready, In;
};
#include "TitleScene.hpp"
#include "UserData.hpp"
#include "MainUISceneBuddyListLayer.hpp"
#include "MainUISceneInventoryLayer.hpp"
#include "MainUISceneMultiplayerLayer.hpp"
#include "MainUISceneStoreLayer.hpp"
#include "GameScene.hpp"
#include "Client.hpp"
CCScene *MainUIScene::mainUIScene;
bool MainUIScene::ready;
bool MainUIScene::In;
CCScene* MainUIScene::scene()
{
	// 'scene' is an autorelease object
	mainUIScene = CCScene::create();
	// 'layer' is an autorelease object
	MainUIScene *layer = MainUIScene::create();
	// add layer as a child to scene
	mainUIScene->addChild (layer, 2, 10);
	// return the scene
	return mainUIScene;
}
// on "init" you need to initialize your instance
bool MainUIScene::init()
{
	if (!CCLayer::init() )
		{
			return false;
		}
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic ("audio/bg_6.ogg");
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic ("audio/bg_6.ogg", true);
	SimpleAudioEngine::sharedEngine()->preloadEffect ("audio/ef_0.ogg");
	size = CCDirector::sharedDirector()->getWinSize();
	CCMenu* pMenu = CCMenu::create();
	pMenu->setPosition ( CCPointZero );
	this->addChild (pMenu, 2);
	//UserData::current->fetchBasicData();
	//UserData::current->fetchExtraData();
	//Client::client->connectRemote();
	CCLayer *backgroundLayer = CCLayer::create();
	mainUIScene->addChild (backgroundLayer, 0);
	CCSprite *background = CCSprite::create ("image/ui/bg.jpg");
	background->setPosition ( ccp (size.width / 2, size.height / 2) );
	background->setScaleX (size.width / background->getContentSize().width);
	background->setScaleY (size.height / background->getContentSize().height);
	backgroundLayer->addChild (background, -2);
	CCSprite *blackBackground = CCSprite::create ("image/ui/black_blank.png");
	blackBackground->setScaleX (size.width / blackBackground->getContentSize().width);
	blackBackground->setScaleY (size.height / blackBackground->getContentSize().height);
	blackBackground->setPosition (ccp (size.width / 2, size.height / 2) );
	backgroundLayer->addChild (blackBackground, -1);
	CCSprite *ui_right = CCSprite::create ("image/ui/ui_right.png");
	ui_right->setScale (size.height / ui_right->getContentSize().height);
	ui_right->setPosition (ccp (ui_right->boundingBox().size.width / 2, size.height / 2) );
	this->addChild (ui_right, 1, 10);
	CCLabelTTF *logoutLabel = CCLabelTTF::create ("登\t出", "fonts/FZKaTong-M19T.ttf", 25);
	logoutLabel->setColor (ccYELLOW);
	CCMenuItemLabel *logoutItem = CCMenuItemLabel::create (logoutLabel, this, menu_selector (MainUIScene::logout) );
	logoutItem->setPosition (ccp (ui_right->boundingBox().size.width / 128 * 67, size.height / 64 * 59) );
	pMenu->addChild (logoutItem);
	face = CCSprite::create();
	showAvatar (UserData::current->face);
	face->setScale (size.height / ui_right->getContentSize().height * 1.12);
	face->setPosition (ccp (ui_right->boundingBox().size.width / 128 * 67, size.height / 64 * 39) );
	this->addChild (face, 2, 999);
	CCLabelTTF *usernameLabel = CCLabelTTF::create (UserData::current->username, "fonts/FZKaTong-M19T.ttf", 30);
	usernameLabel->setPosition (ccp (ui_right->boundingBox().size.width / 128 * 67, size.height / 64 * 49) );
	usernameLabel->setColor (ccMAGENTA);
	this->addChild (usernameLabel, 2);
	multiplayerItem = CCMenuItemImage::create ("image/ui/button_normal.png", "image/ui/button_selected.png", this, menu_selector (MainUIScene::multiplayerSelected) );
	multiplayerItem->setScaleX ( (size.width - ui_right->boundingBox().size.width) / 4 / multiplayerItem->getContentSize().width);
	multiplayerItem->setScaleY (size.height / 8 / multiplayerItem->getContentSize().height);
	multiplayerItem->setPosition (ccp (ui_right->boundingBox().size.width + multiplayerItem->boundingBox().size.width / 2, size.height - multiplayerItem->boundingBox().size.height / 2) );
	pMenu->addChild (multiplayerItem);
	CCLabelTTF *multiplayerLabel = CCLabelTTF::create ("多人模式", "fonts/FZKaTong-M19T.ttf", 25);
	multiplayerLabel->setPosition (multiplayerItem->getPosition() );
	multiplayerLabel->setColor (ccYELLOW);
	this->addChild (multiplayerLabel, 3);
	buddylistItem = CCMenuItemImage::create ("image/ui/button_normal.png", "image/ui/button_selected.png", this, menu_selector (MainUIScene::buddylistSelected) );
	buddylistItem->setScaleX ( (size.width - ui_right->boundingBox().size.width) / 4 / multiplayerItem->getContentSize().width);
	buddylistItem->setScaleY (size.height / 8 / multiplayerItem->getContentSize().height);
	buddylistItem->setPosition (ccp (ui_right->boundingBox().size.width + multiplayerItem->boundingBox().size.width * 3 / 2, size.height - multiplayerItem->boundingBox().size.height / 2) );
	pMenu->addChild (buddylistItem);
	CCLabelTTF *buddylistLabel = CCLabelTTF::create ("我的好友", "fonts/FZKaTong-M19T.ttf", 25);
	buddylistLabel->setPosition (buddylistItem->getPosition() );
	buddylistLabel->setColor (ccYELLOW);
	this->addChild (buddylistLabel, 3);
	inventoryItem = CCMenuItemImage::create ("image/ui/button_normal.png", "image/ui/button_selected.png", this, menu_selector (MainUIScene::inventorySelected) );
	inventoryItem->setScaleX ( (size.width - ui_right->boundingBox().size.width) / 4 / multiplayerItem->getContentSize().width);
	inventoryItem->setScaleY (size.height / 8 / multiplayerItem->getContentSize().height);
	inventoryItem->setPosition (ccp (ui_right->boundingBox().size.width + multiplayerItem->boundingBox().size.width * 5 / 2, size.height - multiplayerItem->boundingBox().size.height / 2) );
	pMenu->addChild (inventoryItem);
	CCLabelTTF *inventoryLabel = CCLabelTTF::create ("我的物品", "fonts/FZKaTong-M19T.ttf", 25);
	inventoryLabel->setPosition (inventoryItem->getPosition() );
	inventoryLabel->setColor (ccYELLOW);
	this->addChild (inventoryLabel, 3);
	storeItem = CCMenuItemImage::create ("image/ui/button_normal.png", "image/ui/button_selected.png", this, menu_selector (MainUIScene::storeSelected) );
	storeItem->setScaleX ( (size.width - ui_right->boundingBox().size.width) / 4 / multiplayerItem->getContentSize().width);
	storeItem->setScaleY (size.height / 8 / multiplayerItem->getContentSize().height);
	storeItem->setPosition (ccp (ui_right->boundingBox().size.width + multiplayerItem->boundingBox().size.width * 7 / 2, size.height - multiplayerItem->boundingBox().size.height / 2) );
	pMenu->addChild (storeItem);
	CCLabelTTF *storeLabel = CCLabelTTF::create ("道具商店", "fonts/FZKaTong-M19T.ttf", 25);
	storeLabel->setPosition (storeItem->getPosition() );
	storeLabel->setColor (ccYELLOW);
	this->addChild (storeLabel, 3, 100);
	buttonSurround = CCSprite::create ("image/ui/button_surround.png");
	buttonSurround->setScaleX (multiplayerItem->boundingBox().size.width / buttonSurround->getContentSize().width);
	buttonSurround->setScaleY (multiplayerItem->boundingBox().size.height / buttonSurround->getContentSize().height);
	this->addChild (buttonSurround, 3, 11);
	CCSprite *rankSprite = CCSprite::create ("image/ui/rank.png");
	rankSprite->setScale (size.height / 16 / rankSprite->getContentSize().height);
	rankSprite->setPosition (ccp (ui_right->boundingBox().size.width / 128 * 37, size.height / 64 * 27) );
	this->addChild (rankSprite, 2);
	char rankPath[80];
	sprintf (rankPath, "image/rank/rank%d.png", UserData::current->rank);
	CCSprite *rank = CCSprite::create (rankPath);
	rank->setScale (rankSprite->boundingBox().size.width / rank->getContentSize().width);
	rank->setPosition (rankSprite->getPosition() );
	this->addChild (rank, 3);
	char rankMessage[20];
	sprintf (rankMessage, "Lv.%d", UserData::current->rank);
	CCLabelTTF *rankLabel = CCLabelTTF::create (rankMessage, "fonts/FZKaTong-M19T.ttf", 25);
	rankLabel->setPosition (ccp (ui_right->boundingBox().size.width / 128 * 82, rankSprite->getPositionY() ) );
	this->addChild (rankLabel, 3);
	CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage ("image/ui/item_55.png");
	CCTexture2D* texture2 = CCTextureCache::sharedTextureCache()->addImage ("image/ui/item_57.png");
	float w = texture->getContentSize().width ;
	float h = texture->getContentSize().height;
	CCAnimation *animation = CCAnimation::create();
	animation->setDelayPerUnit (0.5f);
	//for(int i = 0; i <2; i ++)
	animation->addSpriteFrameWithTexture (texture, CCRectMake (0, 0, w, h) );
	animation->addSpriteFrameWithTexture (texture2, CCRectMake (0, 0, w, h) );
	CCAnimate *animate = CCAnimate::create (animation);
	CCSprite *coin = CCSprite::create ("image/ui/item_55.png", CCRectMake (0, 0, w, h) );
	coin->setScale (rankSprite->boundingBox().size.width / coin->getContentSize().width * 1.4);
	coin->runAction (CCRepeatForever::create (animate) );
	coin->setPosition (ccp (rank->getPositionX(), size.height / 128 * 45) );
	this->addChild (coin, 2);
	showCoinNum (UserData::current->coinNum);
	okButtonItem = CCMenuItemImage::create ("image/ui/ok_button_normal.png", "image/ui/ok_button_selected.png", "image/ui/ok_button_disabled.png", this, menu_selector (MainUIScene::okButtonClicked) );
	okButtonItem->setScale (size.height / ui_right->getContentSize().height);
	okButtonItem->setPosition (ccp (ui_right->boundingBox().size.width / 128 * 67, size.height / 512 * 77) );
	//	okButtonItem->setEnabled(false);
	pMenu->addChild (okButtonItem);
	emotion = CCSprite::create();
	showEmotion (UserData::current->emotion);
	emotion->setPosition (ccp (face->getPositionX() - face->boundingBox().size.width / 4, face->getPositionY() + face->boundingBox().size.height / 4) );
	this->addChild (emotion, 3);
	multiplayerSelected();
	this->schedule(schedule_selector(MainUIScene::okButton));
	return true;
}
void MainUIScene::showAvatar (int num)
{
	char facePath[80];
	sprintf (facePath, "image/face/face%d.png", num);
	face->initWithFile (facePath);
}
void MainUIScene::showEmotion (int num)
{
	emotion->stopAllActions();
	char emotionPath[80];
	sprintf (emotionPath, "image/emotion/emotion%d.png", num);
	CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage (emotionPath);
	float w = texture->getContentSize().width / 2;
	float h = texture->getContentSize().height;
	emotion->setContentSize (CCSizeMake (w, h) );
	CCAnimation *animation = CCAnimation::create();
	animation->setDelayPerUnit (0.5f);

	for (int i = 0; i < 2; i ++)
		animation->addSpriteFrameWithTexture (texture, CCRectMake (i * w, 0, w, h) );

	CCAnimate *animate = CCAnimate::create (animation);
	emotion->runAction (CCRepeatForever::create (animate) );
	emotion->setScale (face->boundingBox().size.height / 2 / emotion->boundingBox().size.height);
}
void MainUIScene::showCoinNum (int num)
{
	char coinNum[80];
	sprintf (coinNum, "%d", UserData::current->coinNum);
	this->removeChild (coinLabel);
	coinLabel = CCLabelTTF::create (coinNum, "fonts/FZKaTong-M19T.ttf", 25);
	coinLabel->setPosition (ccp (this->getChildByTag (10)->boundingBox().size.width / 128 * 82, size.height / 128 * 45) );
	this->addChild (coinLabel, 2, 10000);
}
void MainUIScene::logout()
{
	SimpleAudioEngine::sharedEngine()->playEffect ("audio/ef_0.ogg");
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	UserData::current->userid = -1;
	CCScene *pScene = TitleScene::scene();
	CCDirector::sharedDirector()->replaceScene (CCTransitionFlipY::create (0.5f, pScene) );
}
void MainUIScene::multiplayerSelected()
{
	SimpleAudioEngine::sharedEngine()->playEffect ("audio/ef_0.ogg");
	buttonSurround->stopAllActions();
	CCAction *move = CCEaseExponentialOut::create (CCMoveTo::create (0.5, multiplayerItem->getPosition() ) );
	buttonSurround->runAction (move);
	mainUIScene->removeChildByTag (1, true);
	CCLayer *multiplayerLayer = MainUISceneMultiplayerLayer::create();
	mainUIScene->addChild (multiplayerLayer, 1, 1);
}
void MainUIScene::buddylistSelected()
{
	SimpleAudioEngine::sharedEngine()->playEffect ("audio/ef_0.ogg");
	buttonSurround->stopAllActions();
	CCAction *move = CCEaseExponentialOut::create (CCMoveTo::create (0.5, buddylistItem->getPosition() ) );
	buttonSurround->runAction (move);
	mainUIScene->removeChildByTag (1, true);
	CCLayer *buddylistLayer = MainUISceneBuddyListLayer::create();
	mainUIScene->addChild (buddylistLayer, 1, 1);
}
void MainUIScene::inventorySelected()
{
	SimpleAudioEngine::sharedEngine()->playEffect ("audio/ef_0.ogg");
	buttonSurround->stopAllActions();
	CCAction *move = CCEaseExponentialOut::create (CCMoveTo::create (0.5, inventoryItem->getPosition() ) );
	buttonSurround->runAction (move);
	mainUIScene->removeChildByTag (1, true);
	CCLayer *inventoryLayer = MainUISceneInventoryLayer::create();
	mainUIScene->addChild (inventoryLayer, 1, 1);
}
void MainUIScene::storeSelected()
{
	SimpleAudioEngine::sharedEngine()->playEffect ("audio/ef_0.ogg");
	buttonSurround->stopAllActions();
	CCAction *move = CCEaseExponentialOut::create (CCMoveTo::create (0.5, storeItem->getPosition() ) );
	buttonSurround->runAction (move);
	mainUIScene->removeChildByTag (1, true);
	CCLayer *storeLayer = MainUISceneStoreLayer::create();
	mainUIScene->addChild (storeLayer, 1, 1);
}
void MainUIScene::okButtonClicked()
{
	if(MainUIScene::ready){
		char msg[255];
		memset(msg,0,sizeof(msg));
		sprintf(msg, "%s","ok");
		send(Client::client->sockfd,msg, sizeof(msg),0);
	}
}
void MainUIScene::okButton(){
	if(MainUIScene::ready){
	char rMsg[255];
	fd_set fdr;
	struct timeval timeout={0,0};
	FD_ZERO(&fdr);
	FD_SET(Client::client->sockfd, &fdr);

	switch(select(Client::client->sockfd+1, &fdr, NULL, NULL, &timeout)){
		case -1:
			break;
		case 0:
			break;
		default:
			if(FD_ISSET(Client::client->sockfd, &fdr)){
				int r= recv(Client::client->sockfd, rMsg, sizeof(rMsg), 0);
				if(r>=0)
					if(strcmp(rMsg,"ok")==0){
						this->unschedule(schedule_selector(MainUIScene::okButton));
						CCScene *pScene = GameScene::scene();
						CCDirector::sharedDirector()->replaceScene (CCTransitionFlipY::create (0.5f, pScene));
						break;
					}
			}
	}
	}
}
#endif
