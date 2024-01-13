#include "Ice.h"

USING_NS_CC;

Ice* Ice::create(const Vec2& position) {
    Ice* pRet = new(std::nothrow) Ice();
    if (pRet && pRet->init()) {
        pRet->autorelease();
        pRet->_initialPosition = position;  // 设置初始位置
        pRet->setPosition(position);        // 设置当前位置
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
    // 启用每帧更新
    this->scheduleUpdate();
    // 初始化Ice特有的属性，如图像、动画、物理属性等
    this->setTexture("trap/SnowBlock.png");  // 替换为实际的纹理路径
    CCASSERT(this->getTexture() != nullptr, "SnowBlock Texture not loaded!");

    this->setScale(0.76); // 设置缩放因子为0.7
    this->setName("ground");

    // 计算缩放后的物理体尺寸
    Size scaledSize = Size(this->getContentSize().width , this->getContentSize().height);

    // 设置物理体
    auto physicsBody = PhysicsBody::createBox(scaledSize);
    physicsBody->setDynamic(true); // 动态，但初始时不受重力影响
    physicsBody->setGravityEnable(false); // 初始不受重力影响
    physicsBody->setCategoryBitmask(0x04); // 冰块的类别掩码
    physicsBody->setCollisionBitmask(0x02); // 假设玩家的类别掩码是0x02
    physicsBody->setContactTestBitmask(0xFFFFFFFF); // 接触测试掩码

    // ... 其他物理属性设置
    this->setPhysicsBody(physicsBody);

    _activated = false;
    canActive = true; // 初始可以激活

    return true;
}

void Ice::activate() {
    if (!canActive) return; // 如果已经激活，直接返回

    // 播放摇晃声音
    cocos2d::AudioEngine::play2d("music/game_01_fallingblock_ice_shake_01.mp3", false);
    CCLOG("Ice shake music have been played");

    // 更改状态
    _activated = true;
    canActive = false; // 防止再次激活

    // 设置0.5秒后开始下落
    auto delayForGravity = DelayTime::create(0.7f);
    auto enableGravity = CallFunc::create([this]() {
        auto physicsBody = this->getPhysicsBody();
        if (physicsBody) {
            physicsBody->setGravityEnable(true);
            this->setName("Spikeweed");  // 修改名称以反映新状态
            // 播放冰块碰撞声音
            cocos2d::AudioEngine::play2d("music/game_01_fallingblock_ice_impact_03.mp3", false);
            CCLOG("Ice impact music have been played");
        }
        });

    // 设置2秒后更改名字为ground
    auto delayForChangeName = DelayTime::create(0.6189f);
    auto changeNameToGround = CallFunc::create([this]() {
        this->setName("ground");
        auto physicsBody = this->getPhysicsBody();
        if (physicsBody) {
            physicsBody->setGravityEnable(false);
            physicsBody->setDynamic(false);
            physicsBody->setCategoryBitmask(0x01);
            physicsBody->setCollisionBitmask(0x02); // 可以与分类为0x02的物体发生碰撞
            physicsBody->setContactTestBitmask(0xFFFFFFFF);
        }
        // 如果还需要停止下落或者其他逻辑，可以在这里添加
        });

    this->runAction(Sequence::create(delayForGravity, enableGravity, delayForChangeName, changeNameToGround, nullptr));
}

// 更新函数
void Ice::update(float dt) {
    this->checkForPlayer();
}

void Ice::checkForPlayer() {
    // 这里你需要定义射线的起始点和方向
    Vec2 rayStart = this->getPosition(); // 例如中心点
    Vec2 rayEnd = rayStart - Vec2(0, 1000); // 向下延伸，长度可以调整

    auto rayCallback = [this](PhysicsWorld& world, const PhysicsRayCastInfo& info, void* data)->bool {
        auto node = info.shape->getBody()->getNode();
        if (node && node->getName() == "player") {
            // 如果射线检测到Player，激活冰块
            this->activate();
            return false; // 停止射线检测
        }
        return true; // 继续射线检测
        };

    Director::getInstance()->getRunningScene()->getPhysicsWorld()->rayCast(rayCallback, rayStart, rayEnd, nullptr);

}

void Ice::reset() {
    // 重置基本属性
    this->setPosition(_initialPosition); // 假设有一个初始位置变量_initialPosition
    this->setName("ground");
    this->setVisible(true); // 确保可见

    // 重置纹理
    this->setTexture("trap/SnowBlock.png"); // 或任何初始纹理

    // 重置物理属性
    auto physicsBody = this->getPhysicsBody();
    if (physicsBody) {
        // 重新创建物理体来重置所有属性，或者逐个重置需要的属性
        this->removeComponent(physicsBody); // 先移除当前的物理体
        auto newPhysicsBody = PhysicsBody::createBox(this->getContentSize());
        newPhysicsBody->setDynamic(true); // 动态，但不受重力影响
        newPhysicsBody->setGravityEnable(false); // 初始不受重力影响
        newPhysicsBody->setCategoryBitmask(0x04); // 冰块的类别掩码
        newPhysicsBody->setCollisionBitmask(0x02); // 假设玩家的类别掩码是0x02
        newPhysicsBody->setContactTestBitmask(0xFFFFFFFF); // 接触测试掩码

        this->setPhysicsBody(newPhysicsBody);
    }

    // 重置状态变量
    _activated = false;
    canActive = true; // 可以重新激活
}
