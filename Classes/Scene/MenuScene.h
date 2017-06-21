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
*  @file     MenuScene.h
*  @brief    游戏进入的第二个场景，包含菜单栏及音乐控制按钮
*  
*
*  @author   李坤
*  @email    1061152718@qq.com
*  @version  4.0.1.5(版本号)
*  @date     2017/01/15
*  @license  Massachusetts Institute of Technology License (MIT)
*
*----------------------------------------------------------------------------
*  Remark         : Description
*----------------------------------------------------------------------------
*  Change History :
*  <Date>     | <Version> | <Author>       | <Description>
*----------------------------------------------------------------------------
*  2017/04/15 | 2.0.1.1   | 李坤            | Create file
*----------------------------------------------------------------------------
*
*****************************************************************************/

#ifndef _MenuScene_h_
#define _MenuScene_h_

#include "cocos2d.h"
#include "GameScene.h"
#include "AudioControlScene.h"
#include "NetMenu.h"

/**
* 	@brief  游戏主菜单界面\n
* 			创建游戏主菜单, 包括开始游戏按钮, 游戏设置按钮, 退出游戏按钮, 音乐开关按钮
*/

class GameMenu : public cocos2d::Layer
{
public:

	Scene * loadScene;		///指向加载界面的指针
	bool 	is_paused;		///游戏音乐是否被暂停

	/**
	* @brief  	创建加载场景 
	*/
	void createLoadScene();

	/**
	* @brief  	创建主菜单场景
	*/
	static cocos2d::Scene* createScene();

	/**
	* @brief  	当点击音乐按钮时调用该函数
	*        	-<em>音乐正在播放</em> 静音
    *        	-<em>音乐不在播放</em> 开启声音
    * 
    * @param 	psender    指针, 指向调用回调函数的CCObject
	*/
	void menuMusicCallBack(cocos2d::Ref* pSender);

	/**
	* @brief  	自定义的initial函数, 向场景中添加各个按钮并定义相关的回调函数
	*/
	virtual bool init();

	/**
	* @brief  	在create()后调用init(), 并让相应对象加入cocos自动内存管理系统
	*/
	CREATE_FUNC(GameMenu);
};


#endif /* _MenuScene_h_ */
