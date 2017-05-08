
#include <stdio.h>
#include <iostream>
#include "GameScene.h"
using namespace std;
Scene* Game::createScene()
{
	auto scene = Scene::create();
	auto layer = Game::create();

	scene->addChild(layer);

	return scene;
}

bool Game::init()
{
	if (!Layer::init())
	{
		return false;
	}

	//�����Ļ�����ڣ���С
	Size visibleSize = Director::getInstance()->getVisibleSize();
	//�������ԭ��
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//�����Ļ�����ڣ���Ⱥ͸߶�
	_screenWidth = visibleSize.width;
	_screenHeight = visibleSize.height;

	//ͨ����ͼ��������
	_tileMap = TMXTiledMap::create("map.tmx");
	//���õ�ͼ��ê��Ϊ���½ǣ����ͼLayer�ϵ���Ԫ�ص�����ԭ��Ϊ��0��0��
	_tileMap->setAnchorPoint(Vec2::ZERO);
	//���õ�ͼ��λ��Ϊ��Ļ��
	_tileMap->setPosition(Vec2::ZERO + Vec2(origin.x, origin.y));

	this->addChild(_tileMap);
	//��õ�ͼ�е�collidable��
	_collidable = _tileMap->getLayer("collidable");
	//��collidable���ò��ɼ�
	_collidable->setVisible(false);

	//�������
	_player = Sprite::create("player_down.png");
	//�������ê��
	_player->setAnchorPoint(Vec2(_player->getContentSize().width / 2 / _player->getContentSize().width, _tileMap->getTileSize().height / 2 / _player->getContentSize().height));
	//����ҷ����ڵ�ͼ���½ǣ���TileCoordΪ��1��1���ĸ��ӵ����ģ�
	_player->setPosition(origin.x + 60, 60 + origin.y);

	//��ҵ�zOrderΪ2
	_tileMap->addChild(_player, 2);
	float x = _player->getPosition().x;
	float y = _player->getPosition().y;

	//���ĸ�������ƶ���ͼ���뻺��
	_player_texture_left = CCTextureCache::sharedTextureCache()->addImage("player_left.png");
	_player_texture_right = CCTextureCache::sharedTextureCache()->addImage("player_right.png");
	_player_texture_up = CCTextureCache::sharedTextureCache()->addImage("player_up.png");
	_player_texture_down = CCTextureCache::sharedTextureCache()->addImage("player_down.png");

	// ���������¼�������
	auto keyBoardListener = EventListenerKeyboard::create();

	//��ĳ����������ʱ��map�ж�Ӧ�������ֵ����Ϊtrue��Ϊʲô�������ɲο����������
	keyBoardListener->onKeyPressed = [=](EventKeyboard::KeyCode code, Event* event) {
		_keys[code] = true;
	};
	//��ĳ�������ɿ�ʱ��map�ж�Ӧ�������ֵ����Ϊfalse
	keyBoardListener->onKeyReleased = [=](EventKeyboard::KeyCode code, Event* event) {
		_keys[code] = false;
	};

	// ��Ӧ�����¼�����
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyBoardListener, this);

	this->scheduleUpdate();

	return true;
}


Animate* Game::getAnimateByName(std::string animName, float delay, int animNum)
{
	//����һ������
	Animation* animation = Animation::create();

	//�Ѷ���֡���붯��
	for (unsigned int i = 1; i <= animNum; i++) {
		std::string frameName = animName;
		//�ڶ���֡���ƺ�������
		frameName.append(StringUtils::format("%d", i)).append(".png");
		animation->addSpriteFrameWithFile(frameName.c_str());
	}

	//���ö�����ʱ
	animation->setDelayPerUnit(delay);
	//�ڲ����궯��ʱ�ָ�����ʼ֡
	animation->setRestoreOriginalFrame(true);
	Animate* animate = Animate::create(animation);

	return animate;

}


Vec2 Game::tileCoordForPosition(Point position)
{

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// CC_CONTENT_SCALE_FACTOR Retina����2�����򷵻�1
	// ���λ�õ�x���Ե�ͼ�Ŀ��õ����ǵ�ͼ����ĵڼ������ӣ�tile��
	// ��ͼ����㣺15[����] * 40[ͼ��Ŀ�] = 600[��ͼ��]
	// ���羫���ڵ�x������60���������ڵ�ͼ�ĸ��Ӽ��㣺60[����λ��] / 40[ͼ��Ŀ�] = 1 [����]
	int x = (int)((position.x - origin.x) / (_tileMap->getTileSize().width / CC_CONTENT_SCALE_FACTOR()));
	float pointHeight = _tileMap->getTileSize().height / CC_CONTENT_SCALE_FACTOR();
	int y = (int)((_tileMap->getMapSize().height * pointHeight - position.y) / pointHeight);

	return Vec2(x, y);
}

Vec2 Game::positionForTileCoord(const cocos2d::Vec2 &tileCoord) {
	//��TileCoordת��ΪtileMap�ķ�����������꣨��(1,1)תΪ(60,60)��
	Size mapSize = _tileMap->getMapSize();
	Size tileSize = _tileMap->getTileSize();
	int x = tileCoord.x * tileSize.width + tileSize.width / 2;
	int y = (mapSize.height - tileCoord.y)*tileSize.height - tileSize.height / 2;
	return Vec2(x, y);
}

// ��������ƶ���������ƶ�ʱ��ķ���
float Game::getPlayerMoveTime(Vec2 startPos, Vec2 endPos) {
	// �������յ������ת��ΪTileMap����
	Vec2 tileStart = tileCoordForPosition(startPos);
	Vec2 tileEnd = tileCoordForPosition(endPos);
	// �ƶ�һ�������ʱ��
	float duration = 0.4f;
	// �����ƶ���������ƶ�ʱ�䣬���������ĺͼ���ƽ����
	duration = duration * sqrtf((tileStart.x - tileEnd.x) * (tileStart.x - tileEnd.x)
		+ (tileStart.y - tileEnd.y) * (tileStart.y - tileEnd.y));
	return duration;
}

//������ҽ����ƶ�
void Game::playerMover(Vec2 position) {
	//�����ҵ��ƶ�ʱ��
	float duration = getPlayerMoveTime(_player->getPosition(), position);
	//���ƶ�Ŀ�������ת��ΪtileCoord�����µ����꣨��ʱû���õ���������
	Vec2 coord = tileCoordForPosition(position);
	Vec2 mid = Vec2(coord.x * _tileMap->getTileSize().width, 600 - coord.y * _tileMap->getTileSize().height);

	auto moveTo = MoveTo::create(duration, position);

	_player->runAction(moveTo);
}

bool Game::isKeyPressed(EventKeyboard::KeyCode code) {
	return _keys[code];
}

//ÿ֡����������������������λ�ã����ж��Ƿ����ˮ��
void Game::updatePosition(float delta) {
	//�洢�ĸ�������Ϳո���Ķ�Ӧ�ı�ʶ
	auto
		leftArrow = EventKeyboard::KeyCode::KEY_LEFT_ARROW,
		rightArrow = EventKeyboard::KeyCode::KEY_RIGHT_ARROW,
		upArrow = EventKeyboard::KeyCode::KEY_UP_ARROW,
		downArrow = EventKeyboard::KeyCode::KEY_DOWN_ARROW,
		space = EventKeyboard::KeyCode::KEY_SPACE;
	//ֻҪ���κ�һ��������������ˣ�any_arrow����Ϊfalse
	int any_arrow = isKeyPressed(leftArrow) + isKeyPressed(rightArrow) + isKeyPressed(upArrow) + isKeyPressed(downArrow);

	//���ͬʱ���·�����Ϳո���������һ��ˮ��
	if ((isKeyPressed(space) && any_arrow)) {
		keyPressedDuration(space);
		_keys[space] = false;
	}
	//������·����������ҽ����ƶ���������ƶ��жϣ����ֻ���¿ո��������ҷ���һ��ˮ��
	if (isKeyPressed(leftArrow)) {
		keyPressedDuration(leftArrow);
	}
	else if (isKeyPressed(rightArrow)) {
		keyPressedDuration(rightArrow);
	}
	else if (isKeyPressed(upArrow)) {
		keyPressedDuration(upArrow);
	}
	else if (isKeyPressed(downArrow)) {
		keyPressedDuration(downArrow);
	}
	else if (isKeyPressed(space)) {
		keyPressedDuration(space);
	}
}

void Game::update(float delta) {
	Node::update(delta);
	//�������λ�ã����ж��Ƿ����ˮ��
	updatePosition(delta);

}


void Game::keyPressedDuration(EventKeyboard::KeyCode code) {
	//����ƫ�Ƶ�����������ÿһ֡��ǰ�ߵ����ص㣬�޸������޸��ٶ�
	//��������̳����õ�ԭ����ͬ
	int offsetX = 0, offsetY = 0;
	//���ĸ�ֵ����������ײ���  ����ֻ�ܼ�����εĲ���
	float
		collidableAmendLeftX = 0,
		collidableAmendRightX = 0,
		collidableAmendUpY = 0,
		collidableAmendDownY = 0;

	//����Texture��ֵ
	if (_keys[EventKeyboard::KeyCode::KEY_LEFT_ARROW] == true) {
		_player->setTexture(_player_texture_left);
	}
	else if (_keys[EventKeyboard::KeyCode::KEY_RIGHT_ARROW] == true) {
		_player->setTexture(_player_texture_right);
	}
	else if (_keys[EventKeyboard::KeyCode::KEY_UP_ARROW] == true) {
		_player->setTexture(_player_texture_up);
	}
	else if (_keys[EventKeyboard::KeyCode::KEY_DOWN_ARROW] == true) {
		_player->setTexture(_player_texture_down);
	}

	//������߷��򣬲����ݲ�ͬ�ķ���ȷ��ƫ��ֵ������ײ��ֵ
	//�õ�һ��case����
	//���������ߵ�ʱ�� ��ֻҪȷ��һ�����ϸ������������ֵ�Ϳ�����
	switch (code) {
		//case1 ������ΪLeftʱ  ˵����ʱ������������
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		offsetX = -2;
		collidableAmendLeftX = -_player->getContentSize().width / 2;  //����֪����ֻ��player���ĵ������  ������ֵ�Ǽ�����ȷ��ƫ�Ƶ�������
		collidableAmendUpY = _tileMap->getTileSize().height / 2 - 6;    // +6 +4 �ǲ�Ҫ�����ж������εĶ˵�
		collidableAmendDownY = -_tileMap->getTileSize().height / 2 + 4;
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		offsetX = 2;
		collidableAmendRightX = _player->getContentSize().width / 2;
		collidableAmendUpY = _tileMap->getTileSize().height / 2 - 6;
		collidableAmendDownY = -_tileMap->getTileSize().height / 2 + 4;
		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		offsetY = 2;
		collidableAmendUpY = _tileMap->getTileSize().height / 2;
		collidableAmendLeftX = -_player->getContentSize().width / 2 + 6;
		collidableAmendRightX = _player->getContentSize().width / 2 - 6;
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		offsetY = -2;
		collidableAmendLeftX = -_player->getContentSize().width / 2 + 6;
		collidableAmendRightX = _player->getContentSize().width / 2 - 6;
		collidableAmendDownY = -_tileMap->getTileSize().height / 2 - 3;
		break;
	case EventKeyboard::KeyCode::KEY_SPACE: {
		//��ȡ��ҵ�λ��
		Vec2 _tilePlayer = tileCoordForPosition(_player->getPosition());
		Vec2 _popPosition = positionForTileCoord(_tilePlayer);

		//����һ�����ݣ�������������ڸ��ӵ�����
		Sprite* pop = Sprite::create("pop.png");
		pop->setPosition(_popPosition);

		//���ݵ�zorderҪ����ҵ�
		_tileMap->addChild(pop, 1);

		//�����ݱ�����vector�����ݱ�ը����ʱ
		DelayTime * _delayDelete = DelayTime::create(2.5f);
		//�����ݷ��õ����ݱ���������vector����ʱ
		DelayTime * _delayPush = DelayTime::create(0.5f);
		auto callFunc1 = CallFunc::create([=] {
			_popVector.pushBack(pop);
		});
		auto callFunc2 = CallFunc::create([=] {
			//���ݱ�ըʱ����ȡ��ͼ���ִ�ĵ�һ�������õ����ݲ�ɾ����
			auto pop = _popVector.at(0);
			_tileMap->removeChild(pop);
			_popVector.eraseObject(pop);
		});
		//����һ�����ж�������������->���ݼ�������vector->���ݱ�ը
		auto action = Sequence::create(_delayPush, callFunc1, _delayDelete, callFunc2, NULL);
		pop->runAction(action);

		break;
	}
	default:
		offsetY = offsetX = 0;
		break;
	}



	//��������ײ�����ĸ���, �ֱ�����ĸ��� ���ϣ����£����ϣ�����  ע������Ǹ�ƫ����  ����������Ҫȷ������������
	Vec2 amendLeftUp = Vec2(offsetX + collidableAmendLeftX, offsetY + collidableAmendUpY),
		amendLeftDown = Vec2(offsetX + collidableAmendLeftX, offsetY + collidableAmendDownY),
		amendRightUp = Vec2(offsetX + collidableAmendRightX, offsetY + collidableAmendUpY),
		amendRightDown = Vec2(offsetX + collidableAmendRightX, offsetY + collidableAmendDownY);

	//����Ҫ����ײ����������, �øղŸ��ݷ���ȷ������ײֵ����ʼ��������   �����������ҳ���ȷ����ײ����������  ������
	Vec2 collidableTest1, collidableTest2;

	switch (code) {
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		collidableTest1 = tileCoordForPosition(_player->getPosition() + amendLeftUp);

		collidableTest2 = tileCoordForPosition(_player->getPosition() + amendLeftDown);
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		collidableTest1 = tileCoordForPosition(_player->getPosition() + amendRightUp);
		collidableTest2 = tileCoordForPosition(_player->getPosition() + amendRightDown);

		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:

		collidableTest1 = tileCoordForPosition(_player->getPosition() + amendLeftUp);
		collidableTest2 = tileCoordForPosition(_player->getPosition() + amendRightUp);
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:

		collidableTest1 = tileCoordForPosition(_player->getPosition() + amendLeftDown);
		collidableTest2 = tileCoordForPosition(_player->getPosition() + amendRightDown);
		break;

	default:
		offsetY = offsetX = 0;
		break;
	}


	//Ŀ��λ�� �������������ײ���û���⣬player�ͻ�move�����λ��
	Vec2 aimLocation = _player->getPosition() + Vec2(offsetX, offsetY);

	//������ڵķ���λ��
	Vec2 orginCoordLocation = tileCoordForPosition(_player->getPosition());

	//������ڷ��������λ�ã�����ֵ��
	Vec2 tileGlPosition = positionForTileCoord(orginCoordLocation);

	//�����ߵ��սǵ�ʱ�����Ǻ����׿�ס����һ����Χ�ڣ����ǿ��԰�æ�Զ���������λ�ã�ʹת���˳��
	//���ĸ����������ж����ת�ǵ��ٽ緶Χ
	//�����Ƕ��ƶ������ȵ��Ż�
	Vec2
		shiftLeftUp = _player->getPosition() + amendLeftUp,
		shiftLeftDown = _player->getPosition() + amendLeftDown,
		shiftRightUp = _player->getPosition() + amendRightUp,
		shiftRightDown = _player->getPosition() + amendRightDown;

	//�������������������æƫ�Ƶ�λ��
	Vec2 shiftLocation;

	//�øղŵõ���������ײ���� ������collidable�������
	//�����ײ�˵Ļ� ����������Ƿ��������ǰ�æƫ�Ƶı�׼
	//��������� ��return  �������� �������Ҳ��ƶ�
	if (collidable(collidableTest1) || collidable(collidableTest2)) {
		//���������ƶ�ʱ��ס��
		//��һ����Χ�ڣ����ǰ�æmove
		//����ͨ���ս�
		if (_keys[EventKeyboard::KeyCode::KEY_LEFT_ARROW] == true) {

			if (shiftLeftDown.y > tileGlPosition.y - 40
				&& shiftLeftDown.y < tileGlPosition.y - 20)
			{
				shiftLocation = _player->getPosition() + Vec2(0, 3);
				this->playerMover(shiftLocation);
			}

		}
		else if (_keys[EventKeyboard::KeyCode::KEY_RIGHT_ARROW] == true) {

			if (shiftRightDown.y > tileGlPosition.y - 40
				&& shiftLeftDown.y < tileGlPosition.y - 20)
			{
				shiftLocation = _player->getPosition() + Vec2(0, 3);
				this->playerMover(shiftLocation);
			}

		}
		else if (_keys[EventKeyboard::KeyCode::KEY_UP_ARROW] == true) {
			if (shiftLeftUp.x > tileGlPosition.x - 40
				&& shiftLeftUp.x < tileGlPosition.x - 20)
			{

				shiftLocation = _player->getPosition() + Vec2(3, 0);
				this->playerMover(shiftLocation);
			}
		}
		else if (_keys[EventKeyboard::KeyCode::KEY_DOWN_ARROW] == true) {
			if (shiftLeftDown.x > tileGlPosition.x - 40
				&& shiftLeftDown.x < tileGlPosition.x - 20)
			{
				shiftLocation = _player->getPosition() + Vec2(3, 0);
				this->playerMover(shiftLocation);
			}
		}
		return;
	}
	//�����ײ���û������,�Ǿ����н���move
	this->playerMover(aimLocation);
}

bool Game::collidable(Vec2 tileCoord) {

	for (int i = 0; i < _popVector.size(); i++) {
		//��ȡ�������ݵ�λ�ã�ת��ΪTileCoord
		Vec2 popPosition = _popVector.at(i)->getPosition();
		Vec2 popPositionForTileMap = tileCoordForPosition(popPosition);

		//�ж����ݵ�λ���Ƿ��뽫Ҫ�˶�����λ���غ�
		if (tileCoord == popPositionForTileMap) {
			return true;
		}

	}

	int tileGid = _collidable->getTileGIDAt(tileCoord);
	if (tileGid) {
		// ʹ��GID������ָ��tile�����ԣ�����һ��Value
		Value properties = _tileMap->getPropertiesForGID(tileGid);
		// ���ص�Valueʵ����һ��ValueMap
		ValueMap map = properties.asValueMap();
		// ����ValueMap���ж��Ƿ��С�����ײ�ġ����壬����У�ֱ�ӷ���
		std::string value = map.at("collidable").asString();
		if (value.compare("true") == 0)
			return true;
		else
			return false;
	}
}


