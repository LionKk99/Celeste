// Brick.cpp
#include "Brick.h"
#include "cocos2d.h"

USING_NS_CC;

Brick* Brick::create(const Vec2& position) {
    Brick* pRet = new(std::nothrow) Brick();
    if (pRet && pRet->init()) {
        pRet->autorelease();
        pRet->setPosition(position); // ���ô����λ��
        pRet->_isNormal = true; // ��ʼʱ��brick��������״̬
        // ������ʼ������
        return pRet;
    }
    else {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool Brick::init() {
    if (!Trap::init()) {
        return false;
    }
    // ��������Brick�ĳ�ʼ����������������Ĭ����ͼ�������������Ե�
    // ��������
    this->setTexture("trap/notsafe_00.png");
    CCASSERT(this->getTexture() != nullptr, "brick Texture not loaded!");
    this->setScale(0.8);
    this->setName("brick");
    //����������
    auto physicsBody = PhysicsBody::createBox(Size(120, 35));
    physicsBody->setDynamic(false);
    // ... ����������������
    this->setPhysicsBody(physicsBody);
    return true;
}

void Brick::toggleVisibility() {
    if (_isNormal) {
        // �����ǰ������״̬������2���������ײ���������
        auto delay = DelayTime::create(2.0f);
        auto hideBrick = CallFunc::create([this]() {
            this->setVisible(false);
            if (this->getPhysicsBody()) {
                this->getPhysicsBody()->removeFromWorld();
            }
            _isNormal = false;
        });

        auto resetDelay = DelayTime::create(2.0f);  // �ȴ�2��������ש��
        auto resetBrick = CallFunc::create([this]() {
            this->resetBrick();  // ����ש��Ϊ�ɼ�����������
        });

        // ���ж�������
        this->runAction(Sequence::create(delay, hideBrick, resetDelay, resetBrick, nullptr));
    }
}

void Brick::resetBrick() {
    // ��������brickΪ�ɼ������ָ�����������
    this->setVisible(true);
    if (this->getPhysicsBody()) {
        // ������һ�ַ�ʽ����������������ָ�����״̬
        // ������Ҫ���沢�ָ�ԭʼ����������
        auto physicsBody = PhysicsBody::createBox(Size(120, 35));
        physicsBody->setDynamic(false);
        // ... ����������������
        this->setPhysicsBody(physicsBody);
    }
    _isNormal = true;
}
