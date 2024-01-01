// Level.h
#ifndef __LEVELBASE_H__
#define __LEVELBASE_H__

#include "cocos2d.h"

class LevelBase : public cocos2d::Layer {
public:
    int _backgroundMusicID = -1; // 背景音乐ID
    
    // 使用静态create函数来创建层
    static LevelBase* create();

    // 初始化函数
    virtual bool init() override;

    // 加载关卡特有内容的函数（抽象函数）
    virtual void loadLevel() = 0;

    // 开始、结束、暂停游戏等通用接口（如果需要）
    virtual void startGame() = 0;
    virtual void endGame() = 0;
    virtual void pauseGame() = 0;

    // 通用的物理世界初始化设置
    virtual cocos2d::Scene* createScene();

    // 析构函数
    virtual ~LevelBase() {};

    // 更多通用函数...

    
};

#endif // __LEVELBASE_H__
