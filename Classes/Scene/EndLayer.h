#pragma once
#ifndef __ENDLAYER_H__
#define __ENDLAYER_H__

#include "cocos2d.h"

class EndLayer : public cocos2d::Layer {
public:
    static EndLayer* create();
    virtual bool init();

    // ���»õ�Ƭ
    void updateSlideShow(float dt);
    // ����չʾ
    void endSlideShow(float dt);

private:
    cocos2d::Sprite* slideShow;
    int currentSlideIndex;
};

#endif // __ENDLAYER_H__
