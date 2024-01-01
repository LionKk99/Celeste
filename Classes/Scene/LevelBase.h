// Level.h
#ifndef __LEVELBASE_H__
#define __LEVELBASE_H__

#include "cocos2d.h"

class LevelBase : public cocos2d::Layer {
public:
    int _backgroundMusicID = -1; // ��������ID
    
    // ʹ�þ�̬create������������
    static LevelBase* create();

    // ��ʼ������
    virtual bool init() override;

    // ���عؿ��������ݵĺ�������������
    virtual void loadLevel() = 0;

    // ��ʼ����������ͣ��Ϸ��ͨ�ýӿڣ������Ҫ��
    virtual void startGame() = 0;
    virtual void endGame() = 0;
    virtual void pauseGame() = 0;

    // ͨ�õ����������ʼ������
    virtual cocos2d::Scene* createScene();

    // ��������
    virtual ~LevelBase() {};

    // ����ͨ�ú���...

    
};

#endif // __LEVELBASE_H__
