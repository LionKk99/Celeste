#pragma once
#include "cocos2d.h"
using namespace cocos2d;
#include "Level3Scene.h"
#include "Player/Player.h"
#include "Trap/Spikeweed.h"
USING_NS_CC;

Scene* Level3Scene::createScene() {
    auto scene = Scene::createWithPhysics();  // 创建一个带有物理世界的场景
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    scene->getPhysicsWorld()->setGravity(Vec2(0, -1200));//重力设置
    scene->getPhysicsWorld()->setSubsteps(60);  // 增加迭代次数

    auto layer = Level3Scene::create();
    scene->addChild(layer);

    return scene;
}
//
void Level3Scene::startGame() {};
void Level3Scene::endGame() {};
void Level3Scene::pauseGame() {};
//
bool Level3Scene::init() {
    if (!LevelBase::init()) {
        return false;
    }
    CCLOG("Starting Level3Scene::init");  // 添加这一行
    if (!Layer::init()) {
        return false;
    }

    // 获取场景的大小
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    //初始化检查点和重生点
    Vec2 checkpoint = Vec2(0, 0);//结束点所在位置
    Vec2 respawnPoint = Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y); // 设置重生位置到屏幕中心

    //创建左右边界
   // 创建左侧边界
    Vec2 leftEdgeStart = Vec2(origin.x, origin.y); // 左下角
    Vec2 leftEdgeEnd = Vec2(origin.x, origin.y + visibleSize.height); // 左上角
    auto leftEdgeBody = PhysicsBody::createEdgeSegment(leftEdgeStart, leftEdgeEnd, PHYSICSBODY_MATERIAL_DEFAULT, 1);
    leftEdgeBody->setDynamic(false);
    leftEdgeBody->setName("edge");

    auto leftEdgeNode = Node::create();
    leftEdgeNode->setPosition(Vec2::ZERO); // 使用原点位置
    leftEdgeNode->setPhysicsBody(leftEdgeBody);
    this->addChild(leftEdgeNode);

    // 创建右侧边界
    Vec2 rightEdgeStart = Vec2(origin.x + visibleSize.width, origin.y); // 右下角 简单乘2
    Vec2 rightEdgeEnd = Vec2(origin.x + visibleSize.width, origin.y + visibleSize.height); // 右上角
    auto rightEdgeBody = PhysicsBody::createEdgeSegment(rightEdgeStart, rightEdgeEnd, PHYSICSBODY_MATERIAL_DEFAULT, 1);
    rightEdgeBody->setDynamic(false);
    rightEdgeBody->setName("edge");

    auto rightEdgeNode = Node::create();
    rightEdgeNode->setPosition(Vec2::ZERO); // 使用原点位置
    rightEdgeNode->setPhysicsBody(rightEdgeBody);
    this->addChild(rightEdgeNode);


    
    // 创建一个物理边界包围盒（边界）
    auto edgeBody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 1, Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    edgeBody->setDynamic(false); // 确保边界不会移动
    edgeBody->setName("edge");
    // 创建一个节点并为其设置物理体
    auto edgeNode = Node::create();
    edgeNode->setPosition(origin);
    edgeNode->setPhysicsBody(edgeBody);

    // 添加节点到场景
    this->addChild(edgeNode);


    // 创建Player对象
    auto player = Player::create("movement/idle/Idle_00/Idle_00-0.png"); 
    player->setPosition(respawnPoint); // 设置位置到屏幕中心
    this->addChild(player); // 将玩家添加到场景中
    player->getPhysicsBody()->getFirstShape()->setFriction(0.5f);
    m_player = player;

    //创建spikeweed陷阱
    //auto spikeweed = Spikeweed::create(Vec2(100, 300), Size(100, 10));
    //this->addChild(spikeweed);
  
    // 加载这一关的特定内容，例如背景、障碍物等
    loadLevel();
    CCLOG("Finished Level3Scene::init");  // 添加这一行
    return true;
}

void Level3Scene::loadLevel() {
    // Level 1 specific content loading
    // For example: setting up the player, platforms, enemies, background, etc.
    // This should include setting up the physics bodies, sprites, and any game logic.


    CCLOG("Starting Level3Scene::loadLevel");  // 添加这一行
    // 添加背景
    
    auto background1 = Sprite::create("level/level/L1/level1_B.png");
    background1->setAnchorPoint(Vec2(0.5, 0.5));
    background1->setScale(0.8, 0.8);
    background1->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2));
    this->addChild(background1, -1);
    

    // 添加平台
    auto platform = Sprite::create(); // 这里创建的是一个透明的平台，没有纹理
    platform->setName("ground");
    platform->setAnchorPoint(Vec2(0.5, 0.5));
    platform->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, platform->getContentSize().height / 2));


    // 创建物理体
    auto physicsBody = PhysicsBody::create();

    // 第一个矩形（下方）    
    Vec2 rectangle1[] = {
        Vec2(-520 * 1.25, -100 * 1.25),
        Vec2(-105 * 1.25, -100 * 1.25),
        Vec2(-105 * 1.25, 60 * 1.25),
        Vec2(-520 * 1.25, 60 * 1.25)
    };
    auto shape1 = PhysicsShapePolygon::create(rectangle1, 4);
    shape1->setRestitution(0.0f);  // 设置反弹系数为0
    physicsBody->addShape(shape1);
    // 第二个矩形（突起）
    Vec2 rectangle2[] = {
        Vec2(-155 * 1.25, -100 * 1.25),
        Vec2(-105 * 1.25, -100 * 1.25),
        Vec2(-105 * 1.25, 120 * 1.25),
        Vec2(-155 * 1.25, 120 * 1.25)
    };
    auto shape2 = PhysicsShapePolygon::create(rectangle2, 4);
    shape2->setRestitution(0.0f);  // 设置反弹系数为0
    physicsBody->addShape(shape2);
    // 第三个矩形（中部）
    Vec2 rectangle3[] = {
        Vec2(540 * 1.25, 140 * 1.25),
        Vec2(540 * 1.25, 118 * 1.25),
        Vec2(405 * 1.25, 118 * 1.25),
        Vec2(405 * 1.25, 140 * 1.25)

    };
    auto shape3 = PhysicsShapePolygon::create(rectangle3, 4);
    shape3->setRestitution(0.0f);  // 设置反弹系数为0
    physicsBody->addShape(PhysicsShapePolygon::create(rectangle3, 4));
    physicsBody->setCategoryBitmask(0x01);
    physicsBody->setCollisionBitmask(0x02);  // 可以与分类为0x02的物体发生碰撞
    physicsBody->setContactTestBitmask(0xFFFFFFFF);
    // 将物理体设置为静态并添加到平台
    physicsBody->setDynamic(false);
    platform->setPhysicsBody(physicsBody);
    platform->getPhysicsBody()->getFirstShape()->setFriction(0.5f);//摩擦系数
    this->addChild(platform);
    CCLOG("Finished Level3Scene::loadLevel");  // 添加这一行
}

void Level3Scene::update(float dt) {
    // 获取玩家位置
    Vec2 playerPosition = m_player->getPosition();

    // 设定阈值
    float xThreshold = 0;  // 例如，屏幕宽度的三分之一

    // 检查玩家是否超过阈值
    if (playerPosition.x > xThreshold) {
        // 计算需要移动的距离
        float offsetX = playerPosition.x - xThreshold;

        // 更新层的位置
        this->setPosition(Vec2(-offsetX, 0));
    }
}

void Level3Scene::onEnter() {
    cocos2d::Layer::onEnter();

    // Play background music
    _backgroundMusicID = cocos2d::AudioEngine::play2d("music/mus_lvl0_intro_loop.mp3", true, 1.0f);  // Remember to replace the path with your actual music file path
}

void Level3Scene::onExit() {
    // Stop background music
    if (cocos2d::AudioEngine::getState(_backgroundMusicID) == cocos2d::AudioEngine::AudioState::PLAYING) {
        cocos2d::AudioEngine::stop(_backgroundMusicID);
    }

    cocos2d::Layer::onExit();
}

