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
 *  @file     Prop.h
 *  @brief    道具的信息类
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

#ifndef Prop_h
#define Prop_h


#include <iostream>
#include "cocos2d.h"
USING_NS_CC;
using namespace ui;
/**
 * @brief 道具类 继承自cocos2d:Sprite
 * 这是Pop的控制类,这里存储着道具的种类
 */
class Prop: public cocos2d::Sprite
{
private:
    int _propType; /// 道具种类
public:
    
    /** 
    * @brief Prop的create函数
    * @param filename文件的名字
    * @return  Prop*
    */
    static Prop* create(const std::string& filename)
    {
        Prop *sprite = new Prop();
        if (sprite && sprite->initWithFile(filename))
        {
            sprite->autorelease();
            return sprite;
        }
        CC_SAFE_DELETE(sprite);
        return nullptr;
    }
    
    /** 
    * @brief Prop的create函数
    * @param filename文件的名字
    * @return  Prop*
    */
    static Prop* createWithSpriteFrameName(const std::string& filename)
    {
        Prop *sprite = new Prop();
        if (sprite && sprite->initWithSpriteFrameName(filename))
        {
            sprite->autorelease();
            return sprite;
        }
        CC_SAFE_DELETE(sprite);
        return nullptr;
    }
    
    /** 
    * @brief 设置道具的类型
    * @param type 类型
    * @return  void
    */
    void setPropType(int type)
    {
        _propType = type;
    }
    
    /** 
    * @brief 获得道具的类型
    * @return  int
    */
    int getPropType()
    {
        return _propType;
    }

};




#endif /* Prop_h */
