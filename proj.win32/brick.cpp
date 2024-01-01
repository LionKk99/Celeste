// Brick.cpp
#include "Brick.h"
#include "cocos2d.h"

USING_NS_CC;

Brick* Brick::create(const Vec2& position) {
    Brick* pRet = new(std::nothrow) Brick();
    if (pRet && pRet->init()) {
        pRet->autorelease();
        pRet->setPosition(position); // 设置传入的位置
        pRet->_isNormal = true; // 初始时，brick处于正常状态
        // 其他初始化代码
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
    // 在这里做Brick的初始化工作，比如设置默认贴图、设置物理属性等
    // 设置纹理
    this->setTexture("trap/notsafe_00.png");
    CCASSERT(this->getTexture() != nullptr, "brick Texture not loaded!");
    //设置物理体
    auto physicsBody = PhysicsBody::createBox(Size(40,5));
    physicsBody->setDynamic(false);
    // ... 其他物理属性设置
    this->setPhysicsBody(physicsBody);
    return true;
}

void Brick::toggleVisibility() {
    if (_isNormal) {
        // 如果当前是正常状态，则隐藏碰撞体积和纹理
        this->setVisible(false);
        if (this->getPhysicsBody()) {
            this->getPhysicsBody()->removeFromWorld();
        }
        _isNormal = false;
        // 设置2秒后恢复
        auto delay = DelayTime::create(2.0f);
        auto callback = CallFunc::create(CC_CALLBACK_0(Brick::resetBrick, this));
        auto sequence = Sequence::create(delay, callback, nullptr);
        this->runAction(sequence);
    }
}

void Brick::resetBrick() {
    // 重新设置brick为可见，并恢复其物理属性
    this->setVisible(true);
    if (this->getPhysicsBody()) {
        // 假设有一种方式来重新添加物理体或恢复它的状态
        // 可能需要保存并恢复原始的物理设置
        auto physicsBody = PhysicsBody::createBox(Size(40, 5));
        physicsBody->setDynamic(false);
        // ... 其他物理属性设置
        this->setPhysicsBody(physicsBody);
    }
    _isNormal = true;
}
