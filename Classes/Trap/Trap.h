#pragma once
#ifndef __TRAP_H__
#define __TRAP_H__

#include "cocos2d.h"

class Trap : public cocos2d::Sprite {
public:
    // ʹ�þ�̬create��������������
    static Trap* create(const std::string& filename);

    // ��ʼ������
    virtual bool init() override;

    // ����򴥷�����Ĺ���
    virtual void activate() = 0;

    // ��������Ƿ񱻼���
    virtual bool isActivated() const;

    // ��������
    virtual ~Trap() {}

protected:
    bool _activated; // ��ʾ�����Ƿ��ѱ�����
};

#endif // __TRAP_H__
