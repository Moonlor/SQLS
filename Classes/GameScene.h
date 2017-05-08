//
//  GameScene.h
//  MX1
//
//  Created by 李坤 on 2017/4/29.
//
//


#ifndef GameScene_h
#define GameScene_h

#include <iostream>
#include "cocos2d.h"


USING_NS_CC;

class Game : public cocos2d::Layer
{
private:
	TMXTiledMap* _tileMap;
	Sprite* _player;

	//四个不同的贴图，分别为向左、右、上、下移动的贴图，当向对应方向移动时，用此贴图进行替换
	CCTexture2D *_player_texture_left;
	CCTexture2D *_player_texture_right;
	CCTexture2D *_player_texture_up;
	CCTexture2D *_player_texture_down;
	//layer的种类 用来判断碰撞
	TMXLayer* _collidable;
	int _screenWidth, _screenHeight;

	//keys用来对键盘上按键进行记录  这是一个map类型 true表示键盘在一直按着 false表示松开
	//具体这个请看左左的键盘教程
	std::map<cocos2d::EventKeyboard::KeyCode, bool> _keys;
	Vector<Sprite* > _popVector;

	int _count; //游戏帧计数器
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	//两个坐标系，接下来的两个函数用来进行两个坐标系的转化
	//第一个用来实现像素坐标向tile坐标的转换 如600，600 --->15,15
	Vec2 tileCoordForPosition(Vec2 position);

	//第二个用来返回指定方格的中心坐标  重点是中心，放置泡泡时会用到这个
	Vec2 positionForTileCoord(const Vec2& tileCoord);

	//通过动画帧的名字从动画帧缓存中得到并创建动画
	Animate* getAnimateByName(std::string animName, float delay, int animNum);

	//计算玩家从startPos移动到endPos所花费的时间
	float getPlayerMoveTime(Vec2 startPos, Vec2 endPos);

	//玩家将移动position
	void playerMover(Vec2 position);

	//判断键盘是否按住，详细请看周左左教程
	bool isKeyPressed(EventKeyboard::KeyCode code);

	//判断传入一个地址，判断这个地址是否可以进行碰撞
	bool collidable(Vec2 tileCoord);

	//键盘在按压期间会发生的事情  这个函数有很多操作，cpp里会详细解释
	void keyPressedDuration(EventKeyboard::KeyCode code);

	//画面更新函数
	void update(float delta);

	//更新位置
	void updatePosition(float delta);

	CREATE_FUNC(Game);
};



#endif /* GameScene_h */

