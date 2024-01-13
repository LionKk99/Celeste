#include "Setting.h"
#include "MainMenuScene.h"
#include "PauseMenu.h"
using namespace cocos2d;
using namespace ui;
Rect getGlobalBoundingBox1(cocos2d::ui::Widget* widget) {
    Size s = widget->getContentSize() * widget->getScale();
    return Rect(widget->getWorldPosition().x - s.width * widget->getAnchorPoint().x,
        widget->getWorldPosition().y - s.height * widget->getAnchorPoint().y,
        s.width, s.height);
}
bool PauseMenu::init()
{
    if (!Scene::init())
    {
        return false;
    }
    //SaveFile(1);
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
    double buttonFontSize = 50;


    Sprite* bgLayer = Sprite::create("mainMenu/quitWindow_00.png");
    bgLayer->setScale(1.0f);
    bgLayer-> setAnchorPoint(Vec2(0.0, 0.0));
    bgLayer->setPosition(Vec2(100, 0));
    this->addChild(bgLayer);



    auto backGame = cocos2d::ui::Button::create();
    backGame->setTitleText("Back to Game");
    backGame->setTitleFontName("fonts/arial.ttf"); // 可以选择你想要的字体
    backGame->setTitleFontSize(buttonFontSize);
    backGame->setTitleColor(Color3B::BLACK);
    backGame->setPosition(Vec2(450, 450));
    backGame->setAnchorPoint(Vec2(0, 0.5));
    this->addChild(backGame);
    backGame->addTouchEventListener([](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
            Director::getInstance()->getRunningScene()->resume();
            Director::getInstance()->popScene();
        }
        });

        // 示例初始化第二个按钮

    const char* saveEffectFile = "music/savesucceed.mp3";

    auto saveGame = cocos2d::ui::Button::create();
    saveGame->setTitleText("Save Game");
    saveGame->setTitleFontName("fonts/arial.ttf");
    saveGame->setTitleFontSize(buttonFontSize);
    saveGame->setTitleColor(Color3B::BLACK);
    saveGame->setPosition(Vec2(450, 360));
    saveGame->setAnchorPoint(Vec2(0, 0.5));
    this->addChild(saveGame);
    saveGame->addTouchEventListener([this, saveEffectFile](Ref* sender, cocos2d::ui::Widget::TouchEventType type) { // 注意这里的[this]
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
            // 播放保存按钮的音效
            cocos2d::AudioEngine::play2d(saveEffectFile);

            this->SaveFile(Player::currentLevel); // 使用this来调用SaveFile
            Director::getInstance()->pause();
            auto scene = PauseOverlay::create();
            Director::getInstance()->pushScene(scene);
        }
        });
        //
    auto saveTable = cocos2d::ui::Button::create();
    saveTable->setTitleText("Save and Quit");
    saveTable->setTitleFontName("fonts/arial.ttf");
    saveTable->setTitleFontSize(buttonFontSize);
    saveTable->setTitleColor(Color3B::BLACK);
    saveTable->setPosition(Vec2(450, 270));
    saveTable->setAnchorPoint(Vec2(0, 0.5));
    this->addChild(saveTable);
    saveTable->addTouchEventListener([this](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
            this->SaveFile(Player::currentLevel); // 使用this来调用SaveFile
            auto scene = PauseOverlay::create();
            Director director;
            director.runWithScene(scene);
            cocos2d::Director::getInstance()->end();
        }
        });
        //
    auto saveMenu = cocos2d::ui::Button::create();
    saveMenu->setTitleText("Save and Quit to Menu");
    saveMenu->setTitleFontName("fonts/arial.ttf");
    saveMenu->setTitleFontSize(buttonFontSize);
    saveMenu->setTitleColor(Color3B::BLACK);
    saveMenu->setPosition(Vec2(450, 180));
    saveMenu->setAnchorPoint(Vec2(0, 0.5));
    this->addChild(saveMenu);
    saveMenu->addTouchEventListener([this](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
            this->SaveFile(Player::currentLevel); // 使用this来调用SaveFile
            auto scene = MainMenuScene::create();
            Director::getInstance()->replaceScene(TransitionFade::create(1.0, scene));
        }
        });
    auto Circle = Sprite::create("mainMenu/mountain_00.png");
    Circle->setScale(0.0317);  // 设置缩放因子
    Circle->setVisible(false);  // 初始时，这个小图标是隐藏的
    this->addChild(Circle);

    auto mouseListener = EventListenerMouse::create();

    mouseListener->onMouseMove = [=](Event* event) {
        EventMouse* e = (EventMouse*)event;
        std::vector<std::pair<cocos2d::ui::Button*, Vec2>> buttons = {
            {backGame, Vec2(421, 450)},
            {saveGame, Vec2(421, 360)},
            {saveTable, Vec2(421, 270)},
            {saveMenu,Vec2(421,180)}
        };

        bool isOverAnyButton = false;

        for (auto& pair : buttons) {
            cocos2d::ui::Button* button = pair.first;
            Vec2 iconPos = pair.second;

            if (button && getGlobalBoundingBox1(button).containsPoint(Vec2(e->getCursorX(), e->getCursorY()))) {
                Circle->setVisible(true);
                Circle->setPosition(iconPos);
                Circle->setLocalZOrder(button->getLocalZOrder() + 1); // 确保小图标出现在相应按钮的前面
                isOverAnyButton = true;
                break;  // 如果鼠标在某个按钮上，我们可以跳出循环
            }
        }

        if (!isOverAnyButton) {
            Circle->setVisible(false);
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
    return true;
};
void PauseMenu::SaveFile(int n)
{
    std::ofstream file("save.txt");
    if (file.is_open()) {
        file << n;
        file.close();
    }
    else {
        CCLOG("file write errow");
    }
}
bool PauseOverlay::init()
{
    if (!Scene::init())
    {
        return false;
    }
    double buttonFontSize = 30;
    // 创建背景层
    auto background = LayerColor::create(Color4B(40, 20, 100, 128));
    this->addChild(background);

    // 添加文本标签
    auto label = Label::createWithTTF("Save Success", "fonts/arial.ttf", 24);
    label->setPosition(Director::getInstance()->getVisibleSize() / 2);
    background->addChild(label);


    // 添加返回按钮
    auto backButton = cocos2d::ui::Button::create();
    backButton->setTitleText("Back");
    backButton->setTitleFontName("fonts/arial.ttf");
    backButton->setTitleFontSize(buttonFontSize);
    backButton->setPosition(Vec2(800, 50));
    backButton->addClickEventListener(CC_CALLBACK_1(PauseOverlay::onBackButtonClicked, this));
    this->addChild(backButton);

    auto Circle = Sprite::create("mainMenu/mountain_00.png");
    Circle->setScale(0.0317);  // 设置缩放因子
    Circle->setVisible(false);  // 初始时，这个小图标是隐藏的
    this->addChild(Circle);

    auto mouseListener = EventListenerMouse::create();



    mouseListener->onMouseMove = [=](Event* event) {
        EventMouse* e = (EventMouse*)event;
        std::pair<cocos2d::ui::Button*, Vec2 > buttons = { backButton,Vec2(900,200) };


        bool isOverAnyButton = false;


        cocos2d::ui::Button* button = buttons.first;
        Vec2 iconPos = buttons.second;

        if (button && getGlobalBoundingBox1(button).containsPoint(Vec2(e->getCursorX(), e->getCursorY()))) {
            Circle->setVisible(true);
            Circle->setPosition(iconPos);
            Circle->setLocalZOrder(button->getLocalZOrder() + 1); // 确保小图标出现在相应按钮的前面
            isOverAnyButton = true;
        }
        if (!isOverAnyButton) {
            Circle->setVisible(false);
        }
    };
    return true;
}

void PauseOverlay::onBackButtonClicked(Ref* sender)
{
    // 取消暂停状态
    Director::getInstance()->resume();
    Director::getInstance()->popScene();

    // 隐藏暂停界面
    this->removeFromParent();

    // 在这里添加其他恢复游戏的逻辑，例如恢复音乐、动画等
    // ...
}


