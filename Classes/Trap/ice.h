#pragma once
#ifndef __ICE_H__
#define __ICE_H__

#include "Trap.h"
#include "audio/include/AudioEngine.h"

class Ice : public Trap {
public:
    static Ice* create(const cocos2d::Vec2& position);    // 修改create方法以接收初始位置

    // 初始化函数
    virtual bool init() override;

    // 激活或触发陷阱的功能
    virtual void activate() override;

    // 析构函数
    virtual ~Ice() {}

    // 每帧调用的更新函数
    void update(float dt);

    // 检查射线是否碰撞到Player
    void checkForPlayer();

    // 重置ice对象到初始状态
    void reset();

protected:
    //音频
    int _impacetMusicId;
    cocos2d::AudioEngine::AudioState _impacetMusicState;

    int _shakeMusicId;
    cocos2d::AudioEngine::AudioState _shakeMusicState;

    cocos2d::Vec2 _initialPosition;  // 记录初始位置
    bool canActive;         // 标记是否可以激活

    // 可以添加特定于Ice的其他成员变量或函数
};

#endif // __ICE_H__
