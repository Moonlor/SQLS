#ifndef __GAME_SCENE__
#define __GAME_SCENE__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "HeroBazzi.hpp"
#include "HeroCappi.hpp"
#include "HeroDao.hpp"
#include "HeroMarid.hpp"
#include "AniReader.hpp"
#include "BubbleClass.hpp"

#include "PropsSpeed.hpp"
#include "PropsRange.hpp"
#include "PropsBubbleNum.hpp"
#include "PropsGold.hpp"
#include "PropsSilver.hpp"
#include "Client.hpp"
#include "math.h"
USING_NS_CC;
using namespace CocosDenshion;
class GameScene : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback    	map->setScale(MAX(size.height/map->getContentSize().height,size.width/map->getContentSize().width));
    void menuButtonCallback(CCObject* pSender);
    void menuCloseCallback(CCObject* pSender);
    // implement the "static node()" method manually
    CREATE_FUNC(GameScene);
    Hero* hero[10];
    int heronum;
    int myheroid;
    Bubble* bubble[85];
    int bhead;
    int btail;
    Props* props[600];
    int propsnum;
	CCSize size;
	virtual void ccTouchesBegan(CCSet* touches, CCEvent* pEvent);
	virtual void ccTouchesMoved(CCSet* touches, CCEvent* pEvent);
	virtual void ccTouchesEnded(CCSet* touches, CCEvent* pEvent);
	virtual void ccTouchesCancelled(CCSet* touches, CCEvent* pEvent);
    virtual void registerWithTouchDispatcher();
    void sendmessage();
    void getcommand();
    void createhero(int type,CCPoint a,float scale);
    void createprops(int type,CCPoint a,float scale);
    void createbubble(CCPoint a,float scale,int range,int heroid);
    void heromove(CCPoint a,int heroid,CCPoint orig);
    void heroencase(int heroid);
    void herolive(int heroid);
    void herodie(int heroid);
    void herogetprops(int heroid,int type);
    void BombCallback(CCNode* obj,void* id);
    void DieCallback(CCNode* obj,void* id);
    void BubbleBomb(int idx);
    void gameover(int type);
    void test();
	CCPoint PositionToTileCoord(CCPoint cocosCoord);
	CCPoint TileCoordToPosition(CCPoint tileCoord);
    CCTMXTiledMap *map;
    //static
    CCTMXLayer *mapBackgroundLayer,*mapItemLayer;
    static CCScene *gameScene;
    CCLayer *statusLayer,*chatLayer;
    int doubleTouchCount,tripleTouchCount;
    CCPoint firstTripleTouchPoint;
    char sMsg[255], rMsg[255];
    void getMsg();
    void sendMoveMsg();
    CCLabelTTF * message;
    bool *fbomb;
};
#include "UserData.hpp"
#include "GameSceneChatLayer.hpp"
#include "GameSceneMessageLayer.hpp"
#include "GameSceneStatusLayer.hpp"
CCScene *GameScene::gameScene;
//CCTMXLayer *GameScene::mapBackgroundLayer,*GameScene::mapItemLayer;
CCScene* GameScene::scene()
{
    // 'scene' is an autorelease object
    gameScene = CCScene::create();
    
    // 'layer' is an autorelease object
    CCLayer *gameLayer = GameScene::create();
    // add layer as a child to scene
    gameScene->addChild(gameLayer,0);
    // return the scene
    return gameScene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
	bhead = 0;
	btail = 0;
	heronum = 0;
	propsnum = 0;
    if ( !CCLayer::init() )
    {
        return false;
    }
    memset(sMsg, 0,sizeof(sMsg));
    memset(rMsg, 0,sizeof(rMsg));
    this->setTouchEnabled(true);
    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("audio/bg_1.ogg");
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic("audio/bg_1.ogg",true);
	SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(1.0f);
	SimpleAudioEngine::sharedEngine()->preloadEffect("audio/ef_0.ogg");
	SimpleAudioEngine::sharedEngine()->preloadEffect("audio/ef_2.ogg");
	SimpleAudioEngine::sharedEngine()->playEffect("audio/ef_7.ogg");
    size = CCDirector::sharedDirector()->getWinSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    statusLayer=GameSceneStatusLayer::create();
    statusLayer->setPosition(ccp(0,size.height));
    chatLayer=GameSceneChatLayer::create();
    chatLayer->setPosition(ccp(0,-size.height));
    gameScene->addChild(chatLayer,1);
    gameScene->addChild(statusLayer,1);
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create("image/ui/CloseNormal.png","image/ui/CloseSelected.png",this,menu_selector(GameScene::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(size.width - pCloseItem->getContentSize().width/2 ,
                                pCloseItem->getContentSize().height/2));


	CCMenuItemImage *pButtonItem = CCMenuItemImage::create("image/ui/item_BB.png","image/ui/item_BB.png",this,menu_selector(GameScene::menuButtonCallback));
	pButtonItem->setScale(2.0);
	pButtonItem->setPosition(ccp(size.width/10*9 - pButtonItem->getContentSize().width/2 ,
	                                size.height/2+ pButtonItem->getContentSize().height/2));
	// create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, pButtonItem,NULL);
    pMenu->setPosition(CCPointZero);
    gameScene->addChild(pMenu, 20);
    map=CCTMXTiledMap::create("map/map_fact.tmx");
    mapBackgroundLayer=map->layerNamed("background");
    mapItemLayer=map->layerNamed("item");
    gameScene->addChild(map,-20);
    map->setVisible(false);
    if(map->getContentSize().height<size.height or map->getContentSize().width<size.width){
    	mapBackgroundLayer->setScale(MAX(size.height/map->getContentSize().height,size.width/map->getContentSize().width));
    	mapItemLayer->setScale(MAX(size.height/map->getContentSize().height,size.width/map->getContentSize().width));
    }
    this->addChild(mapItemLayer,3);
    this->addChild(mapBackgroundLayer,0);
    AniReader::mapw = mapBackgroundLayer->getMapTileSize().width*mapBackgroundLayer->getScale();
    AniReader::maph = mapBackgroundLayer->getMapTileSize().height*mapBackgroundLayer->getScale();

    for(int i=0; i<8;i++){
    	if(UserData::current->roomlist[i]>0){
    		if(UserData::current->roomlist[i]==UserData::current->userid){
    		    			myheroid=heronum;
    		}
    		createhero(UserData::current->character[i],TileCoordToPosition(PositionToTileCoord(ccp(size.width/2,size.height/2))),0.8*mapBackgroundLayer->getScale());
    		hero[heronum-1]->teamid=UserData::current->group[i];
    	}
    }
    doubleTouchCount=0;tripleTouchCount=0;
   schedule(schedule_selector(GameScene::getMsg));
     return true;
}

void GameScene::createhero(int type,CCPoint a,float s)
{
	if (type == 1 or type == 5)
	{
		hero[heronum] = new HeroBazzi();
	}
	else if (type == 2 or type == 6)
	{
		hero[heronum] = new HeroDao();
	}
	else if (type == 3 or type == 7)
	{
		hero[heronum] = new HeroCappi();
	}
	else if (type == 4 or type == 8)
	{
		hero[heronum] = new HeroMarid();
	}
	if (type < 5)
		hero[heronum]->createhero(a,s,heronum,0);
	else
		hero[heronum]->createhero(a,s,heronum,1);
	this->addChild(hero[heronum]->sprite,11);
	heronum++;
}

void GameScene::createbubble(CCPoint a,float s,int r,int heroid)
{
	if (bubble[btail] != NULL)
	{
		delete bubble[btail];
	}
	bubble[btail] = new Bubble(a,s,btail,hero[heroid]->bubble_range,heroid);
	CCFiniteTimeAction *delay;
	delay = CCDelayTime::create(3);
	CCCallFuncND * bomb = CCCallFuncND::create(this,callfuncND_selector(GameScene::BombCallback),(void*)&bubble[btail]->idx);
	CCAction *action;
	action = CCSequence::create(delay,bomb,NULL);
	action->setTag(0);
	bubble[btail]->sprite->runAction(action);
	this->addChild(bubble[btail]->layer,10);
	btail++;
	if (btail > 80)
	{
		btail -= 80;
	}
}

void GameScene::createprops(int type,CCPoint a,float scale)
{
	  if (type == 1)
	  {
		  props[propsnum] = new PropsSpeed();
	  }
	  else if (type == 2)
	  {
		  props[propsnum] = new PropsRange();
	  }
	  else if (type == 3)
	  {
		  props[propsnum] = new PropsBubbleNum();
	  }
	  else if (type == 4)
	  {
		  props[propsnum] = new PropsGold();
	  }
	  else if (type == 5)
	  {
		  props[propsnum] = new PropsSilver();
	  }
	  props[propsnum]->create(a,scale);
	  this->addChild(props[propsnum]->sprite,9);
	  propsnum++;
}

void GameScene::heromove(CCPoint a,int heroid,CCPoint orig)
{
	if (hero[heroid]->isfree && hero[heroid]->islive)
	{
		bool t = true;
		int x = PositionToTileCoord(a).x;
		int y = PositionToTileCoord(a).y;
		int gid=mapItemLayer->tileGIDAt(PositionToTileCoord(a));
		if (!(gid==0 or !mapItemLayer->tileAt(PositionToTileCoord(a))->isVisible()))
			t = false;
		for (int i = 0; i <= 80; i++)
		if (bubble[i] != NULL && bubble[i]->isdelay)
		{
			int x1 = PositionToTileCoord(bubble[i]->sprite->getPosition()).x;
			int y1 = PositionToTileCoord(bubble[i]->sprite->getPosition()).y;
			if (x1 == x && y == y1)
			{
				t = false;
				break;
			}
		}
		if (t)
		{
			int x2 = PositionToTileCoord(hero[heroid]->sprite->getPosition()).x;
			int y2 = PositionToTileCoord(hero[heroid]->sprite->getPosition()).y;
			for (int i = 0; i < propsnum; i++)
			if (props[i]->isdelay)
			{
				int x1 = PositionToTileCoord(props[i]->sprite->getPosition()).x;
				int y1 = PositionToTileCoord(props[i]->sprite->getPosition()).y;
				if (x1 == x2 && y1 == y2)
				{
					herogetprops(heroid,props[i]->type);
					props[i]->remove();
					break;
				}
			}
			hero[heroid]->moveto(a);
		}
		else{
			hero[heroid]->moveto(TileCoordToPosition(orig));
		}
	}
}

void GameScene::herogetprops(int heroid,int type)
{
	if (type == 1)
	{
		if (hero[heroid]->speed < 10)
		hero[heroid]->speed++;
	}
	else if (type == 2)
	{
		if (hero[heroid]->bubble_range < 10)
		hero[heroid]->bubble_range++;
	}
	else if (type == 3)
	{
		if (hero[heroid]->bubble_num < 10)
		hero[heroid]->bubble_num++;
	}
}
void GameScene::heroencase(int heroid)
{
	hero[heroid]->encase();
	CCFiniteTimeAction *delay;
	delay = CCDelayTime::create(3);
	CCCallFuncND * die = CCCallFuncND::create(this,callfuncND_selector(GameScene::DieCallback),(void*)&hero[heroid]->idx);
	CCAction *action;
	action = CCSequence::create(delay,die,NULL);
	action->setTag(0);
	hero[heroid]->sprite->runAction(action);
}

void GameScene::herodie(int heroid)
{
	hero[heroid]->die();
	if (hero[heroid]->teamid == hero[myheroid]->teamid)
	{
		bool t = true;
		for (int i = 0; i < heronum; i++)
		if (hero[i]->teamid == hero[myheroid]->teamid && hero[i]->islive)
		{
			t = false;
			break;
		}
		if  (t)
		{
			gameover(0);
		}
	} else
	{
		bool t = true;
		for (int i = 0; i < heronum; i++)
		if (hero[i]->teamid != hero[myheroid]->teamid && hero[i]->islive)
		{
			t = false;
			break;
		}
		if  (t)
		{
			gameover(1);
		}
	}
}

void GameScene::gameover(int type)
{
	this->setTouchEnabled(false);
	if(statusLayer->getPositionY()!=size.height){
		CCAction *move=CCEaseExponentialOut::create(CCMoveTo::create(0.5,ccp(0,size.height)));
		statusLayer->runAction(move);
	}
	if(chatLayer->getPositionY()!=-size.height){
		CCAction *move=CCEaseExponentialOut::create(CCMoveTo::create(0.5,ccp(0,-size.height)));
		chatLayer->runAction(move);
	}
	GameSceneMessageLayer* messageLayer=GameSceneMessageLayer::create();
	messageLayer->setmessage(type);
	gameScene->addChild(messageLayer,2);
}
void GameScene::menuCloseCallback(CCObject* pSender)
{
	this->setTouchEnabled(false);
	SimpleAudioEngine::sharedEngine()->playEffect("audio/ef_0.ogg");
	if(statusLayer->getPositionY()!=size.height){
		CCAction *move=CCEaseExponentialOut::create(CCMoveTo::create(0.5,ccp(0,size.height)));
		statusLayer->runAction(move);
	}
	if(chatLayer->getPositionY()!=-size.height){
		CCAction *move=CCEaseExponentialOut::create(CCMoveTo::create(0.5,ccp(0,-size.height)));
		chatLayer->runAction(move);
	}
	CCLayer* messageLayer=GameSceneMessageLayer::create();
	gameScene->addChild(messageLayer,2);

}

void GameScene::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent){
}
void GameScene::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent){
}
void GameScene::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent){
	if(pTouches->count()==2 and tripleTouchCount==0){
		CCTouch* touch=dynamic_cast<CCTouch*>(*pTouches->begin());
		float x=ccpAdd(this->getPosition(),touch->getDelta()).x;
		float y=ccpAdd(this->getPosition(),touch->getDelta()).y;
		if(x>0)x=0;if(y>0)y=0;
		if(x<size.width-mapBackgroundLayer->boundingBox().size.width)x=size.width-mapBackgroundLayer->boundingBox().size.width;
		if(y<size.height-mapBackgroundLayer->boundingBox().size.height)y=size.height-mapBackgroundLayer->boundingBox().size.height;
		this->setPosition(ccp(x,y));
		doubleTouchCount=2;
	}
	if(pTouches->count()==3 and tripleTouchCount==0){
		firstTripleTouchPoint=dynamic_cast<CCTouch*>(*pTouches->begin())->getLocationInView();
		tripleTouchCount=3;
	}
	if(pTouches->count()==1 and doubleTouchCount==0 and tripleTouchCount==0)
	{
		if (hero[myheroid]->isfree && hero[myheroid]->islive)
		{
		CCTouch* touch=dynamic_cast<CCTouch*>(pTouches->anyObject());
		CCPoint aim=PositionToTileCoord(convertTouchToNodeSpace(touch));
		CCPoint origin=PositionToTileCoord(hero[myheroid]->sprite->getPosition());
		if(aim.x==origin.x and aim.y==origin.y)return;
		if(abs(aim.x-origin.x)<abs(aim.y-origin.y)){
			sprintf(sMsg, "%d %d %f %f %f %f",0, myheroid, origin.x, origin.y+(aim.y-origin.y)/abs(aim.y-origin.y), origin.x, origin.y);
			scheduleOnce(schedule_selector(GameScene::sendMoveMsg), 0.03f);
		}
		else{
			sprintf(sMsg,"%d %d %f %f %f %f",0, myheroid, origin.x+(aim.x-origin.x)/abs(aim.x-origin.x), origin.y, origin.x, origin.y );
			scheduleOnce(schedule_selector(GameScene::sendMoveMsg), 0.03f);
		}
	}
	}
}
void GameScene::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent){
	if(tripleTouchCount==3){
		CCTouch* touch=dynamic_cast<CCTouch*>(*pTouches->begin());
		if(touch->getLocationInView().y-firstTripleTouchPoint.y>100){
			if(chatLayer->getPositionY()!=-size.height){
				chatLayer->stopAllActions();
				CCAction *move=CCEaseExponentialOut::create(CCMoveTo::create(0.5,ccp(0,-size.height)));
				chatLayer->runAction(move);
			}else{
				statusLayer->stopAllActions();
				CCAction *move=CCEaseExponentialOut::create(CCMoveTo::create(0.5,ccp(0,0)));
				statusLayer->runAction(move);
			}
		}
		if(touch->getLocationInView().y-firstTripleTouchPoint.y<-100){
			if(statusLayer->getPositionY()!=size.height){
				statusLayer->stopAllActions();
				CCAction *move=CCEaseExponentialOut::create(CCMoveTo::create(0.5,ccp(0,size.height)));
				statusLayer->runAction(move);
			}else{
				chatLayer->stopAllActions();
				CCAction *move=CCEaseExponentialOut::create(CCMoveTo::create(0.5,ccp(0,0)));
				chatLayer->runAction(move);
			}
		}
	}
	if(doubleTouchCount>0)doubleTouchCount--;
	if(tripleTouchCount>0)tripleTouchCount--;
	if(pTouches->count()==1 and doubleTouchCount==0 and tripleTouchCount==0)
	{
		if(statusLayer->getPositionY()!=size.height){
			statusLayer->stopAllActions();
			CCAction *move=CCEaseExponentialOut::create(CCMoveTo::create(0.5,ccp(0,size.height)));
			statusLayer->runAction(move);
		}
		if(chatLayer->getPositionY()!=-size.height){
			chatLayer->stopAllActions();
			CCAction *move=CCEaseExponentialOut::create(CCMoveTo::create(0.5,ccp(0,-size.height)));
			chatLayer->runAction(move);
		}
		if (hero[myheroid]->isfree && hero[myheroid]->islive)
		{
			//hero[myheroid]->clearMove();
			//hero[myheroid]->stand();
			//hero->encase();
		}
	}

}
void GameScene::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, 0);
    CCLayer::registerWithTouchDispatcher();
}
CCPoint GameScene::PositionToTileCoord(CCPoint cocosCoord)
{
	int x=cocosCoord.x/mapBackgroundLayer->getScale()/map->getTileSize().width;
	int y=((mapBackgroundLayer->getLayerSize().height-cocosCoord.y)/mapBackgroundLayer->getScale()) / map->getTileSize().height;
	y=y+map->getMapSize().height-1;
	return ccp(x,y);

}
CCPoint GameScene::TileCoordToPosition(CCPoint tileCoord){
	tileCoord.y-=map->getMapSize().height-1;
	float x=map->getTileSize().width*(0.5+tileCoord.x)*mapBackgroundLayer->getScale();
	float y=(mapBackgroundLayer->getLayerSize().height-map->getTileSize().height*(0.5+tileCoord.y-1))*mapBackgroundLayer->getScale();
	return ccp(x,y);
}
void GameScene::menuButtonCallback(CCObject* pSender)
{
	if (hero[myheroid]->isfree && hero[myheroid]->islive)
	{
		if (hero[myheroid]->bubble_num > 0)
		{
			hero[myheroid]->bubble_num--;
			SimpleAudioEngine::sharedEngine()->playEffect("audio/ef_0.ogg");
			char msg[255];
			memset(msg, 0,sizeof(msg));
			sprintf(msg, "%d %f %f %d %d",1, PositionToTileCoord(hero[myheroid]->sprite->getPosition()).x, PositionToTileCoord(hero[myheroid]->sprite->getPosition()).y,hero[myheroid]->bubble_range, myheroid);
			send(Client::client->sockfd, msg, sizeof(msg), 0);
			//schedule(schedule_selector(GameScene::getBombMsg));
			//schedule(schedule_selector(GameScene::getBombMsg));
			//createbubble(TileCoordToPosition(PositionToTileCoord(hero[myheroid]->sprite->getPosition())),mapBackgroundLayer->getScale(),hero[myheroid]->bubble_range,myheroid);
		}
	}
}

void  GameScene::BubbleBomb(int idx)
{
	if (bubble[idx] != NULL && bubble[idx]->isdelay)
	{
		int r = bubble[idx]->range;
		hero[bubble[idx]->heroid]->bubble_num++;
		CCAction *action = NULL;
		action = bubble[idx]->sprite->getActionByTag(0);
		if (action != NULL)
		{
			bubble[idx]->sprite->stopAction(action);
		}
		float x,y;
		x = PositionToTileCoord(bubble[idx]->sprite->getPosition()).x;
		y = PositionToTileCoord(bubble[idx]->sprite->getPosition()).y;
		int range[4]={0,0,0,0};
		while(y-range[0]>0 and range[0]<r and(mapItemLayer->tileGIDAt(ccp(x,y-range[0]))==0 or !mapItemLayer->tileAt(ccp(x,y-range[0]))->isVisible()))range[0]++;
		while(y+range[1]<map->getMapSize().height and range[1]<r and(mapItemLayer->tileGIDAt(ccp(x,y+range[1]))==0 or !mapItemLayer->tileAt(ccp(x,y+range[1]))->isVisible()))range[1]++;
		while(x-range[2]>0 and range[2]<r and(mapItemLayer->tileGIDAt(ccp(x-range[2],y))==0 or !mapItemLayer->tileAt(ccp(x-range[2],y))->isVisible()))range[2]++;
		while(x+range[3]<map->getMapSize().width and range[3]<r and(mapItemLayer->tileGIDAt(ccp(x+range[3],y))==0 or !mapItemLayer->tileAt(ccp(x+range[3],y))->isVisible()))range[3]++;
		if(y-range[0]>0 and mapItemLayer->tileGIDAt(ccp(x,y-range[0]))==25)range[0]--;
		if(y+range[1]<map->getMapSize().height and mapItemLayer->tileGIDAt(ccp(x,y+range[1]))==25)range[1]--;
		if(x-range[2]>0 and mapItemLayer->tileGIDAt(ccp(x-range[2],y))==25)range[2]--;
		if(x+range[3]<map->getMapSize().width and mapItemLayer->tileGIDAt(ccp(x+range[3],y))==25)range[3]--;
		if(y-range[0]>0 and mapItemLayer->tileGIDAt(ccp(x,y-range[0]))==23 and mapItemLayer->tileAt(ccp(x,y-range[0]))->isVisible())fbomb[int(x+map->getMapSize().width*(y-range[0]))]=true;
		if(y+range[1]<map->getMapSize().height and mapItemLayer->tileGIDAt(ccp(x,y+range[1]))==23 and mapItemLayer->tileAt(ccp(x,y+range[1]))->isVisible())fbomb[int(x+(int)map->getMapSize().width*(y+range[1]))]=true;
		if(x-range[2]>0 and mapItemLayer->tileGIDAt(ccp(x-range[2],y))==23 and mapItemLayer->tileAt(ccp(x-range[2],y))->isVisible())fbomb[int(x-range[2]+(int)map->getMapSize().width*y)]=true;
		if(x+range[3]<map->getMapSize().width and mapItemLayer->tileGIDAt(ccp(x+range[3],y))==23 and mapItemLayer->tileAt(ccp(x+range[3],y))->isVisible())fbomb[int(x+range[3]+(int)map->getMapSize().width*y)]=true;
		bubble[idx]->bomb(range[0],range[1],range[2],range[3]);
		for (int i = myheroid; i <= myheroid; i++)
		if (hero[i]->isfree && hero[i]->islive)
		{
			int x1 = PositionToTileCoord(hero[i]->sprite->getPosition()).x;
			int y1 = PositionToTileCoord(hero[i]->sprite->getPosition()).y;
			bool t = false;
			if (x1 == x && y == y1)
			{
				t = true;
			}
			if (!t)
			for (int j = 1; j <= range[0]; j++)
				if (x1 == x && (y-j) == y1)
				{
					t = true;
					break;
				}
			if (!t)
			for (int j = 1; j <= range[1]; j++)
				if (x1 == x && (y+j) == y1)
				{
					t = true;
					break;

				}
			if (!t)
			for (int j = 1; j <= range[2]; j++)
				if (x1 == (x-j) && y == y1)
				{
					t = true;
					break;
				}
			if (!t)
			for (int j = 1; j <= range[3]; j++)
				if (x1 == (x+j) && y == y1)
				{
					t = true;
					break;
				}
			if (t)
			{
				char msg[255];
				sprintf(msg, "%d %d", 3, i);
				send(Client::client->sockfd, msg, sizeof(msg), 0);
				//heroencase(i);
			}
		}
		for (int i = 0; i < propsnum; i++)
		if (props[i]->isdelay)
		{
			int x1 = PositionToTileCoord(props[i]->sprite->getPosition()).x;
			int y1 = PositionToTileCoord(props[i]->sprite->getPosition()).y;
			bool t = false;
			if (x1 == x && y == y1)
			{
				t = true;
			}
			if (!t)
			for (int j = 1; j <= range[0]; j++)
				if (x1 == x && (y-j) == y1)
				{
					t = true;
					break;
				}
			if (!t)
			for (int j = 1; j <= range[1]; j++)
				if (x1 == x && (y+j) == y1)
				{
					t = true;
					break;

				}
			if (!t)
			for (int j = 1; j <= range[2]; j++)
				if (x1 == (x-j) && y == y1)
				{
					t = true;
					break;
				}
			if (!t)
			for (int j = 1; j <= range[3]; j++)
				if (x1 == (x+j) && y == y1)
				{
					t = true;
					break;
				}
			if (t)
			{
				props[i]->remove();
			}
		}
		for (int i = 0; i <= 80; i++)
		{
			if (i != idx && bubble[i] != NULL && bubble[i]->isdelay)
			{
				int x1 = PositionToTileCoord(bubble[i]->sprite->getPosition()).x;
				int y1 = PositionToTileCoord(bubble[i]->sprite->getPosition()).y;
				bool t = false;
				if (x1 == x && y == y1)
				{
					t = true;
				}
				if (!t)
				for (int j = 1; j <= range[0]; j++)
					if (x1 == x && (y-j) == y1)
					{
						t = true;
						break;
					}
				if (!t)
				for (int j = 1; j <= range[1]; j++)
					if (x1 == x && (y+j) == y1)
					{
						t = true;
						break;

					}
				if (!t)
				for (int j = 1; j <= range[2]; j++)
					if (x1 == (x-j) && y == y1)
					{
						t = true;
						break;
					}
				if (!t)
				for (int j = 1; j <= range[3]; j++)
					if (x1 == (x+j) && y == y1)
					{
						t = true;
						break;
					}
				if (t)
				{
					BubbleBomb(i);
				}
			}
		}
	}
}
void GameScene::BombCallback(CCNode* obj,void* id) {
	int idx = *((int*)id);
	fbomb=new bool[int(map->getMapSize().height*map->getMapSize().width)];
	for(int i=0;i<map->getMapSize().height*map->getMapSize().width;i++)
		fbomb[i]=false;
	BubbleBomb(idx);

	for(int x=0;x<map->getMapSize().width;x++)
		for(int y=0;y<map->getMapSize().height;y++)
			if(fbomb[x+y*(int)map->getMapSize().width]){
				mapItemLayer->tileAt(ccp(x,y))->setVisible(false);
				if(rand()%2 && bubble[idx]->heroid==myheroid){
					char msg[255];
					memset(msg,0,sizeof(msg));
					sprintf(msg, "%d, %d, %d, %d", 2, rand()%3+1,x, y );
					send(Client::client->sockfd, msg, sizeof(msg), 0);
					//createprops(rand()%3+1,TileCoordToPosition(ccp(x,y)),mapBackgroundLayer->getScale());
				}
			}
}
void GameScene::getMsg(){
	float x, y, ox,oy;
	int id, type;
	fd_set fdr;
	int sockfd=Client::client->sockfd;
	struct timeval timeout={0,0};
	FD_ZERO(&fdr);
	FD_SET(sockfd, &fdr);
	switch(select(sockfd+1, &fdr, NULL, NULL, &timeout)){
		case -1:
			break;
		case 0:
			break;
		default:
			if(FD_ISSET(sockfd, &fdr)){
				int r= recv(sockfd, rMsg, sizeof(rMsg), 0);
				if(r>=0){
					sscanf(rMsg, "%d",&type);
					if(type==0){
						int n=sscanf(rMsg, "%d %d %f %f %f %f",&type, &id, &x, &y, &ox, &oy);
						if(n==6)
							heromove(TileCoordToPosition(ccp(x, y)),id,ccp(ox, oy));
					}
					else if(type==1){
						int n=sscanf(rMsg, "%d %f %f %d %d",&type, &x, &y, &oy, &id);
						if(n==5)
							createbubble(TileCoordToPosition(ccp(x, y)),mapBackgroundLayer->getScale(),oy,id);
					}
					else if(type==2){
						int randp;
						int n=sscanf(rMsg,  "%d, %d, %f, %f", &type, &randp, &x, &y);
						if(n==4)
							createprops(randp,TileCoordToPosition(ccp(x,y)),mapBackgroundLayer->getScale());
					}
					else if(type==3){
						int n=sscanf(rMsg, "%d %d", &type, &id);
						if(n==2)
							heroencase(id);
					}
				}

			}
			break;
	}
}
void GameScene::sendMoveMsg(){
	send(Client::client->sockfd, sMsg, sizeof(sMsg), 0);
}

void GameScene::DieCallback(CCNode* obj,void* id) {
	int idx = *((int*)id);
	herodie(idx);
}
#endif

