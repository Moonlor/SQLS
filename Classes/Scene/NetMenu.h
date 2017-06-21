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
*  @file     NetMenu.h
*  @brief    网络选择界面, 在此选择局域网对战或互联网\n
*            对战，并启动相应的local server和client
*  
*
*  @author   李坤 宋天佑
*  @email    1061152718@qq.com
*  @version  4.0.1.5(版本号)
*  @date     2017/06/06
*  @license  Massachusetts Institute of Technology License (MIT)
*
*----------------------------------------------------------------------------
*  Remark         : Description
*----------------------------------------------------------------------------
*  Change History :
*  <Date>     | <Version> | <Author>       | <Description>
*----------------------------------------------------------------------------
*  2017/06/06 | 4.0.0.1   | 李坤            | Create file
*----------------------------------------------------------------------------
*
*****************************************************************************/
#ifndef _NetMenu_h_
#define _NetMenu_h_
#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "json/document.h"
#include "../Data/LevelData.h"
#include "../Scene/GameScene.h"
#include "../Scene/MenuScene.h"
#include "ui/CocosGUI.h"
#include "../NetWork/LocalServer.h"
#include "../Scene/RoomScene.h"
#include "../NetWork/Client.h"

USING_NS_CC_EXT;

/**
    * @brief 网络选择界面 \n
    * 在此选择局域网对战或互联网对战，并启动相应的local server和client\n
    * 在局域网模式下, 只有选择创建房间时才会同时启动client和server, 否则只启动client\n
    * 在互联网模式下, 只启动client
*/
class NetMenu : public cocos2d::Layer,public EditBoxDelegate
{
protected:
    int             _selectLevelIndex;      ///地图对应的索引

    std::string     _playerName;            ///玩家名称

    Button *        _nameStartButton;       ///指向输入框的指针
    Button *        _pleaseStartButton;     ///指向确认输入按钮的指针

public:
    /** 
    * @brief    单例对象创建函数
    *
    * @return   指向网络菜单场景的指针
    */
    static cocos2d::Scene* createScene();

    /** 
    * @brief    自定义的初始化函数
    *
    * @return   初始化的结果
    *        -<em>false</em>    初始化失败
    *        -<em>true</em>     初始化成功
    */
    virtual bool init();
    
    /** 
    * @brief    关闭游戏的回调函数
    * 
    * @param    psender         指针, 指向调用回调函数的CCObject
    *
    */
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    /**
    * @brief    在create()后调用init(), 并让相应对象加入cocos自动内存管理系统
    */
    CREATE_FUNC(NetMenu);

    /** 
    * @brief    检测输入框的变化
    * 
    * @param    psender         指针, 指向调用回调函数的CCObject
    * @param    type            要响应的事件类型
    */
    void textFieldEvent(Ref *pSender, cocos2d::ui::TextField::EventType type);

    /** 
    * @brief    当动作开始的时候调用的函数
    * 
    * @param    editBox         指针，指向被操作的输入框对象
    */
    void editBoxEditingDidBegin(EditBox* editBox);

    /** 
    * @brief    当离开输入框的时候调用的函数
    * 
    * @param    editBox         指针，指向被操作的输入框对象
    */
    void editBoxEditingDidEnd(EditBox* editBox);

    /** 
    * @brief    当输入框中内容被改变的时候调用的函数
    * 
    * @param    editBox         指针，指向被操作的输入框对象
    * @param    text            输入的字符串
    */
    void editBoxTextChanged(EditBox* editBox, const std::string& text);

    /** 
    * @brief    按下回车时调用的函数
    * 
    * @param    editBox         指针，指向被操作的输入框对象
    */
    void editBoxReturn(EditBox* editBox);
};

#endif /* _NetMenu_h_ */
