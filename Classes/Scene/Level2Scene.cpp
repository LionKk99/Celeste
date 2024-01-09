#include "cocos2d.h"
using namespace cocos2d;
#include "Level2Scene.h"
#include "Player/Player.h"
#include"Trap/ice.h"
#include "Level3Scene.h"
#include "PauseMenu.h"

cocos2d::Scene* Level2Scene::createScene() {
    auto scene = Scene::createWithPhysics();  // ����һ��������������ĳ���
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    scene->getPhysicsWorld()->setGravity(Vec2(0, -1200));//��������
    scene->getPhysicsWorld()->setSubsteps(60);  // ���ӵ�������

    auto layer = Level2Scene::create();
    scene->addChild(layer);

    return scene;
}


bool Level2Scene::init() {
    CCLOG("Starting Level2Scene::init");  // �����һ��
    if (!Layer::init()) {
        return false;
    }
    initKeyboardListener();
    this->scheduleUpdate();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // ����������
    auto background = Sprite::create("level/xumu/L1/xumu1_LG5.png");  // ��Զ�ı���
    background->setAnchorPoint(Vec2(0.5, 0.5));
    background->setScale(0.8);
    background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(background, -3);

    auto midground = Sprite::create("level/xumu/L1/xumu1_LG4.png");  // �м�㱳��
    midground->setAnchorPoint(Vec2(0.5, 0.5));
    midground->setScale(0.8);
    midground->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(midground, -2);

    //����ice
    auto ice = Ice::create(Vec2(880, 498)); // ����λ��
    this->addChild(ice); // ��ӵ��������������ڵ���

   
    // �������
    auto player = Player::create(2,"movement/idle/Idle_00/Idle_00-0.png");
    player->setPosition(Vec2(0, 260));
    this->addChild(player, 3);
    player->getPhysicsBody()->getFirstShape()->setFriction(0.5f);
    loadLevel();
    // ���ø��·��������ھ���Ч��
    CCLOG("Finished Level2Scene::init");
    return true;
}

void Level2Scene::loadLevel() {
    CCLOG("Starting Level2Scene::loadLevel");  // �����һ��

    // �������ᱳ��
    auto background = Sprite::create("level/xumu/L1/xumu1_LG2.png");
    background->setAnchorPoint(Vec2(0.5, 0.5));
    background->setScale(0.8, 0.8);
    background->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2));
    this->addChild(background, -1);  // �����ڸ��߲㼶�Ը�����������

    // ���ƽ̨
    auto platform = Sprite::create(); // ���ﴴ������һ��͸����ƽ̨��û������
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
    edgeNode->setPosition(Vec2(1280/2,0 )); // ����λ��
    edgeNode->setPhysicsBody(edgeBody); // ���߽������帽�ӵ��ڵ�

    // ���߽�ڵ���ӵ�����
    this->addChild(edgeNode);
    //// ����������
    auto physicsBody = PhysicsBody::create();

    //// �������ζ���
    //Vec2 rectangle1[] = {

    //Vec2(-640, 175),
    //Vec2(-640, 30),
    //Vec2(-220, 30),
    //Vec2(-220, 175),
    //};
    //auto shape1 = PhysicsShapePolygon::create(rectangle1, 4);
    //shape1->setRestitution(0.0f);  // ���÷���ϵ��Ϊ0
    //physicsBody->addShape(shape1);

    //Vec2 rectangle2[] = {
    //Vec2(-60, 175),
    //Vec2(-60, 240),
    //Vec2(-220, 240),
    //Vec2(-220, 175),
    //};
    //auto shape2 = PhysicsShapePolygon::create(rectangle2, 4);
    //shape2->setRestitution(0.0f);  // ���÷���ϵ��Ϊ0
    //physicsBody->addShape(shape2);
    Vec2 polygonPoints1[] = {
Vec2(-500.000, 177),
Vec2(-370, 117),
Vec2(-370, 0), 
Vec2(-648, 0),
Vec2(-646, 180)
    };

    // �����������״
    auto polygonShape1 = PhysicsShapePolygon::create(polygonPoints1, sizeof(polygonPoints1) / sizeof(polygonPoints1[0])); // ʹ�ö������鴴����״
    polygonShape1->setRestitution(0.0f); // ���÷���ϵ��Ϊ0
    physicsBody->addShape(polygonShape1); // ����״��ӵ�������

    Vec2 polygonPoints2[] = {
 Vec2(-480, 180),
 Vec2(-320, 116),
 Vec2(-320, 116), 
 Vec2(-320, 238), 
 Vec2(-480, 238)
    };

    // �����������״
    auto polygonShape2 = PhysicsShapePolygon::create(polygonPoints2, sizeof(polygonPoints2) / sizeof(polygonPoints2[0])); // ʹ�ö������鴴����״
    polygonShape2->setRestitution(0.0f); // ���÷���ϵ��Ϊ0
    physicsBody->addShape(polygonShape2); // ����״��ӵ�������

    Vec2 polygonPoints3[] = {
Vec2(-160, 500), 
Vec2(-60, 500),
Vec2(-60, 430),
Vec2(-160, 430) };
    // �����������״
    auto polygonShape3 = PhysicsShapePolygon::create(polygonPoints3, sizeof(polygonPoints3) / sizeof(polygonPoints3[0])); // ʹ�ö������鴴����״
    polygonShape3->setRestitution(0.0f); // ���÷���ϵ��Ϊ0
    physicsBody->addShape(polygonShape3);

    
    Vec2 polygonPoints4[] = {
Vec2(-155, 142), 
Vec2(-155, 0),

Vec2(40,142),
Vec2(40, 0)
    };

    // �����������״
    auto polygonShape4 = PhysicsShapePolygon::create(polygonPoints4, sizeof(polygonPoints4) / sizeof(polygonPoints4[0])); // ʹ�ö������鴴����״
    polygonShape4->setRestitution(0.0f); // ���÷���ϵ��Ϊ0
    physicsBody->addShape(polygonShape4);

    Vec2 polygonPoints5[] = {
Vec2(0, 140),
Vec2(0, 240), 
Vec2(-60, 140),
Vec2(-60, 240) };

    // �����������״
    auto polygonShape5 = PhysicsShapePolygon::create(polygonPoints5, sizeof(polygonPoints5) / sizeof(polygonPoints5[0])); // ʹ�ö������鴴����״
    polygonShape5->setRestitution(0.0f); // ���÷���ϵ��Ϊ0
    physicsBody->addShape(polygonShape5);

    Vec2 polygonPoints6[] = {
Vec2(40, 140),
Vec2(40, 170),
Vec2(0, 140),
Vec2(0, 170) };

    // �����������״
    auto polygonShape6 = PhysicsShapePolygon::create(polygonPoints6, sizeof(polygonPoints6) / sizeof(polygonPoints5[0])); // ʹ�ö������鴴����״
    polygonShape6->setRestitution(0.0f); // ���÷���ϵ��Ϊ0
    physicsBody->addShape(polygonShape6);
    Vec2 polygonPoints7[] = {
Vec2(140, 170),
Vec2(140, 200),
Vec2(350, 200),
Vec2(350,170) };

    // �����������״
    auto polygonShape7 = PhysicsShapePolygon::create(polygonPoints7, sizeof(polygonPoints7) / sizeof(polygonPoints7[0])); // ʹ�ö������鴴����״
    polygonShape7->setRestitution(0.0f); // ���÷���ϵ��Ϊ0
    physicsBody->addShape(polygonShape7);

    Vec2 polygonPoints8[] = {
Vec2(160, 160),
Vec2(160, 0),
Vec2(330, 160),
Vec2(330,0) };

    // �����������״
    auto polygonShape8 = PhysicsShapePolygon::create(polygonPoints8, sizeof(polygonPoints8) / sizeof(polygonPoints8[0])); // ʹ�ö������鴴����״
    polygonShape8->setRestitution(0.0f); // ���÷���ϵ��Ϊ0
    physicsBody->addShape(polygonShape8);


    Vec2 polygonPoints9[] = {
Vec2(800, 145),
Vec2(800, 0),
Vec2(500, 145),
Vec2(500,0) };

    // �����������״
    auto polygonShape9 = PhysicsShapePolygon::create(polygonPoints9, sizeof(polygonPoints9) / sizeof(polygonPoints9[0])); // ʹ�ö������鴴����״
    polygonShape9->setRestitution(0.0f); // ���÷���ϵ��Ϊ0
    physicsBody->addShape(polygonShape9);

    Vec2 polygonPoints10[] = {
Vec2(800, 240),
Vec2(800, 0),
Vec2(610, 240),
Vec2(610,0) };

    // �����������״
    auto polygonShape10 = PhysicsShapePolygon::create(polygonPoints10, sizeof(polygonPoints10) / sizeof(polygonPoints10[0])); // ʹ�ö������鴴����״
    polygonShape10->setRestitution(0.0f); // ���÷���ϵ��Ϊ0
    physicsBody->addShape(polygonShape10);

    Vec2 polygonPoints11[] = {
Vec2(130, 720),
Vec2(130, 560),
Vec2(350, 720),
Vec2(350,560) };

    // �����������״
    auto polygonShape11 = PhysicsShapePolygon::create(polygonPoints11, sizeof(polygonPoints11) / sizeof(polygonPoints11[0])); // ʹ�ö������鴴����״
    polygonShape11->setRestitution(0.0f); // ���÷���ϵ��Ϊ0
    physicsBody->addShape(polygonShape11);


    physicsBody->setCategoryBitmask(0x01);
    physicsBody->setCollisionBitmask(0x02);  // ���������Ϊ0x02�����巢����ײ
    physicsBody->setContactTestBitmask(0xFFFFFFFF);
    // ������������Ϊ��̬����ӵ�ƽ̨
    physicsBody->setDynamic(false);
    platform->setPhysicsBody(physicsBody);
    platform->getPhysicsBody()->getFirstShape()->setFriction(0.5f);//Ħ��ϵ��
    this->addChild(platform);
    CCLOG("Finished Level2Scene::loadLevel");  // �����һ��
    //// �����������״
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
    // �������ߵ���ʼ����յ�
    Vec2 rayStart = Vec2(1200, 280);
    Vec2 rayEnd = Vec2(1280, 280); // ������Ҫ�����ú�ת����
    bool playerDetected = false;  // ���ڼ�¼�Ƿ��⵽player
    /*(debug)
    // ��������һ����Ա���� drawNode ָ��һ�� DrawNode ʵ��
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

void Level2Scene::update(float dt) {
    if (checkForLevelTransition()) {
        // �л��� Level3Scene����ʹ�ý������
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
        //ESC��
        auto pauseLayer = PauseMenu::create();
        Director::getInstance()->getRunningScene()->pause();
        Director::getInstance()->pushScene(pauseLayer);
    }
}