#pragma once
#ifndef __TRAP_H__
#define __TRAP_H__

#include "cocos2d.h"

class Trap : public cocos2d::Sprite {
public:
    // 使用静态create函数来创建陷阱
    static Trap* create(const std::string& filename);

    // 初始化函数
    virtual bool init() override;

    // 激活或触发陷阱的功能
    virtual void activate() = 0;

    // 检查陷阱是否被激活
    virtual bool isActivated() const;

    // 析构函数
    virtual ~Trap() {}

protected:
    bool _activated; // 表示陷阱是否已被激活
};

#endif // __TRAP_H__
