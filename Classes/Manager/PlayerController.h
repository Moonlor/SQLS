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
*  2017/06/02 | 4.0.1.5  | 宋天佑            | Create file                     
*----------------------------------------------------------------------------
*                                                                            
*****************************************************************************/

#ifndef _playerController_h_
#define _playerController_h_

#include <iostream>
#include "cocos2d.h"
#include "json/document.h"

USING_NS_CC;

class Game;
/**
    * @brief Player类 继承自cocos2d:cocos2d
    * 这是Player的控制类,无论是远程玩家还是本地玩家,我们都会基于一个相同的操控机制,利用指针进行操控,大大减少了无谓的代码量
    */
class Player :public cocos2d::Node
{
    
protected:
    
    std::string         _role;                                                           /// 玩家的角色类型
    Vec2                _bornPosition[4];                                                /// 玩家的出生位置
public:
        /** 
    * @brief Player的create函数
    * @param role  人物角色类型
    * @param scene Game场景 
    * @return  Player*
    */
    static Player* create(std::string role ,Game &scene)                                                    
    {
        Player *player = new Player();
        if (player)
        {
            player->setRole(role);
            player->initLocalPlayer(role, scene);
            player->initRemotePlayer(role, scene);
            
            return player;
        }
        CC_SAFE_DELETE(player);
        return nullptr;
    }
    /** 
    * @brief 初始化本地人物
    * @param roleName  人物角色类型
    * @param scene Game场景 
    * @return  void
    */
    void   initLocalPlayer(std::string roleName, Game &scene);
    /** 
    * @brief 初始化远程人物
    * @param oleName  人物角色类型
    * @param scene Game场景 
    * @return  void
    */                                 
    void    initRemotePlayer(std::string roleName, Game& scene);   
    /** 
    * @brief 设置人物永久走动
    * @param code  按键信息
    * @param scene Game场景 
    * @return  void
    */                             
    void    playerForeverMove(EventKeyboard::KeyCode code, Game &scene); 
    /** 
    * @brief 设置人物永久走动
    * @param code  按键信息
    * @param scene Game场景 
    * @return  void
    */                    
    void    executePlayerForeverMove(Game &scene, EventKeyboard::KeyCode code);
    /** 
    * @brief 每一帧来更新Player信息
    * @param delta Update默认参数
    * @param scene Game场景 
    * @patam code 按键信息
    * @return  void
    */                 
    void     updatePlayerPerFrame(int delta, Game &scene, EventKeyboard::KeyCode code);   
    /** 
    * @brief 设置人物角色
    * @param role  人物角色类型
    * @return void
    */     
    void     setRole(std::string role) { _role = role; } 
    /** 
    * @brief 设置玩家死亡
    * @param roleName  人物角色
    * @param scene     Game场景 
    * @return  void
    */                                        
    void     setPlayerDied(std::string roleName, Game &scene);       
    /** 
    * @brief 得到角色信息
    * @return  string
    */                                  
    std::string       getRole() { return _role;}                                                       
    
};

#endif /* _playerController_h_ */
