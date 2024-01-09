// JumpTable.cpp
#include "cocos2d.h"
#include "JumpTable.h"

USING_NS_CC;

JumpTable* JumpTable::create(const Vec2& position) {
    JumpTable* pRet = new(std::nothrow) JumpTable();
    if (pRet && pRet->init()) {
        pRet->autorelease();
        pRet->setPosition(position);
        pRet->initDefaultTexture();
        pRet->initPhysicsBody();
        return pRet;
    }
    else {
        delete pRet;
        return nullptr;
    }
}

void JumpTable::initPhysicsBody() {
    // 假设所有动画帧都适应128x32的大小
    Size standardSize = Size(128, 32);
    this->setContentSize(standardSize); // 设置JumpTable的内容大小

    auto physicsBody = PhysicsBody::createBox(standardSize);
    physicsBody->setDynamic(false);
    // ... 其他物理属性设置
    this->setPhysicsBody(physicsBody);
}
void JumpTable::playAnimation() {//动画
    // 停止所有正在运行的动画（确保不会与其他动画冲突）
     // 检查音乐的状态
    _jumpTableMusicState = cocos2d::AudioEngine::getState(_jumpTableMusicId);

    // 如果音乐没有播放或者播放已经完成，那么开始播放音乐(mp3格式)
    if (_jumpTableMusicState != cocos2d::AudioEngine::AudioState::PLAYING) {
        _jumpTableMusicId = cocos2d::AudioEngine::play2d("music/game_gen_spring.mp3", false);
    }
    this->stopAllActions();
    Vector<SpriteFrame*> idleFrames;
    auto cache = SpriteFrameCache::getInstance();

    for (int i = 0; i <= 22; i++) {
        std::string frameName = StringUtils::format("JumpTable_00-%d.png", i);
        auto frame = cache->getSpriteFrameByName(frameName);
        if (frame) {
            idleFrames.pushBack(frame);
        }
    }
    auto animation = Animation::createWithSpriteFrames(idleFrames, 0.07f);//共1.54s
    auto animate = Animate::create(animation);
    this->runAction(animate);

}
void JumpTable::initDefaultTexture() {
    // Set the default texture for all JumpTables
    // Replace "path/to/default/texture.png" with the actual path of your default texture
    this->setTexture("JumpTable_00-0.png");
    CCASSERT(this->getTexture() != nullptr, "Texture not loaded!");
}
bool JumpTable::init() {
    if (!Trap::init()) {
        return false;
    }

    // 初始化JumpTable特有的属性，如图像、动画、物理属性等
    // ...
    this->setName("JumpTable");

    _activated = false;
    _canBeActivated = true; // 默认情况下，JumpTable可以被激活

    return true;
}

void JumpTable::activate() {
    if (!_activated && _canBeActivated) {
        // 执行激活JumpTable的代码，比如播放动画
        // ...

        _activated = true;
    }
}

void JumpTable::deactivateTemporarily() {
    // 设置当前JumpTable为不可激活
    _canBeActivated = false;

    // 使用cocos2d-x的调度器，在2秒后重新激活
    auto reactivationFunc = [this]() {
        this->_canBeActivated = true;
        };

    // 调度延迟执行的任务
    this->runAction(Sequence::create(
        DelayTime::create(1.6f), // 等待2秒
        CallFunc::create(reactivationFunc), // 然后调用reactivationFunc
        nullptr // 序列结束
    ));
}

JumpTable::~JumpTable() {
    // 清理代码，如果有的话
}

void JumpTable::setCanBeActivated(bool canBeActivated) {
    _canBeActivated = canBeActivated;
}

bool JumpTable::canBeActivated() const {
    return _canBeActivated;
}
