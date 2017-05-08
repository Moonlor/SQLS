
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


void Game::keyPressedDuration(EventKeyboard::KeyCode code){
    //设置偏移的数量，就是每一帧向前走的像素点，修改它会修改速度
    //跟周左左教程里用的原理相同
    int offsetX = 0, offsetY = 0;
    //这四个值是用来做碰撞检测  补充只能检测菱形的不足
    float
    collidableAmendLeftX = 0,
    collidableAmendRightX = 0,
    collidableAmendUpY = 0,
    collidableAmendDownY = 0;
    
    //设置Texture的值
    if (_keys[EventKeyboard::KeyCode::KEY_LEFT_ARROW] == true) {
        _player->setTexture(_player_texture_left);
    }else if(_keys[EventKeyboard::KeyCode::KEY_RIGHT_ARROW] == true){
        _player->setTexture(_player_texture_right);
    }else if(_keys[EventKeyboard::KeyCode::KEY_UP_ARROW] == true){
        _player->setTexture(_player_texture_up);
    }else if(_keys[EventKeyboard::KeyCode::KEY_DOWN_ARROW] == true){
        _player->setTexture(_player_texture_down);
    }
    
    //检测行走方向，并根据不同的方向确定偏移值与检测碰撞的值
    //拿第一个case举例
    //比如向左走的时候 我只要确定一个左上跟左下两个点的值就可以了
    switch (code) {
            //case1 当按键为Left时  说明此时间人物向左走
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            offsetX = -2;
            collidableAmendLeftX  = - _player->getContentSize().width / 2 ;  //我能知道的只是player中心点的坐标  把三个值是加起来确定偏移的两个点
            collidableAmendUpY = _tileMap->getTileSize().height / 2 - 6;    // +6 +4 是不要用来判断正方形的端点
            collidableAmendDownY = - _tileMap->getTileSize().height / 2 + 4;
            break;
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            offsetX = 2;
            collidableAmendRightX = _player->getContentSize().width / 2;
            collidableAmendUpY = _tileMap->getTileSize().height / 2 - 6;
            collidableAmendDownY = - _tileMap->getTileSize().height / 2 + 4;
            break;
        case EventKeyboard::KeyCode::KEY_UP_ARROW:
            offsetY = 2;
            collidableAmendUpY = _tileMap->getTileSize().height / 2 ;
            collidableAmendLeftX = - _player->getContentSize().width / 2 + 6;
            collidableAmendRightX = _player->getContentSize().width / 2 - 6;
            break;
        case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            offsetY = -2;
            collidableAmendLeftX = - _player->getContentSize().width / 2 + 6;
            collidableAmendRightX = _player->getContentSize().width / 2 - 6;
            collidableAmendDownY = - _tileMap->getTileSize().height / 2 - 3;
            break;
        case EventKeyboard::KeyCode::KEY_SPACE:{
            //获取玩家的位置
            Vec2 _tilePlayer = tileCoordForPosition(_player->getPosition());
            Vec2 _popPosition = positionForTileCoord(_tilePlayer);
            
            //创建一个泡泡，放置在玩家所在格子的中心
            Sprite* pop = Sprite::create("pop.png");
            pop->setPosition(_popPosition);
            
            //泡泡的zorder要比玩家低
            _tileMap->addChild(pop, 1);
            
            //从泡泡被放入vector到泡泡爆炸的延时
            DelayTime * _delayDelete = DelayTime::create(2.5f);
            //从泡泡放置到泡泡被放入泡泡vector的延时
            DelayTime * _delayPush = DelayTime::create(0.5f);
            auto callFunc1 = CallFunc::create([=]{
                _popVector.pushBack(pop);
            });
            auto callFunc2 = CallFunc::create([=]{
                //泡泡爆炸时，获取地图上现存的第一个被放置的泡泡并删除它
                auto pop = _popVector.at(0);
                _tileMap->removeChild(pop);
                _popVector.eraseObject(pop);
            });
            //创建一个序列动作，放置泡泡->泡泡加入泡泡vector->泡泡爆炸
            auto action = Sequence::create(_delayPush,callFunc1,_delayDelete,callFunc2 ,NULL);
            pop->runAction(action);
            
            break;
        }
        default:
            offsetY = offsetX = 0;
            break;
    }
    
    
    
    //用来做碰撞检测的四个点, 分辨代表四个角 左上，左下，右上，右下  注意这个是个偏移量  还不是我们要确定的两个检测点
    Vec2 amendLeftUp = Vec2(offsetX + collidableAmendLeftX ,offsetY + collidableAmendUpY),
    amendLeftDown = Vec2(offsetX + collidableAmendLeftX ,offsetY + collidableAmendDownY),
    amendRightUp = Vec2(offsetX + collidableAmendRightX ,offsetY + collidableAmendUpY),
    amendRightDown = Vec2(offsetX + collidableAmendRightX,offsetY + collidableAmendDownY);
    
    //即将要做碰撞检测的两个点, 用刚才根据方向确定的碰撞值来初始化两个点   到现在终于找出来确定碰撞的两个点了  呜呜呜
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
    
    
    //目标位置 如果接下来的碰撞检测没问题，player就会move到这个位置
    Vec2 aimLocation = _player->getPosition() + Vec2(offsetX,offsetY);
    
    //玩家所在的方格位置
    Vec2 orginCoordLocation = tileCoordForPosition(_player->getPosition());
    
    //玩家所在方格的中心位置（像素值）
    Vec2 tileGlPosition = positionForTileCoord(orginCoordLocation);
    
    //在行走到拐角的时候我们很容易卡住，在一定范围内，我们可以帮忙自动调整他的位置，使转弯更顺滑
    //这四个变量用来判断这个转角的临界范围
    //这算是对移动流畅度的优化
    Vec2
    shiftLeftUp = _player->getPosition() + amendLeftUp,
    shiftLeftDown = _player->getPosition() + amendLeftDown,
    shiftRightUp = _player->getPosition() + amendRightUp,
    shiftRightDown = _player->getPosition() + amendRightDown;
    
    //这个变量用来我们来帮忙偏移的位置
    Vec2 shiftLocation;
    
    //用刚才得到的两个碰撞检测点 来进行collidable函数检测
    //如果碰撞了的话 我们来检测是否满足我们帮忙偏移的标准
    //如果不满足 就return  结束函数 这就让玩家不移动
    if(collidable(collidableTest1) || collidable(collidableTest2)){
        //比如向左移动时候卡住了
        //在一定范围内，我们帮忙move
        //让他通过拐角
        if (_keys[EventKeyboard::KeyCode::KEY_LEFT_ARROW] == true) {
            
            if(shiftLeftDown.y > tileGlPosition.y - 40
               && shiftLeftDown.y < tileGlPosition.y - 20)
            {
                shiftLocation =_player->getPosition() + Vec2(0,3);
                this->playerMover(shiftLocation);
            }
            
        }else if(_keys[EventKeyboard::KeyCode::KEY_RIGHT_ARROW] == true){
            
            if(shiftRightDown.y > tileGlPosition.y - 40
               && shiftLeftDown.y < tileGlPosition.y - 20)
            {
                shiftLocation =_player->getPosition() + Vec2(0,3);
                this->playerMover(shiftLocation);
            }
            
        }else if(_keys[EventKeyboard::KeyCode::KEY_UP_ARROW] == true){
            if(shiftLeftUp.x > tileGlPosition.x - 40
               && shiftLeftUp.x < tileGlPosition.x - 20)
            {
                
                shiftLocation =_player->getPosition() + Vec2(3,0);
                this->playerMover(shiftLocation);
            }
        }else if(_keys[EventKeyboard::KeyCode::KEY_DOWN_ARROW] == true){
            if(shiftLeftDown.x > tileGlPosition.x - 40
               && shiftLeftDown.x < tileGlPosition.x - 20)
            {
                shiftLocation =_player->getPosition() + Vec2(3,0);
                this->playerMover(shiftLocation);
            }
        }
        return;
    }
    //如果碰撞检测没有问题,那就运行进行move
    this->playerMover(aimLocation);
}

bool Game::collidable(Vec2 tileCoord){
    
    for(int i = 0; i < _popVector.size(); i++){
        //获取所有泡泡的位置，转换为TileCoord
        Vec2 popPosition = _popVector.at(i)->getPosition();
        Vec2 popPositionForTileMap = tileCoordForPosition(popPosition);
        
        //判断泡泡的位置是否与将要运动到的位置重合
        if(tileCoord == popPositionForTileMap){
            return true;
        }
        
    }
    
    int tileGid = _collidable->getTileGIDAt(tileCoord);
    if (tileGid) {
        // 使用GID来查找指定tile的属性，返回一个Value
        Value properties = _tileMap->getPropertiesForGID(tileGid);
        // 返回的Value实际是一个ValueMap
        ValueMap map = properties.asValueMap();
        // 查找ValueMap，判断是否有”可碰撞的“物体，如果有，直接返回
        std::string value = map.at("collidable").asString();
        if (value.compare("true") == 0)
            return true;
        else
            return false;
    }
}


