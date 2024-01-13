#ifndef __LEVEL_4_SCENE_H__
#define __LEVEL_4_SCENE_H__
#include "LevelBase.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h" 
#include"Trap/jumpTable.h"
#include"Trap/Spikeweed.h"

class Level4Scene : public LevelBase {
public:
    // ����һ������������0
    static cocos2d::Scene* createScene();

    virtual bool init() override;

    virtual void startGame() override;
    virtual void endGame() override;
    virtual void pauseGame() override;
    // ʵ��loadLevel������������һ�ص�����
    void loadLevel();

    // ʹ��CREATE_FUNC������������ʼ��һ���µ�Layer����
    CREATE_FUNC(Level4Scene);

    virtual void onEnter() override;
    virtual void onExit() override;

    void update(float dt);

    bool checkForLevelTransition();

    void initKeyboardListener();

    void onKeyPressedL4(cocos2d::EventKeyboard::KeyCode, cocos2d::Event*);
};

#endif // __LEVEL_4_SCENE_H__
