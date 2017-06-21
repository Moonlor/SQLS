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
*  15750659557@163.com我们期待能和您互相交流合作, 学习更多的知识.                                      
*                                                                            
*  另外注意: 此项目需要您自行配置cocos环境,安装boost库, 如果遇到相关问题的话, 欢迎将     
*  错误日志发给我们, 您的帮助将有助于改善游戏的体验.                                  
*                                                                            
*  @file     LoadLayer.h                                                  
*  @brief    游戏所需资源加载缓存类                                       
*  Details.                                                                
*                                                                            
*  @author   奇巴图                                                            
*  @email    642701967@qq.com                                               
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
*  2017/06/02 | 4.0.1.5   | 奇巴图          | Create file                     
*----------------------------------------------------------------------------
*                                                                            
*****************************************************************************/

#ifndef __LoadLayer_h_
#define __LoadLayer_h_

#include "iostream"
#include "cocos2d.h"
#include "MenuScene.h"
USING_NS_CC;

/**
    * @brief 预加载类
    * 完成预加载背景音乐、精灵表单等准备工作
    */
class LoadLayer : public cocos2d::Layer
{
protected:
    int 				_sourceCount;									///用来保存游戏需要加载的资源总数
    ProgressTimer* 		_progress;										///进度条
    float 				_progressInterval;								///进度条更新的次数
public:
		/**
     * @brief 静态函数，创建Scene场景
     * @return 类实例的指针
     */
    static 				cocos2d::Scene* createScene();
    	/**
     * @brief init函数，完成初始化操作
     * @retrun bool值
     */					
    virtual 			bool init();

    virtual 			void onEnterTransitionDidFinish();
		/**
     * @brief 加载背景音乐函数
     * @param musicFiles 参数1 音乐文件
     */	
    void 				loadMusic(ValueVector musicFiles);
		/**
     * @brief 
     * @param spriteSheets 参数1 精灵表单
     */	
    void 				loadSpriteSheets(ValueVector spriteSheets);		
    	/**
     * @brief 进度条更新函数
     */	
    void 				progressUpdate();								
    	/**
     * @brief 使用CREATE_FUNC宏创建当前类的对象，返回的对象将会由自动释放池管理内存的释放
     */
    CREATE_FUNC(LoadLayer);												
};

#endif /* _LoadLayer_h_ */
