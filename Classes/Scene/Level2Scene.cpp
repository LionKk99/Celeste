#include "cocos2d.h"
using namespace cocos2d;
#include "Level2Scene.h"
#include "Player/Player.h"
#include"Trap/ice.h"
#include "Level3Scene.h"
#include "PauseMenu.h"

cocos2d::Scene* Level2Scene::createScene() {
    auto scene = Scene::createWithPhysics();  // 创建一个带有物理世界的场景
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    scene->getPhysicsWorld()->setGravity(Vec2(0, -1200));//重力设置
    scene->getPhysicsWorld()->setSubsteps(60);  // 增加迭代次数

    auto layer = Level2Scene::create();
    scene->addChild(layer);

    return scene;
}


bool Level2Scene::init() {
    CCLOG("Starting Level2Scene::init");  // 添加这一行
    if (!Layer::init()) {
        return false;
    }
    initKeyboardListener();
    this->scheduleUpdate();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 创建背景层
    auto background = Sprite::create("level/xumu/L1/xumu1_LG5.png");  // 更远的背景
    background->setAnchorPoint(Vec2(0.5, 0.5));
    background->setScale(0.8);
    background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(background, -3);

    auto midground = Sprite::create("level/xumu/L1/xumu1_LG4.png");  // 中间层背景
    midground->setAnchorPoint(Vec2(0.5, 0.5));
    midground->setScale(0.8);
    midground->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(midground, -2);

    //创建ice
    auto ice = Ice::create(Vec2(880, 498)); // 假设位置
    this->addChild(ice); // 添加到场景或其他父节点中

   
    // 创建玩家
    auto player = Player::create(2,"movement/idle/Idle_00/Idle_00-0.png");
    player->setPosition(Vec2(0, 260));
    this->addChild(player, 3);
    player->getPhysicsBody()->getFirstShape()->setFriction(0.5f);
    loadLevel();
    // 设置更新方法，用于卷轴效果
    CCLOG("Finished Level2Scene::init");
    return true;
}

void Level2Scene::loadLevel() {
    CCLOG("Starting Level2Scene::loadLevel");  // 添加这一行

    // 创建卷轴背景
    auto background = Sprite::create("level/xumu/L1/xumu1_LG2.png");
    background->setAnchorPoint(Vec2(0.5, 0.5));
    background->setScale(0.8, 0.8);
    background->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2));
    this->addChild(background, -1);  // 设置在更高层级以覆盖其他背景

    // 添加平台
    auto platform = Sprite::create(); // 这里创建的是一个透明的平台，没有纹理
    platform->setName("ground");
    platform->setAnchorPoint(Vec2(0.5, 0.5));
    platform->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, platform->getContentSize().height / 2));

    //添加边界
       // 定义边界的顶点
    Vec2 edgeVertices[] = {
        Vec2(-680, 0),                    // 左下角
        Vec2(680, 0),    // 右下角
        Vec2(680, 800),  // 右上角
        Vec2(-680, 800)    // 左上角
    };

    // 创建一个edge形状的物理体
    auto edgeBody = PhysicsBody::createEdgePolygon(edgeVertices, 4);

    // 创建一个空节点用于附加边界物理体
    auto edgeNode = Node::create();
    edgeNode->setPosition(Vec2(1280/2,0 )); // 设置位置
    edgeNode->setPhysicsBody(edgeBody); // 将边界物理体附加到节点

    // 将边界节点添加到场景
    this->addChild(edgeNode);
    //// 创建物理体
    auto physicsBody = PhysicsBody::create();

    //// 定义多边形顶点
    //Vec2 rectangle1[] = {

    //Vec2(-640, 175),
    //Vec2(-640, 30),
    //Vec2(-220, 30),
    //Vec2(-220, 175),
    //};
    //auto shape1 = PhysicsShapePolygon::create(rectangle1, 4);
    //shape1->setRestitution(0.0f);  // 设置反弹系数为0
    //physicsBody->addShape(shape1);

    //Vec2 rectangle2[] = {
    //Vec2(-60, 175),
    //Vec2(-60, 240),
    //Vec2(-220, 240),
    //Vec2(-220, 175),
    //};
    //auto shape2 = PhysicsShapePolygon::create(rectangle2, 4);
    //shape2->setRestitution(0.0f);  // 设置反弹系数为0
    //physicsBody->addShape(shape2);
    Vec2 polygonPoints1[] = {
Vec2(-500.000, 177),
Vec2(-370, 117),
Vec2(-370, 0), 
Vec2(-648, 0),
Vec2(-646, 180)
    };

    // 创建多边形形状
    auto polygonShape1 = PhysicsShapePolygon::create(polygonPoints1, sizeof(polygonPoints1) / sizeof(polygonPoints1[0])); // 使用顶点数组创建形状
    polygonShape1->setRestitution(0.0f); // 设置反弹系数为0
    physicsBody->addShape(polygonShape1); // 将形状添加到物理体

    Vec2 polygonPoints2[] = {
 Vec2(-480, 180),
 Vec2(-320, 116),
 Vec2(-320, 116), 
 Vec2(-320, 238), 
 Vec2(-480, 238)
    };

    // 创建多边形形状
    auto polygonShape2 = PhysicsShapePolygon::create(polygonPoints2, sizeof(polygonPoints2) / sizeof(polygonPoints2[0])); // 使用顶点数组创建形状
    polygonShape2->setRestitution(0.0f); // 设置反弹系数为0
    physicsBody->addShape(polygonShape2); // 将形状添加到物理体

    Vec2 polygonPoints3[] = {
Vec2(-160, 500), 
Vec2(-60, 500),
Vec2(-60, 430),
Vec2(-160, 430) };
    // 创建多边形形状
    auto polygonShape3 = PhysicsShapePolygon::create(polygonPoints3, sizeof(polygonPoints3) / sizeof(polygonPoints3[0])); // 使用顶点数组创建形状
    polygonShape3->setRestitution(0.0f); // 设置反弹系数为0
    physicsBody->addShape(polygonShape3);

    
    Vec2 polygonPoints4[] = {
Vec2(-155, 142), 
Vec2(-155, 0),

Vec2(40,142),
Vec2(40, 0)
    };

    // 创建多边形形状
    auto polygonShape4 = PhysicsShapePolygon::create(polygonPoints4, sizeof(polygonPoints4) / sizeof(polygonPoints4[0])); // 使用顶点数组创建形状
    polygonShape4->setRestitution(0.0f); // 设置反弹系数为0
    physicsBody->addShape(polygonShape4);

    Vec2 polygonPoints5[] = {
Vec2(0, 140),
Vec2(0, 240), 
Vec2(-60, 140),
Vec2(-60, 240) };

    // 创建多边形形状
    auto polygonShape5 = PhysicsShapePolygon::create(polygonPoints5, sizeof(polygonPoints5) / sizeof(polygonPoints5[0])); // 使用顶点数组创建形状
    polygonShape5->setRestitution(0.0f); // 设置反弹系数为0
    physicsBody->addShape(polygonShape5);

    Vec2 polygonPoints6[] = {
Vec2(40, 140),
Vec2(40, 170),
Vec2(0, 140),
Vec2(0, 170) };

    // 创建多边形形状
    auto polygonShape6 = PhysicsShapePolygon::create(polygonPoints6, sizeof(polygonPoints6) / sizeof(polygonPoints5[0])); // 使用顶点数组创建形状
    polygonShape6->setRestitution(0.0f); // 设置反弹系数为0
    physicsBody->addShape(polygonShape6);
    Vec2 polygonPoints7[] = {
Vec2(140, 170),
Vec2(140, 200),
Vec2(350, 200),
Vec2(350,170) };

    // 创建多边形形状
    auto polygonShape7 = PhysicsShapePolygon::create(polygonPoints7, sizeof(polygonPoints7) / sizeof(polygonPoints7[0])); // 使用顶点数组创建形状
    polygonShape7->setRestitution(0.0f); // 设置反弹系数为0
    physicsBody->addShape(polygonShape7);

    Vec2 polygonPoints8[] = {
Vec2(160, 160),
Vec2(160, 0),
Vec2(330, 160),
Vec2(330,0) };

    // 创建多边形形状
    auto polygonShape8 = PhysicsShapePolygon::create(polygonPoints8, sizeof(polygonPoints8) / sizeof(polygonPoints8[0])); // 使用顶点数组创建形状
    polygonShape8->setRestitution(0.0f); // 设置反弹系数为0
    physicsBody->addShape(polygonShape8);


    Vec2 polygonPoints9[] = {
Vec2(800, 145),
Vec2(800, 0),
Vec2(500, 145),
Vec2(500,0) };

    // 创建多边形形状
    auto polygonShape9 = PhysicsShapePolygon::create(polygonPoints9, sizeof(polygonPoints9) / sizeof(polygonPoints9[0])); // 使用顶点数组创建形状
    polygonShape9->setRestitution(0.0f); // 设置反弹系数为0
    physicsBody->addShape(polygonShape9);

    Vec2 polygonPoints10[] = {
Vec2(800, 240),
Vec2(800, 0),
Vec2(610, 240),
Vec2(610,0) };

    // 创建多边形形状
    auto polygonShape10 = PhysicsShapePolygon::create(polygonPoints10, sizeof(polygonPoints10) / sizeof(polygonPoints10[0])); // 使用顶点数组创建形状
    polygonShape10->setRestitution(0.0f); // 设置反弹系数为0
    physicsBody->addShape(polygonShape10);

    Vec2 polygonPoints11[] = {
Vec2(130, 720),
Vec2(130, 560),
Vec2(350, 720),
Vec2(350,560) };

    // 创建多边形形状
    auto polygonShape11 = PhysicsShapePolygon::create(polygonPoints11, sizeof(polygonPoints11) / sizeof(polygonPoints11[0])); // 使用顶点数组创建形状
    polygonShape11->setRestitution(0.0f); // 设置反弹系数为0
    physicsBody->addShape(polygonShape11);


    physicsBody->setCategoryBitmask(0x01);
    physicsBody->setCollisionBitmask(0x02);  // 可以与分类为0x02的物体发生碰撞
    physicsBody->setContactTestBitmask(0xFFFFFFFF);
    // 将物理体设置为静态并添加到平台
    physicsBody->setDynamic(false);
    platform->setPhysicsBody(physicsBody);
    platform->getPhysicsBody()->getFirstShape()->setFriction(0.5f);//摩擦系数
    this->addChild(platform);
    CCLOG("Finished Level2Scene::loadLevel");  // 添加这一行
    //// 创建多边形形状
}
//
void Level2Scene::startGame() {};
void Level2Scene::endGame() {};
void Level2Scene::pauseGame() {};
//
void Level2Scene::onEnter() {
    cocos2d::Layer::onEnter();

    // Play background music
    _backgroundMusicID = cocos2d::AudioEngine::play2d("music/mus_lvl0_intro_loop.mp3", true, 1.0f);  // Remember to replace the path with your actual music file path
}

void Level2Scene::onExit() {
    // Stop background music
    if (cocos2d::AudioEngine::getState(_backgroundMusicID) == cocos2d::AudioEngine::AudioState::PLAYING) {
        cocos2d::AudioEngine::stop(_backgroundMusicID);
    }

    cocos2d::Layer::onExit();
}
bool Level2Scene::checkForLevelTransition() {
    // 设置射线的起始点和终点
    Vec2 rayStart = Vec2(1200, 280);
    Vec2 rayEnd = Vec2(1280, 280); // 这里需要你设置好转换点
    bool playerDetected = false;  // 用于记录是否检测到player
    /*(debug)
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

void Level2Scene::update(float dt) {
    if (checkForLevelTransition()) {
        // 切换到 Level3Scene，不使用渐变过渡
        auto scene = Level3Scene::createScene();
        Director::getInstance()->replaceScene(scene);
    }
}
void Level2Scene::initKeyboardListener() {
    EventListenerKeyboard* listenerkeyPad = EventListenerKeyboard::create();
    listenerkeyPad->onKeyReleased = CC_CALLBACK_2(Level2Scene::onKeyPressedL2, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);
}
void Level2Scene::onKeyPressedL2(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event) {
    if (keycode == EventKeyboard::KeyCode::KEY_ESCAPE) {
        //ESC键
        auto pauseLayer = PauseMenu::create();
        Director::getInstance()->getRunningScene()->pause();
        Director::getInstance()->pushScene(pauseLayer);
    }
}