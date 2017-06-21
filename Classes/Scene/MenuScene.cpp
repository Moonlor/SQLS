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
*  @file     MenuScene.cpp
*  @brief    游戏进入的第二个场景，包含菜单栏及音乐控制按钮
*  
*
*  @author   李坤
*  @email    1061152718@qq.com
*  @version  4.0.1.5(版本号)
*  @date     2017/06/11
*  @license  Massachusetts Institute of Technology License (MIT)
*
*----------------------------------------------------------------------------
*  Remark         : Description
*----------------------------------------------------------------------------
*  Change History :
*  <Date>     | <Version> | <Author>       | <Description>
*----------------------------------------------------------------------------
*  2017/06/08 | 2.0.1.1   | 李坤            | Create file
*----------------------------------------------------------------------------
*
*****************************************************************************/

#include "MenuScene.h"
#include "ui/CocosGUI.h"
#include "AudioControlScene.h"
#include "SimpleAudioEngine.h"
#include "LoadLayer.h"

USING_NS_CC;

using namespace ui;


void GameMenu::createLoadScene()
{
	loadScene = Scene::create();
	LoadLayer * layer = LoadLayer::create();
	loadScene->addChild(layer);
}


Scene* GameMenu::createScene()
{
	auto scene = Scene::create();
	auto layer = GameMenu::create();
	scene->addChild(layer);

	return scene;
}



bool GameMenu::init() {

	if (!Layer::init()) 
	{
		return false;
	}	

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto bg = Sprite::create("bg.png");
    this->addChild(bg);
    bg->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    
	//==============================StartButton===========================
	auto start_button = Button::create("button.png");
	start_button->setScale(1.0);
	start_button->setTitleText("Start");
	start_button->setTitleFontSize(20);
	start_button->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 4 * 3));

	start_button->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED) {
		
			auto transition = TransitionSlideInL::create(0.5, NetMenu::createScene());
			Director::getInstance()->replaceScene(transition);

		}
	});
	this->addChild(start_button);

	//==============================SettingButton========================
	auto set_button = Button::create("button.png");
	set_button->setScale(1.0);
	set_button->setTitleText("Setting");
	set_button->setTitleFontSize(20);
	set_button->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 4 * 2));

	set_button->addTouchEventListener([](Ref* psender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED) {

			auto transition = TransitionSlideInL::create(0.5, AudioControl::createScene());
			Director::getInstance()->replaceScene(transition);

		}
	});
	this->addChild(set_button);


	//==============================QuitButton===========================
	auto close_button = Button::create("button.png");
	close_button->setScale(1.0);
	close_button->setTitleText("Quit");
	close_button->setTitleFontSize(20);
	close_button->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 4 * 1));

	close_button->addTouchEventListener([](Ref *pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED) {

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
			MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
			return;
#endif
			Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			exit(0);
#endif

		}
	});
	this->addChild(close_button);



	//==============================MusicButton===========================
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("The Crave.mp3", true);
	is_paused = false;

	auto music_button = MenuItemImage::create("music.png", "nomusic.png");
	auto pause_button = MenuItemImage::create("nomusic.png","music.png");

	MenuItemToggle *toggleItem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(GameMenu::menuMusicCallBack, this), music_button, pause_button, NULL);
	toggleItem->setScale(1.0f);
	toggleItem->setPosition(Point(origin.x + visibleSize.width * 0.9, origin.y + visibleSize.height * 0.1));
	auto menu = Menu::create(toggleItem, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu);

	return true;
}

void GameMenu::menuMusicCallBack(cocos2d::Ref* pSender)
{
	
	if (is_paused == false)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
		is_paused = true;
	}
	else
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
		is_paused = false;
	}
}
