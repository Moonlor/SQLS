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
 *  @file     PopController.cpp
 *  @brief    泡泡的操控类
 *  Details.
 *
 *  @author   宋天佑
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
 *  2017/06/02 | 4.0.1.5  | 宋天佑            | Create file
 *----------------------------------------------------------------------------
 *
 *****************************************************************************/
#include <stdio.h>
#include "PopController.h"
#include "../Scene/GameScene.h"

void Pop::updatePopPerFrame(int delta, Game &scene, EventKeyboard::KeyCode code)
{
    
    if( whetherPutPop(scene))
    {
        return;
    }
    else
    {
        
        putPop( scene );
    }
    
    _powerAbility = scene._player->getBubble();
    
    DelayTime * delayDelete = DelayTime::create(2.5f);
    DelayTime * delayPush = DelayTime::create(0.5f);
    
    auto callFunc1 = CallFunc::create([=] {
        _popVector->pushBack(_pop);
    });
    scene._realPopVector.pushBack(_pop);
    
    auto callFunc2 = CallFuncN::create( CC_CALLBACK_0(Pop::callFunc2,this));
    auto action = Sequence::create(delayPush, callFunc1,delayDelete,callFunc2,NULL);
    
    _pop->runAction(action);
    
}
void Pop::callFunc2()
{
    changeWaterLengh(_scene,_powerAbility);
    popExplosion(*_scene);
    eraseBlocks(*_scene);
    
}
void Pop::setScene(Game* scene)
{
    _scene = scene;
}
bool Pop::whetherPutPop(Game &scene)
{
    Vec2 tilePlayer = scene.tileCoordForPosition(scene._player->getPosition());
    Vec2 popPosition = scene.positionForTileCoord(tilePlayer);
    
    bool isPutable = true;
    for (int i = 0; i < scene._realPopVector.size(); i++)
    {
        if (abs(scene.tileCoordForPosition(scene._realPopVector.at(i)->getPosition()).x - tilePlayer.x) < 10e-4 && abs(scene.tileCoordForPosition(scene._realPopVector.at(i)->getPosition()).y - tilePlayer.y) < 10e-4)
        {
            isPutable = false;
        }
    }
    
    if (scene._playerPopCount[scene._player->getPlayerName()] >= scene._player->getPopCount())
    {
        isPutable = false;
    }
    
    if (!isPutable)
    {
        return true;
    }else{
        return false;
    }
}
void Pop::putPop(Game &scene)
{
    
    //get the player's position
    
    Vec2 tilePlayer = scene.tileCoordForPosition(scene._player->getPosition());
    Vec2 popPosition = scene.positionForTileCoord(tilePlayer);
    //create a pop and set at the centre of the tile of the player
    _pop = Sprite::create("pop.png");
    _pop->setPosition(popPosition);
    scene._playerPopName[_pop] = scene._player->getPlayerName();
    scene._playerPopCount[scene._player->getPlayerName()] += 1;
    
    //pop's zOrder is smaller than the player
    scene._tileMap->addChild(_pop, 1);
    
    
    _popVector = &scene._popVector;
    
    auto popStayAnimation = Animation::create();
    for (unsigned i = 1;i < 4;++i)
        popStayAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName( StringUtils::format("Popo_%d.png", i)));
    popStayAnimation->setDelayPerUnit(0.8f / 3.0f);
    auto popStayAnimate = Animate::create( popStayAnimation );
    auto repeatPopStayAnimate = RepeatForever::create( popStayAnimate );
    _pop->runAction(repeatPopStayAnimate);
    
    
    log("%zd",scene._realPopVector.size());
    
    
    
}
void Pop::changeWaterLengh(Game *scene,int powerAbility){
    
    log("%zd",scene->_realPopVector.size());
    
    _popEx = scene->_realPopVector.at(0);
    for(int i = 0; i < 4; i++)
        _waterLength[i] = powerAbility;
    
    for (int direction = 0;direction < 4;++direction)
        _waterLength[direction] = powerAbility;
    
    for (int i = 0;i <= _waterLength[0];++i)
    {
        if (scene->collidable(Vec2(scene->tileCoordForPosition(_popEx->getPosition()).x - i - 1, scene->tileCoordForPosition(_popEx->getPosition()).y), POP))
        {
            _waterLength[0] = i;
            break;
        }
    }
    log("%d",_waterLength[0]);
    
    for (int i = 0;i <= _waterLength[1];++i)
    {
        
        if (scene->collidable(Vec2(scene->tileCoordForPosition(_popEx->getPosition()).x + i + 1, scene->tileCoordForPosition(_popEx->getPosition()).y), POP))
        {
            _waterLength[1] = i;
            break;
        }
        
    }
   log("%d",_waterLength[1]);
    for (int i = 0;i <= _waterLength[2];++i)
    {
        if (scene->collidable(Vec2(scene->tileCoordForPosition(_popEx->getPosition()).x, scene->tileCoordForPosition(_popEx->getPosition()).y - i - 1), POP))
        {
            _waterLength[2] = i;
            break;
        }
    }
   log("%d",_waterLength[2]);
    for (int i = 0;i <= _waterLength[3];++i)
    {
        if (scene->collidable(Vec2(scene->tileCoordForPosition(_popEx->getPosition()).x, scene->tileCoordForPosition(_popEx->getPosition()).y + i + 1), POP))
        {
            _waterLength[3] = i;
            break;
        }
    }
    log("%d",_waterLength[3]);
    auto tempPlayer = scene->_player;
    for(int i = 0; i < scene->_playerList.size();i++)
    {
        if(scene->_localPlayerName == scene->_playerList.at(i).player_name)
            continue;
        scene->_player = scene->_remotePlayer[scene->_playerList.at(i).player_name];
        
        if(scene->tileCoordForPosition(_popEx->getPosition()) == scene->tileCoordForPosition(scene->_player->getPosition())){
            //std::cout<<"成功远程玩家发送死亡信息!"<<std::endl;
            std::string code = DEAD_MESSAGE;
            scene->_client->sendMessage(code, scene->_player->getPlayerName());
            scene->_player->setPlayerDead();
            scene->_player->stopAllActions();
        }
    }
    
    if(scene->tileCoordForPosition(_popEx->getPosition()) == scene->tileCoordForPosition(scene->_localPlayer->getPosition())){
        //std::cout<<"成功发送本地玩家死亡信息!"<<std::endl;
        std::string code = DEAD_MESSAGE;
        scene->_client->sendMessage(code, scene->_localPlayerName);
        scene->_localPlayer->setPlayerDead();
        scene->_localPlayer->stopAllActions();
    }
    
}



void Pop::popExplosion(Game &scene){
    auto animation = Animation::create();
    for (unsigned i = 1;i<5;++i)
        animation->addSpriteFrameWithFileName(StringUtils::format("Explosion/CentreExplosion/Explosion_%d.png", i));
    animation->setDelayPerUnit(0.1f);
    auto animate = Animate::create(animation);
    auto hide = Hide::create();
    auto action = Sequence::create(animate, hide, NULL);
    
    Vector<Action*> actionGroup[4];
    Vector<Sprite*> spriteGroup[4];
    std::string AnimationFrameName[4];
    
    AnimationFrameName[0] = "Explosion/Left_%d/Explosion_left_%d.png";
    AnimationFrameName[1] = "Explosion/Right_%d/Explosion_right_%d.png";
    AnimationFrameName[2] = "Explosion/Up_%d/Explosion_up_%d.png";
    AnimationFrameName[3] = "Explosion/Down_%d/Explosion_down_%d.png";
    float popShiftPosition[4] = {-40, 40, 40, -40};
    
    
    for(int dirctions = 0; dirctions < 4; dirctions++)
    {
        if (_waterLength[dirctions] != 0)
        {
            for (int i = 1;i <= _waterLength[dirctions];++i)
            {
                auto sprite = Sprite::create();
                if(dirctions <= 1)
                    sprite->setPosition(Vec2(_popEx->getPositionX() + popShiftPosition[dirctions] * i, _popEx->getPositionY()));
                else
                    sprite->setPosition(Vec2(_popEx->getPositionX(), _popEx->getPositionY() + popShiftPosition[dirctions] * i));
                spriteGroup[dirctions].pushBack(sprite);
                _scene->_tileMap->addChild(sprite, 10);
                int k = 1;
                if (_waterLength[dirctions] == i)
                    k = 2;
                auto animation = Animation::create();
                for (int j = 1;j < 5;++j)
                    animation->addSpriteFrameWithFileName(StringUtils::format(AnimationFrameName[dirctions].c_str(), k, j));
                animation->setDelayPerUnit(0.1f);
                auto animate = Animate::create(animation);
                auto hide = Hide::create();
                auto delay = DelayTime::create(0.f);
                auto action = Sequence::create(delay, animate, hide, NULL);
                actionGroup[dirctions].pushBack(action);
            }
        }
    }
    
    _popEx->runAction(action);
    
    for (int direction = 0;direction < 4;++direction)
    {
        while (_waterLength[direction] != 0 && spriteGroup[direction].size()>0)
        {
            auto sprite = spriteGroup[direction].at(0);
            auto action = actionGroup[direction].at(0);
            sprite->runAction(action);
            spriteGroup[direction].eraseObject(sprite);
            actionGroup[direction].eraseObject(action);
        }
    }
   
    _scene->_playerPopCount[_scene->_playerPopName[_popEx]] -= 1;

    _scene->_popVector.eraseObject(_scene->_popVector.at(0));
    
    _scene->_realPopVector.eraseObject(_popEx);
    
    
}
void Pop::setPorp(Game &scene, int i){
    
    Vec2
    tileCoord=scene.tileCoordForPosition(_scene->_erasePopvector.at(i)->getPosition()) + Vec2(0,-1);
    int tileGid = scene._prop->getTileGIDAt(tileCoord);
    if (tileGid) {
        
        Prop* prop;
        Value properties = scene._tileMap->getPropertiesForGID(tileGid);
        ValueMap map = properties.asValueMap();
        bool isPut = false;
        
        std::string value = map.at("empty").asString();
        if (value == "true")
        {
            isPut = false;
        }
        
        value = map.at("shoe").asString();
        if(value == "true")
        {
            isPut = true;
            prop = Prop::create("Shoe.png");
            prop->setPropType(SHOE);
            _scene->_propVector.pushBack(prop);
        }
        
        value = map.at("water").asString();
        if(value == "true")
        {
            isPut = true;
            prop = Prop::create("Yaoshui.png");
            prop->setPropType(MEDCINE);
            _scene->_propVector.pushBack(prop);
        }
        
        value = map.at("pop").asString();
        if(value == "true")
        {
            isPut = true;
            prop = Prop::create("Paopao.png");
            prop->setPropType(BUBBLE);
            _scene-> _propVector.pushBack(prop);
        }
        
        if(isPut)
        {
            auto propPosition =_scene->_propVector.size() - 1;
            _scene->_tileMap->addChild(_scene->_propVector.at(propPosition),3);
            _scene->_propVector.at(propPosition)->setAnchorPoint(Vec2(0, 0));
            _scene->_propVector.at(propPosition)
            ->setPosition(Vec2(_scene->_erasePopvector.at(i)->getPositionX(),_scene-> _erasePopvector.at(i)->getPositionY() + 10));
            _scene->_propVector.at(propPosition)->setVisible(true);
            
            auto propAnimation = Animation::create();
            
            switch (_scene->_propVector.at(propPosition)->getPropType())
            {
                case SHOE:
                    for (int perFrame = 1;perFrame <= 3;++perFrame)
                        propAnimation->
                        addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("shoes_%d.png", perFrame)));
                    break;
                case BUBBLE:
                    for (int perFrame = 1;perFrame <= 3;++perFrame)
                        propAnimation->
                        addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("add_pop_count_%d.png", perFrame)));
                    break;
                case MEDCINE:
                    for (int perFrame = 1;perFrame <= 3;++perFrame)
                        propAnimation->
                        addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("add_pop_length_%d.png", perFrame)));
                    break;
            }
            propAnimation->setDelayPerUnit(0.1f);
            auto propAnimate = Animate::create(propAnimation);
            auto propForeverAnimate = RepeatForever::create(propAnimate);
            _scene->_propVector.at(propPosition)->runAction(propForeverAnimate);
        }
        
        //================End==================================================
        
    }
    
}
void Pop::eraseBlocks(Game &scene){
    //============  Erase the breakable blocks  =============   created by MrNickel
    
    //get length of bubbles
    int left = _waterLength[0] + 1;
    int right = _waterLength[1] + 1;
    int up = _waterLength[2] + 1;
    int down = _waterLength[3] + 1;
    
    //pop position
    Vec2 popPositionForTileMap = _scene->tileCoordForPosition(_popEx->getPosition());
    Sprite* tileUp, *tileDown;
    
    Sprite* emptySprite = Sprite::create("pop.png");
    emptySprite->setPosition(Vec2(-1, -1));
    
    //get the position of the bubbles
    _scene->_BubblePosition[0] = popPositionForTileMap - Vec2(left, 0);
    _scene-> _BubblePosition[1] = popPositionForTileMap + Vec2(right, 0);
    _scene->_BubblePosition[2] = popPositionForTileMap - Vec2(0, up);
    _scene->_BubblePosition[3] = popPositionForTileMap + Vec2(0, down);
    
    
    
    
    
    //Using loop to erase the blocks
    for (int i = 0; i < 4; ++i)
    {
        //get the tileObject from different dirctions
        // if we can't get a object , add a emptySprite to the vector
        if (_scene->_blockBottom->getTileAt(_scene->_BubblePosition[i]) != nullptr)
        {
            _scene->_erasePopvector.pushBack(_scene->_blockBottom->getTileAt(_scene->_BubblePosition[i]));
        }
        else
        {
            _scene->_erasePopvector.pushBack(emptySprite);
        }
        
        //Get the GID of each tile which soon will be set setVisble(false)
        _scene->_eraseIndex[i] = _scene->_blockBottom->getTileGIDAt(_scene->_BubblePosition[i]);
        
        // use another loop to set the block invisible
        if (_scene->_eraseIndex[i])
        {
            
            Value properties = _scene->_tileMap->getPropertiesForGID(_scene->_eraseIndex[i]);
            ValueMap map = properties.asValueMap();
            
            for (int j = 0; j < 4; j++)
            {
                std::string value = map.at(_scene->_propertyNames[j]).asString();
                if (value == "true")
                {
                    tileUp = _scene->_layerNames.at(j)->getTileAt(_scene->_BubblePosition[i] + Vec2(0, -1));
                    tileDown = _scene->_layerNames.at(j)->getTileAt(_scene->_BubblePosition[i]);
                    tileUp->setVisible(false);
                    tileDown->setVisible(false);
                }
            }
            
            if(_scene->_erasePopvector.at(i)->isVisible())
            {
                auto spriteEx = Sprite::create();
                spriteEx->setAnchorPoint(Vec2::ZERO);
                spriteEx->setPosition(_scene->_erasePopvector.at(i)->getPosition());
                _scene->_tileMap->addChild(spriteEx, 6);
                auto animation = Animation::create();
                animation->addSpriteFrameWithFile("Boom1.png");
                animation->addSpriteFrameWithFile("Boom2.png");
                animation->setDelayPerUnit(0.15f);
                auto animate = Animate::create(animation);
                auto hide = Hide::create();
                auto action = Sequence::create(animate, hide, NULL);
                spriteEx->runAction(action);
                setPorp(*_scene, i);
            }
            
            _scene->_erasePopvector.at(i)->setVisible(false);
        }
    }
    _scene->_erasePopvector.clear();
    
}

