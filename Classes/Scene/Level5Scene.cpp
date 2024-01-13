#include "cocos2d.h"
using namespace cocos2d;
#include "Level5Scene.h"
#include "Player/Player.h"
#include "Level1Scene.h"
#include "Player/Player.h"
#include "Trap/Spikeweed.h"
#include"Trap/jumpTable.h"
#include"Trap/brick.h"
#include"Trap/ice.h"
#include"Level6Scene.h"
#include "PauseMenu.h"

cocos2d::Scene* Level5Scene::createScene() {
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vec2(0, -1200));
    scene->getPhysicsWorld()->setSubsteps(60);

    auto layer = Level5Scene::create();
    scene->addChild(layer);

    return scene;
}

bool Level5Scene::init() {
    CCLOG("Starting Level5Scene::init");
    this->scheduleUpdate();
    initKeyboardListener();
    if (!Layer::init()) {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto player = Player::create(5,"movement/idle/Idle_00/Idle_00-0.png");
    player->setPosition(Vec2(200, 100));
    this->addChild(player, 1);
    player->getPhysicsBody()->getFirstShape()->setFriction(0.5f);
    loadLevel();

    CCLOG("Finished Level5Scene::init");
    return true;
}

void Level5Scene::loadLevel() {
    CCLOG("Starting Level5Scene::loadLevel");

    auto background = Sprite::create("level/level/L2/level2_B.png");
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

    //����spikeweed����
    auto spikeweed1 = Spikeweed::create(Vec2(785, 85), Size(90, 10));
    this->addChild(spikeweed1);
    //����spikeweed����
    auto spikeweed2 = Spikeweed::create(Vec2(880, 180), Size(95, 10));
    this->addChild(spikeweed2);
    //����spikeweed����
    auto spikeweed3 = Spikeweed::create(Vec2(1010, 150), Size(160, 10));
    this->addChild(spikeweed3);
    //����spikeweed����
    auto spikeweed4 = Spikeweed::create(Vec2(1150, 85), Size(120, 10));
    this->addChild(spikeweed4);
    //����spikeweed����
    auto spikeweed5 = Spikeweed::create(Vec2(1230, 117), Size(30, 10));
    this->addChild(spikeweed5);


    auto physicsBody = PhysicsBody::create();


    Vec2 polygonPoints1[] = {
Vec2(-350, 398),
Vec2(-256, 398),
Vec2(-256, 463),
Vec2(-350, 463)


    };

    // �����������״
    auto polygonShape1 = PhysicsShapePolygon::create(polygonPoints1, sizeof(polygonPoints1) / sizeof(polygonPoints1[0])); // ʹ�ö������鴴����״
    polygonShape1->setRestitution(0.0f); // ���÷���ϵ��Ϊ0
    physicsBody->addShape(polygonShape1); // ����״��ӵ�������

    Vec2 polygonPoints2[] = {
Vec2(-283, 300),
Vec2(-256, 300),
Vec2(-256, 395),
Vec2(-283, 395)




    };

    // �����������״
    auto polygonShape2 = PhysicsShapePolygon::create(polygonPoints2, sizeof(polygonPoints2) / sizeof(polygonPoints2[0])); // ʹ�ö������鴴����״
    polygonShape2->setRestitution(0.0f); // ���÷���ϵ��Ϊ0
    physicsBody->addShape(polygonShape2); // ����״��ӵ�������
    ////
    Vec2 polygonPoints3[] = {
Vec2(-646, 4),
Vec2(-350, 4),
Vec2(-350, 48),
Vec2(-646, 48)



    };
    // �����������״
    auto polygonShape3 = PhysicsShapePolygon::create(polygonPoints3, sizeof(polygonPoints3) / sizeof(polygonPoints3[0])); // ʹ�ö������鴴����״
    polygonShape3->setRestitution(0.0f); // ���÷���ϵ��Ϊ0
    physicsBody->addShape(polygonShape3);
    //
    Vec2 polygonPoints4[] = {
Vec2(-646, 47),
Vec2(-578, 47),
Vec2(-578, 108),
Vec2(-646, 108)


    };

    // �����������״
    auto polygonShape4 = PhysicsShapePolygon::create(polygonPoints4, sizeof(polygonPoints4) / sizeof(polygonPoints4[0]));
    polygonShape4->setRestitution(0.0f); // ���÷���ϵ��Ϊ0
    physicsBody->addShape(polygonShape4); // ����״��ӵ�������
    ////
    Vec2 polygonPoints5[] = {
Vec2(-646, 109),
Vec2(-610, 109),
Vec2(-610, 686),
Vec2(-646, 686)

    };

    // �����������״
    auto polygonShape5 = PhysicsShapePolygon::create(polygonPoints5, sizeof(polygonPoints5) / sizeof(polygonPoints5[0])); // ʹ�ö������鴴����״
    polygonShape5->setRestitution(0.0f); // ���÷���ϵ��Ϊ0
    physicsBody->addShape(polygonShape5);
    // 
    //// 
    Vec2 polygonPoints6[] = {
Vec2(95, 4),
Vec2(635, 4),
Vec2(635, 78),
Vec2(95, 78)


    };

    // �����������״
    auto polygonShape6 = PhysicsShapePolygon::create(polygonPoints6, sizeof(polygonPoints6) / sizeof(polygonPoints5[0])); // ʹ�ö������鴴����״
    polygonShape6->setRestitution(0.0f); // ���÷���ϵ��Ϊ0
    physicsBody->addShape(polygonShape6);
    //
    Vec2 polygonPoints7[] = {
Vec2(190, 79),
Vec2(450, 79),
Vec2(450, 142),
Vec2(190, 142)

    };

    // �����������״
    auto polygonShape7 = PhysicsShapePolygon::create(polygonPoints7, sizeof(polygonPoints7) / sizeof(polygonPoints7[0])); // ʹ�ö������鴴����״
    polygonShape7->setRestitution(0.0f); // ���÷���ϵ��Ϊ0
    physicsBody->addShape(polygonShape7);
    ////
    Vec2 polygonPoints8[] = {
Vec2(190, 142),
Vec2(288, 142),
Vec2(288, 175),
Vec2(190, 175)

    };

    // �����������״
    auto polygonShape8 = PhysicsShapePolygon::create(polygonPoints8, sizeof(polygonPoints8) / sizeof(polygonPoints8[0])); // ʹ�ö������鴴����״
    polygonShape8->setRestitution(0.0f); // ���÷���ϵ��Ϊ0
    physicsBody->addShape(polygonShape8);
    ////
    Vec2 polygonPoints9[] = {
Vec2(-127, 178),
Vec2(0, 178),
Vec2(0, 270),
Vec2(-127, 270)




    };

    // �����������״
    auto polygonShape9 = PhysicsShapePolygon::create(polygonPoints9, sizeof(polygonPoints9) / sizeof(polygonPoints9[0])); // ʹ�ö������鴴����״
    polygonShape9->setRestitution(0.0f); // ���÷���ϵ��Ϊ0
    physicsBody->addShape(polygonShape9);
    //
    ////
    Vec2 polygonPoints10[] = {
Vec2(-64, 113),
Vec2(0, 113),
Vec2(0, 178),
Vec2(-64, 178)


    };

    // �����������״
    auto polygonShape10 = PhysicsShapePolygon::create(polygonPoints10, sizeof(polygonPoints10) / sizeof(polygonPoints10[0])); // ʹ�ö������鴴����״
    polygonShape10->setRestitution(0.0f); // ���÷���ϵ��Ϊ0
    physicsBody->addShape(polygonShape10);
    //

    Vec2 polygonPoints11[] = {
Vec2(-32, 80),
Vec2(0, 80),
Vec2(0, 113),
Vec2(-32, 113)



    };

    // �����������״
    auto polygonShape11 = PhysicsShapePolygon::create(polygonPoints11, sizeof(polygonPoints11) / sizeof(polygonPoints11[0]));
    polygonShape11->setRestitution(0.0f);
    physicsBody->addShape(polygonShape11);

    ////
    Vec2 polygonPoints12[] = {
Vec2(-127, 261),
Vec2(-98, 261),
Vec2(-98, 333),
Vec2(-127, 333)


    };

    // �����������״
    auto polygonShape12 = PhysicsShapePolygon::create(polygonPoints12, sizeof(polygonPoints12) / sizeof(polygonPoints12[0]));
    polygonShape12->setRestitution(0.0f);
    physicsBody->addShape(polygonShape12);
    //
    ////
    Vec2 polygonPoints13[] = {
  Vec2(350, 367),
Vec2(450, 367),
Vec2(450, 465),
Vec2(350, 465)



    };

    // �����������״
    auto polygonShape13 = PhysicsShapePolygon::create(polygonPoints13, sizeof(polygonPoints13) / sizeof(polygonPoints13[0]));
    polygonShape13->setRestitution(0.0f);
    physicsBody->addShape(polygonShape13);
    //
    ////
    Vec2 polygonPoints14[] = {
Vec2(350, 275),
Vec2(418, 275),
Vec2(418, 367),
Vec2(350, 367)



    };

    // �����������״
    auto polygonShape14 = PhysicsShapePolygon::create(polygonPoints14, sizeof(polygonPoints14) / sizeof(polygonPoints14[0]));
    polygonShape14->setRestitution(0.0f);
    physicsBody->addShape(polygonShape14);







    // �����17�Ķ���
    Vec2 polygonPoints17[] = {
       Vec2(385, 240),
Vec2(418, 240),
Vec2(418, 275),
Vec2(385, 275)

    };

    // �����������״
    auto polygonShape17 = PhysicsShapePolygon::create(polygonPoints17, sizeof(polygonPoints17) / sizeof(polygonPoints17[0]));
    polygonShape17->setRestitution(0.0f);
    physicsBody->addShape(polygonShape17);

    //// �����18�Ķ���
    Vec2 polygonPoints18[] = {
     Vec2(190, 555),
Vec2(220, 555),
Vec2(220, 687),
Vec2(190, 687)

    };

    // �����������״
    auto polygonShape18 = PhysicsShapePolygon::create(polygonPoints18, sizeof(polygonPoints18) / sizeof(polygonPoints18[0]));
    polygonShape18->setRestitution(0.0f);
    physicsBody->addShape(polygonShape18);

    //// �����19�Ķ���
    Vec2 polygonPoints19[] = {
   Vec2(211, 658),
Vec2(418, 658),
Vec2(418, 730),
Vec2(211, 730)


    };

    // �����������״
    auto polygonShape19 = PhysicsShapePolygon::create(polygonPoints19, sizeof(polygonPoints19) / sizeof(polygonPoints19[0]));
    polygonShape19->setRestitution(0.0f);
    physicsBody->addShape(polygonShape19);

    //// �����20�Ķ���
    Vec2 polygonPoints20[] = {
        Vec2(610, 77),
Vec2(624, 77),
Vec2(624, 730),
Vec2(610, 730)

    };

    // �����������״
    auto polygonShape20 = PhysicsShapePolygon::create(polygonPoints20, sizeof(polygonPoints20) / sizeof(polygonPoints20[0]));
    polygonShape20->setRestitution(0.0f);
    physicsBody->addShape(polygonShape20);

    //// �����21�Ķ���
    Vec2 polygonPoints21[] = {
       Vec2(573, 335),
Vec2(587, 335),
Vec2(587, 730),
Vec2(573, 730)

    };

    // �����������״
    auto polygonShape21 = PhysicsShapePolygon::create(polygonPoints21, sizeof(polygonPoints21) / sizeof(polygonPoints21[0]));
    polygonShape21->setRestitution(0.0f);
    physicsBody->addShape(polygonShape21);

    //// �����22�Ķ���
    Vec2 polygonPoints22[] = {
  Vec2(546, 497),
Vec2(574, 497),
Vec2(574, 730),
Vec2(546, 730)


    };

    // �����������״
    auto polygonShape22 = PhysicsShapePolygon::create(polygonPoints22, sizeof(polygonPoints22) / sizeof(polygonPoints22[0]));
    polygonShape22->setRestitution(0.0f);
    physicsBody->addShape(polygonShape22);


    // �����23�Ķ���
    Vec2 polygonPoints23[] = {
        Vec2(353, 622),
Vec2(418, 622),
Vec2(418, 652),
Vec2(353, 652)

    };

    // �����������״
    auto polygonShape23 = PhysicsShapePolygon::create(polygonPoints23, sizeof(polygonPoints23) / sizeof(polygonPoints23[0]));
    polygonShape23->setRestitution(0.0f);
    physicsBody->addShape(polygonShape23);

    //// �����24�Ķ���
    Vec2 polygonPoints24[] = {
        Vec2(383, 595),
Vec2(418, 595),
Vec2(418, 630),
Vec2(383, 630)

    };

    // �����������״
    auto polygonShape24 = PhysicsShapePolygon::create(polygonPoints24, sizeof(polygonPoints24) / sizeof(polygonPoints24[0]));
    polygonShape24->setRestitution(0.0f);
    physicsBody->addShape(polygonShape24);

    //// �����25�Ķ���
    Vec2 polygonPoints25[] = {
        Vec2(578, 79),
Vec2(605, 79),
Vec2(605, 110),
Vec2(578, 110)

    };

    // �����������״
    auto polygonShape25 = PhysicsShapePolygon::create(polygonPoints25, sizeof(polygonPoints25) / sizeof(polygonPoints25[0]));
    polygonShape25->setRestitution(0.0f);
    physicsBody->addShape(polygonShape25);

    //// �����26�Ķ���
    Vec2 polygonPoints26[] = {
        Vec2(-157, 589),
Vec2(-98, 589),
Vec2(-98, 689),
Vec2(-157, 689)

    };

    // �����������״
    auto polygonShape26 = PhysicsShapePolygon::create(polygonPoints26, sizeof(polygonPoints26) / sizeof(polygonPoints26[0]));
    polygonShape26->setRestitution(0.0f);
    physicsBody->addShape(polygonShape26);

   


    physicsBody->setCategoryBitmask(0x01);
    physicsBody->setCollisionBitmask(0x02);
    physicsBody->setContactTestBitmask(0xFFFFFFFF);
    physicsBody->setDynamic(false);
    platform->setPhysicsBody(physicsBody);
    platform->getPhysicsBody()->getFirstShape()->setFriction(0.5f);
    this->addChild(platform);

    CCLOG("Finished Level5Scene::loadLevel");
}
void Level5Scene::onEnter() {
    cocos2d::Layer::onEnter();

    // Play background music
    _backgroundMusicID = cocos2d::AudioEngine::play2d("music/gameplayBGM.mp3", true, 1.0f);  // Remember to replace the path with your actual music file path
}

void Level5Scene::onExit() {
    // Stop background music
    if (cocos2d::AudioEngine::getState(_backgroundMusicID) == cocos2d::AudioEngine::AudioState::PLAYING) {
        cocos2d::AudioEngine::stop(_backgroundMusicID);
    }

    cocos2d::Layer::onExit();
}
//
void Level5Scene::startGame() {};
void Level5Scene::endGame() {};
void Level5Scene::pauseGame() {};
//
void Level5Scene::initKeyboardListener() {
    EventListenerKeyboard* listenerkeyPad = EventListenerKeyboard::create();
    listenerkeyPad->onKeyReleased = CC_CALLBACK_2(Level5Scene::onKeyPressedL5, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);
}
void Level5Scene::onKeyPressedL5(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event) {
    if (keycode == EventKeyboard::KeyCode::KEY_ESCAPE) {
        //ESC��
        auto pauseLayer = PauseMenu::create();
        Director::getInstance()->getRunningScene()->pause();
        Director::getInstance()->pushScene(pauseLayer);
    }
}
void Level5Scene::update(float dt) {
    if (checkForLevelTransition()) {
        // �л��� Level6Scene����ʹ�ý������
        auto scene = Level6Scene::createScene();
        Director::getInstance()->replaceScene(scene);
    }
}
bool Level5Scene::checkForLevelTransition() {
    // �������ߵ���ʼ����յ�
    Vec2 rayStart = Vec2(1150, 730);
    Vec2 rayEnd = Vec2(1150, 650); // ������Ҫ�����ú�ת����
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