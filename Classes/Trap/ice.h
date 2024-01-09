#pragma once
#ifndef __ICE_H__
#define __ICE_H__

#include "Trap.h"
#include "audio/include/AudioEngine.h"

class Ice : public Trap {
public:
    static Ice* create(const cocos2d::Vec2& position);    // �޸�create�����Խ��ճ�ʼλ��

    // ��ʼ������
    virtual bool init() override;

    // ����򴥷�����Ĺ���
    virtual void activate() override;

    // ��������
    virtual ~Ice() {}

    // ÿ֡���õĸ��º���
    void update(float dt);

    // ��������Ƿ���ײ��Player
    void checkForPlayer();

    // ����ice���󵽳�ʼ״̬
    void reset();

protected:
    //��Ƶ
    int _impacetMusicId;
    cocos2d::AudioEngine::AudioState _impacetMusicState;

    int _shakeMusicId;
    cocos2d::AudioEngine::AudioState _shakeMusicState;

    cocos2d::Vec2 _initialPosition;  // ��¼��ʼλ��
    bool canActive;         // ����Ƿ���Լ���

    // ��������ض���Ice��������Ա��������
};

#endif // __ICE_H__
