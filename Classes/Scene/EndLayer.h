#pragma once
#ifndef __ENDLAYER_H__
#define __ENDLAYER_H__

#include "cocos2d.h"

class EndLayer : public cocos2d::Layer {
public:
    static EndLayer* create();
    virtual bool init();

    // 更新幻灯片
    void updateSlideShow(float dt);
    // 结束展示
    void endSlideShow(float dt);

private:
    cocos2d::Sprite* slideShow;
    int currentSlideIndex;
};

#endif // __ENDLAYER_H__
