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
 *  @file     GameManager.cpp
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
 *  2017/06/02 | 3.0.0.1   | 宋天佑            | Create file
 *----------------------------------------------------------------------------
 *
 *****************************************************************************/

#include <stdio.h>
#include "GameManager.h"
#include "../Scene/GameScene.h"

static Manager* thisManager = nullptr;
template <class Type>
Type stringToNum(const std::string& str){
    std::istringstream iss(str);
    Type num;
    iss >> num;
    return num;
}
/****************************** update scene per frame and dispatch event *************************/
void Manager::updatePerFrame(int delta, Game & scene)
{
    std::string tempCommand;
    while(( tempCommand = scene._client->executeOrder()) != "no")
    {
        scene._command.push(tempCommand);
        std::cout<<"收到的信息:"<<tempCommand<<std::endl;
    }
    
    scene._player = scene._localPlayer;
    
    if(!scene._player->isDead());
        executePlayer(delta, scene);
 

    while(scene._command.size() != 0)
    {
        _command = scene._command.front();
        scene._command.pop();
        if(_command[0] == DEAD_MESSAGE[0])
        {
            std::string name = std::string(&_command[1]);
            if(name == scene._localPlayerName)
            {
//                std::cout << "LocalPlayerDead message" << std::endl;
                scene._player = scene._localPlayer;
                scene._player->stopAllActions();
                scene._localPlayer->setPlayerDead();
                
                executePlayer(delta, scene);
            }
            else{
//                std::cout << "RemotePlayerDead message" << std::endl;
                scene._player = scene._remotePlayer[name];
                scene._player->stopAllActions();
                scene._player -> setPlayerDead();
                executePlayer(delta, scene);
            }
                
        }
        if(_command[0] != KEY_ORDER[0])
        {
            continue;
        }
        readCommand();
        // bool isPress;
        
        if(_playerName == scene._localPlayerName)
            continue;
        scene._player = scene._remotePlayer[_playerName];
        if(_code[0] == 'p')
        {
            scene._player->setKeys(scene._keyPressesedOrder[_code], true);
        }
        else
        {
            scene._player->setKeys(scene._keyReleasedOrder[_code], false);
        }
        
        executePlayer(delta, scene);
        scene._player->setPosition(_playerCurrentPosition);
    }
    for(int i = 0; i < scene._playerList.size(); i++)
    {
        _playerName = scene._playerList.at(i).player_name;
        
        if(_playerName == scene._localPlayerName)
            continue;
        
        else{
            //std::cout << "PlayerName:" << _playerName << std::endl;
            scene._player = scene._remotePlayer[_playerName];
            executePlayer(delta, scene);
        }
    }
}

void Manager::readCommand()
{
    auto leftBracket = _command.find('(');
    auto comma = _command.find(',');
    auto rightBracket = _command.find(')');
    std::string code(_command.begin() + 1, _command.begin() + 3);
    std::string playerName(_command.begin() + 3,_command.begin() + leftBracket);
    std::string spositionX(_command.begin()+ 1 + leftBracket,_command.begin() + comma);
    std::string spositionY(_command.begin()+ 1 + comma,_command.begin() + rightBracket);
    
    //std::cout << playerName << std::endl;
    
    float positionX = stringToNum<float>(spositionX);
    float positionY = stringToNum<float>(spositionY);
    //std::cout << positionX << std::endl;
   // std::cout << positionY << std::endl;
    _code = code;
    _playerName = playerName;
    _playerCurrentPosition = Vec2(positionX, positionY);
    
}

/*********************************** initial player controller object *****************************/
void Manager::playerInit(std::string player, Game & scene)
{
    
    _playerManager = Player::create(player, scene);
    scene.addChild(_playerManager);
    scene._playerManager = _playerManager;
}

/*********************************** initial pop controller object *****************************/
void Manager::popInit(Game & scene)
{
    _popManager = Pop::create(scene);
    _popManager->setScene(&scene);
    scene.addChild(_popManager);
    scene._popManager = _popManager;
}
void Manager::executePlayer(int delta,Game &scene)
{
    if(scene._player->isDead()){
        if(!scene._player->getAlreadyDead())
        {
            scene._player->setAlreadyDead();
            
            _playerManager->setPlayerDied(scene._player->getRoleName(), scene);
        }else{
            
            return;
        }
    }
    
    auto
    leftArrow = EventKeyboard::KeyCode::KEY_LEFT_ARROW,
    rightArrow = EventKeyboard::KeyCode::KEY_RIGHT_ARROW,
    upArrow = EventKeyboard::KeyCode::KEY_UP_ARROW,
    downArrow = EventKeyboard::KeyCode::KEY_DOWN_ARROW,
    space = EventKeyboard::KeyCode::KEY_SPACE;
    
    //any of the direction is pressed and any_arrow is true;
    int any_arrow = scene.isKeyPressed(leftArrow)  +
    scene.isKeyPressed(rightArrow) +
    scene.isKeyPressed(upArrow)    +
    scene.isKeyPressed(downArrow);
    
    //one of the direction and the space is pressed,put a pop
    if ((scene.isKeyPressed(space) && any_arrow)) {
        _popManager->updatePopPerFrame(delta, scene, space);
        _playerManager->updatePlayerPerFrame(delta, scene, space);
        scene._player->setKeys(space, false);
    }
    
    //move when one of the direction is pressed and put a pop when space is pressed
    if (scene.isKeyPressed(leftArrow)) {
        isExecuteForeverMove(leftArrow, scene);
        _playerManager->updatePlayerPerFrame(delta, scene, leftArrow);
    }
    else if (scene.isKeyPressed(rightArrow) ) {
        isExecuteForeverMove(rightArrow, scene);
        _playerManager->updatePlayerPerFrame(delta, scene, rightArrow);
    }
    else if (scene.isKeyPressed(upArrow) ) {
        isExecuteForeverMove(upArrow, scene);
        _playerManager->updatePlayerPerFrame(delta, scene, upArrow);
    }
    else if (scene.isKeyPressed(downArrow) ) {
        isExecuteForeverMove(downArrow, scene);
        _playerManager->updatePlayerPerFrame(delta, scene, downArrow);
    }
    else if (scene.isKeyPressed(space)) {
      
        _popManager->updatePopPerFrame(delta, scene, space);
        _playerManager->updatePlayerPerFrame(delta, scene, space);
    }else{
          isExecuteForeverMove(leftArrow, scene);
          isExecuteForeverMove(rightArrow, scene);
          isExecuteForeverMove(upArrow, scene);
          isExecuteForeverMove(downArrow, scene);
    }

}
bool Manager::isExecuteForeverMove(EventKeyboard::KeyCode code,Game& scene)
{
    if(scene._player->getKeys(code) && scene._player->getRealKeys(code) == false)
    {
        scene._player->setRealKeys(code, true);
        scene._player->stopAllActions();
        scene._playerManager->executePlayerForeverMove(scene, code);
        return true;
    }
    if(scene._player->getRealKeys(code) && scene._player->getKeys(code) == false)
    {
        scene._player->setRealKeys(code, false);
        scene._player->stopAllActions();
        scene._playerManager->executePlayerForeverMove(scene, code);
        return false;
    }
}

