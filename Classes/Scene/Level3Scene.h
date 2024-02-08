#ifndef __LEVEL_3_SCENE_H__
#define __LEVEL_3_SCENE_H__

#include "LevelBase.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h" 

class Level3Scene : public LevelBase {
public:
    // ����һ������������
    static cocos2d::Scene* createScene();

    virtual bool init() override;

    // ʵ��loadLevel������������һ�ص�����
    void loadLevel();

    virtual void startGame() override;
    virtual void endGame() override;
    virtual void pauseGame() override;

    // ʹ��CREATE_FUNC������������ʼ��һ���µ�Layer����
    CREATE_FUNC(Level3Scene);
    
    void update(float dt);
    bool checkForLevelTransition();

    virtual void onEnter() override;
    virtual void onExit() override;

    void initKeyboardListener();

    void onKeyPressedL3(cocos2d::EventKeyboard::KeyCode, cocos2d::Event*);
};

#endif // __LEVEL_3_SCENE_H__