#include "Spikeweed.h"

USING_NS_CC;

Spikeweed* Spikeweed::create(const Vec2& position, const Size& triggerSize) {
    Spikeweed* pRet = new(std::nothrow) Spikeweed();
    if (pRet && pRet->init(position, triggerSize)) {
        pRet->autorelease();
        return pRet;
    }
    else {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool Spikeweed::init(const Vec2& position, const Size& triggerSize) {
    if (!Trap::init()) {
        return false;
    }

    // ���������λ��
    this->setPosition(position);
    _triggerSize = triggerSize;

    // ��������Ϊδ����״̬
    _activated = false;

    // ���������������������
    auto physicsBody = PhysicsBody::createBox(_triggerSize);
    physicsBody->setDynamic(false);
    physicsBody->setCategoryBitmask(0x04);
    physicsBody->setCollisionBitmask(0x00);
    physicsBody->setContactTestBitmask(0xFFFFFFFF);
    this->setPhysicsBody(physicsBody);
    this->setName("Spikeweed");

    return true;
}

void Spikeweed::activate() {
    _activated = true;
    // ���弤��ʱ��Ч����������
}

bool Spikeweed::isActivated() const {
    return _activated;
}
