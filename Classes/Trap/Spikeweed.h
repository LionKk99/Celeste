#pragma once
#ifndef __SPIKEWEED_H__
#define __SPIKEWEED_H__

#include "Trap.h"

class Spikeweed : public Trap {
public:
    // 修改create函数以包括位置和触发区域大小
    static Spikeweed* create(const cocos2d::Vec2& position, const cocos2d::Size& triggerSize);

    // 初始化函数
    virtual bool init(const cocos2d::Vec2& position, const cocos2d::Size& triggerSize);

    // 实现激活或触发Spikeweed的功能
    virtual void activate() override;

    // 检查Spikeweed是否被激活
    virtual bool isActivated() const override;

protected:
    cocos2d::Size _triggerSize;  // 触发区域大小
    bool _activated;             // 表示陷阱是否已被激活
};

#endif // __SPIKEWEED_H__

