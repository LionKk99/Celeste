#ifndef __LEVEL_1_SCENE_H__
#define __LEVEL_1_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h" 

class Level1Scene : public cocos2d::Layer {
public:
    // ����һ������������
    static cocos2d::Scene* createScene();

    virtual bool init() override;

    // ʵ��loadLevel������������һ�ص�����
    void loadLevel();

    // ʹ��CREATE_FUNC������������ʼ��һ���µ�Layer����
    CREATE_FUNC(Level1Scene);
};

#endif // __LEVEL_1_SCENE_H__
