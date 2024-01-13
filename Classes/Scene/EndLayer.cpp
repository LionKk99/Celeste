#include "EndLayer.h"
#include "MainMenuScene.h"
#include "audio/include/AudioEngine.h"

USING_NS_CC;
using namespace std;

// 预定义图片路径和音乐文件路径
const vector<string> imageFiles = { "level/endLevel/final1.data.png", "level/endLevel/final2.data.png","level/endLevel/final3.data.png","level/endLevel/final4.data.png","level/endLevel/final5.data.png" };
const char* musicFile = "music/wow_so_secret.mp3";

bool EndLayer::init() {
    if (!Layer::init()) {
        return false;
    }

    // 每3秒更新一次图片
    this->schedule([this](float dt) {
        this->updateSlideShow(dt);
        }, 3.0f, "slideShowScheduler");

    // 15秒后结束展示
    this->scheduleOnce([this](float dt) {
        this->endSlideShow(dt);
        }, 15.0f, "endShowScheduler");

    // 播放音乐
    cocos2d::AudioEngine::play2d(musicFile, false);

    // 初始化第一张图片
    currentSlideIndex = 0;
    slideShow = Sprite::create(imageFiles[currentSlideIndex]);
    slideShow->setPosition(Director::getInstance()->getVisibleSize() / 2);
    // 设置图片缩放为0.55
    slideShow->setScale(0.55f);
    this->addChild(slideShow);

    return true;
}

void EndLayer::updateSlideShow(float dt) {
    currentSlideIndex++;
    if (currentSlideIndex < imageFiles.size()) {
        slideShow->setTexture(imageFiles[currentSlideIndex]);
    }
}

void EndLayer::endSlideShow(float dt) {
    // 跳转到主界面
    auto scene = MainMenuScene::create();
    Director::getInstance()->replaceScene(TransitionFade::create(1.0, scene));
}

EndLayer* EndLayer::create() {
    EndLayer* pRet = new(std::nothrow) EndLayer();
    if (pRet && pRet->init()) {
        pRet->autorelease();
        return pRet;
    }
    else {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}
