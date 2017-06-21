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
*  15750659557@163.com，我们期待能和您互相交流合作, 学习更多的知识.                                      
*                                                                            
*  另外注意: 此项目需要您自行配置cocos环境,安装boost库, 如果遇到相关问题的话, 欢迎将     
*  错误日志发给我们, 您的帮助将有助于改善游戏的体验.                                  
*                                                                            
*  @file     PlayerController.h                                                  
*  @brief    人物的操控类                                       
*  Details.                                                                
*                                                                            
*  @author   宋天佑 李坤                                                            
*  @email    13167211978@qq.com                                               
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
*  2017/06/02 | 3.0.0.1   | 宋天佑            | Create file                     
*----------------------------------------------------------------------------
*                                                                            
*****************************************************************************/
#include <stdio.h>
#include "PlayerController.h"
#include "../Data/Role.h"
#include "../Scene/GameScene.h"

void Player::initLocalPlayer(std::string roleName, Game &scene)
{

    _bornPosition[0] = (Vec2(60,60));
    _bornPosition[1] = (Vec2(620,60));
    _bornPosition[2] = (Vec2(60,620));
    _bornPosition[3] = (Vec2(620,620));

    if (scene._inputData == NULL) {
        scene._localPlayer = Role::create("player_down.png");
    }
    else {
        scene._localPlayer = Role::create(scene._inputData->getData());
    }

    Vec2 bornPosition;
    for(int i = 0; i < scene._playerList.size(); i++)
    {
       if( scene._localPlayerName == scene._playerList.at(i).player_name)
       {
           bornPosition = _bornPosition[scene._playerList.at(i).player_id - 1];
       }
    }

    scene._localPlayer->setRoleName(roleName);
    scene._localPlayer->setPlayerName(scene._localPlayerName);
    scene._localPlayer->setPosition(scene._originSize.x + bornPosition.x, bornPosition.y + scene._originSize.y);
    scene._localPlayer->setAnchorPoint(Vec2(scene._localPlayer->getContentSize().width / 2 / scene.         _localPlayer->getContentSize().width, scene._tileMap->getTileSize().height / 2 / scene._localPlayer->getContentSize().height));
    scene._localPlayer->setSpeed(2);
    scene._localPlayer->setPopCount(2);
    scene._localPlayer->setBubble(2);
    scene._tileMap->addChild(scene._localPlayer, 4);
    
}
void Player::initRemotePlayer(std::string roleName, Game &scene)
{

    Vec2 bornPosition;
    for(int i = 0; i < scene._playerList.size(); i++)
    {
        if(scene._localPlayerName == scene._playerList.at(i).player_name)
            continue;
        bornPosition = _bornPosition[scene._playerList.at(i).player_id - 1];
        auto RoleName = scene._playerList.at(i).player_role;
        if(RoleName == "player")
            scene._remotePlayer[scene._playerList.at(i).player_name] = Role::create("player_down.png");
        else
            scene._remotePlayer[scene._playerList.at(i).player_name] = Role::create("player2_down.png");
        
        auto tempPlayer = scene._remotePlayer[scene._playerList.at(i).player_name];
        tempPlayer->setRoleName(RoleName);
        tempPlayer->setPlayerName(scene._playerList.at(i).player_name);
        tempPlayer->setPosition(scene._originSize.x + bornPosition.x, bornPosition.y + scene._originSize.y);
        tempPlayer->setAnchorPoint(Vec2(tempPlayer->getContentSize().width / 2 / tempPlayer->getContentSize().width, scene._tileMap->getTileSize().height / 2 / tempPlayer->getContentSize().height));
        tempPlayer->setSpeed(2);
        tempPlayer->setPopCount(2);
        tempPlayer->setBubble(2);
        scene._tileMap->addChild(tempPlayer, 3);
    }
}

void Player::playerForeverMove(EventKeyboard::KeyCode code, Game &scene)
{
    
    
    Animate* animate;
    
    switch (code) {
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            animate = scene.getAnimateByName(scene._player->getRoleName() + "_left_", 0.1f, 6);
            break;
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            animate = scene.getAnimateByName(scene._player->getRoleName() + "_right_", 0.1f, 6);
            break;
        case EventKeyboard::KeyCode::KEY_UP_ARROW:
            animate = scene.getAnimateByName(scene._player->getRoleName() + "_up_", 0.05f, 6);
            break;
        case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            animate = scene.getAnimateByName(scene._player->getRoleName() + "_down_", 0.1f, 6);
            break;
        default:
            return;
    }
    
    auto repeatAnimate = RepeatForever::create(animate);
    scene._player->runAction(repeatAnimate);
}

void Player::executePlayerForeverMove(Game &scene, EventKeyboard::KeyCode code)
{
    if(scene._player->isDead() == true)
        return;
    
    auto
    leftArrow = EventKeyboard::KeyCode::KEY_LEFT_ARROW,
    rightArrow = EventKeyboard::KeyCode::KEY_RIGHT_ARROW,
    upArrow = EventKeyboard::KeyCode::KEY_UP_ARROW,
    downArrow = EventKeyboard::KeyCode::KEY_DOWN_ARROW,
    space = EventKeyboard::KeyCode::KEY_SPACE;
    
    if (scene.isKeyPressed(leftArrow)) {
        this->playerForeverMove(leftArrow, scene);
    }
    else if (scene.isKeyPressed(rightArrow)) {
        this->playerForeverMove(rightArrow, scene);
    }
    else if (scene.isKeyPressed(upArrow)) {
        this->playerForeverMove(upArrow, scene);
    }
    else if (scene.isKeyPressed(downArrow)) {
        this->playerForeverMove(downArrow, scene);
    }
    else {
        if ((EventKeyboard::KeyCode::KEY_LEFT_ARROW == code)) {
            scene._player->setTexture(scene._player->getRoleName() + "_left.png");
        }
        else if (EventKeyboard::KeyCode::KEY_RIGHT_ARROW == code) {
            scene._player->setTexture(scene._player->getRoleName() + "_right.png");
        }
        else if (EventKeyboard::KeyCode::KEY_UP_ARROW == code) {
            scene._player->setTexture(scene._player->getRoleName() + "_up.png");
        }
        else if (EventKeyboard::KeyCode::KEY_DOWN_ARROW == code) {
            scene._player->setTexture(scene._player->getRoleName() + "_down.png");
        }
    }
}

void Player::updatePlayerPerFrame(int delta, Game &scene, EventKeyboard::KeyCode code)
{
    int offsetX = 0, offsetY = 0;
    int collideOffsetX = 0, collideOffsetY = 0;
    float
    collidableAmendLeftX = 0,
    collidableAmendRightX = 0,
    collidableAmendUpY = 0,
    collidableAmendDownY = 0;

    switch (code) {
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            offsetX = -scene._player->getSpeed();
            collideOffsetX = -2;
            collidableAmendLeftX = -scene._player->getContentSize().width / 2;
            collidableAmendUpY = scene._tileMap->getTileSize().height / 2 - 6;
            collidableAmendDownY = -scene._tileMap->getTileSize().height / 2 + 4;
            break;
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            offsetX = scene._player->getSpeed();
            collideOffsetX = 2;
            collidableAmendRightX = scene._player->getContentSize().width / 2 - 2;
            collidableAmendUpY = scene._tileMap->getTileSize().height / 2 - 6;
            collidableAmendDownY = -scene._tileMap->getTileSize().height / 2 + 4;
            break;
        case EventKeyboard::KeyCode::KEY_UP_ARROW:
            offsetY = scene._player->getSpeed();
            collideOffsetY = 2;
            collidableAmendUpY = scene._tileMap->getTileSize().height / 2;
            collidableAmendLeftX = -scene._player->getContentSize().width / 2 + 6;
            collidableAmendRightX = scene._player->getContentSize().width / 2 - 6;
            break;
        case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            offsetY = -scene._player->getSpeed();
            collideOffsetY = -2;
            collidableAmendLeftX = -scene._player->getContentSize().width / 2 + 6;
            collidableAmendRightX = scene._player->getContentSize().width / 2 - 6;
            collidableAmendDownY = -scene._tileMap->getTileSize().height / 2 - 3;
            break;
        default:
            break;
    }
    
    Vec2 amendLeftUp = Vec2(collideOffsetX + collidableAmendLeftX, collideOffsetY + collidableAmendUpY),
    amendLeftDown = Vec2(collideOffsetX  + collidableAmendLeftX, collideOffsetY + collidableAmendDownY),
    amendRightUp = Vec2(collideOffsetX  + collidableAmendRightX, collideOffsetY + collidableAmendUpY),
    amendRightDown = Vec2(collideOffsetX  + collidableAmendRightX, collideOffsetY + collidableAmendDownY);
    
    Vec2 collidableTest1, collidableTest2;
    switch (code) {
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            collidableTest1 = scene.tileCoordForPosition(scene._player->getPosition() + amendLeftUp);
            collidableTest2 = scene.tileCoordForPosition(scene._player->getPosition() + amendLeftDown);
            break;
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            collidableTest1 = scene.tileCoordForPosition(scene._player->getPosition() + amendRightUp);
            collidableTest2 = scene.tileCoordForPosition(scene._player->getPosition() + amendRightDown);
            break;
        case EventKeyboard::KeyCode::KEY_UP_ARROW:
            collidableTest1 = scene.tileCoordForPosition(scene._player->getPosition() + amendLeftUp);
            collidableTest2 = scene.tileCoordForPosition(scene._player->getPosition() + amendRightUp);
            break;
        case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            collidableTest1 = scene.tileCoordForPosition(scene._player->getPosition() + amendLeftDown);
            collidableTest2 = scene.tileCoordForPosition(scene._player->getPosition() + amendRightDown);
            break;
        default:
            offsetY = offsetX = 0;
            break;
    }

    Vec2 aimLocation = scene._player->getPosition() + Vec2(offsetX, offsetY);
    
    Vec2 orginCoordLocation = scene.tileCoordForPosition(scene._player->getPosition());
    
    Vec2 tileGlPosition = scene.positionForTileCoord(orginCoordLocation);

    Vec2
    shiftLeftUp = scene._player->getPosition() + amendLeftUp,
    shiftLeftDown = scene._player->getPosition() + amendLeftDown,
    shiftRightUp = scene._player->getPosition() + amendRightUp,
    shiftRightDown = scene._player->getPosition() + amendRightDown;
    
    Vec2 shiftLocation;
    
    if (scene.collidable(collidableTest1,PLAYER) || scene.collidable(collidableTest2,PLAYER)) {
        if (scene._player->getKeys(EventKeyboard::KeyCode::KEY_LEFT_ARROW) == true) {
            if (shiftLeftDown.y > tileGlPosition.y - 40
                && shiftLeftDown.y < tileGlPosition.y - 20)
            {
                shiftLocation = scene._player->getPosition() + Vec2(0, 3);
                scene._player->setPosition(shiftLocation);
            }
        }
        else if (scene._player->getKeys(EventKeyboard::KeyCode::KEY_RIGHT_ARROW) == true) {
            if (shiftRightDown.y > tileGlPosition.y - 40
                && shiftLeftDown.y < tileGlPosition.y - 20)
            {
                shiftLocation = scene._player->getPosition() + Vec2(0, 3);
                scene._player->setPosition(shiftLocation);
            }
        }
        else if (scene._player->getKeys(EventKeyboard::KeyCode::KEY_UP_ARROW) == true)
        {
            if (shiftLeftUp.x > tileGlPosition.x - 40
                && shiftLeftUp.x < tileGlPosition.x - 20)
            {
                shiftLocation = scene._player->getPosition() + Vec2(3, 0);
                scene._player->setPosition(shiftLocation);
            }
            else if(shiftRightUp.x < tileGlPosition.x + 40
                    && shiftRightUp.x > tileGlPosition.x + 20)
            {
                shiftLocation = scene._player->getPosition() + Vec2(-3, 0);
                scene._player->setPosition(shiftLocation);
            }
        }
        else if (scene._player->getKeys(EventKeyboard::KeyCode::KEY_DOWN_ARROW) == true)
        {
            if (shiftLeftDown.x > tileGlPosition.x - 40
                && shiftLeftDown.x < tileGlPosition.x - 20)
            {
                shiftLocation = scene._player->getPosition() + Vec2(3, 0);
                scene._player->setPosition(shiftLocation);
            }
            else if(shiftRightDown.x < tileGlPosition.x + 40
                    && shiftRightDown.x > tileGlPosition.x + 20)
            {
                shiftLocation = scene._player->getPosition() + Vec2(-3, 0);
                scene._player->setPosition(shiftLocation);
            }
        }
        return;
    }

    scene._player->setPosition(aimLocation);
}

void Player::setPlayerDied(std::string roleName, Game &scene)
{
   scene._player->stopAllActions();
    auto bigPop = Sprite::create("BigPopoShape_4.png");
    scene.addChild(bigPop, 4);
    bigPop->setPosition(Vec2(scene._player->getPosition().x, scene._player->getPosition().y - 20 + scene._player->getContentSize().height/2));
    bigPop->setScale(1.5);
    auto popShape = Animation::create();
    for (int i = 1;i <= 4 ;++i){
        popShape->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("BigPopoShape_%d.png",i)));
    }
    popShape->setDelayPerUnit(0.2f);
    auto popShaping = Animate::create(popShape);
    
    auto popShape2 = Animation::create();
    for (int i = 5;i <= 12 ;++i){
        popShape2->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("BigPopoShape_%d.png",i)));
    }
    popShape2->setDelayPerUnit(0.4f);
    auto popShaping2 = Animate::create(popShape2);
    Role * playerObject = scene._player;
    auto playerDie = CallFunc::create([=]{
        auto animation = Animation::create();
        if(roleName == "player"){
            for (int i = 1;i <= 10 ;++i)
                animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("Role1Die_%d.png",i)));
        }else{
            for (int i = 1;i <= 10 ;++i)
                animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("Role2Die_%d.png",i)));
        }
        animation->setDelayPerUnit(0.3f);
        auto animate = Animate::create(animation);
        
        auto popBlast = Animation::create();
        for (int i = 1;i <= 6 ;++i){
            popBlast->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("BigPopoBlast_%d.png",i)));
        }
        popBlast->setDelayPerUnit(0.1f);
        auto popBlasting = Animate::create(popBlast);
        bigPop->runAction(popBlasting);
        playerObject->runAction(animate);
    });

    auto action = Sequence::create(popShaping, popShaping2,playerDie,NULL);
    bigPop->runAction(action);
}

