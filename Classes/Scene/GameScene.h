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
*  @file     GameScene.h                                                  
*  @brief    游戏整体的操控类                                       
*  Details.                                                                
*                                                                            
*  @author   宋天佑  李坤                                                           
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


#ifndef _GameScene_h_
#define _GameScene_h_

#include <iostream>
#include "../NetWork/Client.h"
#include "cocos2d.h"
#include "json/document.h"
#include "ui/CocosGUI.h"
#include "../Data/ExternData.h"
#include "../Data/LevelData.h"
#include "../Manager/GameManager.h"
#include "../Manager/PopController.h"
#include "../Data/Role.h"
#include "../Data/Prop.h"
#include "../NetWork/MessageCode.h"

enum Type
{
    PLAYER = 1,
    POP = 2
};

enum PropType
{
    SHOE = 1,
    MEDCINE = 2,
    BUBBLE = 3
};

USING_NS_CC; 
/**
    * @brief Game类 继承自cocos2d:Layer
    * 这是整个游戏具体实现的类, 场面的宏大难以想象,每次看这个类,作者自己都会感到害怕\n
    * 收个类中包括了管理的manager类,包括了本地人物与远程人物,包括了泡泡,包括了地图的操控,以及坐标的一些转换.\n
    * SQLS泡泡堂项目从这个类开始.\n
    * 不说了,再说就要落泪了orz...
    */
class Game : public cocos2d::Layer
{
private:
    Size              _visibleSize;                       ///视窗大小    
    Vec2              _originSize;                        ///原始大小             
    int               _screenWidth;                       ///视窗宽度
    int               _screenHeight;                      ///视窗高度
    int               _eraseIndex[4];                     ///四个方向图块的删除顺序
    char              _propertyNames[4][10];              ///图块的属性
    Vec2              _BubblePosition[4];                 ///爆炸的坐标
    
    
    

    LevelData*                _inputData;                 ///选择人物时的数据
    Game*                     _thisScene;                 ///等于This指针
    Sprite*                   _breakableBlock;            ///可以被删除的block
    Sprite*                   _pop;                       ///泡泡
    std::vector<PlayerData>   _playerList;                ///所有远程玩家的信息
    
    
     
    std::string               _tileFile;                  ///地图的名字信息
    TMXTiledMap*              _tileMap;                   ///瓦片地图
    TMXLayer*                 _collidable;                ///collidable层
    TMXLayer*                 _blockBottom;               ///blockBottom层
    TMXLayer*                 _prop;                      ///prop层
    Vector<TMXLayer*>         _layerNames;                ///每个层的名字
    EventKeyboard::KeyCode    _code;                      ///每次按下的键盘信息
    
    
    Vector<Sprite*>           _moneyCode;                 ///二维码
    Vector<Sprite*>           _popVector;                 ///泡泡Vector
    Vector<Sprite*>           _realPopVector;             ///真正的泡泡Vector(用来延时爆炸)
    Vector<Sprite*>           _breakableBlockVector;      ///可以被摧毁的图块的集合
    Vector<Sprite*>           _erasePopvector;            ///将要爆炸的pop
    Vector<Prop*>             _propVector;                ///道具Vector
    Vector<Button*>           _endButton;                 ///结束游戏按钮

    
    
    
    Role*                              _player;           ///当前被操控的玩家
    Role*                              _localPlayer;      ///本地玩家
    Client*                            _client;           ///服务端指针
    std::string                        _localPlayerName;  ///本地玩家的名字
    Player*                            _playerManager;    ///玩家操控Manager
    Manager*                           _sceneManager;     ///scene Manager
    Pop*                               _popManager;       ///泡泡操纵者
    std::map<std::string, Role*>       _remotePlayer;     ///远程玩家的集合

    
    
    

    std::map<std::string,int>                         _playerPopCount;      ///每个玩家可以防止的泡泡数量
    std::map<Sprite*,std::string>                     _playerPopName;       ///每个泡泡对应的玩家Name
    std::map<std::string,EventKeyboard::KeyCode>      _keyPressesedOrder;   ///键盘按下的按键
    std::map<std::string,EventKeyboard::KeyCode>      _keyReleasedOrder;    ///键盘松开的按键
    std::map<EventKeyboard::KeyCode,std::string>      _keyPressesedOrders;  ///键盘按下的按键
    std::map<EventKeyboard::KeyCode,std::string>      _keyReleasedOrders;   ///键盘松开的按键
    std::map<EventKeyboard::KeyCode, bool>            _keys;                ///记录按键是否按下
    std::queue<std::string>                           _command;             ///每次读取的命令
    
public:
    
    friend class Manager;
    friend class Player;
    friend class Pop;
    friend class Client;
    
    /** 
    * @brief 初始化各种信息
    * @return bool
    */
    
    virtual bool init();

    /** 
    * @brief 初始化瓦片地图
    * @return bool
    */
    void initTileMap();
    
    /** 
    * @brief 初始化网络传来的命令
    * @return void
    */
    void initKeyCommand();
    
    /** 
    * @brief 初始化breakBlock
    * @return void
    */
    void initBreakableSprite();
    /** 
    * @brief 初始远程玩家
    * @return void
    */
    void initRemotePlayer();
    
    /** 
    * @brief Game的create函数
    * @param data 参数1
    * @param client 参数2
    * @param playerName 参数3
    * @return  Scene*
    */
    static Scene* createScene(LevelData &data,Client* client,std::string playerName);
    /** 
    * @brief 像素坐标与tileCoord坐标之间的转变
    * @param Const position 参数1
    * @return  Vec2
    */
    Vec2 tileCoordForPosition(const Vec2& position);
    /** 
    * @brief 像素坐标与tileCoord坐标之间的转变
    * @param Const position 参数1
    * @return  Vec2
    */
    Vec2 positionForTileCoord(const Vec2& tileCoord);
    /** 
    * @brief 根据名字得到动画
    * @param animName 参数1
    * @param delay 参数2
    * @param animNum 参数3
    * @return  Animate*
    */
    Animate* getAnimateByName(std::string animName, float delay, int animNum);
    /** 
    * @brief 是不是Press了正确的键
    * @param code 参数1
    * @return  bool
    */
    bool pressTest(EventKeyboard::KeyCode code);
        /** 
    * @brief 是否Press了
    * @param code 参数1
    * @return  bool
    */
    bool isKeyPressed(EventKeyboard::KeyCode code);
    /** 
    * @brief 判断此坐标是否会碰撞
    * @param tileCoord 参数1
    * @param type 参数2
    * @return  bool
    */
    bool collidable(Vec2 tileCoord,int type);
    
    /** 
    * @brief 对于画面的每一帧进行升级
    * @param delta 参数1
    * @return  void
    */
    void update(float delta);
    /** 
    * @brief 初始化PopVector
    * @return  void
    */
    void initPopCountVector();
    /** 
    * @brief 在按键按下的时候我们要做的事情
    * @return  void
    */
    void keyPressedDuration(EventKeyboard::KeyCode code);
    /** 
    * @brief 像素坐标与tileCoord坐标之间的转变
    * @param left 参数1
    * @param right 参数2
    * @param up 参数3
    * @param down 参数4
    * @return  void
    */
    void eraseBlocks(Sprite* pop,float left,float right,float up,float down);
    /** 
    * @brief 是否接触到了道具
    * @param tileCoord 是否接触到人物
    * @param type      是泡泡接触到道具还是人物
    * @return  void
    */
    void PropCollidableTest(Vec2 tileCoord,int type);
    
    
    CREATE_FUNC(Game);
};



#endif /* GameScene_h */

