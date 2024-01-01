#pragma once
// Brick.h
#pragma once
#include "Trap.h"

class Brick : public Trap {
public:
    static Brick* create(const cocos2d::Vec2& position); // 静态创建函数

    virtual bool init() override; // 初始化函数
    void toggleVisibility();      // 切换可见性和碰撞的函数
    void activate() {};
    // ... 可能还需要其他成员函数或变量 ...


    bool _isNormal; // 表示brick是否处于正常状态
    void resetBrick(); // 用于重置brick状态的辅助函数
};
