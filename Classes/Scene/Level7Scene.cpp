#include "cocos2d.h"
using namespace cocos2d;
#include "Level7Scene.h"
#include "Player/Player.h"
#include "Trap/Spikeweed.h"
#include"Trap/jumpTable.h"
#include"Trap/brick.h"
#include"Trap/ice.h"
#include"Level7Scene.h"
#include "PauseMenu.h"
#include "EndLayer.h"

cocos2d::Scene* Level7Scene::createScene() {
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vec2(0, -1200));
    scene->getPhysicsWorld()->setSubsteps(60);

    auto layer = Level7Scene::create();
    scene->addChild(layer);

    return scene;
}

bool Level7Scene::init() {
    CCLOG("Starting Level7Scene::init");
    this->scheduleUpdate();
    initKeyboardListener();
    if (!Layer::init()) {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto player = Player::create(7, "movement/idle/Idle_00/Idle_00-0.png");
    player->setPosition(Vec2(100, 150));
    this->addChild(player, 1);
    player->getPhysicsBody()->getFirstShape()->setFriction(0.5f);
    loadLevel();

    CCLOG("Finished Level7Scene::init");
    return true;
}

void Level7Scene::loadLevel() {
    CCLOG("Starting Level7Scene::loadLevel");

    auto background = Sprite::create("level/endLevel/EndLevel_B.png");
    background->setAnchorPoint(Vec2(0.5, 0.5));
    background->setScale(0.8, 0.8);
    background->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2));
    this->addChild(background, 0);

    auto platform = Sprite::create();
    platform->setName("ground");
    platform->setAnchorPoint(Vec2(0.5, 0.5));
    platform->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, platform->getContentSize().height / 2));

    auto physicsBody = PhysicsBody::create();

    Vec2 polygonPoints1[] = {
Vec2(-635, 4),
Vec2(-352, 4),
Vec2(-352, 78),
Vec2(-635, 78)


    };

    // 创建多边形形状
    auto polygonShape1 = PhysicsShapePolygon::create(polygonPoints1, sizeof(polygonPoints1) / sizeof(polygonPoints1[0])); // 使用顶点数组创建形状
    polygonShape1->setRestitution(0.0f); // 设置反弹系数为0
    physicsBody->addShape(polygonShape1); // 将形状添加到物理体

    Vec2 polygonPoints2[] = {
Vec2(-384, 75),
Vec2(-309, 75),
Vec2(-309, 174),
Vec2(-384, 174)




    };

    // 创建多边形形状
    auto polygonShape2 = PhysicsShapePolygon::create(polygonPoints2, sizeof(polygonPoints2) / sizeof(polygonPoints2[0])); // 使用顶点数组创建形状
    polygonShape2->setRestitution(0.0f); // 设置反弹系数为0
    physicsBody->addShape(polygonShape2); // 将形状添加到物理体
    //////

    Vec2 polygonPoints3[] = {
Vec2(-317, 6),
Vec2(319, 6),
Vec2(319, 270),
Vec2(-317, 270)




    };
    auto polygonShape3 = PhysicsShapePolygon::create(polygonPoints3, sizeof(polygonPoints3) / sizeof(polygonPoints3[0])); // 使用顶点数组创建形状
    polygonShape3->setRestitution(0.0f); // 设置反弹系数为0
    physicsBody->addShape(polygonShape3);
    //
    Vec2 polygonPoints4[] = {
Vec2(294, 5),
Vec2(640, 5),
Vec2(640, 210),
Vec2(294, 210)




    };

    // 创建多边形形状
    auto polygonShape4 = PhysicsShapePolygon::create(polygonPoints4, sizeof(polygonPoints4) / sizeof(polygonPoints4[0]));
    polygonShape4->setRestitution(0.0f); // 设置反弹系数为0
    physicsBody->addShape(polygonShape4); // 将形状添加到物理体
    //////
    Vec2 polygonPoints5[] = {
Vec2(-415, 79),
Vec2(-378, 79),
Vec2(-378, 110),
Vec2(-415, 110)

    };

    // 创建多边形形状
    auto polygonShape5 = PhysicsShapePolygon::create(polygonPoints5, sizeof(polygonPoints5) / sizeof(polygonPoints5[0])); // 使用顶点数组创建形状
    polygonShape5->setRestitution(0.0f); // 设置反弹系数为0
    physicsBody->addShape(polygonShape5);
    // 

    physicsBody->setCategoryBitmask(0x01);
    physicsBody->setCollisionBitmask(0x02);
    physicsBody->setContactTestBitmask(0xFFFFFFFF);
    physicsBody->setDynamic(false);
    platform->setPhysicsBody(physicsBody);
    platform->getPhysicsBody()->getFirstShape()->setFriction(0.5f);
    this->addChild(platform);

    CCLOG("Finished Level7Scene::loadLevel");
}


void Level7Scene::onEnter() {
    cocos2d::Layer::onEnter();

    // Play background music
    _backgroundMusicID = cocos2d::AudioEngine::play2d("music/gameplayBGM.mp3", true, 1.0f);  // Remember to replace the path with your actual music file path
}

void Level7Scene::onExit() {
    // Stop background music
    if (cocos2d::AudioEngine::getState(_backgroundMusicID) == cocos2d::AudioEngine::AudioState::PLAYING) {
        cocos2d::AudioEngine::stop(_backgroundMusicID);
    }

    cocos2d::Layer::onExit();
}
//
void Level7Scene::startGame() {};
void Level7Scene::endGame() {};
void Level7Scene::pauseGame() {};
//
void Level7Scene::initKeyboardListener() {
    EventListenerKeyboard* listenerkeyPad = EventListenerKeyboard::create();
    listenerkeyPad->onKeyReleased = CC_CALLBACK_2(Level7Scene::onKeyPressedL7, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);
}
void Level7Scene::onKeyPressedL7(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event) {
    if (keycode == EventKeyboard::KeyCode::KEY_ESCAPE) {
        //ESC键
        auto pauseLayer = PauseMenu::create();
        Director::getInstance()->getRunningScene()->pause();
        Director::getInstance()->pushScene(pauseLayer);
    }
}
void Level7Scene::update(float dt) {
    if (checkForLevelTransition()) {
        // 暂停所有音乐和音效
        cocos2d::AudioEngine::pauseAll();
        // 切换到 EndLayer，不使用渐变过渡
        
        //创建新的 Scene
        auto scene = Scene::create();
        // 创建 EndLayer 并添加到 Scene
        auto endLayer = EndLayer::create();
        scene->addChild(endLayer);

        // 切换到新创建的 Scene
        Director::getInstance()->replaceScene(scene);

        //切换到结束界面



    }
}
bool Level7Scene::checkForLevelTransition() {
    // 设置射线的起始点和终点
    Vec2 rayStart = Vec2(680, 280);
    Vec2 rayEnd = Vec2(750, 280); // 这里需要你设置好转换点
    bool playerDetected = false;  // 用于记录是否检测到player
    /*
    // （debug）假设你有一个成员变量 drawNode 指向一个 DrawNode 实例
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