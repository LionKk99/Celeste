#ifndef __LEVEL_6_SCENE_H__
#define __LEVEL_6_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h" 
#include "LevelBase.h"

class Level6Scene : public LevelBase {
public:
    // 创建一个场景并返回
    static cocos2d::Scene* createScene();

    virtual bool init() override;

    // 实现loadLevel方法，加载这一关的内容
    void loadLevel();

    // 使用CREATE_FUNC宏来创建并初始化一个新的Layer对象
    CREATE_FUNC(Level6Scene);

    virtual void startGame() override;
    virtual void endGame() override;
    virtual void pauseGame() override;

    virtual void onEnter() override;
    virtual void onExit() override;

    void update(float dt);

    bool checkForLevelTransition();

    void initKeyboardListener();

    void onKeyPressedL6(cocos2d::EventKeyboard::KeyCode, cocos2d::Event*);
};

#endif // __LEVEL_6_SCENE_H__
