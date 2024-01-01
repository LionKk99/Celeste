#pragma once
#ifndef __SPIKEWEED_H__
#define __SPIKEWEED_H__

#include "Trap.h"

class Spikeweed : public Trap {
public:
    // �޸�create�����԰���λ�úʹ��������С
    static Spikeweed* create(const cocos2d::Vec2& position, const cocos2d::Size& triggerSize);

    // ��ʼ������
    virtual bool init(const cocos2d::Vec2& position, const cocos2d::Size& triggerSize);

    // ʵ�ּ���򴥷�Spikeweed�Ĺ���
    virtual void activate() override;

    // ���Spikeweed�Ƿ񱻼���
    virtual bool isActivated() const override;

protected:
    cocos2d::Size _triggerSize;  // ���������С
    bool _activated;             // ��ʾ�����Ƿ��ѱ�����
};

#endif // __SPIKEWEED_H__

