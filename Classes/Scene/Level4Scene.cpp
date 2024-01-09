#include "cocos2d.h"
using namespace cocos2d;
#include "Level4Scene.h"
#include "Player/Player.h"
#include "PauseMenu.h"

cocos2d::Scene* Level4Scene::createScene() {
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vec2(0, -1200));
    scene->getPhysicsWorld()->setSubsteps(60);

    auto layer = Level4Scene::create();
    scene->addChild(layer);

    return scene;
}

bool Level4Scene::init() {
    CCLOG("Starting Level4Scene::init");
    if (!Layer::init()) {
        return false;
    }
    initKeyboardListener();
    this->scheduleUpdate();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto player = Player::create(4,"movement/idle/Idle_00/Idle_00-0.png");
    player->setPosition(Vec2(100, 300));
    this->addChild(player, 1);
    player->getPhysicsBody()->getFirstShape()->setFriction(0.5f);
    loadLevel();

    CCLOG("Finished Level4Scene::init");
    return true;
}

void Level4Scene::loadLevel() {
    CCLOG("Starting Level4Scene::loadLevel");

    auto background = Sprite::create("level/level/L1/level1_B.png"); 
    background->setAnchorPoint(Vec2(0.5, 0.5));
    background->setScale(0.8, 0.8);
    background->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2));
    this->addChild(background, 0);

    auto platform = Sprite::create();
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
    edgeNode->setPosition(Vec2(1280 / 2, 0)); // 设置位置
    edgeNode->setPhysicsBody(edgeBody); // 将边界物理体附加到节点

    // 将边界节点添加到场景
    this->addChild(edgeNode);


    //创建jumpTable
  
    // 获取第一帧的SpriteFrame
    auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("JumpTable_00-0.png");

    // 检查是否成功获取到
    if (frame) {
        // 设置JumpTable的纹理为获取到的帧

       // 不能自己设置分割的物理体积的位置

        auto jumpTableSprite = JumpTable::create(Vec2(480, 200)); // 假设你有这样一个方法来创建JumpTable实例
        jumpTableSprite->setSpriteFrame(frame); // 使用第一帧作为纹理
        auto physicsBody = PhysicsBody::createBox(Size(50,10));// 设置物理形状
        physicsBody->setPositionOffset(Vec2(0, -130));// 设置物理体的位置偏移
        physicsBody->setDynamic(false);

        jumpTableSprite->setPhysicsBody(physicsBody);

        this->addChild(jumpTableSprite);
    }
    else {
        // 如果没有找到帧，可能需要输出错误或采取其他行动
        CCLOG("Error: Cannot find the first frame of JumpTable in SpriteFrameCache");
    }

    //创建spikeweed陷阱
    auto spikeweed1 = Spikeweed::create(Vec2(720, 280), Size(160, 10));
    this->addChild(spikeweed1);
    //创建spikeweed陷阱
    auto spikeweed2 = Spikeweed::create(Vec2(794,75), Size(70, 30));
    this->addChild(spikeweed2);
    //创建spikeweed陷阱
    auto spikeweed3 = Spikeweed::create(Vec2(862, 45), Size(60, 10));
    this->addChild(spikeweed3);

    auto physicsBody = PhysicsBody::create();

    Vec2 polygonPoints1[] = {
Vec2(-635, 16),
Vec2(-610, 16),
Vec2(-610, 335),
Vec2(-635, 335)

    };

    // 创建多边形形状
    auto polygonShape1 = PhysicsShapePolygon::create(polygonPoints1, sizeof(polygonPoints1) / sizeof(polygonPoints1[0])); // 使用顶点数组创建形状
    polygonShape1->setRestitution(0.0f); // 设置反弹系数为0
    physicsBody->addShape(polygonShape1); // 将形状添加到物理体
//
    Vec2 polygonPoints2[] = {
Vec2(-600, 15),
Vec2(-135, 15),
Vec2(-135, 78),
Vec2(-600, 78)



    };

    // 创建多边形形状
    auto polygonShape2 = PhysicsShapePolygon::create(polygonPoints2, sizeof(polygonPoints2) / sizeof(polygonPoints2[0])); // 使用顶点数组创建形状
    polygonShape2->setRestitution(0.0f); // 设置反弹系数为0
    physicsBody->addShape(polygonShape2); // 将形状添加到物理体
//
    Vec2 polygonPoints3[] = {
Vec2(-572, 460),
Vec2(-358, 460),
Vec2(-358, 669),
Vec2(-572, 669)


    };
    // 创建多边形形状
    auto polygonShape3 = PhysicsShapePolygon::create(polygonPoints3, sizeof(polygonPoints3) / sizeof(polygonPoints3[0])); // 使用顶点数组创建形状
    polygonShape3->setRestitution(0.0f); // 设置反弹系数为0
    physicsBody->addShape(polygonShape3);

    Vec2 polygonPoints4[] = {
   Vec2(-188, 562),
Vec2(-162, 562),
Vec2(-162, 689),
Vec2(-188, 689)


    };

    // 创建多边形形状
    auto polygonShape4 = PhysicsShapePolygon::create(polygonPoints4, sizeof(polygonPoints4) / sizeof(polygonPoints4[0]));
    polygonShape4->setRestitution(0.0f); // 设置反弹系数为0
    physicsBody->addShape(polygonShape4); // 将形状添加到物理体
//
    Vec2 polygonPoints5[] = {
Vec2(-188, 80),
Vec2(-127, 80),
Vec2(-127, 139),
Vec2(-188, 139)

    };

    // 创建多边形形状
    auto polygonShape5 = PhysicsShapePolygon::create(polygonPoints5, sizeof(polygonPoints5) / sizeof(polygonPoints5[0])); // 使用顶点数组创建形状
    polygonShape5->setRestitution(0.0f); // 设置反弹系数为0
    physicsBody->addShape(polygonShape5);
// 
// 
    Vec2 polygonPoints6[] = {
Vec2(0, 560),
Vec2(153, 560),
Vec2(153, 660),
Vec2(0, 660)

    };

    // 创建多边形形状
    auto polygonShape6 = PhysicsShapePolygon::create(polygonPoints6, sizeof(polygonPoints6) / sizeof(polygonPoints5[0])); // 使用顶点数组创建形状
    polygonShape6->setRestitution(0.0f); // 设置反弹系数为0
    physicsBody->addShape(polygonShape6);
//
    Vec2 polygonPoints7[] = {
Vec2(32, 431),
Vec2(125, 431),
Vec2(125, 547),
Vec2(32, 547)

    };

    // 创建多边形形状
    auto polygonShape7 = PhysicsShapePolygon::create(polygonPoints7, sizeof(polygonPoints7) / sizeof(polygonPoints7[0])); // 使用顶点数组创建形状
    polygonShape7->setRestitution(0.0f); // 设置反弹系数为0
    physicsBody->addShape(polygonShape7);
//
    Vec2 polygonPoints8[] = {
Vec2(27, 4),
Vec2(120, 4),
Vec2(120, 203),
Vec2(27, 203)

    };

    // 创建多边形形状
    auto polygonShape8 = PhysicsShapePolygon::create(polygonPoints8, sizeof(polygonPoints8) / sizeof(polygonPoints8[0])); // 使用顶点数组创建形状
    polygonShape8->setRestitution(0.0f); // 设置反弹系数为0
    physicsBody->addShape(polygonShape8);
//
    Vec2 polygonPoints9[] = {
Vec2(253,4),
Vec2(612, 4),
Vec2(612, 143),
Vec2(253, 143)



    };

    // 创建多边形形状
    auto polygonShape9 = PhysicsShapePolygon::create(polygonPoints9, sizeof(polygonPoints9) / sizeof(polygonPoints9[0])); // 使用顶点数组创建形状
    polygonShape9->setRestitution(0.0f); // 设置反弹系数为0
    physicsBody->addShape(polygonShape9);
//
//
    Vec2 polygonPoints10[] = {
Vec2(283, 525),
Vec2(428, 525),
Vec2(428, 688),
Vec2(283, 688)

    };

    // 创建多边形形状
    auto polygonShape10 = PhysicsShapePolygon::create(polygonPoints10, sizeof(polygonPoints10) / sizeof(polygonPoints10[0])); // 使用顶点数组创建形状
    polygonShape10->setRestitution(0.0f); // 设置反弹系数为0
    physicsBody->addShape(polygonShape10);


    Vec2 polygonPoints11[] = {
Vec2(388, 400),
Vec2(443, 400),
Vec2(443, 520),
Vec2(388, 520)


    };

    // 创建多边形形状
    auto polygonShape11 = PhysicsShapePolygon::create(polygonPoints11, sizeof(polygonPoints11) / sizeof(polygonPoints11[0]));
    polygonShape11->setRestitution(0.0f);
    physicsBody->addShape(polygonShape11);

//
    Vec2 polygonPoints12[] = {
 Vec2(384, 140),
Vec2(576, 140),
Vec2(576, 238),
Vec2(384, 238)

    };

    // 创建多边形形状
    auto polygonShape12 = PhysicsShapePolygon::create(polygonPoints12, sizeof(polygonPoints12) / sizeof(polygonPoints12[0]));
    polygonShape12->setRestitution(0.0f);
    physicsBody->addShape(polygonShape12);
//
//
    Vec2 polygonPoints13[] = {
  Vec2(580, 153),
Vec2(619, 153),
Vec2(619, 750),
Vec2(580, 750)


    };

    // 创建多边形形状
    auto polygonShape13 = PhysicsShapePolygon::create(polygonPoints13, sizeof(polygonPoints13) / sizeof(polygonPoints13[0]));
    polygonShape13->setRestitution(0.0f);
    physicsBody->addShape(polygonShape13);

//
    Vec2 polygonPoints14[] = {
Vec2(420, 340),
Vec2(450, 340),
Vec2(450, 750),
Vec2(420, 750)


    };

    // 创建多边形形状
    auto polygonShape14 = PhysicsShapePolygon::create(polygonPoints14, sizeof(polygonPoints14) / sizeof(polygonPoints14[0]));
    polygonShape14->setRestitution(0.0f);
    physicsBody->addShape(polygonShape14);

    physicsBody->setCategoryBitmask(0x01);
    physicsBody->setCollisionBitmask(0x02);
    physicsBody->setContactTestBitmask(0xFFFFFFFF);
    physicsBody->setDynamic(false);
    platform->setPhysicsBody(physicsBody);
    platform->getPhysicsBody()->getFirstShape()->setFriction(0.5f);
    this->addChild(platform);

    CCLOG("Finished Level4Scene::loadLevel");
}
void Level4Scene::onEnter() {
    cocos2d::Layer::onEnter();

    // Play background music
    _backgroundMusicID = cocos2d::AudioEngine::play2d("music/mus_lvl0_intro_loop.mp3", true, 1.0f);  // Remember to replace the path with your actual music file path
}

void Level4Scene::onExit() {
    // Stop background music
    if (cocos2d::AudioEngine::getState(_backgroundMusicID) == cocos2d::AudioEngine::AudioState::PLAYING) {
        cocos2d::AudioEngine::stop(_backgroundMusicID);
    }

    cocos2d::Layer::onExit();
}
//
void Level4Scene::startGame() {};
void Level4Scene::endGame() {};
void Level4Scene::pauseGame() {};
//
void Level4Scene::initKeyboardListener() {
    EventListenerKeyboard* listenerkeyPad = EventListenerKeyboard::create();
    listenerkeyPad->onKeyReleased = CC_CALLBACK_2(Level4Scene::onKeyPressedL4, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);
}
void Level4Scene::onKeyPressedL4(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event) {
    if (keycode == EventKeyboard::KeyCode::KEY_ESCAPE) {
        //ESC键
        auto pauseLayer = PauseMenu::create();
        Director::getInstance()->getRunningScene()->pause();
        Director::getInstance()->pushScene(pauseLayer);
    }
}