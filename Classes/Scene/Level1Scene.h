#ifndef __LEVEL_1_SCENE_H__
#define __LEVEL_1_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h" 

class Level1Scene : public cocos2d::Layer {
public:
    // 创建一个场景并返回
    static cocos2d::Scene* createScene();

    virtual bool init() override;

    // 实现loadLevel方法，加载这一关的内容
    void loadLevel();

    // 使用CREATE_FUNC宏来创建并初始化一个新的Layer对象
    CREATE_FUNC(Level1Scene);
};

#endif // __LEVEL_1_SCENE_H__
