#include "cocos2d.h"
using namespace cocos2d;
#include "Level3Scene.h"
#include "Level4Scene.h"
#include "Player/Player.h"
#include "Trap/Spikeweed.h"
#include "PauseMenu.h"

cocos2d::Scene* Level3Scene::createScene() {
    auto scene = Scene::createWithPhysics();  // 创建一个带有物理世界的场景
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    scene->getPhysicsWorld()->setGravity(Vec2(0, -1200));//重力设置
    scene->getPhysicsWorld()->setSubsteps(60);  // 增加迭代次数

    auto layer = Level3Scene::create();
    scene->addChild(layer);

    return scene;
}


bool Level3Scene::init() {
    CCLOG("Starting Level3Scene::init");  // 添加这一行
    if (!Layer::init()) {
        return false;
    }
    initKeyboardListener();
    this->scheduleUpdate();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

   



    // 创建玩家
    auto player = Player::create(3, "movement/idle/Idle_00/Idle_00-0.png");
    player->setPosition(Vec2(70, 200));
    this->addChild(player, 1);
    player->getPhysicsBody()->getFirstShape()->setFriction(0.5f);
    loadLevel();
    // 设置更新方法，用于卷轴效果
    CCLOG("Finished Level3Scene::init");
    return true;
}

void Level3Scene::loadLevel() {
    CCLOG("Starting Level3Scene::loadLevel");  // 添加这一行

    // 创建卷轴背景
    // 
    auto background = Sprite::create("level/level/L0/level0_B.png");
    background->setAnchorPoint(Vec2(0.5, 0.5));
    background->setScale(0.8, 0.8);
    background->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2));
    this->addChild(background, 0);  // 设置在更高层级以覆盖其他背景

     // 设置在更高层级以覆盖其他背景

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
    edgeNode->setPosition(Vec2(1280 / 2, 0)); // 设置位置
    edgeNode->setPhysicsBody(edgeBody); // 将边界物理体附加到节点

    // 将边界节点添加到场景
    this->addChild(edgeNode);
    

    //创建spikeweed陷阱
    auto spikeweed1 = Spikeweed::create(Vec2(235, 50), Size(160, 10));
    this->addChild(spikeweed1);

    //创建spikeweed陷阱
    auto spikeweed2 = Spikeweed::create(Vec2(500, 120), Size(160, 10));
    this->addChild(spikeweed2);
    
    //创建spikeweed陷阱
    auto spikeweed3 = Spikeweed::create(Vec2(767, 200), Size(130, 10));
    this->addChild(spikeweed3);

    //创建spikeweed陷阱
    auto spikeweed4 = Spikeweed::create(Vec2(880, 86), Size(90, 10));
    this->addChild(spikeweed4);

    //创建spikeweed陷阱
    auto spikeweed5 = Spikeweed::create(Vec2(1080, 365), Size(10, 60));
    this->addChild(spikeweed5);


    //// 创建物理体
    auto physicsBody = PhysicsBody::create();

    // 定义多边形顶点并创建多边形形状
    // ...[Add all the polygon definitions here similar to the original code]...
    Vec2 polygonPoints1[] = {
Vec2(-622, 143),
Vec2(-480, 143),
Vec2(-480, 8),
Vec2(-622, 8)
    };

    // 创建多边形形状
    auto polygonShape1 = PhysicsShapePolygon::create(polygonPoints1, sizeof(polygonPoints1) / sizeof(polygonPoints1[0])); // 使用顶点数组创建形状
    polygonShape1->setRestitution(0.0f); // 设置反弹系数为0
    physicsBody->addShape(polygonShape1); // 将形状添加到物理体

    Vec2 polygonPoints2[] = {
Vec2(-634, 428),
Vec2(-575, 428),
Vec2(-575, 639),
Vec2(-634, 639)


    };

    // 创建多边形形状
    auto polygonShape2 = PhysicsShapePolygon::create(polygonPoints2, sizeof(polygonPoints2) / sizeof(polygonPoints2[0])); // 使用顶点数组创建形状
    polygonShape2->setRestitution(0.0f); // 设置反弹系数为0
    physicsBody->addShape(polygonShape2); // 将形状添加到物理体

    Vec2 polygonPoints3[] = {
Vec2(-323, 209),
Vec2(-222, 209),
Vec2(-222, 15),
Vec2(-323, 15)
    };
    // 创建多边形形状
    auto polygonShape3 = PhysicsShapePolygon::create(polygonPoints3, sizeof(polygonPoints3) / sizeof(polygonPoints3[0])); // 使用顶点数组创建形状
    polygonShape3->setRestitution(0.0f); // 设置反弹系数为0
    physicsBody->addShape(polygonShape3);

    Vec2 polygonPoints4[] = {
    Vec2(-510, 488),
Vec2(-359, 488),
Vec2(-359, 639),
Vec2(-510, 639)

    };

    // 创建多边形形状
    auto polygonShape4 = PhysicsShapePolygon::create(polygonPoints4, sizeof(polygonPoints4) / sizeof(polygonPoints4[0]));
    polygonShape4->setRestitution(0.0f); // 设置反弹系数为0
    physicsBody->addShape(polygonShape4); // 将形状添加到物理体

    Vec2 polygonPoints5[] = {
Vec2(-608, 151),
Vec2(-635, 151),
Vec2(-635, 425),
Vec2(-608, 425)
    };

    // 创建多边形形状
    auto polygonShape5 = PhysicsShapePolygon::create(polygonPoints5, sizeof(polygonPoints5) / sizeof(polygonPoints5[0])); // 使用顶点数组创建形状
    polygonShape5->setRestitution(0.0f); // 设置反弹系数为0
    physicsBody->addShape(polygonShape5);

    Vec2 polygonPoints6[] = {
Vec2(60, 400),
Vec2(60, 50),
Vec2(-60, 400),
Vec2(-60, 50)
    };

    // 创建多边形形状
    auto polygonShape6 = PhysicsShapePolygon::create(polygonPoints6, sizeof(polygonPoints6) / sizeof(polygonPoints5[0])); // 使用顶点数组创建形状
    polygonShape6->setRestitution(0.0f); // 设置反弹系数为0
    physicsBody->addShape(polygonShape6);

    Vec2 polygonPoints7[] = {
Vec2(-230, 650),
Vec2(-125, 650),
Vec2(-125, 530),
Vec2(-230, 530)
    };

    // 创建多边形形状
    auto polygonShape7 = PhysicsShapePolygon::create(polygonPoints7, sizeof(polygonPoints7) / sizeof(polygonPoints7[0])); // 使用顶点数组创建形状
    polygonShape7->setRestitution(0.0f); // 设置反弹系数为0
    physicsBody->addShape(polygonShape7);

    Vec2 polygonPoints8[] = {
Vec2(-136, 621),
Vec2(-68, 621),
Vec2(-68, 696),
Vec2(-136, 696)
    };

    // 创建多边形形状
    auto polygonShape8 = PhysicsShapePolygon::create(polygonPoints8, sizeof(polygonPoints8) / sizeof(polygonPoints8[0])); // 使用顶点数组创建形状
    polygonShape8->setRestitution(0.0f); // 设置反弹系数为0
    physicsBody->addShape(polygonShape8);

    Vec2 polygonPoints9[] = {
Vec2(-64, 650),
Vec2(27, 650),
Vec2(27, 706),
Vec2(-64,706)


    };

    // 创建多边形形状
    auto polygonShape9 = PhysicsShapePolygon::create(polygonPoints9, sizeof(polygonPoints9) / sizeof(polygonPoints9[0])); // 使用顶点数组创建形状
    polygonShape9->setRestitution(0.0f); // 设置反弹系数为0
    physicsBody->addShape(polygonShape9);


    Vec2 polygonPoints10[] = {
Vec2(38, 589),
Vec2(65, 589),
Vec2(65, 696),
Vec2(38, 696)
    };

    // 创建多边形形状
    auto polygonShape10 = PhysicsShapePolygon::create(polygonPoints10, sizeof(polygonPoints10) / sizeof(polygonPoints10[0])); // 使用顶点数组创建形状
    polygonShape10->setRestitution(0.0f); // 设置反弹系数为0
    physicsBody->addShape(polygonShape10);


    Vec2 polygonPoints11[] = {
   Vec2(352, 401),
Vec2(624, 401),
Vec2(624, 497),
Vec2(352, 497)

    };

    // 创建多边形形状
    auto polygonShape11 = PhysicsShapePolygon::create(polygonPoints11, sizeof(polygonPoints11) / sizeof(polygonPoints11[0]));
    polygonShape11->setRestitution(0.0f);
    physicsBody->addShape(polygonShape11);


    Vec2 polygonPoints12[] = {
   Vec2(515, 214),
Vec2(628, 214),
Vec2(628, 732),
Vec2(515, 732)

    };

    // 创建多边形形状
    auto polygonShape12 = PhysicsShapePolygon::create(polygonPoints12, sizeof(polygonPoints12) / sizeof(polygonPoints12[0]));
    polygonShape12->setRestitution(0.0f);
    physicsBody->addShape(polygonShape12);


    Vec2 polygonPoints13[] = {
  Vec2(479, 487),
Vec2(629, 487),
Vec2(629, 655),
Vec2(479, 655)

    };

    // 创建多边形形状
    auto polygonShape13 = PhysicsShapePolygon::create(polygonPoints13, sizeof(polygonPoints13) / sizeof(polygonPoints13[0]));
    polygonShape13->setRestitution(0.0f);
    physicsBody->addShape(polygonShape13);


    Vec2 polygonPoints14[] = {
    Vec2(68, 688),
Vec2(345, 688),
Vec2(345, 715),
Vec2(68, 715)

    };

    // 创建多边形形状
    auto polygonShape14 = PhysicsShapePolygon::create(polygonPoints14, sizeof(polygonPoints14) / sizeof(polygonPoints14[0]));
    polygonShape14->setRestitution(0.0f);
    physicsBody->addShape(polygonShape14);

    Vec2 polygonPoints15[] = {
   Vec2(445, 562),
Vec2(479, 562),
Vec2(479, 596),
Vec2(445, 596)

    };

    // 创建多边形形状
    auto polygonShape15 = PhysicsShapePolygon::create(polygonPoints15, sizeof(polygonPoints15) / sizeof(polygonPoints15[0]));
    polygonShape15->setRestitution(0.0f);
    physicsBody->addShape(polygonShape15);

    Vec2 polygonPoints16[] = {
       Vec2(540, 141),
Vec2(618, 141),
Vec2(618, 202),
Vec2(540, 202)

    };

    // 创建多边形形状
    auto polygonShape16 = PhysicsShapePolygon::create(polygonPoints16, sizeof(polygonPoints16) / sizeof(polygonPoints16[0]));
    polygonShape16->setRestitution(0.0f);
    physicsBody->addShape(polygonShape16);

    //    // 多边形17的顶点
    Vec2 polygonPoints17[] = {
     Vec2(580, 110),
Vec2(618, 110),
Vec2(618, 142),
Vec2(580, 142)


    };

    // 创建多边形形状
    auto polygonShape17 = PhysicsShapePolygon::create(polygonPoints17, sizeof(polygonPoints17) / sizeof(polygonPoints17[0]));
    polygonShape17->setRestitution(0.0f);
    physicsBody->addShape(polygonShape17);
    //
    //    //// 多边形18的顶点
    Vec2 polygonPoints18[] = {
Vec2(608, 6),
Vec2(630, 6),
Vec2(630, 108),
Vec2(608, 108)


    };

    // 创建多边形形状
    auto polygonShape18 = PhysicsShapePolygon::create(polygonPoints18, sizeof(polygonPoints18) / sizeof(polygonPoints18[0]));
    polygonShape18->setRestitution(0.0f);
    physicsBody->addShape(polygonShape18);
    //
    //    //// 多边形19的顶点
    Vec2 polygonPoints19[] = {
Vec2(56, 5),
Vec2(188, 5),
Vec2(188, 198),
Vec2(56, 198)



    };

    // 创建多边形形状
    auto polygonShape19 = PhysicsShapePolygon::create(polygonPoints19, sizeof(polygonPoints19) / sizeof(polygonPoints19[0]));
    polygonShape19->setRestitution(0.0f);
    physicsBody->addShape(polygonShape19);

    //    //// 多边形20的顶点
    Vec2 polygonPoints20[] = {
     Vec2(183, 4),
Vec2(283, 4),
Vec2(283, 78),
Vec2(183, 78)


    };






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
void Level3Scene::onEnter() {
    cocos2d::Layer::onEnter();

    // Play background music
    _backgroundMusicID = cocos2d::AudioEngine::play2d("music/gameplayBGM.mp3", true, 1.0f);  // Remember to replace the path with your actual music file path
}

void Level3Scene::onExit() {
    // Stop background music
    if (cocos2d::AudioEngine::getState(_backgroundMusicID) == cocos2d::AudioEngine::AudioState::PLAYING) {
        cocos2d::AudioEngine::stop(_backgroundMusicID);
    }

    cocos2d::Layer::onExit();
}
//
void Level3Scene::startGame() {};
void Level3Scene::endGame() {};
void Level3Scene::pauseGame() {};
//
bool Level3Scene::checkForLevelTransition() {
    // 设置射线的起始点和终点
    Vec2 rayStart = Vec2(1080, 730);
    Vec2 rayEnd = Vec2(1080, 710); // 这里需要你设置好转换点
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

void Level3Scene::update(float dt) {
    if (checkForLevelTransition()) {
        // 切换到 Level3Scene，不使用渐变过渡
        auto scene = Level4Scene::createScene();
        Director::getInstance()->replaceScene(scene);
    }
}
void Level3Scene::initKeyboardListener() {
    EventListenerKeyboard* listenerkeyPad = EventListenerKeyboard::create();
    listenerkeyPad->onKeyReleased = CC_CALLBACK_2(Level3Scene::onKeyPressedL3, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);
}
void Level3Scene::onKeyPressedL3(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event) {
    if (keycode == EventKeyboard::KeyCode::KEY_ESCAPE) {
        //ESC键
        auto pauseLayer = PauseMenu::create();
        Director::getInstance()->getRunningScene()->pause();
        Director::getInstance()->pushScene(pauseLayer);
    }
}