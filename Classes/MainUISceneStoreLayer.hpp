#ifndef __MAINUI_SCENE_STORE_LAYER__
#define __MAINUI_SCENE_STORE_LAYER__
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "ItemData.h"
USING_NS_CC;

using namespace CocosDenshion;
class MainUISceneStoreLayer : public CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
	virtual void ccTouchesBegan(CCSet* touches, CCEvent* pEvent);
	virtual void ccTouchesMoved(CCSet* touches, CCEvent* pEvent);
	virtual void ccTouchesEnded(CCSet* touches, CCEvent* pEvent);
	virtual void ccTouchesCancelled(CCSet* touches, CCEvent* pEvent);
    virtual void registerWithTouchDispatcher();
    void isSelect();
    void noSelect();
    // a selector callback
    // implement the "static node()" method manually
    CREATE_FUNC(MainUISceneStoreLayer);
	CCSize size;
	CCSprite *ui_right, *menuItem;
	CCLayer *chooseLayer;
	CCMenu* pMenu;
	int doubleTouchCount,n;

	float w,h;
};
// onserverField "init" you need to initialize your instance
#include "MainUISceneChooseLayer.hpp"
bool MainUISceneStoreLayer::init()
{
	if (!CCLayer::init()){
		return false;
	}
	this->setTouchEnabled(true);
	size = CCDirector::sharedDirector()->getWinSize();
	ui_right =(CCSprite*) MainUIScene::mainUIScene->getChildByTag(10)->getChildByTag(10);
	menuItem =(CCSprite*) MainUIScene::mainUIScene->getChildByTag(10)->getChildByTag(11);

	n=-1;

	w=(size.width-ui_right->boundingBox().size.width)/3;
	h=w/8*5;
	for(int i=0; i<MAX_NUM_ITEM; i++)
	{
	    CCSprite *itemBackground=CCSprite::create("image/ui/itemBackground.png");
		itemBackground->setScaleX(w/itemBackground->getContentSize().width);
		itemBackground->setScaleY(h/itemBackground->getContentSize().height);
		itemBackground->setPosition(ccp(ui_right->boundingBox().size.width+(i%3)*w+w/2, size.height-menuItem->boundingBox().size.height-(i/3)*h-h/2));
		this->addChild(itemBackground);

		char itemPath[80];
		sprintf(itemPath, "image/store/face%d.png", i+1);
		CCSprite *item=CCSprite::create(itemPath);
		item->setScaleX(h/item->getContentSize().width*1/2);
		item->setScaleY(h/item->getContentSize().height*1/2);
		item->setPosition(ccp(itemBackground->getPositionX()-4*h/10,itemBackground->getPositionY()));
		this->addChild(item,2,i);

		CCLabelTTF *name = CCLabelTTF::create(itemName[i], "fonts/FZKaTong-M19T.ttf", 25);
		name->setPositionX(itemBackground->getPositionX()+h/3);
		name->setPositionY(itemBackground->getPositionY()+h/4);
		name->setColor(ccYELLOW);
		this->addChild(name, 2);

		char itemvalue[80];
		sprintf(itemvalue, "￥ %d", value[i]);
		CCLabelTTF *value=CCLabelTTF::create(itemvalue, "font/FZKaTong-M19T.ttf", 25);
		value->setPositionX(itemBackground->getPositionX()+h/3);
		value->setPositionY(itemBackground->getPositionY()-h/4);
		value->setColor(ccYELLOW);
		this->addChild(value,2);
	}
	doubleTouchCount=0;
	return true;
}
void MainUISceneStoreLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent){
	if(pTouches->count()==1){
		CCTouch *touch = dynamic_cast<CCTouch*>(pTouches->anyObject());
		CCPoint position=convertTouchToNodeSpace(touch);
		if(position.x-ui_right->boundingBox().size.width>0 && position.y+menuItem->boundingBox().size.height-size.height<0){
			n=(int)(position.x-ui_right->boundingBox().size.width)/w+int((size.height-menuItem->boundingBox().size.height-position.y)/h)*3;
			if(n<MAX_NUM_ITEM and this->getChildByTag(1111)==NULL){
			    CCSprite *itemBackground=CCSprite::create("image/ui/itemSelected.png");
				itemBackground->setScaleX(w/itemBackground->getContentSize().width);
				itemBackground->setScaleY(h/itemBackground->getContentSize().height);
				itemBackground->setPosition(ccp(ui_right->boundingBox().size.width+(n%3)*w+w/2, size.height-menuItem->boundingBox().size.height-(n/3)*h-h/2));
				this->addChild(itemBackground,1,1111);
			}
		}
	}
}
void MainUISceneStoreLayer::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent){
}
void MainUISceneStoreLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent){
	if(pTouches->count()==2){
		if(this->getChildByTag(1111)!=NULL){
			this->removeChildByTag(1111, true);
		}
			if(h*((MAX_NUM_ITEM+2)/3)+menuItem->boundingBox().size.height > size.height){
				CCTouch* touch=dynamic_cast<CCTouch*>(*pTouches->begin());
				float y=ccpAdd(this->getPosition(),touch->getDelta()).y;
				if(y>(h*((MAX_NUM_ITEM+2)/3)+menuItem->boundingBox().size.height)-size.height) y=(h*((MAX_NUM_ITEM+2)/3)+menuItem->boundingBox().size.height)-size.height;
				if(y<0) y=0;
				this->setPositionY(y);
			}
		doubleTouchCount=2;
	}
	if(pTouches->count()==1 and doubleTouchCount==0){
		CCTouch *touch = dynamic_cast<CCTouch*>(pTouches->anyObject());
		CCPoint position=convertTouchToNodeSpace(touch);
		if(position.x-ui_right->boundingBox().size.width>0 && position.y+menuItem->boundingBox().size.height-size.height<0){
			n=(int)(position.x-ui_right->boundingBox().size.width)/w+int((size.height-menuItem->boundingBox().size.height-position.y)/h)*3;
			if(n<MAX_NUM_ITEM){
				this->getChildByTag(1111)->setPosition(ccp(ui_right->boundingBox().size.width+(n%3)*w+w/2, size.height-menuItem->boundingBox().size.height-(n/3)*h-h/2));
			}
			else{
				if(this->getChildByTag(1111)!=NULL)
					this->removeChildByTag(1111,true);
			}
		}
		else{
			if(this->getChildByTag(1111)!=NULL)
				this->removeChildByTag(1111,true);
		}
	}
}
void MainUISceneStoreLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent){
	if(pTouches->count()==1 and doubleTouchCount==0 and n!=-1 and n<MAX_NUM_ITEM){
		this->setTouchEnabled(false);
		chooseLayer=MainUISceneChooseLayer::create();
		MainUIScene::mainUIScene->addChild(chooseLayer,4);

		CCSprite *blackboard = CCSprite::create("image/ui/black_blank.png");
		blackboard->setScaleX(size.width/2/blackboard->getContentSize().width);
		blackboard->setScaleY(size.height*9/10/blackboard->getContentSize().height);
		blackboard->setPosition(ccp(size.width/2, size.height/2));
		chooseLayer->addChild(blackboard,0,1);

		CCSprite *blackboard2 = CCSprite::create("image/ui/black_blank.png");
		blackboard2->setScaleX(size.width/2/blackboard->getContentSize().width);
		blackboard2->setScaleY(size.height*9/20/blackboard->getContentSize().height);
		blackboard2->setPosition(ccp(size.width/2, size.height*3/4));
		chooseLayer->addChild(blackboard,1);

		char itemSelect[80];
		sprintf(itemSelect, "image/store/face%d.png", n+1);
		CCSprite* selectedItem = CCSprite::create(itemSelect);
		selectedItem->setScale(blackboard->boundingBox().size.height/3/selectedItem->getContentSize().height);
		selectedItem->setPosition(ccp(size.width/2,size.height/2+blackboard->boundingBox().size.height/5));
		chooseLayer->addChild(selectedItem,2);

		CCLabelTTF *message = CCLabelTTF::create("确定购买？", "fonts/FZKaTong-M19T.ttf", 30);
		message->setPosition(ccp(size.width/2, size.height/3));
		message->setColor(ccYELLOW);
		chooseLayer->addChild(message, 2);

		CCLabelTTF *numLabel = CCLabelTTF::create("x 1", "fonts/FZKaTong-M19T.ttf", 30);
		numLabel->setPosition(ccp(size.width/2, size.height/2));
		numLabel->setColor(ccYELLOW);
		chooseLayer->addChild(numLabel,2);

		pMenu=CCMenu::create();
		pMenu->setPosition( CCPointZero );
		chooseLayer->addChild(pMenu,4);
		CCLabelTTF *isLabel = CCLabelTTF::create("是", "fonts/FZKaTong-M19T.ttf", 32);
		//isSelected->setPosition(ccp(size.width/2-blackboard->boundingBox().size.width/4, size.height/5));
		isLabel->setColor(ccYELLOW);
		CCMenuItemLabel *isItem=CCMenuItemLabel::create(isLabel,this,menu_selector(MainUISceneStoreLayer::isSelect));
		isItem->setPosition(ccp(size.width/2-blackboard->boundingBox().size.width/4, size.height/6));
		pMenu->addChild(isItem);

		CCLabelTTF *noLabel = CCLabelTTF::create("否", "fonts/FZKaTong-M19T.ttf", 32);
		//noSelected->setPosition(ccp(size.width/2+blackboard->boundingBox().size.width/4, size.height/5));
		noLabel->setColor(ccYELLOW);
		CCMenuItemLabel *noItem=CCMenuItemLabel::create(noLabel,this,menu_selector(MainUISceneStoreLayer::noSelect));
		noItem->setPosition(ccp(size.width/2+blackboard->boundingBox().size.width/4, size.height/6));
		pMenu->addChild(noItem);
	}
	else if(doubleTouchCount!=0){
		doubleTouchCount--;
		if(this->getChildByTag(1111)!=NULL){
			this->removeChildByTag(1111, true);
		}
	}
}
void MainUISceneStoreLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, 0);
    CCLayer::registerWithTouchDispatcher();
}
void MainUISceneStoreLayer::noSelect()
{
	MainUIScene::mainUIScene->removeChild(chooseLayer, true);
	this->removeChildByTag(1111, true);
	this->setTouchEnabled(true);
}
void MainUISceneStoreLayer::isSelect()
{
	UserData::current->item[n]++;
	UserData::current->coinNum-=value[n];
	UserData::current->updateMoney();
	MainUIScene::mainUIScene->removeChild(chooseLayer, true);
	((MainUIScene *)MainUIScene::mainUIScene->getChildByTag(10))->showCoinNum(UserData::current->coinNum);
	this->removeChildByTag(1111, true);
	this->setTouchEnabled(true);

}
#endif
