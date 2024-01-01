#include "LevelBase.h"

USING_NS_CC; // 使用Cocos2d-x命名空间

// 初始化函数
bool LevelBase::init() {
    // 在这里执行初始化操作
    if (!Layer::init()) {
        return false;
    }

    // 初始化代码，例如背景音乐ID等
    _backgroundMusicID = -1;  // 假设-1为未初始化的音乐ID
    
    // 可以在这里进行一些关卡的初始化设置

    return true;
}

// 通用的物理世界初始化设置
cocos2d::Scene* LevelBase::createScene() {
    // 创建一个场景对象，可能包含物理
    auto scene = Scene::createWithPhysics();
    // 对物理世界进行设置
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    // 这里不再创建 LevelBase 的实例，而是留给具体的子类
    // auto layer = LevelBase::create();
    // scene->addChild(layer);

    return scene;
}


