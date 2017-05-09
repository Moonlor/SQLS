#ifndef __MAINUI_SCENE_MULTIPLAYER_LAYER__
#define __MAINUI_SCENE_MULTIPLAYER_LAYER__
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "UserData.hpp"
#include "MainUIScene.hpp"
#include "Client.hpp"
USING_NS_CC;
using namespace CocosDenshion;
/**
@brief
*/
class MainUISceneMultiplayerLayer : public CCLayer
{
	public:
		// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	/**
	@brief 初始化函数
	*/
		virtual bool init();  //
		//  //
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
		@brief //参数，房间号
		*/
		void houseShow (int number);
		/**
		@brief 改变自己的角色
		*/
		void changeCharactor();//

		/**
		@brief 离开房间
		*/
		void leave();//
		/**
		@brief 用来显示第number号房间信息，例如某玩家加入
		*/
		void Show (); //
		/**
		@brief 对话框确定选项
		*/
		void characterSelect(CCObject*);//
		void groupSelect(CCObject*);
		/**
		@brief implement the "static node()" method manually
		*/
		// a selector callback
		//
		CREATE_FUNC (MainUISceneMultiplayerLayer);
		/**
		@brief
		*/
		CCSize size;
		/**
		@brief
		*/
		CCSprite *ui_right, *menuItem;
		/**
		@brief
		*/
		float w, h;
		/**
		@brief
		*/
		CCMenu *pMenu, *pMenu2;

		CCMenuItemImage *characterItem;
		int beforePosition;
		int userid[8];
		bool isIn, opeEnable, needOpe;
};
// onserverField "init" you need to initialize your instance
bool MainUISceneMultiplayerLayer::init()
{

	if (!CCLayer::init() )
		{
			return false;
		}
	//this->setTouchEnabled (true);
	isIn=false;
	opeEnable=false;
	needOpe=true;

	size = CCDirector::sharedDirector()->getWinSize();
	memset(userid, 0, sizeof(userid));
	ui_right = CCSprite::create ("image/ui/ui_right.png");
	ui_right->setScale (size.height / ui_right->getContentSize().height);
	menuItem = CCSprite::create ("image/ui/button_normal.png");
	menuItem->setScaleY (size.height / 8 / menuItem->getContentSize().height);

	w = (size.width-ui_right->boundingBox().size.width - 15)/4;
	h = (size.height-menuItem->boundingBox().size.height)/2-5;

	pMenu = CCMenu::create();
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu,1);
	pMenu2 = CCMenu::create();
	pMenu2->setPosition(CCPointZero);
	this->addChild(pMenu2,1);
	for(int i=0; i<8; i++){
		characterItem = CCMenuItemImage::create ("image/ui/itemBackground.png", "image/ui/itemSelected.png", this, menu_selector(MainUISceneMultiplayerLayer::characterSelect));
		characterItem->setScaleX(w/characterItem->getContentSize().width);
		characterItem->setScaleY(h/characterItem->getContentSize().height*4/5);
		characterItem->setPositionX(ui_right->boundingBox().size.width+(5+w)*(i%4)+w/2);
		characterItem->setPositionY(size.height - menuItem->boundingBox().size.height-(h+5)*(i/4)-h*2/5);
		pMenu->addChild(characterItem,0,i);

		CCLabelTTF *groupLabel = CCLabelTTF::create ("队\t\t伍", "fonts/FZKaTong-M19T.ttf", 32);
		//logoutLabel->setColor (ccYELLOW);
		CCMenuItemLabel *groupItem = CCMenuItemLabel::create (groupLabel, this, menu_selector (MainUISceneMultiplayerLayer::groupSelect) );
		groupItem->setPosition (ccp (ui_right->boundingBox().size.width+(5+w)*(i%4)+w/2, size.height - menuItem->boundingBox().size.height-(h+5)*(i/4)-h*2/5-h/2) );
		pMenu2->addChild (groupItem, 0, i);

		char temp[80];
		sprintf(temp, "image/ui/teamBackground_%d.png", i);
		CCSprite* groupColor=CCSprite::create(temp);
		groupColor->setScaleX(w/groupColor->getContentSize().width);
		groupColor->setScaleY(h/characterItem->getContentSize().height/5);
		groupColor->setPositionX(ui_right->boundingBox().size.width+(5+w)*(i%4)+w/2);
		groupColor->setPositionY(pMenu2->getChildByTag(i)->getPositionY());
		this->addChild(groupColor, 0, 5000+i);
	}
	pMenu->setEnabled(false);
	pMenu2->setEnabled(false);
	schedule(schedule_selector(MainUISceneMultiplayerLayer::Show),2.0f);
	return true;
}
void MainUISceneMultiplayerLayer::ccTouchesBegan (CCSet* touches, CCEvent* pEvent)
{
}
void MainUISceneMultiplayerLayer::ccTouchesMoved (CCSet* touches, CCEvent* pEvent)
{
}
void MainUISceneMultiplayerLayer::ccTouchesEnded (CCSet* touches, CCEvent* pEvent)
{
}
void MainUISceneMultiplayerLayer::ccTouchesCancelled (CCSet* touches, CCEvent* pEvent)
{
}
void MainUISceneMultiplayerLayer::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate (this, 0);
	CCLayer::registerWithTouchDispatcher();
}
void MainUISceneMultiplayerLayer::houseShow (int number)
{
}
void MainUISceneMultiplayerLayer::changeCharactor()
{
}

void MainUISceneMultiplayerLayer::leave()
{
}
void MainUISceneMultiplayerLayer::Show ()
{
	UserData::current->fetchRoomData();
	for(int i=0; i<8; i++){
		if(UserData::current->roomlist[i]>0){
			if(userid[i]!=UserData::current->roomlist[i]&&needOpe){
				userid[i]=UserData::current->roomlist[i];
				if(userid[i]==UserData::current->userid){
					UserData::current->myPosition=i;
					isIn=true;
				}
				UserData* user = new UserData(UserData::current->roomlist[i]);
				sprintf(user->server, "%s", UserData::current->server);
				user->fetchBasicData();
				char facePath[40];
				sprintf (facePath, "image/face/face%d.png", user->face);
				CCSprite*face = CCSprite::create(facePath);
				if(this->getChildByTag(i)!=NULL){
					this->removeChildByTag(i);
					this->removeChildByTag(100+i);
					this->removeChildByTag(1000+i);
				}
				face->setScale(w/4/face->getContentSize().width);
				face->setPositionX(pMenu->getChildByTag(i)->getPosition().x-pMenu->getChildByTag(i)->boundingBox().size.width/4);
				face->setPositionY(pMenu->getChildByTag(i)->getPosition().y+pMenu->getChildByTag(i)->boundingBox().size.height/4);
				this->addChild(face, 2, i);

				CCLabelTTF *nameLabel = CCLabelTTF::create (user->username, "fonts/FZKaTong-M19T.ttf", 25);
				nameLabel->setPositionX(pMenu->getChildByTag(i)->getPosition().x+pMenu->getChildByTag(i)->boundingBox().size.width/4);
				nameLabel->setPositionY(pMenu->getChildByTag(i)->getPosition().y+pMenu->getChildByTag(i)->boundingBox().size.height/3);
				this->addChild(nameLabel, 2, 100+i);

				char temp[30];
				sprintf(temp, "level %d", user->rank);
				CCLabelTTF *levelLabel = CCLabelTTF::create (temp, "fonts/FZKaTong-M19T.ttf", 25);
				levelLabel->setPositionX(pMenu->getChildByTag(i)->getPosition().x+pMenu->getChildByTag(i)->boundingBox().size.width/4);
				levelLabel->setPositionY(pMenu->getChildByTag(i)->getPosition().y+pMenu->getChildByTag(i)->boundingBox().size.height/6);
				this->addChild(levelLabel, 2, 1000+i);

				delete user;
			}
			if(this->getChildByTag(10000+i)!=NULL){
				this->removeChildByTag(10000+i);
			}
			char temp[80];
			sprintf(temp, "image/friend/friend_%d.png", UserData::current->character[i]);
			CCSprite* character=CCSprite::create(temp);
			character->setScale(w/2/character->getContentSize().width);
			character->setPositionX(pMenu->getChildByTag(i)->getPosition().x);
			character->setPositionY(pMenu->getChildByTag(i)->getPosition().y-pMenu->getChildByTag(i)->boundingBox().size.height/5);
			this->addChild(character, 2, 10000+i);

		}
		else{
			userid[i]=0;
			if(this->getChildByTag(i)!=NULL){
				this->removeChildByTag(i);
				this->removeChildByTag(100+i);
				this->removeChildByTag(1000+i);
				this->removeChildByTag(10000+i);
			}
		}
		char temp[80];
		sprintf(temp, "image/ui/teamBackground_%d.png", UserData::current->group[i]);
		CCSprite* groupColor=CCSprite::create(temp);
		groupColor->setScale(this->getChildByTag(5000+i)->getScale());
		groupColor->setPosition(this->getChildByTag(5000+i)->getPosition());
		if(this->getChildByTag(5000+i)!=NULL){
			this->removeChildByTag(5000+i);
		}
		this->addChild(groupColor, 0, 5000+i);
	}
	if(!opeEnable){
		opeEnable=true;
		pMenu->setEnabled(true);
		pMenu2->setEnabled(true);
	}
}
void MainUISceneMultiplayerLayer::characterSelect(CCObject* pSender){
	CCMenuItem *m = (CCMenuItem*)(pSender);
	if(this->getChildByTag(m->getTag())== NULL){
		if(isIn){
			UserData::current->updatePos(UserData::current->myPosition+1, m->getTag()+1);
		}
		else{
			UserData::current->updatePos(m->getTag()+1, m->getTag()+1);
			needOpe=false;
		}
		if(!MainUIScene::ready){
			if(!MainUIScene::In){
				Client::client->connectRemote();
				MainUIScene::In=true;
			}
			MainUIScene::ready=true;
		}
		Show();
		needOpe=true;
	}
	else{
		 if(UserData::current->myPosition==m->getTag()){
			 UserData::current->updateHero(m->getTag()+1,UserData::current->character[m->getTag()]%8+1);
			 needOpe=false;
			 Show();
			 needOpe=true;
		 }
	}
}
void MainUISceneMultiplayerLayer::groupSelect(CCObject* pSender){
	CCMenuItem *m = (CCMenuItem*)(pSender);
	if(UserData::current->myPosition==m->getTag()){
		UserData::current->updateTeam(m->getTag()+1, (UserData::current->group[m->getTag()]+1)%8);
		Show();
	}
}
#endif
