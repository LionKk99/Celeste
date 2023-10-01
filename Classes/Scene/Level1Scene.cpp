#include "cocos2d.h"
using namespace cocos2d;
#include "Level1Scene.h"
#include "Player/Player.h"

cocos2d::Scene* Level1Scene::createScene() {
    auto scene = Scene::createWithPhysics();  // ����һ��������������ĳ���
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);    

    scene->getPhysicsWorld()->setGravity(Vec2(0, -1200));//��������
    scene->getPhysicsWorld()->setSubsteps(60);  // ���ӵ�������

    auto layer = Level1Scene::create();
    scene->addChild(layer);

    return scene;
}


bool Level1Scene::init() {
    if (!Layer::init()) {
        return false;
    }    

    // ��ȡ�����Ĵ�С
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

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
    auto player = Player::create("movement/idle/Idle_00/Idle_00-0.png"); // ��ֻ��һ��ʾ������������ҪΪ���ṩһ����ʼ�ľ���ͼƬ·��
    player->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y)); // ����λ�õ���Ļ����
    this->addChild(player); // �������ӵ�������
    player->getPhysicsBody()->getFirstShape()->setFriction(0.5f);
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

    // ������һ�ص��ض����ݣ����米�����ϰ����
    loadLevel();

    return true;
}

void Level1Scene::loadLevel() {
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
    Vec2 rectangle1[] = {
        Vec2(-540 * 1.25, -100 * 1.25),
        Vec2(540 * 1.25, -100 * 1.25),
        Vec2(540 * 1.25, 90 * 1.25),
        Vec2(-540 * 1.25, 90 * 1.25)
    };
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

}

