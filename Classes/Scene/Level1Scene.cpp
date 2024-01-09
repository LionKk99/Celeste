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
    auto scene = Scene::createWithPhysics();  // ����һ��������������ĳ���
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    scene->getPhysicsWorld()->setGravity(Vec2(0, -1200));//��������
    scene->getPhysicsWorld()->setSubsteps(60);  // ���ӵ�������

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
    CCLOG("Starting Level1Scene::init");  // �����һ��
    if (!Layer::init()) {
        return false;
    }
    initKeyboardListener();
    
    // ��ȡ�����Ĵ�С
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    //��ʼ�������������
    Vec2 checkpoint = Vec2(0,0);//����������λ��
    Vec2 respawnPoint = Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y); // ��������λ�õ���Ļ����

    //�������ұ߽�
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

    
    // ����һ������߽��Χ�У��߽磩
    auto edgeBody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 1, Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    edgeBody->setDynamic(false); // ȷ���߽粻���ƶ�
    edgeBody->setName("edge");
    // ����һ���ڵ㲢Ϊ������������
    auto edgeNode = Node::create();
    edgeNode->setPosition(origin);
    edgeNode->setPhysicsBody(edgeBody);

    // ��ӽڵ㵽����
    this->addChild(edgeNode);


    // ����Player����
    auto player = Player::create(1,"movement/idle/Idle_00/Idle_00-0.png"); // ��ֻ��һ��ʾ������������ҪΪ���ṩһ����ʼ�ľ���ͼƬ·��
    player->setPosition(respawnPoint); // ����λ�õ���Ļ����
    this->addChild(player); // �������ӵ�������
    player->getPhysicsBody()->getFirstShape()->setFriction(0.5f);

    //����spikeweed����
    auto spikeweed = Spikeweed::create(Vec2(400, 120), Size(100, 10));
    this->addChild(spikeweed);

    //����jumpTable
  /*
    // ��ȡ��һ֡��SpriteFrame
    auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("JumpTable_00-0.png");

    // ����Ƿ�ɹ���ȡ��
    if (frame) {
        // ����JumpTable������Ϊ��ȡ����֡
        
        //�����Լ����÷ָ�����������λ��
        
        auto jumpTableSprite = JumpTable::create(Vec2(700, 140)); // ������������һ������������JumpTableʵ��
        jumpTableSprite->setSpriteFrame(frame); // ʹ�õ�һ֡��Ϊ����            
        auto physicsBody = PhysicsBody::createBox(Size(100,10));// ����������״
        physicsBody->setPositionOffset(Vec2(0, -100));// �����������λ��ƫ��
        physicsBody->setDynamic(false);

        jumpTableSprite->setPhysicsBody(physicsBody);

        this->addChild(jumpTableSprite);
    }
    else {
        // ���û���ҵ�֡��������Ҫ���������ȡ�����ж�
        CCLOG("Error: Cannot find the first frame of JumpTable in SpriteFrameCache");
    }*/
    //����brick
    auto brick = Brick::create(Vec2(100, 200));
    this->addChild(brick); // ��brick��ӵ�����
    /*
    //��ײ������   
    auto contactListener = cocos2d::EventListenerPhysicsContact::create();
    contactListener->onContactBegin = [this](cocos2d::PhysicsContact& contact) {
        auto nodeA = contact.getShapeA()->getBody()->getNode();
        auto nodeB = contact.getShapeB()->getBody()->getNode();

        if ((nodeA && nodeA->getName() == "player" && nodeB && nodeB->getName() == "ground") ||
            (nodeB && nodeB->getName() == "player" && nodeA && nodeA->getName() == "ground")) {
            // ��������ڵ�����
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
            // ������Ҳ��ڵ�����
            auto player = dynamic_cast<Player*>(nodeA->getName() == "player" ? nodeA : nodeB);
            if (player) {
                player->setOnGround(false);
                CCLOG("NO Landed");
            }
        }
        };

    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
    */
    //����ice
    auto ice = Ice::create(Vec2(700, 400)); // ����λ��
    this->addChild(ice); // ��ӵ��������������ڵ���

    // ������һ�ص��ض����ݣ����米�����ϰ����
    loadLevel();
    CCLOG("Finished Level1Scene::init");  // �����һ��
    return true;
}

void Level1Scene::loadLevel() {
    // Level 1 specific content loading
    // For example: setting up the player, platforms, enemies, background, etc.
    // This should include setting up the physics bodies, sprites, and any game logic.


    CCLOG("Starting Level1Scene::loadLevel");  // �����һ��
    // ��ӱ���
    auto background = Sprite::create("level/xumu/L0/xumu0_B.png");
    background->setAnchorPoint(Vec2(0.5, 0.5));
    background->setScale(0.8, 0.8);
    background->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2));
    this->addChild(background, -1);

    // ���ƽ̨
    auto platform = Sprite::create(); // ���ﴴ������һ��͸����ƽ̨��û������
    platform->setName("ground");
    platform->setAnchorPoint(Vec2(0.5, 0.5));
    platform->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, platform->getContentSize().height / 2));


    // ����������
    auto physicsBody = PhysicsBody::create();

    // ��һ�����Σ��·���
    //����λ��
    Vec2 rectangle1[] = {
        Vec2(-540 * 1.25, -100 * 1.25),
        Vec2(540 * 1.25, -100 * 1.25),
        Vec2(540 * 1.25, 90 * 1.25),
        Vec2(-540 * 1.25, 90 * 1.25)
    };
    //����λ��
    /*
    Vec2 rectangle1[] = {
        Vec2(-200 * 1.25, -100 * 1.25),
        Vec2(540 * 1.25, -100 * 1.25),
        Vec2(540 * 1.25, 90 * 1.25),
        Vec2(-200 * 1.25, 90 * 1.25)
    };*/
    auto shape1 = PhysicsShapePolygon::create(rectangle1, 4);
    shape1->setRestitution(0.0f);  // ���÷���ϵ��Ϊ0
    physicsBody->addShape(shape1);
    // �ڶ������Σ��м䣩
    Vec2 rectangle2[] = {
        Vec2(380 * 1.25, 118 * 1.25),
        Vec2(380 * 1.25, 90 * 1.25),
        Vec2(540 * 1.25, 90 * 1.25),
        Vec2(540 * 1.25, 118 * 1.25)
    };
    auto shape2 = PhysicsShapePolygon::create(rectangle2, 4);
    shape2->setRestitution(0.0f);  // ���÷���ϵ��Ϊ0
    physicsBody->addShape(shape2);
    // ���������Σ��Ϸ���
    Vec2 rectangle3[] = {
        Vec2(540 * 1.25, 140 * 1.25),
        Vec2(540 * 1.25, 118 * 1.25),
        Vec2(405 * 1.25, 118 * 1.25),
        Vec2(405 * 1.25, 140 * 1.25)

    };
    auto shape3 = PhysicsShapePolygon::create(rectangle3, 4);
    shape3->setRestitution(0.0f);  // ���÷���ϵ��Ϊ0
    physicsBody->addShape(PhysicsShapePolygon::create(rectangle3, 4));
    physicsBody->setCategoryBitmask(0x01);
    physicsBody->setCollisionBitmask(0x02);  // ���������Ϊ0x02�����巢����ײ
    physicsBody->setContactTestBitmask(0xFFFFFFFF);
    // ������������Ϊ��̬����ӵ�ƽ̨
    physicsBody->setDynamic(false);
    platform->setPhysicsBody(physicsBody);
    platform->getPhysicsBody()->getFirstShape()->setFriction(0.5f);//Ħ��ϵ��
    this->addChild(platform);
    CCLOG("Finished Level1Scene::loadLevel");  // �����һ��
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
        // �л��� Level2Scene����ʹ�ý������
        auto scene = Level2Scene::createScene();
        Director::getInstance()->replaceScene(scene);
    }
}
bool Level1Scene::checkForLevelTransition() {
    // �������ߵ���ʼ����յ�
    Vec2 rayStart = Vec2(1280, 300);
    Vec2 rayEnd = Vec2(1150, 300); // ������Ҫ�����ú�ת����
    bool playerDetected = false;  // ���ڼ�¼�Ƿ��⵽player
    /*��debug��
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
void Level1Scene::initKeyboardListener() {
    EventListenerKeyboard* listenerkeyPad = EventListenerKeyboard::create();
    listenerkeyPad->onKeyReleased = CC_CALLBACK_2(Level1Scene::onKeyPressedL1, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);
}
void Level1Scene::onKeyPressedL1(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event) {
    if (keycode == EventKeyboard::KeyCode::KEY_ESCAPE) {
        //ESC��
        auto pauseLayer = PauseMenu::create();
        Director::getInstance()->getRunningScene()->pause();
        Director::getInstance()->pushScene(pauseLayer);
    }
}