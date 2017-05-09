#ifndef __MAINUI_SCENE_INVENTORY_LAYER__
#define __MAINUI_SCENE_INVENTORY_LAYER__
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "MainUISceneStoreLayer.hpp"
#include "ItemData.h"
#include "MainUISceneChooseLayer.hpp"
#include "UserData.hpp"
USING_NS_CC;
using namespace CocosDenshion;
/**
@brief
*/
class MainUISceneInventoryLayer : public CCLayer
{
	public:

	/**
	@brief 初始化函数
	*/
		virtual bool init(); //
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
		@brief 对话框确定选项
		*/
		void isSelect();//
		/**
		@brief 对话框取消选项
		*/
		void noSelect();//
		/**
		@brief 出售商品
		*/
		void sellItemSelected(); //
		/**
		@brief 穿戴道具
		*/
		void dressOnItemSelected();//
		/**
		@brief 脱下道具
		*/
		void putOffItemSelected(); //
		/**
		@brief 对应不同选择，设置不同selectNumber
		*/
		void select();//

		/**
		@brief
		*/
		CREATE_FUNC (MainUISceneInventoryLayer);
		/**
		@brief
		*/
		CCSize size;
		/**
		@brief
		*/
		CCSprite *ui_right, *menuItem, *buttonSurround;
		/**
		@brief
		*/
		CCLayer *chooseLayer, *selectLayer;
		/**
		@brief
		*/
		CCMenu* pMenu;
		/**
		@brief
		*/
		CCMenuItemImage *dressOnItem, *putOffItem, *sellItem;
		/**
		@brief
		*/
		int n, myNum, doubleTouchCount, selectItemNumber[MAX_NUM_ITEM], selectedNumber, sellEnable;
		/**
		@brief
		*/
		float w, h;
};
// onserverField "init" you need to initialize your instance
bool MainUISceneInventoryLayer::init()
{
	//目标：双指上下滑动可以翻滚
	//对每个物品显示一个背景格子，样子
	//对可以重叠的物品显示数量
	//点击一个道具即表示使用（例如头像，特效等等），右下角显示对勾，该道具背景变色
	if (!CCLayer::init() )
		{
			return false;
		}

	this->setTouchEnabled (true);
	size = CCDirector::sharedDirector()->getWinSize();
	ui_right = (CCSprite*) MainUIScene::mainUIScene->getChildByTag (10)->getChildByTag (10);
	menuItem = (CCSprite*) MainUIScene::mainUIScene->getChildByTag (10)->getChildByTag (11);
	w = (size.width - ui_right->boundingBox().size.width) / 3;
	h = w / 8 * 5;
	n = -1;
	myNum = 0;

	for (int i = 0; i < MAX_NUM_ITEM; i++)
		{
			if (UserData::current->item[i] > 0)
				{
					CCSprite *itemBackground = CCSprite::create ("image/ui/itemBackground.png");
					itemBackground->setScaleX (w / itemBackground->getContentSize().width);
					itemBackground->setScaleY (h / itemBackground->getContentSize().height);
					itemBackground->setPosition (ccp (ui_right->boundingBox().size.width + (myNum % 3) *w + w / 2, size.height - menuItem->boundingBox().size.height - (myNum / 3) *h - h / 2) );
					this->addChild (itemBackground, 0, 1000 + myNum);
					char itemPath[80];
					sprintf (itemPath, "image/store/face%d.png", i+1);
					CCSprite *item = CCSprite::create (itemPath);
					item->setScaleX (h / item->getContentSize().width * 1 / 2);
					item->setScaleY (h / item->getContentSize().height * 1 / 2);
					item->setPosition (ccp (itemBackground->getPositionX() - 4 * h / 10, itemBackground->getPositionY() ) );
					this->addChild (item, 2, myNum);
					CCLabelTTF *name = CCLabelTTF::create (itemName[i], "fonts/FZKaTong-M19T.ttf", 25);
					name->setPositionX (itemBackground->getPositionX() + h / 3);
					name->setPositionY (itemBackground->getPositionY() + h / 4);
					name->setColor (ccYELLOW);
					this->addChild (name, 2, 100 + myNum);
					char num[5];
					sprintf (num, "x %d", UserData::current->item[i]);
					CCLabelTTF *value = CCLabelTTF::create (num, "font/FZKaTong-M19T.ttf", 25);
					value->setPositionX (itemBackground->getPositionX() + h / 3);
					value->setPositionY (itemBackground->getPositionY() - h / 4);
					value->setColor (ccYELLOW);
					this->addChild (value, 2, 10000 + myNum);

					if (UserData::current->itemSelect[i])
						{
							CCSprite *ok = CCSprite::create ("image/ui/common_gate.png");
							ok->setScaleX (h * 2 / 5 / ok->getContentSize().width);
							ok->setScaleY (h * 2 / 5 / ok->getContentSize().height);
							ok->setPositionX (itemBackground->getPositionX() - w / 3);
							ok->setPositionY (itemBackground->getPositionY() - h / 3);
							this->addChild (ok, 2, 100000 + myNum);
						}

					selectItemNumber[myNum] = i;
					myNum++;
				}
		}

	return true;
}
void MainUISceneInventoryLayer::ccTouchesBegan (CCSet *pTouches, CCEvent *pEvent)
{
	if (pTouches->count() == 1)
		{
			CCTouch *touch = dynamic_cast<CCTouch*> (pTouches->anyObject() );
			CCPoint position = convertTouchToNodeSpace (touch);

			if (position.x - ui_right->boundingBox().size.width > 0 && position.y + menuItem->boundingBox().size.height - size.height < 0)
				{
					n = (int) (position.x - ui_right->boundingBox().size.width) / w + int ( (size.height - menuItem->boundingBox().size.height - position.y) / h) * 3;

					if (n < myNum and this->getChildByTag (1111) == NULL)
						{
							CCSprite *itemBackground = CCSprite::create ("image/ui/itemSelected.png");
							itemBackground->setScaleX (w / itemBackground->getContentSize().width);
							itemBackground->setScaleY (h / itemBackground->getContentSize().height);
							itemBackground->setPosition (ccp (ui_right->boundingBox().size.width + (n % 3) *w + w / 2, size.height - menuItem->boundingBox().size.height - (n / 3) *h - h / 2) );
							this->addChild (itemBackground, 1, 1111);
						}
				}
		}
}
void MainUISceneInventoryLayer::ccTouchesCancelled (CCSet *pTouches, CCEvent *pEvent)
{
}
void MainUISceneInventoryLayer::ccTouchesMoved (CCSet *pTouches, CCEvent *pEvent)
{
	if (pTouches->count() == 2)
		{
			if (this->getChildByTag (1111) != NULL)
				{
					this->removeChildByTag (1111, true);
				}

			if (h * ( (myNum + 2) / 3) + menuItem->boundingBox().size.height > size.height)
				{
					CCTouch* touch = dynamic_cast<CCTouch*> (*pTouches->begin() );
					float y = ccpAdd (this->getPosition(), touch->getDelta() ).y;

					if (y > (h * ( (myNum + 2) / 3) + menuItem->boundingBox().size.height) - size.height) y = (h * ( (myNum + 2) / 3) + menuItem->boundingBox().size.height) - size.height;

					if (y < 0) y = 0;

					this->setPositionY (y);
				}

			doubleTouchCount = 2;
		}

	if (pTouches->count() == 1 and doubleTouchCount == 0)
		{
			CCTouch *touch = dynamic_cast<CCTouch*> (pTouches->anyObject() );
			CCPoint position = convertTouchToNodeSpace (touch);

			if (position.x - ui_right->boundingBox().size.width > 0 && position.y + menuItem->boundingBox().size.height - size.height < 0)
				{
					n = (int) (position.x - ui_right->boundingBox().size.width) / w + int ( (size.height - menuItem->boundingBox().size.height - position.y) / h) * 3;

					if (n < MAX_NUM_ITEM)
						{
							this->getChildByTag (1111)->setPosition (ccp (ui_right->boundingBox().size.width + (n % 3) *w + w / 2, size.height - menuItem->boundingBox().size.height - (n / 3) *h - h / 2) );
						}

					else
						{
							if (this->getChildByTag (1111) != NULL)
								this->removeChildByTag (1111, true);
						}
				}

			else
				{
					if (this->getChildByTag (1111) != NULL)
						this->removeChildByTag (1111, true);
				}
		}
}
void MainUISceneInventoryLayer::ccTouchesEnded (CCSet *pTouches, CCEvent *pEvent)
{
	if (pTouches->count() == 1 and doubleTouchCount == 0 and n != -1 and n < myNum)
		{
			this->setTouchEnabled (false);
			chooseLayer = MainUISceneChooseLayer::create();
			this->addChild (chooseLayer, 4);
			pMenu = CCMenu::create();
			pMenu->setPosition ( CCPointZero );
			chooseLayer->addChild (pMenu, 1, 1);
			sellItem = CCMenuItemImage::create ("image/ui/button_normal.png", "image/ui/button_selected.png", this, menu_selector (MainUISceneInventoryLayer::sellItemSelected) );
			sellItem->setScaleX (size.width * 2 / 5 / sellItem->getContentSize().width);
			sellItem->setScaleY (size.height / 5 / sellItem->getContentSize().height);
			sellItem->setPosition (ccp (size.width / 2, size.height / 2 + sellItem->boundingBox().size.height) );
			pMenu->addChild (sellItem);
			CCLabelTTF *sellLabel = CCLabelTTF::create ("出售道具", "fonts/FZKaTong-M19T.ttf", 25);
			sellLabel->setPosition (sellItem->getPosition() );
			sellLabel->setColor (ccYELLOW);
			chooseLayer->addChild (sellLabel, 2);
			dressOnItem = CCMenuItemImage::create ("image/ui/button_normal.png", "image/ui/button_selected.png", this, menu_selector (MainUISceneInventoryLayer::dressOnItemSelected) );
			dressOnItem->setScaleX (size.width * 2 / 5 / dressOnItem->getContentSize().width);
			dressOnItem->setScaleY (size.height / 5 / dressOnItem->getContentSize().height);
			dressOnItem->setPosition (ccp (size.width / 2, size.height / 2) );
			pMenu->addChild (dressOnItem);
			CCLabelTTF *dressLabel = CCLabelTTF::create ("穿戴装备", "fonts/FZKaTong-M19T.ttf", 25);
			dressLabel->setPosition (dressOnItem->getPosition() );
			dressLabel->setColor (ccYELLOW);
			chooseLayer->addChild (dressLabel, 2);
			putOffItem = CCMenuItemImage::create ("image/ui/button_normal.png", "image/ui/button_selected.png", this, menu_selector (MainUISceneInventoryLayer::putOffItemSelected) );
			putOffItem->setScaleX (size.width * 2 / 5 / putOffItem->getContentSize().width);
			putOffItem->setScaleY (size.height / 5 / dressOnItem->getContentSize().height);
			putOffItem->setPosition (ccp (size.width / 2, size.height / 2 - putOffItem->boundingBox().size.height) );
			pMenu->addChild (putOffItem);
			CCLabelTTF *putLabel = CCLabelTTF::create ("脱去装备", "fonts/FZKaTong-M19T.ttf", 25);
			putLabel->setPosition (putOffItem->getPosition() );
			putLabel->setColor (ccYELLOW);
			chooseLayer->addChild (putLabel, 2);
			CCSprite *blackboard = CCSprite::create ("image/ui/black_blank.png");
			blackboard->setScaleX (dressOnItem->boundingBox().size.width / blackboard->getContentSize().width);
			blackboard->setScaleY (dressOnItem->boundingBox().size.height / blackboard->getContentSize().height);

			if (UserData::current->itemSelect[selectItemNumber[n]])
				{
					blackboard->setPosition (dressOnItem->getPosition() );
					chooseLayer->addChild (blackboard, 3);
				}

			else
				{
					blackboard->setPosition (putOffItem->getPosition() );
					chooseLayer->addChild (blackboard, 3);
				}

			buttonSurround = CCSprite::create ("image/ui/button_surround.png");
			buttonSurround->setScaleX (sellItem->boundingBox().size.width / buttonSurround->getContentSize().width);
			buttonSurround->setScaleY (sellItem->boundingBox().size.height / buttonSurround->getContentSize().height);
			buttonSurround->stopAllActions();
			CCAction *move = CCEaseExponentialOut::create (CCMoveTo::create (0.5, sellItem->getPosition() ) );
			buttonSurround->runAction (move);
			chooseLayer->addChild (buttonSurround, 3, 111);

			if (this->getChildByTag (1111) != NULL)
				this->removeChildByTag (1111, true);
		}

	else if (doubleTouchCount != 0)
		{
			doubleTouchCount--;

			if (this->getChildByTag (1111) != NULL)
				{
					this->removeChildByTag (1111, true);
				}
		}
}
void MainUISceneInventoryLayer::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate (this, 0);
	CCLayer::registerWithTouchDispatcher();
}
void MainUISceneInventoryLayer::noSelect()
{
	this->removeChild (selectLayer, true);
	//this->removeChildByTag(1111, true);
	this->setTouchEnabled (true);
}
void MainUISceneInventoryLayer::isSelect()
{
	if (selectedNumber == 1)
		{
			if (sellEnable == 1)
				{
					UserData::current->item[selectItemNumber[n]]--;
					UserData::current->coinNum += value[selectItemNumber[n]] / 2;

					if (UserData::current->item[selectItemNumber[n]] == 0)
						{
							this->removeChildByTag (n);
							this->removeChildByTag (100 + n);
							this->removeChildByTag (1000 + n);
							this->removeChildByTag (10000 + n);
						}

					else
						{
							this->removeChildByTag (10000 + n);
							char num[5];
							sprintf (num, "x %d", UserData::current->item[selectItemNumber[n]]);
							CCLabelTTF *value = CCLabelTTF::create (num, "font/FZKaTong-M19T.ttf", 25);
							value->setPositionX (this->getChildByTag (1000 + n)->getPositionX() + h / 3);
							value->setPositionY (this->getChildByTag (1000 + n)->getPositionY() - h / 4);
							value->setColor (ccYELLOW);
							this->addChild (value, 2, 10000 + n);
						}

					( (MainUIScene *) MainUIScene::mainUIScene->getChildByTag (10) )->showCoinNum (UserData::current->coinNum);
				}
		}

	else if (selectedNumber == 2)
		{
			UserData::current->itemSelect[selectItemNumber[n]] = true;
			CCSprite *ok = CCSprite::create ("image/ui/common_gate.png");
			ok->setScaleX (h * 2 / 5 / ok->getContentSize().width);
			ok->setScaleY (h * 2 / 5 / ok->getContentSize().height);
			ok->setPositionX (this->getChildByTag (1000 + n)->getPositionX() - w / 3);
			ok->setPositionY (this->getChildByTag (1000 + n)->getPositionY() - h / 3);
			this->addChild (ok, 2, 100000 + n);
			UserData::current->updateAvatar(selectItemNumber[n]+1);
			if(MainUIScene::mainUIScene->getChildByTag (10)->getChildByTag(999)!=NULL){
				MainUIScene::mainUIScene->getChildByTag (10)->removeChildByTag(999);
			}
			char facePath[80];
			sprintf (facePath, "image/face/face%d.png", selectItemNumber[n]+1);
			CCSprite*face = CCSprite::create(facePath);
			face->setScale (size.height / ui_right->getContentSize().height * 1.12);
			face->setPosition (ccp (ui_right->boundingBox().size.width / 128 * 67, size.height / 64 * 39) );
			MainUIScene::mainUIScene->getChildByTag (10)->addChild (face, 2, 999);
		}

	else
		{
			UserData::current->itemSelect[selectItemNumber[n]] = false;
			this->removeChildByTag (100000 + n);
		}

	this->removeChild (selectLayer, true);
	this->setTouchEnabled (true);
}
void MainUISceneInventoryLayer::sellItemSelected()
{
	//buttonSurround->stopAllActions();
	//CCAction *move=CCEaseExponentialOut::create(CCMoveTo::create(0.5,sellItem->getPosition()));
	//buttonSurround->runAction(move);
	selectedNumber = 1;
	this->removeChild (chooseLayer, true);
	select();
}
void MainUISceneInventoryLayer::dressOnItemSelected()
{
	buttonSurround->stopAllActions();
	CCAction *move = CCEaseExponentialOut::create (CCMoveTo::create (0.5, dressOnItem->getPosition() ) );
	buttonSurround->runAction (move);

	if (!UserData::current->itemSelect[selectItemNumber[n]])
		{
			selectedNumber = 2;
			this->removeChild (chooseLayer, true);
			select();
		}

	//this->removeChild(chooseLayer, true);
}
void MainUISceneInventoryLayer::putOffItemSelected()
{
	buttonSurround->stopAllActions();
	CCAction *move = CCEaseExponentialOut::create (CCMoveTo::create (0.5, putOffItem->getPosition() ) );
	buttonSurround->runAction (move);

	if (UserData::current->itemSelect[selectItemNumber[n]])
		{
			selectedNumber = 3;
			this->removeChild (chooseLayer, true);
			select();
		}

	//this->removeChild(chooseLayer, true);
}
void MainUISceneInventoryLayer::select()
{
	selectLayer = MainUISceneChooseLayer::create();
	this->addChild (selectLayer, 4);
	CCSprite *blackboard = CCSprite::create ("image/ui/black_blank.png");
	blackboard->setScaleX (size.width / 2 / blackboard->getContentSize().width);
	blackboard->setScaleY (size.height * 9 / 10 / blackboard->getContentSize().height);
	blackboard->setPosition (ccp (size.width / 2, size.height / 2) );
	selectLayer->addChild (blackboard, 0, 1);
	CCSprite *blackboard2 = CCSprite::create ("image/ui/black_blank.png");
	blackboard2->setScaleX (size.width / 2 / blackboard->getContentSize().width);
	blackboard2->setScaleY (size.height * 9 / 20 / blackboard->getContentSize().height);
	blackboard2->setPosition (ccp (size.width / 2, size.height * 3 / 4) );
	selectLayer->addChild (blackboard, 1, 2);
	char itemSelect[80];
	sprintf (itemSelect, "image/store/face%d.png", selectItemNumber[n]+1);
	CCSprite* selectedItem = CCSprite::create (itemSelect);
	selectedItem->setScale (blackboard->boundingBox().size.height / 3 / selectedItem->getContentSize().height);
	selectedItem->setPosition (ccp (size.width / 2, size.height / 2 + blackboard->boundingBox().size.height / 5) );
	selectLayer->addChild (selectedItem, 2);
	CCLabelTTF *message;

	if (selectedNumber == 1)
		{
			if (UserData::current->item[selectItemNumber[n]] == 1 && UserData::current->itemSelect[selectItemNumber[n]])
				{
					message = CCLabelTTF::create ("仅有一件 正在穿戴 \n    无法出售", "fonts/FZKaTong-M19T.ttf", 30);
					sellEnable = 0;
				}

			else
				{
					message = CCLabelTTF::create ("确认出售？", "fonts/FZKaTong-M19T.ttf", 30);
					sellEnable = 1;
				}

			char valuetmp[10];
			sprintf (valuetmp, "￥ %d", int (value[selectItemNumber[n]] / 2) );
			CCLabelTTF *numLabel = CCLabelTTF::create (valuetmp, "fonts/FZKaTong-M19T.ttf", 30);
			numLabel->setPosition (ccp (size.width / 2, size.height / 2) );
			numLabel->setColor (ccYELLOW);
			selectLayer->addChild (numLabel, 2);
		}

	else if (selectedNumber == 2)
		{
			message = CCLabelTTF::create ("确认穿戴装备？", "fonts/FZKaTong-M19T.ttf", 30);
		}

	else
		message = CCLabelTTF::create ("确认脱下装备？", "fonts/FZKaTong-M19T.ttf", 30);

	message->setPosition (ccp (size.width / 2, size.height / 3) );
	message->setColor (ccYELLOW);
	selectLayer->addChild (message, 2);
	CCMenu* Menu = CCMenu::create();
	Menu->setPosition ( CCPointZero );
	selectLayer->addChild (Menu, 4);
	CCLabelTTF *isLabel = CCLabelTTF::create ("确认", "fonts/FZKaTong-M19T.ttf", 32);
	//isSelected->setPosition(ccp(size.width/2-blackboard->boundingBox().size.width/4, size.height/5));
	isLabel->setColor (ccYELLOW);
	CCMenuItemLabel *isItem = CCMenuItemLabel::create (isLabel, this, menu_selector (MainUISceneInventoryLayer::isSelect) );
	isItem->setPosition (ccp (size.width / 2 - blackboard->boundingBox().size.width / 4, size.height / 6) );
	Menu->addChild (isItem);
	CCLabelTTF *noLabel = CCLabelTTF::create ("取消", "fonts/FZKaTong-M19T.ttf", 32);
	//noSelected->setPosition(ccp(size.width/2+blackboard->boundingBox().size.width/4, size.height/5));
	noLabel->setColor (ccYELLOW);
	CCMenuItemLabel *noItem = CCMenuItemLabel::create (noLabel, this, menu_selector (MainUISceneInventoryLayer::noSelect) );
	noItem->setPosition (ccp (size.width / 2 + blackboard->boundingBox().size.width / 4, size.height / 6) );
	Menu->addChild (noItem);
}
#endif
