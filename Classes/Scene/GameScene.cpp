/*****************************************************************************
 *  Copyright (C) 2017 宋天佑 13167211978@163.com
 *
 *  此文件属于软件学院2017c++大项目泡泡堂选题的项目文件.
 *
 *  此项目是开源项目, 在期末答辩之后, 我们可能会在假期里对一些因时间不够未完成的功能进
 *  行补充, 以及适配windows平台, 在未来如果技术允许的情况下, 会酌情开发ios版本和anroid
 *  版本, 期待您能够为这个开源项目提供宝贵意见, 帮助我们做得更好, 如果能够贡献宝贵的代
 *  码那就更令人开心了.
 *
 *  本项目遵守MIT开源协议, 这也就是说, 您需要遵守的唯一条件就是在修改后的代码或者发行
 *  包包含原作者的许可信息. 除非获得原作者的特殊许可, 在任何项目(包括商业项目)里使用
 *  本项目的文件都需要包含作者的许可.
 *
 *  如果对项目有疑问或者其他建议, 欢迎联系13167211978@163.com, 1061152718@qq.com,
 *  我们期待能和您互相交流合作, 学习更多的知识.
 *
 *  另外注意: 此项目需要您自行配置cocos环境,安装boost库, 如果遇到相关问题的话, 欢迎将
 *  错误日志发给我们, 您的帮助将有助于改善游戏的体验.
 *
 *  @file     GameScene.cpp
 *  @brief    游戏整体的操控类
 *  Details.
 *
 *  @author   宋天佑 李坤
 *  @email    13167211978@163.com
 *  @version  4.0.1.5(版本号)
 *  @date      2017/06/02
 *  @license  Massachusetts Institute of Technology License (MIT)
 *
 *----------------------------------------------------------------------------
 *  Remark         : Description
 *----------------------------------------------------------------------------
 *  Change History :
 *  <Date>     | <Version> | <Author>       | <Description>
 *----------------------------------------------------------------------------
 *  2017/06/10 | 3.0.0.1   | 宋天佑            | Create file
 *----------------------------------------------------------------------------
 *
 *****************************************************************************/


#include <stdio.h>
#include <iostream>
#include "GameScene.h"
#include <cstdlib>
#include <ctime>
#include "RoomScene.h"
#include "SearchScene.h"

using namespace std;
static int _mapIndex = 1;
static std::string splayerName;
static Client* clients;
//a static pointer which is gong to be used to make LevelData oject reference count nonzero
static LevelData* ptr = NULL;




Scene*Game::createScene(LevelData &data,Client* client,std::string playerName)
{
    auto scene = Scene::create();
    data.retain();
    
    
    _mapIndex = data.getmapIndex();
    clients = client;
    splayerName = playerName;
    //Make LevelData oject reference count nonzero
    ptr = &data;
    
    auto layer = Game::create();
    
    scene->addChild(layer);
    return scene;
}






bool Game::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    _thisScene = this;
    _client = clients;
    _inputData = ptr;
    // rapidjson object
    rapidjson::Document document;
    _localPlayerName = splayerName;
    _playerList = ptr->player_list;
    
    // using json
    std::string filePath = FileUtils::getInstance()->
    fullPathForFilename(StringUtils::format("%d.data", _mapIndex));
    std::string contentStr = FileUtils::getInstance()->getStringFromFile(filePath);
    document.Parse<0>(contentStr.c_str());
    
    // get the map file
    _tileFile = document["tileFile"].GetString();
    
    //=======   add sprites on breakable blocks  ====   created by MrNickel
    
    
    //    _client = Client::create();
    //    this->addChild(_client);
    initTileMap();
    initKeyCommand();
    initBreakableSprite();
    
    
    auto moneyCode1 = Button::create("Magic.png", "code1.png");
    auto moneyCode2 = Button::create("Magic.png", "code2.png");
    
    moneyCode1->setPosition(Vec2(_originSize.x + _visibleSize.width / 4, _originSize.y + _visibleSize.height / 7 * 4));
    moneyCode2->setPosition(Vec2(_originSize.x + _visibleSize.width / 4 * 3, _originSize.y + _visibleSize.height / 7 * 4));
    moneyCode1->setScale(0.3);
    moneyCode2->setScale(0.3);
    moneyCode1->setVisible(false);
    moneyCode2->setVisible(false);
    this->addChild(moneyCode1);
    this->addChild(moneyCode2);
    _endButton.pushBack(moneyCode1);
    _endButton.pushBack(moneyCode2);

    
    _endButton.pushBack(moneyCode1);
    _endButton.pushBack(moneyCode2);
    
    //==============================QuitButton===========================
    auto close_button = Button::create("button.png");
    close_button->setScale(1.0);
    close_button->setTitleText("Quit");
    close_button->setTitleFontSize(20);
    close_button->setPosition(Vec2(_originSize.x + _visibleSize.width / 2, _originSize.y + _visibleSize.height / 7 * 2));
    
    close_button->addTouchEventListener([](Ref *pSender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED) {
            
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
            MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
            return;
#endif
            Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            exit(0);
#endif
            
        }
    });
    this->addChild(close_button);
    close_button->setVisible(false);
    _endButton.pushBack(close_button);

    
    if (ptr != nullptr)
    {
        _sceneManager = Manager::create(ptr->getmapIndex(), ptr->getRole(), *this);
        this->addChild(_sceneManager);
    }
    else {
        _sceneManager = Manager::create(1, "player", *this);
        this->addChild(_sceneManager);
    }
    
    
    // create keyBoardListener
    auto keyBoardListener = EventListenerKeyboard::create();
    
    //key value is true when it is pressed
    keyBoardListener->onKeyPressed = [=](EventKeyboard::KeyCode code, Event* event)
    {
        
        if(_localPlayer->isDead())
        {
            return ;
        }
        if(!(code == EventKeyboard::KeyCode::KEY_LEFT_ARROW ||
             code == EventKeyboard::KeyCode::KEY_RIGHT_ARROW ||
             code == EventKeyboard::KeyCode::KEY_UP_ARROW ||
             code == EventKeyboard::KeyCode::KEY_DOWN_ARROW ||
             code == EventKeyboard::KeyCode::KEY_SPACE)   )
        {
            return;
        }
        _localPlayer->setKeys(code, true);
        _client->sendMessage(KEY_ORDER,_keyPressesedOrders[code] + _localPlayerName + _localPlayer->getPlayerPosition());
    };
    
    //key value is false when it is not pressed
    keyBoardListener->onKeyReleased = [=](EventKeyboard::KeyCode code, Event* event) {
        if(_localPlayer->isDead())
        {
            return ;
        }
        if(!(code == EventKeyboard::KeyCode::KEY_LEFT_ARROW ||
             code == EventKeyboard::KeyCode::KEY_RIGHT_ARROW ||
             code == EventKeyboard::KeyCode::KEY_UP_ARROW ||
             code == EventKeyboard::KeyCode::KEY_DOWN_ARROW ||
             code == EventKeyboard::KeyCode::KEY_SPACE)   )
        {
            return;
        }
        _localPlayer->setKeys(code,false);
        
        _client->sendMessage(KEY_ORDER,_keyReleasedOrders[code] + _localPlayerName + _localPlayer->getPlayerPosition());
        // _localPlayer->stopAllActions();
        
        //_playerManager->executePlayerForeverMove(*_thisScene, code);
        
    };
    
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyBoardListener, this);
    this->scheduleUpdate();
    return true;
}


Animate* Game::getAnimateByName(std::string animName, float delay, int animNum)
{
    //create an Animation
    Animation* animation = Animation::create();
    
    //put the animationFrame into the animation
    for (unsigned int i = 1; i <= animNum; i++) {
        std::string frameName = animName;
        
        //number the animationFrame
        frameName.append(StringUtils::format("%d", i)).append(".png");
        animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName.c_str()));
    }
    
    //set Delay
    animation->setDelayPerUnit(delay);
    
    //come to the original frame when the animation is over
    animation->setRestoreOriginalFrame(true);
    Animate* animate = Animate::create(animation);
    
    return animate;
}

Vec2 Game::tileCoordForPosition(const Vec2& position)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    int x = static_cast<int>(((position.x - origin.x)
                              /(_tileMap->getTileSize().width / CC_CONTENT_SCALE_FACTOR())));
    float pointHeight = _tileMap->getTileSize().height / CC_CONTENT_SCALE_FACTOR();
    int y = static_cast<int>(((_tileMap->getMapSize().height * pointHeight - position.y) / pointHeight));
    
    return Vec2(x, y);
}


Vec2 Game::positionForTileCoord(const cocos2d::Vec2 &tileCoord) {
    
    //TileCoord converts to the centre of the check of the tilemap. for example 1 1->60 60
    Size mapSize = _tileMap->getMapSize();
    Size tileSize = _tileMap->getTileSize();
    
    int x = tileCoord.x * tileSize.width + tileSize.width / 2;
    int y = (mapSize.height - tileCoord.y)*tileSize.height - tileSize.height / 2;
    
    return Vec2(x, y);
}


bool Game::isKeyPressed(EventKeyboard::KeyCode code) {
    return _player->getKeys(code);
}


void Game::update(float delta) {
    Node::update(delta);
    
    bool if_all_dead = false;
    
    auto it = _remotePlayer.begin();
    while(it != _remotePlayer.end())
    {
        if(it->first == _localPlayerName)
            continue;
        if(it->second->isDead() == true){
            if_all_dead = true;
        }else{
            if_all_dead = false;
            break;
        }
        it ++;
    }
    
    if(_localPlayer->isDead() == true || if_all_dead == true){
        for (auto i : _endButton) {
            i->setVisible(true);
        }
    }
    
    //update the player's Position and check whether to put the pop
    _sceneManager->updatePerFrame(delta, *this);
}


bool Game::collidable(Vec2 tileCoord, int type)
{
    
    PropCollidableTest(tileCoord, type);
    
    auto tempPlayer = _player;
    for(int i = 0; i < _playerList.size(); i++)
    {
        if(_playerList.at(i).player_name == _localPlayerName)
            continue;
        
        _player = _remotePlayer[_playerList.at(i).player_name];
        if(type == POP && tileCoord == tileCoordForPosition(_player->getPosition()))
        {
            std::cout<<"成功远程玩家发送死亡信息!"<<std::endl;
            std::string code = DEAD_MESSAGE;
            _client->sendMessage(code, _player->getPlayerName());
            _player->setPlayerDead();
        }
    }
    if(type == POP && tileCoord == tileCoordForPosition(_localPlayer->getPosition()))
    {
        std::cout<<"成功远程玩家发送死亡信息!"<<std::endl;
        std::string code = DEAD_MESSAGE;
        _client->sendMessage(code, _localPlayer->getPlayerName());
        _localPlayer->setPlayerDead();
    }
    _player = tempPlayer;
    
    
    for (int i = 0; i < _popVector.size(); i++) {
        
        //get the pop's position and transite it to the tillCoord.
        Vec2 popPosition = _popVector.at(i)->getPosition();
        Vec2 popPositionForTileMap = tileCoordForPosition(popPosition);
        
        //decide whether the pop position is same with the player's next position
        if (tileCoord == popPositionForTileMap && type == PLAYER) {
            
            return true;
        }
    }
    
    
    for (int i = 0; i < _breakableBlockVector.size(); i++)
    {
        
        Vec2 popPosition = _breakableBlockVector.at(i)->getPosition();
        Sprite* erase = _breakableBlockVector.at(i);
        Vec2 popPositionForTileMap = tileCoordForPosition(popPosition);
        if (tileCoord == popPositionForTileMap)
        {
            if (type == POP)
                _breakableBlockVector.eraseObject(erase);
            
            return true;
        }
    }
    
    
    int tileGid = _collidable->getTileGIDAt(tileCoord);
    if (tileGid) {
        // using gid to find the property of tilegid ,return a value
        Value properties = _tileMap->getPropertiesForGID(tileGid);
        
        // return a value map;
        ValueMap map = properties.asValueMap();
        
        //if there is a "collidable" object
        std::string value = map.at("collidable").asString();
        if (value.compare("true") == 0)
        {
            return true;
        }
        else
            return false;
    }
}

void Game::PropCollidableTest(cocos2d::Vec2 tileCoord, int type)
{
    for(int i = 0; i < _propVector.size(); i++)
    {
        Vec2 propPosition = tileCoordForPosition(_propVector.at(i)-> getPosition());
        if(propPosition == tileCoord)
        {
            //if POP contact prop,it will be erased
            if(type == POP)
            {
                _propVector.at(i)->setVisible(false);
                _propVector.erase(i);
            }
            else if(type == PLAYER)
            {
                int propType = _propVector.at(i)->getPropType();
                int playerSpeed = _player->getSpeed();
                int playerPopCount = _player->getPopCount();
                int playerBubbleCount = _player->getBubble();
                switch (propType)
                {
                    case SHOE:
                        _player->setSpeed(playerSpeed + 1);
                        break;
                    case BUBBLE:
                        _player->setPopCount(playerPopCount + 1);
                        break;
                    case MEDCINE:
                        _player->setBubble(playerBubbleCount + 1);
                        break;
                }
                log("%f,%d,%d",_player->getSpeed(),_player->getPopCount(),_player->getBubble());
                _propVector.at(i)->setVisible(false);
                _propVector.erase(i);
            }
        }
    }
}

void Game::initBreakableSprite()
{
    for (int i = 0; i < 17; i++)
    {
        for (int j = 0; j < 17; j++)
        {
            int breakableGid = _blockBottom->getTileGIDAt(Vec2(i, j));
            
            if (breakableGid)
            {
                Value properties = _tileMap->getPropertiesForGID(breakableGid);
                ValueMap map = properties.asValueMap();
                
                std::string value = map.at("breakable").asString();
                if (value == "true")
                {
                    auto test = Sprite::create("pop.png");
                    test->setAnchorPoint(Vec2(0.5, 0.5));
                    test->setPosition(positionForTileCoord(Vec2(i, j)));
                    auto hide = Hide::create();
                    test->runAction(hide);
                    _breakableBlockVector.pushBack(test);
                    _tileMap->addChild(test, 1);
                }
            }
        }
    }
}

void Game::initTileMap()
{
    //get the height and width of the screen(window)
    _visibleSize = Director::getInstance()->getVisibleSize();
    _originSize = Director::getInstance()->getVisibleOrigin();
    _screenWidth = _visibleSize.width;
    _screenHeight = _visibleSize.height;
    //create the scene via the map file
    _tileMap = TMXTiledMap::create(_tileFile);
    _tileMap->setAnchorPoint(Vec2::ZERO);
    _tileMap->setPosition(Vec2::ZERO + Vec2(_originSize.x, _originSize.y));
    this->addChild(_tileMap);
    
    //get the different layer
    _collidable = _tileMap->getLayer("collidable");
    _blockBottom = _tileMap->getLayer("blockBottom");
    _prop = _tileMap->getLayer("Props");
    
    //make the layer unvisible
    _collidable->setVisible(false);
    _prop->setVisible(false);
    _blockBottom->setVisible(true);
    
    _layerNames.pushBack(_tileMap->getLayer("redBlockTop"));
    _layerNames.pushBack(_tileMap->getLayer("woodBlockTop"));
    _layerNames.pushBack(_tileMap->getLayer("yellowBlockTop"));
    _layerNames.pushBack(_tileMap->getLayer("bushBlockTop"));
    
    strcpy(_propertyNames[0], "red");
    strcpy(_propertyNames[1], "wood");
    strcpy(_propertyNames[2], "yellow");
    strcpy(_propertyNames[3], "bush");
    
}


void Game::initKeyCommand()
{
    _keyPressesedOrders[EventKeyboard::KeyCode::KEY_LEFT_ARROW] = "pl";
    _keyPressesedOrders[EventKeyboard::KeyCode::KEY_RIGHT_ARROW] = "pr";
    _keyPressesedOrders[EventKeyboard::KeyCode::KEY_UP_ARROW] = "pu";
    _keyPressesedOrders[EventKeyboard::KeyCode::KEY_DOWN_ARROW] = "pd";
    _keyPressesedOrders[EventKeyboard::KeyCode::KEY_SPACE] = "ps";
    
    _keyReleasedOrders[EventKeyboard::KeyCode::KEY_LEFT_ARROW] = "rl";
    _keyReleasedOrders[EventKeyboard::KeyCode::KEY_RIGHT_ARROW] = "rr";
    _keyReleasedOrders[EventKeyboard::KeyCode::KEY_UP_ARROW] = "ru";
    _keyReleasedOrders[EventKeyboard::KeyCode::KEY_DOWN_ARROW] = "rd";
    _keyReleasedOrders[EventKeyboard::KeyCode::KEY_SPACE] = "rs";
    
    _keyPressesedOrder["pl"] = EventKeyboard::KeyCode::KEY_LEFT_ARROW;
    _keyPressesedOrder["pr"] = EventKeyboard::KeyCode::KEY_RIGHT_ARROW;
    _keyPressesedOrder["pu"] = EventKeyboard::KeyCode::KEY_UP_ARROW;
    _keyPressesedOrder["pd"] = EventKeyboard::KeyCode::KEY_DOWN_ARROW;
    _keyPressesedOrder["ps"] = EventKeyboard::KeyCode::KEY_SPACE;
    
    _keyReleasedOrder["rl"] = EventKeyboard::KeyCode::KEY_LEFT_ARROW;
    _keyReleasedOrder["rr"] = EventKeyboard::KeyCode::KEY_RIGHT_ARROW;
    _keyReleasedOrder["ru"] = EventKeyboard::KeyCode::KEY_UP_ARROW;
    _keyReleasedOrder["rd"] = EventKeyboard::KeyCode::KEY_DOWN_ARROW;
    _keyReleasedOrder["rs"] = EventKeyboard::KeyCode::KEY_SPACE;
    
}
bool Game::pressTest(EventKeyboard::KeyCode code)
{
    if(code == EventKeyboard::KeyCode::KEY_LEFT_ARROW ||
       code == EventKeyboard::KeyCode::KEY_RIGHT_ARROW ||
       code == EventKeyboard::KeyCode::KEY_UP_ARROW ||
       code == EventKeyboard::KeyCode::KEY_DOWN_ARROW ||
       code == EventKeyboard::KeyCode::KEY_SPACE   )
        return true;
    else
        return false;
    
}
void Game::initPopCountVector()
{
    for(int i = 0; i < _playerList.size();i++)
    {
        _playerPopCount[_playerList.at(i).player_name] = 0;
    }
    _playerPopCount[_localPlayerName] = 0;
}
