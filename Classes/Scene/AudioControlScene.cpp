/*****************************************************************************
*  Copyright (C) 2017 宋天佑 13167211978@qq.com                                  
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
*  @file     AudioControlScene.cpp                                                 
*  @brief    游戏音量控制类                                       
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
*  2017/06/02 | 4.0.1。5  | 宋天佑          | Create file                     
*----------------------------------------------------------------------------
*                                                                            
*****************************************************************************/

#include "AudioControlScene.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace ui;

Scene* AudioControl::createScene() {
	auto scene = Scene::create();
	auto layer = AudioControl::create();
	scene->addChild(layer);
	return scene;
}


bool AudioControl::init() {
	if (!Layer::create())
	{
		return false;
	}

	Size visible_size = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto bg = Sprite::create("bg.png");
    this->addChild(bg);
    bg->setPosition(Vec2(visible_size.width/2, visible_size.height/2));

	auto music_text = Label::createWithTTF("MusicSound", "fonts/Marker Felt.ttf", 32);
	music_text->setPosition(Vec2(visible_size.width*0.25 + origin.x, origin.y + visible_size.height*0.7));
	this->addChild(music_text);

	auto music_slider = Slider::create();
	music_slider->loadBarTexture("sliderTrack.png");
	music_slider->loadSlidBallTextures("sliderThumb.png", "sliderThumb.png", "");
	music_slider->loadProgressBarTexture("sliderProgress.png");
	float musicPercent = UserDefault::getInstance()->getFloatForKey("musicPercent");

	if (musicPercent == 0.0f) {
		musicPercent = 100.0f;
	}

	music_slider->setPercent(musicPercent);
	music_slider->setPosition(Vec2(origin.x + visible_size.width*0.6, origin.y + visible_size.height*0.7));

	music_slider->addEventListener([=](Ref* pSender, Slider::EventType type) {

		if (type == Slider::EventType::ON_PERCENTAGE_CHANGED) {
			int percent = music_slider->getPercent();
			CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(static_cast<float>(percent) / 100);
			UserDefault::getInstance()->setFloatForKey("musicPercent", percent);
		}
	});
	this->addChild(music_slider);
	auto sound_text = Label::createWithTTF("EffectSound", "fonts/Marker Felt.ttf", 32);
	sound_text->setPosition(Vec2(origin.x + visible_size.width*0.25, origin.y + visible_size.height*0.5));
	this->addChild(sound_text);


	auto effects_slider = Slider::create();
	effects_slider->loadBarTexture("sliderTrack.png");
	effects_slider->loadSlidBallTextures("sliderThumb.png", "sliderThumb.png", "");
	effects_slider->loadProgressBarTexture("sliderProgress.png");
	float effectPercent = UserDefault::getInstance()->getFloatForKey("effectPercent");

	if (effectPercent == 0.0f) {
		effectPercent = 100.0f;
	}
	effects_slider->setPercent(effectPercent);
	effects_slider->setPosition(Vec2(origin.x + visible_size.width*0.6, origin.y + visible_size.height*0.5));
	effects_slider->addEventListener([=](Ref* pSender, Slider::EventType type) {
		if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
		{
			int percent = effects_slider->getPercent();
			CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(static_cast<float> (percent) / 100);
			UserDefault::getInstance()->setFloatForKey("effectPercent", percent);
		}
	});
	
	this->addChild(effects_slider);
	auto return_button = Button::create("back_button.png");

	return_button->cocos2d::Node::setScale((visible_size.width * 0.1 / return_button->getContentSize().width));
	return_button->setPosition(Vec2(origin.x + visible_size.width - return_button->getContentSize().width / 2, origin.y + return_button->getContentSize().height / 2));
	return_button->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED) {
			auto transition = TransitionSlideInL::create(0.5, GameMenu::createScene());
			Director::getInstance()->replaceScene(transition);
		}
	});

	this->addChild(return_button);

	return true;
}

























