

#include "AudioControlScene.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace ui;

Scene* AudioControl::createScene(){
    auto scene = Scene::create();
    auto layer = AudioControl::create();
    scene->addChild(layer);
    return scene;
}


bool AudioControl::init(){
    if(!Layer::create())
    {
        return false;
    }
    
    Size visible_size = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    //----------------------------------------------背景音乐
    
    auto music_text = Text::create("背景音乐","Arial",20);
    music_text->setPosition(Vec2(visible_size.width*0.25 + origin.x, origin.y + visible_size.height*0.7));
    this->addChild(music_text);
    
    // 创建一个滑动条
    auto music_slider = Slider::create();
    // 设置滑动条的纹理
    music_slider->loadBarTexture("sliderTrack.png");
    // 设置滑动条的滚轮纹理
    music_slider->loadSlidBallTextures("sliderThumb.png","sliderThumb.png","");
    // 设置处理滑动条的进度条纹理
    music_slider->loadProgressBarTexture("sliderProgress.png");
    // 获取之前设置的背景音乐音量
    float musicPercent = UserDefault::getInstance()->getFloatForKey("musicPercent");
    // 如果是第一次进入设置场景，设置音乐滑动条默认初始值为100
    if(musicPercent == 0.0f){
        musicPercent = 100.0f;
    }
    // 设置音乐滑动条的初始值
    music_slider->setPercent(musicPercent);
    music_slider->setPosition(Vec2(origin.x + visible_size.width*0.6, origin.y +  visible_size.height*0.7));
    // 添加事件监听器,调整音乐音量
    music_slider->addEventListener([=](Ref* pSender, Slider::EventType type){
        // 当滑块的百分比发生变化时
        if(type == Slider::EventType::ON_PERCENTAGE_CHANGED){
            // 获得滑动条百分比
            int percent = music_slider->getPercent();
             // 设置背景音乐值
            CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(float(percent)/100);
            // 存储设置的背景音乐值
            UserDefault::getInstance()->setFloatForKey("musicPercent", percent);
        }
    });
    this->addChild(music_slider);
    //---------------------------------------------音效
    auto sound_text = Text::create("音效音量", "Arial", 20);
    sound_text->setPosition(Vec2(origin.x + visible_size.width*0.25, origin.y + visible_size.height*0.5));
    this->addChild(sound_text);
    
    // 创建一个滑动条
    auto effects_slider = Slider::create();
    // 设置滑动条的纹理
    effects_slider->loadBarTexture("sliderTrack.png");
    // 设置滑动条的滚轮纹理
    effects_slider->loadSlidBallTextures("sliderThumb.png", "sliderThumb.png", "");
    // 设置处理滑动条的进度条纹理
    effects_slider->loadProgressBarTexture("sliderProgress.png");
    // 获取之前设置的音效音量
    float effectPercent = UserDefault::getInstance()->getFloatForKey("effectPercent");
    // 如果是第一次进入设置场景，设置音效滑动条默认初始值为100
    if (effectPercent == 0.0f) {
        effectPercent = 100.0f;
    }
    // 设置音效滑动条的初始值
    effects_slider->setPercent(effectPercent);
    effects_slider->setPosition(Vec2(origin.x + visible_size.width*0.6, origin.y + visible_size.height*0.5));
    // 添加事件监听器,调整音效音量
    effects_slider->addEventListener([=](Ref* pSender,Slider::EventType type){
        // 当滑块的百分比发生变化时
        if(type == Slider::EventType::ON_PERCENTAGE_CHANGED)
        {
            // 获得滑动条百分比
            int percent = effects_slider->getPercent();
            // 设置背景音效值
            CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(float(percent)/100);
            // 存储设置的背景音乐值
            UserDefault::getInstance()->setFloatForKey("effectPercent", percent);
        }
    });
    this->addChild(effects_slider);
    //-------------------------------------------返回按钮（具体功能可参照之前两例）
    auto return_button = Button::create("back_button.png");
    
    return_button->cocos2d::Node::setScale((visible_size.width * 0.1 / return_button->getContentSize().width));
    return_button->setPosition(Vec2(origin.x + visible_size.width - return_button->getContentSize().width/2, origin.y + return_button->getContentSize().height/2));
    
    //添加触摸事件监听器
    return_button->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type){
        if(type == Widget::TouchEventType::ENDED){
            //创建换回GameMenu的转换
            auto transition = TransitionSlideInL::create(0.5, GameMenu::createScene());
            //进行转换
            Director::getInstance()->replaceScene(transition);
        }
    });
    
    this->addChild(return_button);
    
    return true;
}

























