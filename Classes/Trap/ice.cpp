#include "Ice.h"

USING_NS_CC;

Ice* Ice::create(const Vec2& position) {
    Ice* pRet = new(std::nothrow) Ice();
    if (pRet && pRet->init()) {
        pRet->autorelease();
        pRet->_initialPosition = position;  // ���ó�ʼλ��
        pRet->setPosition(position);        // ���õ�ǰλ��
        return pRet;
    }
    else {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool Ice::init() {
    if (!Trap::init()) {
        return false;
    }
    // ����ÿ֡����
    this->scheduleUpdate();
    // ��ʼ��Ice���е����ԣ���ͼ�񡢶������������Ե�
    this->setTexture("trap/SnowBlock.png");  // �滻Ϊʵ�ʵ�����·��
    CCASSERT(this->getTexture() != nullptr, "SnowBlock Texture not loaded!");

    this->setScale(0.76); // ������������Ϊ0.7
    this->setName("ground");

    // �������ź��������ߴ�
    Size scaledSize = Size(this->getContentSize().width , this->getContentSize().height);

    // ����������
    auto physicsBody = PhysicsBody::createBox(scaledSize);
    physicsBody->setDynamic(true); // ��̬������ʼʱ��������Ӱ��
    physicsBody->setGravityEnable(false); // ��ʼ��������Ӱ��
    physicsBody->setCategoryBitmask(0x04); // ������������
    physicsBody->setCollisionBitmask(0x02); // ������ҵ����������0x02
    physicsBody->setContactTestBitmask(0xFFFFFFFF); // �Ӵ���������

    // ... ����������������
    this->setPhysicsBody(physicsBody);

    _activated = false;
    canActive = true; // ��ʼ���Լ���

    return true;
}

void Ice::activate() {
    if (!canActive) return; // ����Ѿ����ֱ�ӷ���

    // ����ҡ������
    cocos2d::AudioEngine::play2d("music/game_01_fallingblock_ice_shake_01.mp3", false);
    CCLOG("Ice shake music have been played");

    // ����״̬
    _activated = true;
    canActive = false; // ��ֹ�ٴμ���

    // ����0.5���ʼ����
    auto delayForGravity = DelayTime::create(0.7f);
    auto enableGravity = CallFunc::create([this]() {
        auto physicsBody = this->getPhysicsBody();
        if (physicsBody) {
            physicsBody->setGravityEnable(true);
            this->setName("Spikeweed");  // �޸������Է�ӳ��״̬
            // ���ű�����ײ����
            cocos2d::AudioEngine::play2d("music/game_01_fallingblock_ice_impact_03.mp3", false);
            CCLOG("Ice impact music have been played");
        }
        });

    // ����2����������Ϊground
    auto delayForChangeName = DelayTime::create(0.6189f);
    auto changeNameToGround = CallFunc::create([this]() {
        this->setName("ground");
        auto physicsBody = this->getPhysicsBody();
        if (physicsBody) {
            physicsBody->setGravityEnable(false);
            physicsBody->setDynamic(false);
            physicsBody->setCategoryBitmask(0x01);
            physicsBody->setCollisionBitmask(0x02); // ���������Ϊ0x02�����巢����ײ
            physicsBody->setContactTestBitmask(0xFFFFFFFF);
        }
        // �������Ҫֹͣ������������߼����������������
        });

    this->runAction(Sequence::create(delayForGravity, enableGravity, delayForChangeName, changeNameToGround, nullptr));
}

// ���º���
void Ice::update(float dt) {
    this->checkForPlayer();
}

void Ice::checkForPlayer() {
    // ��������Ҫ�������ߵ���ʼ��ͷ���
    Vec2 rayStart = this->getPosition(); // �������ĵ�
    Vec2 rayEnd = rayStart - Vec2(0, 1000); // �������죬���ȿ��Ե���

    auto rayCallback = [this](PhysicsWorld& world, const PhysicsRayCastInfo& info, void* data)->bool {
        auto node = info.shape->getBody()->getNode();
        if (node && node->getName() == "player") {
            // ������߼�⵽Player���������
            this->activate();
            return false; // ֹͣ���߼��
        }
        return true; // �������߼��
        };

    Director::getInstance()->getRunningScene()->getPhysicsWorld()->rayCast(rayCallback, rayStart, rayEnd, nullptr);

}

void Ice::reset() {
    // ���û�������
    this->setPosition(_initialPosition); // ������һ����ʼλ�ñ���_initialPosition
    this->setName("ground");
    this->setVisible(true); // ȷ���ɼ�

    // ��������
    this->setTexture("trap/SnowBlock.png"); // ���κγ�ʼ����

    // ������������
    auto physicsBody = this->getPhysicsBody();
    if (physicsBody) {
        // ���´����������������������ԣ��������������Ҫ������
        this->removeComponent(physicsBody); // ���Ƴ���ǰ��������
        auto newPhysicsBody = PhysicsBody::createBox(this->getContentSize());
        newPhysicsBody->setDynamic(true); // ��̬������������Ӱ��
        newPhysicsBody->setGravityEnable(false); // ��ʼ��������Ӱ��
        newPhysicsBody->setCategoryBitmask(0x04); // ������������
        newPhysicsBody->setCollisionBitmask(0x02); // ������ҵ����������0x02
        newPhysicsBody->setContactTestBitmask(0xFFFFFFFF); // �Ӵ���������

        this->setPhysicsBody(newPhysicsBody);
    }

    // ����״̬����
    _activated = false;
    canActive = true; // �������¼���
}
