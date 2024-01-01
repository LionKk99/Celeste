
#ifndef __LEVEL_1_SCENE_H__
#define __LEVEL_1_SCENE_H__

#include "LevelBase.h"

class Level1Scene : public LevelBase {
public:
    static cocos2d::Scene* createScene();

    virtual bool init() override;

    virtual void loadLevel() override;

    virtual void startGame() override;
    virtual void endGame() override;
    virtual void pauseGame() override;
    CREATE_FUNC(Level1Scene);

    virtual void onEnter() override;
    virtual void onExit() override;
};

#endif // __LEVEL_1_SCENE_H__
