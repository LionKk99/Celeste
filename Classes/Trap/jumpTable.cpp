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
    // �������ж���֡����Ӧ128x32�Ĵ�С
    Size standardSize = Size(128, 32);
    this->setContentSize(standardSize); // ����JumpTable�����ݴ�С

    auto physicsBody = PhysicsBody::createBox(standardSize);
    physicsBody->setDynamic(false);
    // ... ����������������
    this->setPhysicsBody(physicsBody);
}
void JumpTable::playAnimation() {//����
    // ֹͣ�����������еĶ�����ȷ������������������ͻ��
     // ������ֵ�״̬
    _jumpTableMusicState = cocos2d::AudioEngine::getState(_jumpTableMusicId);

    // �������û�в��Ż��߲����Ѿ���ɣ���ô��ʼ��������(mp3��ʽ)
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
    auto animation = Animation::createWithSpriteFrames(idleFrames, 0.07f);//��1.54s
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

    // ��ʼ��JumpTable���е����ԣ���ͼ�񡢶������������Ե�
    // ...
    this->setName("JumpTable");

    _activated = false;
    _canBeActivated = true; // Ĭ������£�JumpTable���Ա�����

    return true;
}

void JumpTable::activate() {
    if (!_activated && _canBeActivated) {
        // ִ�м���JumpTable�Ĵ��룬���粥�Ŷ���
        // ...

        _activated = true;
    }
}

void JumpTable::deactivateTemporarily() {
    // ���õ�ǰJumpTableΪ���ɼ���
    _canBeActivated = false;

    // ʹ��cocos2d-x�ĵ���������2������¼���
    auto reactivationFunc = [this]() {
        this->_canBeActivated = true;
        };

    // �����ӳ�ִ�е�����
    this->runAction(Sequence::create(
        DelayTime::create(1.6f), // �ȴ�2��
        CallFunc::create(reactivationFunc), // Ȼ�����reactivationFunc
        nullptr // ���н���
    ));
}

JumpTable::~JumpTable() {
    // ������룬����еĻ�
}

void JumpTable::setCanBeActivated(bool canBeActivated) {
    _canBeActivated = canBeActivated;
}

bool JumpTable::canBeActivated() const {
    return _canBeActivated;
}
