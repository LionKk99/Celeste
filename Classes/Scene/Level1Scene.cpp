#include "cocos2d.h"
using namespace cocos2d;
#include "Level1Scene.h"
#include "Player/Player.h"

cocos2d::Scene* Level1Scene::createScene() {
    auto scene = Scene::createWithPhysics();  // 创建一个带有物理世界的场景
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);    

    scene->getPhysicsWorld()->setGravity(Vec2(0, -1200));//重力设置
    scene->getPhysicsWorld()->setSubsteps(60);  // 增加迭代次数

    auto layer = Level1Scene::create();
    scene->addChild(layer);

    return scene;
}


bool Level1Scene::init() {
    if (!Layer::init()) {
        return false;
    }    

    // 获取场景的大小
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 创建一个物理边界包围盒（边界）
    auto edgeBody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 1, Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    edgeBody->setDynamic(false); // 确保边界不会移动
    edgeBody->setName("edge");
    // 创建一个节点并为其设置物理体
    auto edgeNode = Node::create();
    edgeNode->setPosition(origin);
    edgeNode->setPhysicsBody(edgeBody);

    // 添加节点到场景
    this->addChild(edgeNode);

    
    // 创建Player对象
    auto player = Player::create("movement/idle/Idle_00/Idle_00-0.png"); // 这只是一个示例，您可能需要为其提供一个初始的精灵图片路径
    player->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y)); // 设置位置到屏幕中心
    this->addChild(player); // 将玩家添加到场景中
    player->getPhysicsBody()->getFirstShape()->setFriction(0.5f);
    /*
    //碰撞监听器   
    auto contactListener = cocos2d::EventListenerPhysicsContact::create();
    contactListener->onContactBegin = [this](cocos2d::PhysicsContact& contact) {
        auto nodeA = contact.getShapeA()->getBody()->getNode();
        auto nodeB = contact.getShapeB()->getBody()->getNode();

        if ((nodeA && nodeA->getName() == "player" && nodeB && nodeB->getName() == "ground") ||
            (nodeB && nodeB->getName() == "player" && nodeA && nodeA->getName() == "ground")) {
            // 设置玩家在地面上
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
            // 设置玩家不在地面上
            auto player = dynamic_cast<Player*>(nodeA->getName() == "player" ? nodeA : nodeB);
            if (player) {
                player->setOnGround(false);
                CCLOG("NO Landed");
            }
        }
        };

    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
    */

    // 加载这一关的特定内容，例如背景、障碍物等
    loadLevel();

    return true;
}

void Level1Scene::loadLevel() {
    // 添加背景
    auto background = Sprite::create("level/xumu/L0/xumu0_B.png");
    background->setAnchorPoint(Vec2(0.5, 0.5));
    background->setScale(0.8, 0.8);
    background->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2));
    this->addChild(background, -1);

    // 添加平台
    auto platform = Sprite::create(); // 这里创建的是一个透明的平台，没有纹理
    platform->setName("ground");
    platform->setAnchorPoint(Vec2(0.5, 0.5));
    platform->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, platform->getContentSize().height / 2));
    

    // 创建物理体
    auto physicsBody = PhysicsBody::create();

    // 第一个矩形（下方）
    Vec2 rectangle1[] = {
        Vec2(-540 * 1.25, -100 * 1.25),
        Vec2(540 * 1.25, -100 * 1.25),
        Vec2(540 * 1.25, 90 * 1.25),
        Vec2(-540 * 1.25, 90 * 1.25)
    };
    auto shape1 = PhysicsShapePolygon::create(rectangle1, 4);
    shape1->setRestitution(0.0f);  // 设置反弹系数为0
    physicsBody->addShape(shape1);
    // 第二个矩形（中间）
    Vec2 rectangle2[] = {
        Vec2(380 * 1.25, 118 * 1.25),
        Vec2(380 * 1.25, 90 * 1.25),
        Vec2(540 * 1.25, 90 * 1.25),
        Vec2(540 * 1.25, 118 * 1.25)
    };
    auto shape2 = PhysicsShapePolygon::create(rectangle2, 4);
    shape2->setRestitution(0.0f);  // 设置反弹系数为0
    physicsBody->addShape(shape2);
    // 第三个矩形（上方）
    Vec2 rectangle3[] = {
        Vec2(540 * 1.25, 140 * 1.25),
        Vec2(540 * 1.25, 118 * 1.25),
        Vec2(405 * 1.25, 118 * 1.25),
        Vec2(405 * 1.25, 140 * 1.25)
        
    }; 
    auto shape3 = PhysicsShapePolygon::create(rectangle3, 4);
    shape3->setRestitution(0.0f);  // 设置反弹系数为0
    physicsBody->addShape(PhysicsShapePolygon::create(rectangle3, 4));    
    physicsBody->setCategoryBitmask(0x01);
    physicsBody->setCollisionBitmask(0x02);  // 可以与分类为0x02的物体发生碰撞
    physicsBody->setContactTestBitmask(0xFFFFFFFF);
    // 将物理体设置为静态并添加到平台
    physicsBody->setDynamic(false);    
    platform->setPhysicsBody(physicsBody);     
    platform->getPhysicsBody()->getFirstShape()->setFriction(0.5f);//摩擦系数
    this->addChild(platform);

}

