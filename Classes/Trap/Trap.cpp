#include "Trap.h"

USING_NS_CC;

bool Trap::init() {
    // ������ִ�г�ʼ������
    if (!Sprite::init()) {
        return false;
    }

    // ���������Ĭ�����ԣ���δ������״̬
    _activated = false;

    // ��ʼ�����룬�������������Ĭ��ͼ�񡢶������������Ե�

    return true;
}

bool Trap::isActivated() const {
    return _activated;
}

// �������ܵ�ͨ�����巽��...
