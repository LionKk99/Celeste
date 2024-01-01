#pragma once
#include "LevelBase.h"
#include "Player/Player.h"
class Level3Scene : public LevelBase {
public:
    static cocos2d::Scene* createScene();

    virtual bool init() override;

    virtual void loadLevel() override;

    virtual void startGame() override;
    virtual void endGame() override;
    virtual void pauseGame() override;
    CREATE_FUNC(Level3Scene);

    void update(float dt);
    virtual void onEnter() override;
    virtual void onExit() override;

    Player* m_player;
};

