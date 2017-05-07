
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
    if(!Layer::init())
    {
        return false;
    }
    
    //获得屏幕（窗口）大小
    Size visibleSize = Director::getInstance()->getVisibleSize();
    //获得坐标原点
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    //获得屏幕（窗口）宽度和高度
    _screenWidth = visibleSize.width;
    _screenHeight = visibleSize.height;
    
    //通过地图创建场景
    _tileMap = TMXTiledMap::create("map.tmx");
    //设置地图的锚点为左下角，则地图Layer上的子元素的坐标原点为（0，0）
    _tileMap->setAnchorPoint(Vec2::ZERO);
    //设置地图的位置为屏幕的
    _tileMap->setPosition(Vec2::ZERO + Vec2(origin.x, origin.y));
    
    this->addChild(_tileMap);
    //获得地图中的collidable层
    _collidable = _tileMap->getLayer("collidable");
    //让collidable层变得不可见
    _collidable->setVisible(false);
    
    //创建玩家
    _player = Sprite::create("player_down.png");
    //设置玩家锚点
    _player->setAnchorPoint(Vec2(_player->getContentSize().width/2/_player->getContentSize().width,_tileMap->getTileSize().height/2 / _player->getContentSize().height));
    //把玩家放置在地图左下角，即TileCoord为（1，1）的格子的中心；
    _player->setPosition(origin.x + 60, 60  + origin.y);
    
    //玩家的zOrder为2
    _tileMap->addChild(_player, 2);
    float x = _player->getPosition().x;
    float y = _player->getPosition().y;
    
    //将四个方向的移动贴图加入缓存
    _player_texture_left = CCTextureCache::sharedTextureCache()->addImage("player_left.png");
    _player_texture_right = CCTextureCache::sharedTextureCache()->addImage("player_right.png");
    _player_texture_up = CCTextureCache::sharedTextureCache()->addImage("player_up.png");
    _player_texture_down = CCTextureCache::sharedTextureCache()->addImage("player_down.png");
    
    // 创建键盘事件监听器
    auto keyBoardListener = EventListenerKeyboard::create();
    
    //当某个键被按下时，map中对应这个键的值被设为true（为什么这样做可参考左左的文章
    keyBoardListener->onKeyPressed = [=](EventKeyboard::KeyCode code,Event* event){
        _keys[code] = true;
    };
    //当某个键被松开时，map中对应这个键的值被设为false
    keyBoardListener->onKeyReleased = [=](EventKeyboard::KeyCode code,Event* event){
        _keys[code] = false;
    };
    
    // 响应触摸事件函数
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyBoardListener, this);
    
    this->scheduleUpdate();
    
    return true;
}


Animate* Game::getAnimateByName(std::string animName, float delay, int animNum)
{
    //创建一个动画
    Animation* animation = Animation::create();
    
    //把动画帧加入动画
    for(unsigned int i = 1; i <= animNum; i++){
        std::string frameName = animName;
        //在动画帧名称后加上序号
        frameName.append(StringUtils::format("%d",i)).append(".png");
        animation->addSpriteFrameWithFile(frameName.c_str());
    }
    
    //设置动画延时
    animation->setDelayPerUnit(delay);
    //在播放完动画时恢复到初始帧
    animation->setRestoreOriginalFrame(true);
    Animate* animate = Animate::create(animation);
    
    return animate;
    
}


Vec2 Game::tileCoordForPosition(Point position)
{
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // CC_CONTENT_SCALE_FACTOR Retina返回2，否则返回1
    // 玩家位置的x除以地图的宽，得到的是地图横向的第几个格子（tile）
    // 地图宽计算：15[格子] * 40[图块的宽] = 600[地图宽]
    // 假如精灵在的x坐标是60，则精灵所在地图的格子计算：60[精灵位置] / 40[图块的宽] = 1 [格子]
    int x = (int)((position.x - origin.x )/ (_tileMap->getTileSize().width / CC_CONTENT_SCALE_FACTOR()));
    float pointHeight = _tileMap->getTileSize().height / CC_CONTENT_SCALE_FACTOR();
    int y = (int)((_tileMap->getMapSize().height * pointHeight - position.y) / pointHeight);
    
    return Vec2(x,y);
}

Vec2 Game::positionForTileCoord(const cocos2d::Vec2 &tileCoord){
    //将TileCoord转换为tileMap的方格的中心坐标（如(1,1)转为(60,60)）
    Size mapSize = _tileMap->getMapSize();
    Size tileSize = _tileMap->getTileSize();
    int x = tileCoord.x * tileSize.width + tileSize.width/2;
    int y = (mapSize.height-tileCoord.y)*tileSize.height - tileSize.height/2;
    return Vec2(x, y);
}

// 根据玩家移动距离计算移动时间的方法
float Game::getPlayerMoveTime(Vec2 startPos, Vec2 endPos){
    // 将起点和终点的坐标转换为TileMap坐标
    Vec2 tileStart = tileCoordForPosition(startPos);
    Vec2 tileEnd = tileCoordForPosition(endPos);
    // 移动一个网格的时间
    float duration = 0.4f;
    // 根据移动网格计算移动时间，横纵坐标差的和计算平方根
    duration = duration * sqrtf((tileStart.x - tileEnd.x) * (tileStart.x - tileEnd.x)
                                + (tileStart.y - tileEnd.y) * (tileStart.y - tileEnd.y));
    return duration;
}

//操纵玩家进行移动
void Game::playerMover(Vec2 position){
    //获得玩家的移动时间
    float duration = getPlayerMoveTime(_player->getPosition(), position);
    //将移动目标的坐标转换为tileCoord坐标下的坐标（暂时没有用到这个结果）
    Vec2 coord = tileCoordForPosition(position);
    Vec2 mid = Vec2(coord.x * _tileMap->getTileSize().width , 600 - coord.y * _tileMap->getTileSize().height);

    auto moveTo = MoveTo::create(duration, position);
    
    _player->runAction(moveTo);
}

bool Game::isKeyPressed(EventKeyboard::KeyCode code){
    return _keys[code];
}

//每帧都调用这个函数，更新玩家位置，并判断是否放下水泡
void Game::updatePosition(float delta){
    //存储四个方向键和空格键的对应的标识
    auto
    leftArrow = EventKeyboard::KeyCode::KEY_LEFT_ARROW,
    rightArrow = EventKeyboard::KeyCode::KEY_RIGHT_ARROW,
    upArrow = EventKeyboard::KeyCode::KEY_UP_ARROW,
    downArrow = EventKeyboard::KeyCode::KEY_DOWN_ARROW,
    space = EventKeyboard::KeyCode::KEY_SPACE;
    //只要有任何一个方向键被按下了，any_arrow都不为false
    int any_arrow = isKeyPressed(leftArrow) + isKeyPressed(rightArrow) + isKeyPressed(upArrow) + isKeyPressed(downArrow);
    
    //如果同时按下方向键和空格键，则放下一个水泡
    if((isKeyPressed(space) && any_arrow)){
        keyPressedDuration(space);
        _keys[space] = false;
    }
    //如果按下方向键，则玩家进行移动，则进行移动判断，如果只按下空格键，则玩家放下一个水泡
    if(isKeyPressed(leftArrow)) {
        keyPressedDuration(leftArrow);
    } else if(isKeyPressed(rightArrow)) {
        keyPressedDuration(rightArrow);
    } else if(isKeyPressed(upArrow)) {
        keyPressedDuration(upArrow);
    } else if(isKeyPressed(downArrow)) {
        keyPressedDuration(downArrow);
    } else if(isKeyPressed(space)){
        keyPressedDuration(space);
    }
}

void Game::update(float delta){
    Node::update(delta);
    //更新玩家位置，并判断是否放下水泡
    updatePosition(delta);

}

