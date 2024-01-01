#pragma once
// Brick.h
#pragma once
#include "Trap.h"

class Brick : public Trap {
public:
    static Brick* create(const cocos2d::Vec2& position); // ��̬��������

    virtual bool init() override; // ��ʼ������
    void toggleVisibility();      // �л��ɼ��Ժ���ײ�ĺ���
    void activate() {};
    // ... ���ܻ���Ҫ������Ա��������� ...


    bool _isNormal; // ��ʾbrick�Ƿ�������״̬
    void resetBrick(); // ��������brick״̬�ĸ�������
};
