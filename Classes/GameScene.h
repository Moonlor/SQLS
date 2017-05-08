//
//  GameScene.h
//  MX1
//
//  Created by ���� on 2017/4/29.
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

	//�ĸ���ͬ����ͼ���ֱ�Ϊ�����ҡ��ϡ����ƶ�����ͼ�������Ӧ�����ƶ�ʱ���ô���ͼ�����滻
	CCTexture2D *_player_texture_left;
	CCTexture2D *_player_texture_right;
	CCTexture2D *_player_texture_up;
	CCTexture2D *_player_texture_down;
	//layer������ �����ж���ײ
	TMXLayer* _collidable;
	int _screenWidth, _screenHeight;

	//keys�����Լ����ϰ������м�¼  ����һ��map���� true��ʾ������һֱ���� false��ʾ�ɿ�
	//��������뿴����ļ��̳̽�
	std::map<cocos2d::EventKeyboard::KeyCode, bool> _keys;
	Vector<Sprite* > _popVector;

	int _count; //��Ϸ֡������
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	//��������ϵ����������������������������������ϵ��ת��
	//��һ������ʵ������������tile�����ת�� ��600��600 --->15,15
	Vec2 tileCoordForPosition(Vec2 position);

	//�ڶ�����������ָ���������������  �ص������ģ���������ʱ���õ����
	Vec2 positionForTileCoord(const Vec2& tileCoord);

	//ͨ������֡�����ִӶ���֡�����еõ�����������
	Animate* getAnimateByName(std::string animName, float delay, int animNum);

	//������Ҵ�startPos�ƶ���endPos�����ѵ�ʱ��
	float getPlayerMoveTime(Vec2 startPos, Vec2 endPos);

	//��ҽ��ƶ�position
	void playerMover(Vec2 position);

	//�жϼ����Ƿ�ס����ϸ�뿴������̳�
	bool isKeyPressed(EventKeyboard::KeyCode code);

	//�жϴ���һ����ַ���ж������ַ�Ƿ���Խ�����ײ
	bool collidable(Vec2 tileCoord);

	//�����ڰ�ѹ�ڼ�ᷢ��������  ��������кܶ������cpp�����ϸ����
	void keyPressedDuration(EventKeyboard::KeyCode code);

	//������º���
	void update(float delta);

	//����λ��
	void updatePosition(float delta);

	CREATE_FUNC(Game);
};



#endif /* GameScene_h */

