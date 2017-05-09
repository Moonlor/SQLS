#ifndef __MAINUI_SCENE_CHOOSE_LAYER__
#define __MAINUI_SCENE_CHOOSE_LAYER__
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace CocosDenshion;
/**
@brief
*/
class MainUISceneChooseLayer : public CCLayer
{
	public:
	/**
	@brief 		// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone

	*/
		virtual bool init();
		// a selector callback
		//
		/**
		@brief implement the "static node()" method manually
		*/
		CREATE_FUNC (MainUISceneChooseLayer);
};
// onserverField "init" you need to initialize your instance
bool MainUISceneChooseLayer::init()
{
	if (!CCLayer::init() )
		{
			return false;
		}

	//this->setTouchEnabled(true);
	return true;
}
#endif
