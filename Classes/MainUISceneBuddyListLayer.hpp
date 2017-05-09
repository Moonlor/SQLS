#ifndef __MAINUI_SCENE_BUDDYLIST_LAYER__
#define __MAINUI_SCENE_BUDDYLIST_LAYER__
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "MainUISceneChooseLayer.hpp"
#include "ItemData.h"

USING_NS_CC;
using namespace CocosDenshion;
/**
@brief
*/
class MainUISceneBuddyListLayer : public CCLayer
{
	public:
		// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	/**
	@brief 初始化函数
	*/
		virtual bool init();//
		//
		/**
		@brief 触摸响应相关函数
		*/
		virtual void ccTouchesBegan (CCSet* touches, CCEvent* pEvent);
		/**
		@brief
		*/
		virtual void ccTouchesMoved (CCSet* touches, CCEvent* pEvent);
		/**
		@brief
		*/
		virtual void ccTouchesEnded (CCSet* touches, CCEvent* pEvent);
		/**
		@brief
		*/
		virtual void ccTouchesCancelled (CCSet* touches, CCEvent* pEvent);
		/**
		@brief
		*/
		virtual void registerWithTouchDispatcher();


		/**
		@brief 删除好友，完成好友列表的更新
		*/
		void deletefriend();//
		/**
		@brief 赠送好友道具，显示玩家物品所有东西
		*/
		void giftItem(); //
		/**
		@brief 选择物品，进行赠送，修改玩家道具相应属性
		*/
		void chooseGift(); //
		/**
		@brief 对话框确定选项
		*/
		void isSelect();  //
		/**
		@brief 对话框取消选项
		*/
		void noSelect(); //
		// a selector callback
		//
		/**
		@brief implement the "static node()" method manually
		*/
		CREATE_FUNC (MainUISceneBuddyListLayer);
		/**
		@brief
		*/
		CCSize size;
		/**
		@brief
		*/
		int buddyNum;
		/**
		@brief
		*/
		CCSprite *ui_right, *menuItem, *buttonSurround;
		/**
		@brief
		*/
		float w, h;
};
// onserverField "init" you need to initialize your instance
bool MainUISceneBuddyListLayer::init()
{
	//目标：双指上下滑动可以翻滚好友列表
	//点击某一个好友显示是否删除
	//好友要显示是否在线，离线时头像为灰色，显示好友等级，名称，头像，头像可能会有来自商店的特效道具
	//在好友头像上显示表情
	if (!CCLayer::init() )
		{
			return false;
		}

	size = CCDirector::sharedDirector()->getWinSize();
	this->setTouchEnabled(true);
	ui_right = (CCSprite*) MainUIScene::mainUIScene->getChildByTag (10)->getChildByTag (10);
	menuItem = (CCSprite*) MainUIScene::mainUIScene->getChildByTag (10)->getChildByTag (11);
	w = (size.width - ui_right->boundingBox().size.width);
	h = w*1/6;
	buddyNum = 5;

	CCMenu* pMenu = CCMenu::create();
	pMenu->setPosition ( CCPointZero );
	this->addChild (pMenu, 2);
	for(int i=1; i<=5; i++){
		CCSprite *itemBackground = CCSprite::create ("image/ui/itemBackground.png");
		itemBackground->setScaleX (w / itemBackground->getContentSize().width);
		itemBackground->setScaleY (h / itemBackground->getContentSize().height);
		itemBackground->setPosition (ccp (ui_right->boundingBox().size.width + w / 2, size.height - menuItem->boundingBox().size.height - (i-1) *h - h / 2) );
		this->addChild (itemBackground, 0, 10000 + i);
		char facePath[30];
		sprintf(facePath, "image/friend/friend_%d.png", i);
		CCSprite *face = CCSprite::create(facePath);
		face->setScaleX (h / face->getContentSize().width * 2/ 3);
		face->setScaleY (h / face->getContentSize().height * 2 /3);
		face->setPosition (ccp (itemBackground->getPositionX() - itemBackground->boundingBox().size.width/3, itemBackground->getPositionY() ) );
		this->addChild (face, 1, i);
		CCLabelTTF *name = CCLabelTTF::create ("无名氏", "fonts/FZKaTong-M19T.ttf", 25);
		name->setPositionX (itemBackground->getPositionX());
		name->setPositionY (itemBackground->getPositionY() + h / 4);
		name->setColor (ccYELLOW);
		this->addChild (name, 1, 100 + i);

		CCLabelTTF *level = CCLabelTTF::create("Level 1", "fonts/FZKaTong-M19T.ttf", 25);
		level->setPositionX(itemBackground->getPositionX());
		level->setPositionY(itemBackground->getPositionY()- h / 4);
		level->setColor(ccYELLOW);
		this->addChild(level, 1, 1000 + i);

		CCMenuItemImage *delFriendItem = CCMenuItemImage::create ("image/ui/button_normal.png", "image/ui/button_selected.png", this, menu_selector (MainUISceneBuddyListLayer::deletefriend) );
		delFriendItem->setScaleX (itemBackground->boundingBox().size.width/4/delFriendItem->getContentSize().width);
		delFriendItem->setScaleY (itemBackground->boundingBox().size.height/2/delFriendItem->getContentSize().height);
		delFriendItem->setPosition (ccp(itemBackground->getPositionX()+itemBackground->boundingBox().size.width/3, itemBackground->getPositionY()));
		pMenu->addChild (delFriendItem,0, i);
		CCLabelTTF *deleteFriendLabel = CCLabelTTF::create("删除好友", "fonts/FZKaTong-M19T.ttf", 30);
		deleteFriendLabel->setPosition (delFriendItem->getPosition() );
		deleteFriendLabel->setColor (ccYELLOW);
		this->addChild (deleteFriendLabel, 3, 2000+i);
	}
	return true;
}
void MainUISceneBuddyListLayer::ccTouchesBegan (CCSet* touches, CCEvent* pEvent)
{
}
void MainUISceneBuddyListLayer::ccTouchesMoved (CCSet* touches, CCEvent* pEvent)
{
	if (touches->count() == 2){
		if (h * buddyNum + menuItem->boundingBox().size.height > size.height){
						CCTouch* touch = dynamic_cast<CCTouch*> (*touches->begin() );
						float y = ccpAdd (this->getPosition(), touch->getDelta() ).y;

						if (y > (h * buddyNum + menuItem->boundingBox().size.height - size.height)){
							y = h * buddyNum + menuItem->boundingBox().size.height - size.height;
						}
						if (y < 0) y = 0;

						this->setPositionY (y);
					}

			}
}
void MainUISceneBuddyListLayer::ccTouchesEnded (CCSet* touches, CCEvent* pEvent)
{
}
void MainUISceneBuddyListLayer::ccTouchesCancelled (CCSet* touches, CCEvent* pEvent)
{
}
void MainUISceneBuddyListLayer::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate (this, 0);
	CCLayer::registerWithTouchDispatcher();
}
void MainUISceneBuddyListLayer::deletefriend()
{

}
void MainUISceneBuddyListLayer::giftItem()
{
}
void MainUISceneBuddyListLayer::chooseGift()
{
}
#endif
