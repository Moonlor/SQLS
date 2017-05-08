

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

	//----------------------------------------------��������

	auto music_text = Text::create("��������", "Arial", 20);
	music_text->setPosition(Vec2(visible_size.width*0.25 + origin.x, origin.y + visible_size.height*0.7));
	this->addChild(music_text);

	// ����һ��������
	auto music_slider = Slider::create();
	// ���û�����������
	music_slider->loadBarTexture("sliderTrack.png");
	// ���û������Ĺ�������
	music_slider->loadSlidBallTextures("sliderThumb.png", "sliderThumb.png", "");
	// ���ô��������Ľ���������
	music_slider->loadProgressBarTexture("sliderProgress.png");
	// ��ȡ֮ǰ���õı�����������
	float musicPercent = UserDefault::getInstance()->getFloatForKey("musicPercent");
	// ����ǵ�һ�ν������ó������������ֻ�����Ĭ�ϳ�ʼֵΪ100
	if (musicPercent == 0.0f) {
		musicPercent = 100.0f;
	}
	// �������ֻ������ĳ�ʼֵ
	music_slider->setPercent(musicPercent);
	music_slider->setPosition(Vec2(origin.x + visible_size.width*0.6, origin.y + visible_size.height*0.7));
	// ����¼�������,������������
	music_slider->addEventListener([=](Ref* pSender, Slider::EventType type) {
		// ������İٷֱȷ����仯ʱ
		if (type == Slider::EventType::ON_PERCENTAGE_CHANGED) {
			// ��û������ٷֱ�
			int percent = music_slider->getPercent();
			// ���ñ�������ֵ
			CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(float(percent) / 100);
			// �洢���õı�������ֵ
			UserDefault::getInstance()->setFloatForKey("musicPercent", percent);
		}
	});
	this->addChild(music_slider);
	//---------------------------------------------��Ч
	auto sound_text = Text::create("��Ч����", "Arial", 20);
	sound_text->setPosition(Vec2(origin.x + visible_size.width*0.25, origin.y + visible_size.height*0.5));
	this->addChild(sound_text);

	// ����һ��������
	auto effects_slider = Slider::create();
	// ���û�����������
	effects_slider->loadBarTexture("sliderTrack.png");
	// ���û������Ĺ�������
	effects_slider->loadSlidBallTextures("sliderThumb.png", "sliderThumb.png", "");
	// ���ô��������Ľ���������
	effects_slider->loadProgressBarTexture("sliderProgress.png");
	// ��ȡ֮ǰ���õ���Ч����
	float effectPercent = UserDefault::getInstance()->getFloatForKey("effectPercent");
	// ����ǵ�һ�ν������ó�����������Ч������Ĭ�ϳ�ʼֵΪ100
	if (effectPercent == 0.0f) {
		effectPercent = 100.0f;
	}
	// ������Ч�������ĳ�ʼֵ
	effects_slider->setPercent(effectPercent);
	effects_slider->setPosition(Vec2(origin.x + visible_size.width*0.6, origin.y + visible_size.height*0.5));
	// ����¼�������,������Ч����
	effects_slider->addEventListener([=](Ref* pSender, Slider::EventType type) {
		// ������İٷֱȷ����仯ʱ
		if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
		{
			// ��û������ٷֱ�
			int percent = effects_slider->getPercent();
			// ���ñ�����Чֵ
			CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(float(percent) / 100);
			// �洢���õı�������ֵ
			UserDefault::getInstance()->setFloatForKey("effectPercent", percent);
		}
	});
	this->addChild(effects_slider);
	//-------------------------------------------���ذ�ť�����幦�ܿɲ���֮ǰ������
	auto return_button = Button::create("back_button.png");

	return_button->cocos2d::Node::setScale((visible_size.width * 0.1 / return_button->getContentSize().width));
	return_button->setPosition(Vec2(origin.x + visible_size.width - return_button->getContentSize().width / 2, origin.y + return_button->getContentSize().height / 2));

	//��Ӵ����¼�������
	return_button->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED) {
			//��������GameMenu��ת��
			auto transition = TransitionSlideInL::create(0.5, GameMenu::createScene());
			//����ת��
			Director::getInstance()->replaceScene(transition);
		}
	});

	this->addChild(return_button);

	return true;
}

























