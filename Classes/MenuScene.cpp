//
//  MenuScene.cpp
//  Airfight
//
//  Created by 李坤 on 2017/4/15.
//	Changed by 奇巴图 on 2017/5/8
//

#include "MenuScene.h"
#include "ui/CocosGUI.h"
#include "AudioControlScene.h"

USING_NS_CC;

using namespace ui;

Scene* GameMenu::createScene()
{
	// 创建一个场景对象，该对象将会由自动释放池管理内存的释放
	auto scene = Scene::create();
	// 创建层对象，该对象将会由自动释放池管理内存的释放
	auto layer = GameMenu::create();
	// 将GameMenu层作为子节点添加到场景
	scene->addChild(layer);
	// 返回场景对象
	return scene;
}



bool GameMenu::init() {
	if (!Layer::init()) {
		return false;
	}
	//获得屏幕（窗口）大小
	Size visibleSize = Director::getInstance()->getVisibleSize();
	//获得坐标原点
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	//==============================开始游戏按钮
	//创建一个按钮
	auto start_button = Button::create("button.png");
	start_button->setScale(1.0);
	start_button->setTitleText("Start");
	//start_button->setTitleFontName("微软雅黑");
	start_button->setTitleFontSize(20);
	//将按钮放在窗口高度三分之一的位置
	start_button->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height*0.75));
	//创建触摸事件监听器
	start_button->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED) {
			//创建场景转换
			auto transition = TransitionSlideInL::create(0.5, Game::createScene());
			//进行转换
			Director::getInstance()->replaceScene(transition);
		}
	});
	this->addChild(start_button);


	//==============================游戏设置按钮
	auto set_button = Button::create("button.png");
	set_button->setScale(1.0);
	set_button->setTitleText("Setting");
	//set_button->setTitleFontName("微软雅黑");
	set_button->setTitleFontSize(20);
	set_button->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height*0.5));
	set_button->addTouchEventListener([](Ref* psender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED) {
			auto transition = TransitionSlideInL::create(0.5, AudioControl::createScene());
			Director::getInstance()->replaceScene(transition);
		}
	});
	this->addChild(set_button);


	//==============================退出游戏按钮
	auto close_button = Button::create("button.png");
	close_button->setScale(1.0);
	close_button->setTitleText("Quit");
	//close_button->setTitleFontName("微软雅黑");
	close_button->setTitleFontSize(20);
	close_button->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height*0.25));
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

	return true;


}
