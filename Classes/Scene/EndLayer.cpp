#include "EndLayer.h"
#include "MainMenuScene.h"
#include "audio/include/AudioEngine.h"

USING_NS_CC;
using namespace std;

// Ԥ����ͼƬ·���������ļ�·��
const vector<string> imageFiles = { "level/endLevel/final1.data.png", "level/endLevel/final2.data.png","level/endLevel/final3.data.png","level/endLevel/final4.data.png","level/endLevel/final5.data.png" };
const char* musicFile = "music/wow_so_secret.mp3";

bool EndLayer::init() {
    if (!Layer::init()) {
        return false;
    }

    // ÿ3�����һ��ͼƬ
    this->schedule([this](float dt) {
        this->updateSlideShow(dt);
        }, 3.0f, "slideShowScheduler");

    // 15������չʾ
    this->scheduleOnce([this](float dt) {
        this->endSlideShow(dt);
        }, 15.0f, "endShowScheduler");

    // ��������
    cocos2d::AudioEngine::play2d(musicFile, false);

    // ��ʼ����һ��ͼƬ
    currentSlideIndex = 0;
    slideShow = Sprite::create(imageFiles[currentSlideIndex]);
    slideShow->setPosition(Director::getInstance()->getVisibleSize() / 2);
    // ����ͼƬ����Ϊ0.55
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
    // ��ת��������
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
