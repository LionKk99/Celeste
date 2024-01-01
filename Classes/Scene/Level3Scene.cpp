#pragma once
#include "cocos2d.h"
using namespace cocos2d;
#include "Level3Scene.h"
#include "Player/Player.h"
#include "Trap/Spikeweed.h"
USING_NS_CC;

Scene* Level3Scene::createScene() {
    auto scene = Scene::createWithPhysics();  // ����һ��������������ĳ���
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    scene->getPhysicsWorld()->setGravity(Vec2(0, -1200));//��������
    scene->getPhysicsWorld()->setSubsteps(60);  // ���ӵ�������

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
    CCLOG("Starting Level3Scene::init");  // �����һ��
    if (!Layer::init()) {
        return false;
    }

    // ��ȡ�����Ĵ�С
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    //��ʼ�������������
    Vec2 checkpoint = Vec2(0, 0);//����������λ��
    Vec2 respawnPoint = Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y); // ��������λ�õ���Ļ����

    //�������ұ߽�
   // �������߽�
    Vec2 leftEdgeStart = Vec2(origin.x, origin.y); // ���½�
    Vec2 leftEdgeEnd = Vec2(origin.x, origin.y + visibleSize.height); // ���Ͻ�
    auto leftEdgeBody = PhysicsBody::createEdgeSegment(leftEdgeStart, leftEdgeEnd, PHYSICSBODY_MATERIAL_DEFAULT, 1);
    leftEdgeBody->setDynamic(false);
    leftEdgeBody->setName("edge");

    auto leftEdgeNode = Node::create();
    leftEdgeNode->setPosition(Vec2::ZERO); // ʹ��ԭ��λ��
    leftEdgeNode->setPhysicsBody(leftEdgeBody);
    this->addChild(leftEdgeNode);

    // �����Ҳ�߽�
    Vec2 rightEdgeStart = Vec2(origin.x + visibleSize.width, origin.y); // ���½� �򵥳�2
    Vec2 rightEdgeEnd = Vec2(origin.x + visibleSize.width, origin.y + visibleSize.height); // ���Ͻ�
    auto rightEdgeBody = PhysicsBody::createEdgeSegment(rightEdgeStart, rightEdgeEnd, PHYSICSBODY_MATERIAL_DEFAULT, 1);
    rightEdgeBody->setDynamic(false);
    rightEdgeBody->setName("edge");

    auto rightEdgeNode = Node::create();
    rightEdgeNode->setPosition(Vec2::ZERO); // ʹ��ԭ��λ��
    rightEdgeNode->setPhysicsBody(rightEdgeBody);
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
    auto player = Player::create("movement/idle/Idle_00/Idle_00-0.png"); 
    player->setPosition(respawnPoint); // ����λ�õ���Ļ����
    this->addChild(player); // �������ӵ�������
    player->getPhysicsBody()->getFirstShape()->setFriction(0.5f);
    m_player = player;

    //����spikeweed����
    //auto spikeweed = Spikeweed::create(Vec2(100, 300), Size(100, 10));
    //this->addChild(spikeweed);
  
    // ������һ�ص��ض����ݣ����米�����ϰ����
    loadLevel();
    CCLOG("Finished Level3Scene::init");  // �����һ��
    return true;
}

void Level3Scene::loadLevel() {
    // Level 1 specific content loading
    // For example: setting up the player, platforms, enemies, background, etc.
    // This should include setting up the physics bodies, sprites, and any game logic.


    CCLOG("Starting Level3Scene::loadLevel");  // �����һ��
    // ��ӱ���
    
    auto background1 = Sprite::create("level/level/L1/level1_B.png");
    background1->setAnchorPoint(Vec2(0.5, 0.5));
    background1->setScale(0.8, 0.8);
    background1->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2));
    this->addChild(background1, -1);
    

    // ���ƽ̨
    auto platform = Sprite::create(); // ���ﴴ������һ��͸����ƽ̨��û������
    platform->setName("ground");
    platform->setAnchorPoint(Vec2(0.5, 0.5));
    platform->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, platform->getContentSize().height / 2));


    // ����������
    auto physicsBody = PhysicsBody::create();

    // ��һ�����Σ��·���    
    Vec2 rectangle1[] = {
        Vec2(-520 * 1.25, -100 * 1.25),
        Vec2(-105 * 1.25, -100 * 1.25),
        Vec2(-105 * 1.25, 60 * 1.25),
        Vec2(-520 * 1.25, 60 * 1.25)
    };
    auto shape1 = PhysicsShapePolygon::create(rectangle1, 4);
    shape1->setRestitution(0.0f);  // ���÷���ϵ��Ϊ0
    physicsBody->addShape(shape1);
    // �ڶ������Σ�ͻ��
    Vec2 rectangle2[] = {
        Vec2(-155 * 1.25, -100 * 1.25),
        Vec2(-105 * 1.25, -100 * 1.25),
        Vec2(-105 * 1.25, 120 * 1.25),
        Vec2(-155 * 1.25, 120 * 1.25)
    };
    auto shape2 = PhysicsShapePolygon::create(rectangle2, 4);
    shape2->setRestitution(0.0f);  // ���÷���ϵ��Ϊ0
    physicsBody->addShape(shape2);
    // ���������Σ��в���
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
    CCLOG("Finished Level3Scene::loadLevel");  // �����һ��
}

void Level3Scene::update(float dt) {
    // ��ȡ���λ��
    Vec2 playerPosition = m_player->getPosition();

    // �趨��ֵ
    float xThreshold = 0;  // ���磬��Ļ��ȵ�����֮һ

    // �������Ƿ񳬹���ֵ
    if (playerPosition.x > xThreshold) {
        // ������Ҫ�ƶ��ľ���
        float offsetX = playerPosition.x - xThreshold;

        // ���²��λ��
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

