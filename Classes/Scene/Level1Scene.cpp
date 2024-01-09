#pragma once
#include "cocos2d.h"
using namespace cocos2d;
#include "Level1Scene.h"
#include "Player/Player.h"
#include "Trap/Spikeweed.h"
#include"Trap/jumpTable.h"
#include"Trap/brick.h"
#include"Trap/ice.h"
#include"Level2Scene.h"
#include "PauseMenu.h"

USING_NS_CC;

Scene* Level1Scene::createScene() {
    auto scene = Scene::createWithPhysics();  // 创建一个带有物理世界的场景
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    scene->getPhysicsWorld()->setGravity(Vec2(0, -1200));//重力设置
    scene->getPhysicsWorld()->setSubsteps(60);  // 增加迭代次数

    auto layer = Level1Scene::create();
    scene->addChild(layer);

    return scene;
}
//
void Level1Scene::startGame() {};
void Level1Scene::endGame(){};
void Level1Scene::pauseGame(){};
//
bool Level1Scene::init() {
    this->scheduleUpdate();
    if (!LevelBase::init()) {
        return false;
    }
    CCLOG("Starting Level1Scene::init");  // 添加这一行
    if (!Layer::init()) {
        return false;
    }
    initKeyboardListener();
    
    // 获取场景的大小
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    //初始化检查点和重生点
    Vec2 checkpoint = Vec2(0,0);//结束点所在位置
    Vec2 respawnPoint = Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y); // 设置重生位置到屏幕中心

    //创建左右边界
    auto leftEdge = PhysicsBody::createEdgeSegment(Vec2(origin.x, origin.y), Vec2(origin.x, origin.y + visibleSize.height), PHYSICSBODY_MATERIAL_DEFAULT, 1);
    leftEdge->setName("edge");
    leftEdge->setDynamic(false);
    auto leftEdgeNode = Node::create();
    leftEdgeNode->setPhysicsBody(leftEdge);
    this->addChild(leftEdgeNode);

    auto rightEdge = PhysicsBody::createEdgeSegment(Vec2(origin.x + visibleSize.width, origin.y), Vec2(origin.x + visibleSize.width, origin.y + visibleSize.height), PHYSICSBODY_MATERIAL_DEFAULT, 1);
    rightEdge->setName("edge");
    rightEdge->setDynamic(false);
    auto rightEdgeNode = Node::create();
    rightEdgeNode->setPhysicsBody(rightEdge);
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
    auto player = Player::create(1,"movement/idle/Idle_00/Idle_00-0.png"); // 这只是一个示例，您可能需要为其提供一个初始的精灵图片路径
    player->setPosition(respawnPoint); // 设置位置到屏幕中心
    this->addChild(player); // 将玩家添加到场景中
    player->getPhysicsBody()->getFirstShape()->setFriction(0.5f);

    //创建spikeweed陷阱
    auto spikeweed = Spikeweed::create(Vec2(400, 120), Size(100, 10));
    this->addChild(spikeweed);

    //创建jumpTable
  /*
    // 获取第一帧的SpriteFrame
    auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("JumpTable_00-0.png");

    // 检查是否成功获取到
    if (frame) {
        // 设置JumpTable的纹理为获取到的帧
        
        //不能自己设置分割的物理体积的位置
        
        auto jumpTableSprite = JumpTable::create(Vec2(700, 140)); // 假设你有这样一个方法来创建JumpTable实例
        jumpTableSprite->setSpriteFrame(frame); // 使用第一帧作为纹理            
        auto physicsBody = PhysicsBody::createBox(Size(100,10));// 设置物理形状
        physicsBody->setPositionOffset(Vec2(0, -100));// 设置物理体的位置偏移
        physicsBody->setDynamic(false);

        jumpTableSprite->setPhysicsBody(physicsBody);

        this->addChild(jumpTableSprite);
    }
    else {
        // 如果没有找到帧，可能需要输出错误或采取其他行动
        CCLOG("Error: Cannot find the first frame of JumpTable in SpriteFrameCache");
    }*/
    //创建brick
    auto brick = Brick::create(Vec2(100, 200));
    this->addChild(brick); // 将brick添加到场景
    /*
    //碰撞监听器   
    auto contactListener = cocos2d::EventListenerPhysicsContact::create();
    contactListener->onContactBegin = [this](cocos2d::PhysicsContact& contact) {
        auto nodeA = contact.getShapeA()->getBody()->getNode();
        auto nodeB = contact.getShapeB()->getBody()->getNode();

        if ((nodeA && nodeA->getName() == "player" && nodeB && nodeB->getName() == "ground") ||
            (nodeB && nodeB->getName() == "player" && nodeA && nodeA->getName() == "ground")) {
            // 设置玩家在地面上
            auto player = dynamic_cast<Player*>(nodeA->getName() == "player" ? nodeA : nodeB);
            if (player) {
                player->setOnGround(true);
                CCLOG("Landed.");
            }
        }
        return true;
        };

    contactListener->onContactSeparate = [this](cocos2d::PhysicsContact& contact) {
        auto nodeA = contact.getShapeA()->getBody()->getNode();
        auto nodeB = contact.getShapeB()->getBody()->getNode();

        if ((nodeA && nodeA->getName() == "player" && nodeB && nodeB->getName() == "ground") ||
            (nodeB && nodeB->getName() == "player" && nodeA && nodeA->getName() == "ground")) {
            // 设置玩家不在地面上
            auto player = dynamic_cast<Player*>(nodeA->getName() == "player" ? nodeA : nodeB);
            if (player) {
                player->setOnGround(false);
                CCLOG("NO Landed");
            }
        }
        };

    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
    */
    //创建ice
    auto ice = Ice::create(Vec2(700, 400)); // 假设位置
    this->addChild(ice); // 添加到场景或其他父节点中

    // 加载这一关的特定内容，例如背景、障碍物等
    loadLevel();
    CCLOG("Finished Level1Scene::init");  // 添加这一行
    return true;
}

void Level1Scene::loadLevel() {
    // Level 1 specific content loading
    // For example: setting up the player, platforms, enemies, background, etc.
    // This should include setting up the physics bodies, sprites, and any game logic.


    CCLOG("Starting Level1Scene::loadLevel");  // 添加这一行
    // 添加背景
    auto background = Sprite::create("level/xumu/L0/xumu0_B.png");
    background->setAnchorPoint(Vec2(0.5, 0.5));
    background->setScale(0.8, 0.8);
    background->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2));
    this->addChild(background, -1);

    // 添加平台
    auto platform = Sprite::create(); // 这里创建的是一个透明的平台，没有纹理
    platform->setName("ground");
    platform->setAnchorPoint(Vec2(0.5, 0.5));
    platform->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, platform->getContentSize().height / 2));


    // 创建物理体
    auto physicsBody = PhysicsBody::create();

    // 第一个矩形（下方）
    //正常位置
    Vec2 rectangle1[] = {
        Vec2(-540 * 1.25, -100 * 1.25),
        Vec2(540 * 1.25, -100 * 1.25),
        Vec2(540 * 1.25, 90 * 1.25),
        Vec2(-540 * 1.25, 90 * 1.25)
    };
    //测试位置
    /*
    Vec2 rectangle1[] = {
        Vec2(-200 * 1.25, -100 * 1.25),
        Vec2(540 * 1.25, -100 * 1.25),
        Vec2(540 * 1.25, 90 * 1.25),
        Vec2(-200 * 1.25, 90 * 1.25)
    };*/
    auto shape1 = PhysicsShapePolygon::create(rectangle1, 4);
    shape1->setRestitution(0.0f);  // 设置反弹系数为0
    physicsBody->addShape(shape1);
    // 第二个矩形（中间）
    Vec2 rectangle2[] = {
        Vec2(380 * 1.25, 118 * 1.25),
        Vec2(380 * 1.25, 90 * 1.25),
        Vec2(540 * 1.25, 90 * 1.25),
        Vec2(540 * 1.25, 118 * 1.25)
    };
    auto shape2 = PhysicsShapePolygon::create(rectangle2, 4);
    shape2->setRestitution(0.0f);  // 设置反弹系数为0
    physicsBody->addShape(shape2);
    // 第三个矩形（上方）
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
    CCLOG("Finished Level1Scene::loadLevel");  // 添加这一行
}


void Level1Scene::onEnter() {
    cocos2d::Layer::onEnter();

    // Play background music
    _backgroundMusicID = cocos2d::AudioEngine::play2d("music/mus_lvl0_intro_loop.mp3", true, 1.0f);  // Remember to replace the path with your actual music file path
}

void Level1Scene::onExit() {
    // Stop background music
    if (cocos2d::AudioEngine::getState(_backgroundMusicID) == cocos2d::AudioEngine::AudioState::PLAYING) {
        cocos2d::AudioEngine::stop(_backgroundMusicID);
    }

    cocos2d::Layer::onExit();
}

void Level1Scene::update(float dt) {
    if (checkForLevelTransition()) {
        // 切换到 Level2Scene，不使用渐变过渡
        auto scene = Level2Scene::createScene();
        Director::getInstance()->replaceScene(scene);
    }
}
bool Level1Scene::checkForLevelTransition() {
    // 设置射线的起始点和终点
    Vec2 rayStart = Vec2(1280, 300);
    Vec2 rayEnd = Vec2(1150, 300); // 这里需要你设置好转换点
    bool playerDetected = false;  // 用于记录是否检测到player
    /*（debug）
    // 假设你有一个成员变量 drawNode 指向一个 DrawNode 实例
    auto drawNode = DrawNode::create();
    this->addChild(drawNode);
    
    // 绘制射线
    drawNode->clear(); // 清除之前的绘制内容
    drawNode->drawLine(rayStart, rayEnd, Color4F::BLUE); //蓝色表示射线
*/
    auto rayCallback = [&playerDetected](PhysicsWorld& world, const PhysicsRayCastInfo& info, void* data)->bool {
        auto node = info.shape->getBody()->getNode();
        if (node && node->getName() == "player") {
            // 如果射线检测到Player
            playerDetected = true;  // 记录检测到player
            return false; // 停止射线检测
        }
        return true; // 继续射线检测
        };

    Director::getInstance()->getRunningScene()->getPhysicsWorld()->rayCast(rayCallback, rayStart, rayEnd, nullptr);
    return playerDetected;  // 返回是否检测到player
}
void Level1Scene::initKeyboardListener() {
    EventListenerKeyboard* listenerkeyPad = EventListenerKeyboard::create();
    listenerkeyPad->onKeyReleased = CC_CALLBACK_2(Level1Scene::onKeyPressedL1, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);
}
void Level1Scene::onKeyPressedL1(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event) {
    if (keycode == EventKeyboard::KeyCode::KEY_ESCAPE) {
        //ESC键
        auto pauseLayer = PauseMenu::create();
        Director::getInstance()->getRunningScene()->pause();
        Director::getInstance()->pushScene(pauseLayer);
    }
}