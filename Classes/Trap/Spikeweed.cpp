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

    // 设置陷阱的位置
    this->setPosition(position);
    _triggerSize = triggerSize;

    // 设置陷阱为未激活状态
    _activated = false;

    // 创建并设置陷阱的物理体
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
    // 陷阱激活时的效果、动画等
}

bool Spikeweed::isActivated() const {
    return _activated;
}
