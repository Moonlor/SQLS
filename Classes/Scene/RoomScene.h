/*****************************************************************************
 *  Copyright (C) 2017 李坤 13167211978@163.com
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
 *  @file     RoomScene.h
 *  @brief    房间信息类
 *  Details.
 *
 *  @author   李坤 宋天佑
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
 *  2017/06/02 | 4.0.1.5  | 李坤            | Create file
 *----------------------------------------------------------------------------
 *
 *****************************************************************************/

#ifndef _RoomScene_h_
#define _RoomScene_h_

#include <iostream>
#include "cocos2d.h"
#include "json/document.h"
#include "../Data/LevelData.h"
#include "../Scene/GameScene.h"
#include "../NetWork/LocalServer.h"
#include "../NetWork/Client.h"
#include "../Data/ExternData.h"
/**
 * @brief 房间类,开房开房开房！继承自cocos2d:Layer
 * 这里房间信息可以实时更新,玩家可以随便聊天
 */
class RoomScene : public cocos2d::Layer
{
protected:
    Vector<Sprite*>     _exist_player;             ///目前房间里的Player
    int                 _selectLevelIndex;         ///目前选择的地图索引
    Sprite*             _room_ptr;                 ///房间背景
    std::string         _owner_player_name;        ///房主的姓名
    PlayerData*         _owner_player_data;        ///房主的个人信息
    Button*             _start_game_button;        ///开始游戏按钮
    std::string         _chatString;               ///聊天信息
    TextField*          _chatWindow;               ///聊天输入框
public:
    LevelData*          _game_data;                ///游戏信息

    /** 
    * @brief                    Room的static create函数
    * @param    c               客户端指针
    * @param    mode            服务端or客户端
    * @param    _playerName     玩家姓名
    * @return   Scene*
    */
    static cocos2d::Scene* createScene(Client* c, int mode, std::string _playerName);

    /** 
    * @brief 做一些初始化的工作
    * @return  bool
    */
    virtual bool init();
    /** 
    * @brief 聊天输入框活动函数
    * @param type 活动的种类
    * @param pSender 信息传输指针
    * @return  void
    */
    void textFieldEvent(Ref *pSender, cocos2d::ui::TextField::EventType type);
    /** 
    * @brief 初始化服务端
    * @return  void
    */
    bool initForServer(void);
    /** 
    * @brief 初始化客户端
    * @return  void
    */
    bool initForClient(void);
    /** 
    * @brief 找到玩家ID
    * @return  int
    */
    int  findPlayerId();
    /** 
    * @brief 每帧对房间进行更新
    * @return  void
    */
    void update(float delta);
 
    CREATE_FUNC(RoomScene);
};

#endif /* _RoomScene_h_ */
