/*****************************************************************************
*  Copyright (C) 2017 李坤 1061152718@qq.com
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
*  @file     SearchScene.cpp
*  @brief    选择加入游戏时进入该场景，可搜索并显示房间列表
*  
*
*  @author   宋天佑 李坤
*  @email    1061152718@qq.com
*  @version  4.0.1.5
*  @date     2017/06/06
*  @license  Massachusetts Institute of Technology License (MIT)
*
*----------------------------------------------------------------------------
*  Remark         : Description
*----------------------------------------------------------------------------
*  Change History :
*  <Date>     | <Version> | <Author>       | <Description>
*----------------------------------------------------------------------------
*  2017/06/06 | 4.0.0.1   | 宋天佑            | Create file
*----------------------------------------------------------------------------
*
*****************************************************************************/

#include <stdio.h>
#include "MenuScene.h"
#include "ui/CocosGUI.h"
#include "SearchScene.h"
#include "MessageCode.h"

USING_NS_CC;
using namespace ui;

int loop = 0;                                       ///用于计时，每100帧向网络中发送一次查询信息

static Client *             client = nullptr;       ///指向client的指针，用于传输数据和接受指令
static std::string          _static_player_name;    ///玩家名称
std::vector<std::string>    room_owner;             ///房间拥有者的昵称

Scene* SearchScene::createScene(Client* c, std::string _playerName)
{
    client = c;
    _static_player_name = _playerName;

    auto scene = Scene::create();
    auto layer = SearchScene::create();

    scene->addChild(layer);

    return scene;
}

bool SearchScene::init()
{
    _player_name = _static_player_name;

    if (!Layer::init())
    {
        return false;
    }

    this->addChild(client);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto RoomListBg = Sprite::create("RoomList_bg.png");
    RoomListBg->setScale(0.7f,0.7f);
    RoomListBg->setPosition(origin / 2 + visibleSize / 2);
    _room_list_bg = RoomListBg;
    this->addChild(RoomListBg,1);
    
    auto bg = Sprite::create("bg.png");
    bg->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    this->addChild(bg);
    
    this->scheduleUpdate();

    return true;
}

void SearchScene::update(float delta)
{
    
    if(client->_search_finished)
    {
        loop++;

        if(loop / 100 > 1 ){
            loop = 0;
            client->sendMessage(QUERY_FOR_ROOM, "|||||||||||");
        }

        std::string temp = client->executeOrder();

        if(temp != "no"){

//            std::cout << "search: " << temp << std::endl;

            if(temp[0] == ANSWER_FOR_ROOM[0]){

//                std::cout << "Find you\n";
                std::string owner_name = temp.substr(1, temp.size() - 1);
                
                bool if_exist = false;
                for (int i = 0; i < room_owner.size(); i++) {
                    if(owner_name == room_owner.at(i)){
                        if_exist = true;
                    }
                }
                
                if(!if_exist)
                {
                    room_owner.push_back(owner_name);
                    
                    auto board = Button::create("PlayerBar.png", "PlayerBar.png");
                    auto in_button = Button::create("RoomList_connect.png");
                    board->addChild(in_button);

                    in_button->setPosition(Vec2(board->getContentSize().width / 6 * 5, board->getContentSize().height/2));
                    in_button->setScale(0.8f,0.7f);
                    
                    button_list.pushBack(in_button);
                    std::string button_owner_name = owner_name;
                    
                    in_button->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
                        if (type == Widget::TouchEventType::ENDED) {

                            client->_filter_mode = true;
                            client->sensitive_word = button_owner_name;
                            
                            auto transition = TransitionSlideInL::create(0.5, RoomScene::createScene(client, 1, _player_name));
                            Director::getInstance()->pushScene(transition);
                            
                        }
                    });
                    board->setTitleText(owner_name);
                    board->setTitleFontSize(30);
                    _room_list_bg->addChild(board);
                    
                    board->setPosition(Vec2(
                                            _room_list_bg->getContentSize().width/2,
                                            (_room_list_bg->getContentSize().height - 90)/5 * (5 - room_owner.size())));
                }
            }
        }
    }
    
}
