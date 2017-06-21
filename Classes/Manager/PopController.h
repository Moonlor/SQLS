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
 *  @file     PopController.h
 *  @brief    泡泡的操控类
 *  Details.
 *
 *  @author   宋天佑 孙允鑫
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

#ifndef _PopController_h_
#define _PopController_h_

#include <iostream>
#include "cocos2d.h"
#include "json/document.h"

USING_NS_CC;

class Game;
/**
 * @brief Pop类 继承自cocos2d:Node
 * 这是Pop的控制类,这里控制着泡泡的释放与爆炸！
 */
class Pop :public cocos2d::Node
{
    
protected:
    
    int                _powerAbility;         ///每个人的水泡的长度
    int                _waterLength[4];       ///四个方向水柱的长度
    Sprite*            _pop;                  ///具体操控的泡泡
    Sprite*            _popEx;                ///即将要爆炸的泡泡
    Game*              _scene;                ///Game 场景
    Vector<Sprite*>*   _popVector;            ///整体泡泡的Vector
    
public:
    
    /** 
    * @brief Pop的create函数
    * @param scene Game场景 
    * @return  Pop*
    */
    static Pop* create(Game &scene)
    {
        Pop *pop = new Pop();
        if (pop)
        {
            return pop;
        }
        CC_SAFE_DELETE(pop);
        return nullptr;
    }
    /** 
    * @brief 初始化场景信息
    * @param scene Game场景 
    * @return  void
    */
    void setScene(Game* scene);
    /** 
    * @brief 每一帧升级泡泡
    * @param scene Game场景 
    * @param delta Update函数默认参数
    * @param code 按键的信息(空格)
    * @return  void
    */
    void updatePopPerFrame(int delta, Game &scene, EventKeyboard::KeyCode code);
    /** 
    * @brief 回调函数,设置泡泡爆炸的时间是2.5秒
    * @return  void
    */
    void callFunc2();
    /** 
    * @brief 判断是否放置泡泡
    * @param scene Game场景 
    * @return  void
    */
    bool whetherPutPop(Game& scene);
    /** 
    * @brief 放置泡泡
    * @param scene Game场景 
    * @return  void
    */
    void putPop(Game &scene);
    /** 
    * @brief 改变水花的长度
    * @param scene Game场景 
    * @powerAbibity 每个人的水花的长度
    * @return  void
    */
    void changeWaterLengh(Game* scene, int powerAbility);
    /** 
    * @brief 泡泡的爆炸
    * @param scene Game场景 
    * @return  void
    */
    void popExplosion(Game & scene);
        /** 
    * @brief 进行图块的消去
    * @param scene Game场景 
    * @return  void
    */
    void eraseBlocks(Game& scene);
    /** 
    * @brief 放置道具
    * @param scene Game场景 
    * @ i    放置道具的种类
    * @return  void
    */
    void setPorp(Game& scene,int i);
    
    
};


#endif /* _PopController_h_ */
