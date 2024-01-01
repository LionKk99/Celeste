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
    //����������
    auto physicsBody = PhysicsBody::createBox(Size(40,5));
    physicsBody->setDynamic(false);
    // ... ����������������
    this->setPhysicsBody(physicsBody);
    return true;
}

void Brick::toggleVisibility() {
    if (_isNormal) {
        // �����ǰ������״̬����������ײ���������
        this->setVisible(false);
        if (this->getPhysicsBody()) {
            this->getPhysicsBody()->removeFromWorld();
        }
        _isNormal = false;
        // ����2���ָ�
        auto delay = DelayTime::create(2.0f);
        auto callback = CallFunc::create(CC_CALLBACK_0(Brick::resetBrick, this));
        auto sequence = Sequence::create(delay, callback, nullptr);
        this->runAction(sequence);
    }
}

void Brick::resetBrick() {
    // ��������brickΪ�ɼ������ָ�����������
    this->setVisible(true);
    if (this->getPhysicsBody()) {
        // ������һ�ַ�ʽ����������������ָ�����״̬
        // ������Ҫ���沢�ָ�ԭʼ����������
        auto physicsBody = PhysicsBody::createBox(Size(40, 5));
        physicsBody->setDynamic(false);
        // ... ����������������
        this->setPhysicsBody(physicsBody);
    }
    _isNormal = true;
}
