#include "LevelBase.h"

USING_NS_CC; // ʹ��Cocos2d-x�����ռ�

// ��ʼ������
bool LevelBase::init() {
    // ������ִ�г�ʼ������
    if (!Layer::init()) {
        return false;
    }

    // ��ʼ�����룬���米������ID��
    _backgroundMusicID = -1;  // ����-1Ϊδ��ʼ��������ID
    
    // �������������һЩ�ؿ��ĳ�ʼ������

    return true;
}

// ͨ�õ����������ʼ������
cocos2d::Scene* LevelBase::createScene() {
    // ����һ���������󣬿��ܰ�������
    auto scene = Scene::createWithPhysics();
    // �����������������
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    // ���ﲻ�ٴ��� LevelBase ��ʵ���������������������
    // auto layer = LevelBase::create();
    // scene->addChild(layer);

    return scene;
}


