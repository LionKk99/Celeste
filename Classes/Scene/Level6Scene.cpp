#include "cocos2d.h"
using namespace cocos2d;
#include "Level6Scene.h"
#include "Player/Player.h"
#include "Trap/Spikeweed.h"
#include"Trap/jumpTable.h"
#include"Trap/brick.h"
#include"Trap/ice.h"
#include"Level7Scene.h"
#include "PauseMenu.h"

cocos2d::Scene* Level6Scene::createScene() {
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vec2(0, -1200));
    scene->getPhysicsWorld()->setSubsteps(60);

    auto layer = Level6Scene::create();
    scene->addChild(layer);

    return scene;
}

bool Level6Scene::init() {
    CCLOG("Starting Level6Scene::init");
    this->scheduleUpdate();
    initKeyboardListener();
    if (!Layer::init()) {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto player = Player::create(6, "movement/idle/Idle_00/Idle_00-0.png");
    player->setPosition(Vec2(200, 100));
    this->addChild(player, 1);
    player->getPhysicsBody()->getFirstShape()->setFriction(0.5f);
    loadLevel();

    CCLOG("Finished Level6Scene::init");
    return true;
}

void Level6Scene::loadLevel() {
    CCLOG("Starting Level6Scene::loadLevel");

    auto background = Sprite::create("level/level/L3/level3.png");
    background->setAnchorPoint(Vec2(0.5, 0.5));
    background->setScale(0.8, 0.8);
    background->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2));
    this->addChild(background, 0);

    auto platform = Sprite::create();
    platform->setName("ground");
    platform->setAnchorPoint(Vec2(0.5, 0.5));
    platform->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, platform->getContentSize().height / 2));

    //��ӱ߽�
      // ����߽�Ķ���
    Vec2 edgeVertices[] = {
        Vec2(-680, 0),                    // ���½�
        Vec2(680, 0),    // ���½�
        Vec2(680, 800),  // ���Ͻ�
        Vec2(-680, 800)    // ���Ͻ�
    };

    // ����һ��edge��״��������
    auto edgeBody = PhysicsBody::createEdgePolygon(edgeVertices, 4);

    // ����һ���սڵ����ڸ��ӱ߽�������
    auto edgeNode = Node::create();
    edgeNode->setPosition(Vec2(1280 / 2, 0)); // ����λ��
    edgeNode->setPhysicsBody(edgeBody); // ���߽������帽�ӵ��ڵ�

    // ���߽�ڵ���ӵ�����
    this->addChild(edgeNode);


    //����brick1
    auto brick1 = Brick::create(Vec2(975, 103));
    this->addChild(brick1); // ��brick��ӵ�����
    //����brick2
    auto brick2 = Brick::create(Vec2(1137, 193));
    this->addChild(brick2); // ��brick��ӵ�����
    //����brick3
    auto brick3 = Brick::create(Vec2(945, 290));
    this->addChild(brick3); // ��brick��ӵ�����
    //����brick4
    auto brick4 = Brick::create(Vec2(1008, 452));
    this->addChild(brick4); // ��brick��ӵ�����
    
    auto physicsBody = PhysicsBody::create();
    Vec2 polygonPoints1[] = {
    Vec2(-626, 3), Vec2(-514, 3), Vec2(-514, 106), Vec2(-626, 106)


    };

    // �����������״
    auto polygonShape1 = PhysicsShapePolygon::create(polygonPoints1, sizeof(polygonPoints1) / sizeof(polygonPoints1[0])); // ʹ�ö������鴴����״
    polygonShape1->setRestitution(0.0f); // ���÷���ϵ��Ϊ0
    physicsBody->addShape(polygonShape1); // ����״��ӵ�������

    Vec2 polygonPoints2[] = {
Vec2(-503, 5), Vec2(185, 5), Vec2(185, 47), Vec2(-503, 47)




    };

    // �����������״
    auto polygonShape2 = PhysicsShapePolygon::create(polygonPoints2, sizeof(polygonPoints2) / sizeof(polygonPoints2[0])); // ʹ�ö������鴴����״
    polygonShape2->setRestitution(0.0f); // ���÷���ϵ��Ϊ0
    physicsBody->addShape(polygonShape2); // ����״��ӵ�������
    //////
    Vec2 polygonPoints3[] = {
Vec2(-190, 48), Vec2(-130, 48), Vec2(-130, 108), Vec2(-190, 108)



    };
    // �����������״
    auto polygonShape3 = PhysicsShapePolygon::create(polygonPoints3, sizeof(polygonPoints3) / sizeof(polygonPoints3[0])); // ʹ�ö������鴴����״
    polygonShape3->setRestitution(0.0f); // ���÷���ϵ��Ϊ0
    physicsBody->addShape(polygonShape3);
    ////
    Vec2 polygonPoints4[] = {
Vec2(-626, 98), Vec2(-608, 98), Vec2(-608, 686), Vec2(-626, 686)



    };

    // �����������״
    auto polygonShape4 = PhysicsShapePolygon::create(polygonPoints4, sizeof(polygonPoints4) / sizeof(polygonPoints4[0]));
    polygonShape4->setRestitution(0.0f); // ���÷���ϵ��Ϊ0
    physicsBody->addShape(polygonShape4); // ����״��ӵ�������
    //////
    Vec2 polygonPoints5[] = {
Vec2(-625, 558), Vec2(-514, 558), Vec2(-514, 686), Vec2(-625, 686)

    };

    // �����������״
    auto polygonShape5 = PhysicsShapePolygon::create(polygonPoints5, sizeof(polygonPoints5) / sizeof(polygonPoints5[0])); // ʹ�ö������鴴����״
    polygonShape5->setRestitution(0.0f); // ���÷���ϵ��Ϊ0
    physicsBody->addShape(polygonShape5);
    // 
    ////// 
    Vec2 polygonPoints6[] = {
Vec2(-503, 661), Vec2(-447, 661), Vec2(-447, 688), Vec2(-503, 688)


    };

    // �����������״
    auto polygonShape6 = PhysicsShapePolygon::create(polygonPoints6, sizeof(polygonPoints6) / sizeof(polygonPoints6[0])); // ʹ�ö������鴴����״
    polygonShape6->setRestitution(0.0f); // ���÷���ϵ��Ϊ0
    physicsBody->addShape(polygonShape6);
    ////
    Vec2 polygonPoints7[] = {
Vec2(-30, 661), Vec2(63, 661), Vec2(63, 688), Vec2(-30, 688)

    };

    // �����������״
    auto polygonShape7 = PhysicsShapePolygon::create(polygonPoints7, sizeof(polygonPoints7) / sizeof(polygonPoints7[0])); // ʹ�ö������鴴����״
    polygonShape7->setRestitution(0.0f); // ���÷���ϵ��Ϊ0
    physicsBody->addShape(polygonShape7);
    //////
    Vec2 polygonPoints8[] = {
Vec2(190, 660), Vec2(616, 660), Vec2(616, 730), Vec2(190, 730)

    };

    // �����������״
    auto polygonShape8 = PhysicsShapePolygon::create(polygonPoints8, sizeof(polygonPoints8) / sizeof(polygonPoints8[0])); // ʹ�ö������鴴����״
    polygonShape8->setRestitution(0.0f); // ���÷���ϵ��Ϊ0
    physicsBody->addShape(polygonShape8);
    //////
    Vec2 polygonPoints9[] = {
Vec2(518, 628), Vec2(617, 650), Vec2(617, 628), Vec2(518, 650)




    };

    // �����������״
    auto polygonShape9 = PhysicsShapePolygon::create(polygonPoints9, sizeof(polygonPoints9) / sizeof(polygonPoints9[0])); // ʹ�ö������鴴����״
    polygonShape9->setRestitution(0.0f); // ���÷���ϵ��Ϊ0
    physicsBody->addShape(polygonShape9);
    //
    //////
    Vec2 polygonPoints10[] = {
Vec2(608, 19), Vec2(635, 19), Vec2(635, 580), Vec2(608, 580)

    };

    // �����������״
    auto polygonShape10 = PhysicsShapePolygon::create(polygonPoints10, sizeof(polygonPoints10) / sizeof(polygonPoints10[0])); // ʹ�ö������鴴����״
    polygonShape10->setRestitution(0.0f); // ���÷���ϵ��Ϊ0
    physicsBody->addShape(polygonShape10);
    //
    ////
    Vec2 polygonPoints11[] = {
Vec2(-608, 304), Vec2(-576, 304), Vec2(-576, 558), Vec2(-608, 558)



    };

    // �����������״
    auto polygonShape11 = PhysicsShapePolygon::create(polygonPoints11, sizeof(polygonPoints11) / sizeof(polygonPoints11[0]));
    polygonShape11->setRestitution(0.0f);
    physicsBody->addShape(polygonShape11);
    ////
    //////
    Vec2 polygonPoints12[] = {
Vec2(579, 560), Vec2(637, 560), Vec2(637, 620), Vec2(579, 620)


    };

    // �����������״
    auto polygonShape12 = PhysicsShapePolygon::create(polygonPoints12, sizeof(polygonPoints12) / sizeof(polygonPoints12[0]));
    polygonShape12->setRestitution(0.0f);
    physicsBody->addShape(polygonShape12);
    ////
    //////
    Vec2 polygonPoints13[] = {
Vec2(-441, 735), Vec2(63, 735), Vec2(63, 691), Vec2(-441, 691)



    };

    // �����������״
    auto polygonShape13 = PhysicsShapePolygon::create(polygonPoints13, sizeof(polygonPoints13) / sizeof(polygonPoints13[0]));
    polygonShape13->setRestitution(0.0f);
    physicsBody->addShape(polygonShape13);
    //
    //////
    Vec2 polygonPoints14[] = {
Vec2(-64, 469), Vec2(188, 469), Vec2(188, 528), Vec2(-64, 528)




    };

    // �����������״
    auto polygonShape14 = PhysicsShapePolygon::create(polygonPoints14, sizeof(polygonPoints14) / sizeof(polygonPoints14[0]));
    polygonShape14->setRestitution(0.0f);
    physicsBody->addShape(polygonShape14);

    Vec2 polygonPoints15[] = {
    Vec2(-30, 438), Vec2(188, 438), Vec2(188, 467), Vec2(-30, 467)
    };

    // �����������״
    auto polygonShape15 = PhysicsShapePolygon::create(polygonPoints15, sizeof(polygonPoints15) / sizeof(polygonPoints15[0]));
    polygonShape15->setRestitution(0.0f);
    physicsBody->addShape(polygonShape15);


    //
    //    // �����16�Ķ���
    Vec2 polygonPoints16[] = {
      Vec2(-1, 340), Vec2(188, 340), Vec2(188, 426), Vec2(-1, 426)
    };

    // �����������״
    auto polygonShape16 = PhysicsShapePolygon::create(polygonPoints16, sizeof(polygonPoints16) / sizeof(polygonPoints16[0]));
    polygonShape16->setRestitution(0.0f);
    physicsBody->addShape(polygonShape16);

    // �����17�Ķ���
    Vec2 polygonPoints17[] = {
     Vec2(95, 240), Vec2(130, 240), Vec2(130, 560), Vec2(95, 560)
    };

    // �����������״
    auto polygonShape17 = PhysicsShapePolygon::create(polygonPoints17, sizeof(polygonPoints17) / sizeof(polygonPoints17[0]));
    polygonShape17->setRestitution(0.0f);
    physicsBody->addShape(polygonShape17);

    //    //// �����18�Ķ���
    Vec2 polygonPoints18[] = {
     Vec2(131, 174),
Vec2(159, 174),
Vec2(159, 560),
Vec2(131, 560)


    };

    // �����������״
    auto polygonShape18 = PhysicsShapePolygon::create(polygonPoints18, sizeof(polygonPoints18) / sizeof(polygonPoints18[0]));
    polygonShape18->setRestitution(0.0f);
    physicsBody->addShape(polygonShape18);
    //
    //    //// �����19�Ķ���
    Vec2 polygonPoints19[] = {
  Vec2(159, 303),
Vec2(188, 303),
Vec2(188, 560),
Vec2(159, 560)



    };

    // �����������״
    auto polygonShape19 = PhysicsShapePolygon::create(polygonPoints19, sizeof(polygonPoints19) / sizeof(polygonPoints19[0]));
    polygonShape19->setRestitution(0.0f);
    physicsBody->addShape(polygonShape19);
    //
    //    //// �����20�Ķ���
    Vec2 polygonPoints20[] = {
  Vec2(185, 365),
Vec2(211, 365),
Vec2(211, 435),
Vec2(185, 435)


    };

    // �����������״
    auto polygonShape20 = PhysicsShapePolygon::create(polygonPoints20, sizeof(polygonPoints20) / sizeof(polygonPoints20[0]));
    polygonShape20->setRestitution(0.0f);
    physicsBody->addShape(polygonShape20);
    //
    //  
   


    physicsBody->setCategoryBitmask(0x01);
    physicsBody->setCollisionBitmask(0x02);
    physicsBody->setContactTestBitmask(0xFFFFFFFF);
    physicsBody->setDynamic(false);
    platform->setPhysicsBody(physicsBody);
    platform->getPhysicsBody()->getFirstShape()->setFriction(0.5f);
    this->addChild(platform);

    CCLOG("Finished Level6Scene::loadLevel");
}

void Level6Scene::onEnter() {
    cocos2d::Layer::onEnter();

    // Play background music
    _backgroundMusicID = cocos2d::AudioEngine::play2d("music/gameplayBGM.mp3", true, 1.0f);  // Remember to replace the path with your actual music file path
}

void Level6Scene::onExit() {
    // Stop background music
    if (cocos2d::AudioEngine::getState(_backgroundMusicID) == cocos2d::AudioEngine::AudioState::PLAYING) {
        cocos2d::AudioEngine::stop(_backgroundMusicID);
    }

    cocos2d::Layer::onExit();
}
//
void Level6Scene::startGame() {};
void Level6Scene::endGame() {};
void Level6Scene::pauseGame() {};
//
void Level6Scene::initKeyboardListener() {
    EventListenerKeyboard* listenerkeyPad = EventListenerKeyboard::create();
    listenerkeyPad->onKeyReleased = CC_CALLBACK_2(Level6Scene::onKeyPressedL6, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);
}
void Level6Scene::onKeyPressedL6(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event) {
    if (keycode == EventKeyboard::KeyCode::KEY_ESCAPE) {
        //ESC��
        auto pauseLayer = PauseMenu::create();
        Director::getInstance()->getRunningScene()->pause();
        Director::getInstance()->pushScene(pauseLayer);
    }
}
void Level6Scene::update(float dt) {
    if (checkForLevelTransition()) {
        // �л��� Level7Scene����ʹ�ý������
        auto scene = Level7Scene::createScene();
        Director::getInstance()->replaceScene(scene);
    }
}
bool Level6Scene::checkForLevelTransition() {
    // �������ߵ���ʼ����յ�
    Vec2 rayStart = Vec2(800, 750);
    Vec2 rayEnd = Vec2(800, 690); // ������Ҫ�����ú�ת����
    bool playerDetected = false;  // ���ڼ�¼�Ƿ��⵽player
    /*
    // ��debug����������һ����Ա���� drawNode ָ��һ�� DrawNode ʵ��
    auto drawNode = DrawNode::create();
    this->addChild(drawNode);

    // ��������
    drawNode->clear(); // ���֮ǰ�Ļ�������
    drawNode->drawLine(rayStart, rayEnd, Color4F::BLUE); //��ɫ��ʾ����
*/
    auto rayCallback = [&playerDetected](PhysicsWorld& world, const PhysicsRayCastInfo& info, void* data)->bool {
        auto node = info.shape->getBody()->getNode();
        if (node && node->getName() == "player") {
            // ������߼�⵽Player
            playerDetected = true;  // ��¼��⵽player
            return false; // ֹͣ���߼��
        }
        return true; // �������߼��
        };

    Director::getInstance()->getRunningScene()->getPhysicsWorld()->rayCast(rayCallback, rayStart, rayEnd, nullptr);
    return playerDetected;  // �����Ƿ��⵽player
}