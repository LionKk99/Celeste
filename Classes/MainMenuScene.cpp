#include "MainMenuScene.h"
#include "/Celeste/Classes/Scene/Level1Scene.h"

USING_NS_CC;
//获取正确的边界框
Rect getGlobalBoundingBox(cocos2d::ui::Widget* widget) {
    Size s = widget->getContentSize() * widget->getScale();
    return Rect(widget->getWorldPosition().x - s.width * widget->getAnchorPoint().x,
        widget->getWorldPosition().y - s.height * widget->getAnchorPoint().y,
        s.width, s.height);
}
// 初始化方法
bool MainMenuScene::init()
{
    float scaleFactor = 0.9f; // 这里是缩放因子，可以根据需要调整
    // 1. 首先，调用父类的初始化方法
    if (!Scene::init())
    {
        return false;
    }

    // 2. 设置设计分辨率
    Director::getInstance()->getOpenGLView()->setDesignResolutionSize(1280, 720, ResolutionPolicy::NO_BORDER);     
        

    // 3. 加载并设置背景图片
    
    // 创建一个新的背景层
    Sprite * bgLayer = Sprite::create("mainMenu/Start_BG.png"); // 这里用你的背景图的路径
    bgLayer->setScale(scaleFactor); // 使用和其他背景相同的缩放
    bgLayer->setAnchorPoint(Vec2(0, 0));
    bgLayer->setPosition(Vec2(0, 0)); // 背景图片通常从屏幕的左下角开始
    this->addChild(bgLayer); // 先添加这个背景层，这样它就会在其他层的后面

    bg1 = Sprite::create("mainMenu/sky_02.png");
    bg2 = Sprite::create("mainMenu/sky_02.png");

    bg1->setScale(scaleFactor);
    bg2->setScale(scaleFactor);

    // 将Y位置下移。例如，下移5%的高度
    float yOffset = Director::getInstance()->getVisibleSize().height * 0.05;

    bg1->setAnchorPoint(Vec2(0, 0.5));
    bg1->setPosition(Vec2(0, 360 - yOffset)); // 720的一半是360

    bg2->setAnchorPoint(Vec2(0, 0.5));
    bg2->setPosition(Vec2(bg1->getContentSize().width * scaleFactor, 360 - yOffset));
    

    // 创建标题图
    titleSprite = Sprite::create("mainMenu/title_00.png"); // 这里使用你的标题图的路径
    // 如果你想对标题图进行缩放，可以使用titleSprite->setScale(scaleFactor); 
    titleSprite->setAnchorPoint(Vec2(0.5, 1)); // 设置锚点为上中
    titleSprite->setPosition(Vec2(1280 / 2, 650)); // 将标题放在屏幕的中上方
    titleSprite->setScale(scaleFactor); // 使用和其他背景相同的缩放
    this->addChild(titleSprite, 1); // 使用z-order为1确保标题出现在所有背景之上    
   

    this->addChild(bg1);
    this->addChild(bg2);

    // 4. 调用更新方法使背景图片动起来
    this->schedule(CC_CALLBACK_1(MainMenuScene::updateBackground, this), "bg_update_key");

    
    // 5. 添加按钮
    
    // 初始化表示选中状态的雪山小图标，但不立即添加到场景
    mountainSprite = Sprite::create("mainMenu/mountain_00.png");
    mountainSprite->setScale(0.0317);  // 设置缩放因子
    mountainSprite->setVisible(false);  // 初始时，这个小图标是隐藏的
    this->addChild(mountainSprite);   
  

    double buttonFontSize = 50;

  // 示例初始化第一个按钮
    newGame = cocos2d::ui::Button::create();
    newGame->setTitleText("New Game");
    newGame->setTitleFontName("fonts/gill-sans-mt-condensed/Gill Sans MT.ttf"); // 可以选择你想要的字体
    newGame->setTitleFontSize(buttonFontSize);
    newGame->setPosition(Vec2(1000, 470));
    newGame->setAnchorPoint(Vec2(0, 0.5));
    this->addChild(newGame);
    newGame->addTouchEventListener([](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
            // 切换到 Level1Scene，不使用渐变过渡
            auto scene = Level1Scene::createScene();
            Director::getInstance()->replaceScene(scene);
            //Director::getInstance()->replaceScene(TransitionFade::create(1.0, scene)); // 使用一个渐隐渐现的过渡动画，持续1秒(未知错误)
        }
        });   

    // 示例初始化第二个按钮
    continueButton = cocos2d::ui::Button::create();
    continueButton->setTitleText("Continue");
    continueButton->setTitleFontName("fonts/gill-sans-mt-condensed/Gill Sans MT.ttf");
    continueButton->setTitleFontSize(buttonFontSize);
    continueButton->setPosition(Vec2(1000, 400)); 
    continueButton->setAnchorPoint(Vec2(0, 0.5));   
    this->addChild(continueButton);
    //
    setting = cocos2d::ui::Button::create();
    setting->setTitleText("Setting");
    setting->setTitleFontName("fonts/gill-sans-mt-condensed/Gill Sans MT.ttf");
    setting->setTitleFontSize(buttonFontSize);
    setting->setPosition(Vec2(1000, 330));
    setting->setAnchorPoint(Vec2(0, 0.5));
    this->addChild(setting);
    //
    staff = cocos2d::ui::Button::create();
    staff->setTitleText("Staff");
    staff->setTitleFontName("fonts/gill-sans-mt-condensed/Gill Sans MT.ttf");
    staff->setTitleFontSize(buttonFontSize);
    staff->setPosition(Vec2(1000, 260));
    staff->setAnchorPoint(Vec2(0, 0.5));
    this->addChild(staff);
    //
    quit = cocos2d::ui::Button::create();
    quit->setTitleText("Quit");
    quit->setTitleFontName("fonts/gill-sans-mt-condensed/Gill Sans MT.ttf");
    quit->setTitleFontSize(buttonFontSize);
    quit->setPosition(Vec2(1000, 190));
    quit->setAnchorPoint(Vec2(0, 0.5));
    this->addChild(quit);

    //添加退出时显示的界面
    auto visibleSize = Director::getInstance()->getVisibleSize();

    exitDialog = Sprite::create("mainMenu/quitWindow_00.png");
    exitDialog->setPosition(Director::getInstance()->getVisibleSize() / 2);
    exitDialog->setScale(0.7);  // 设置缩放因子
    exitDialog->setVisible(false); // 初始时隐藏它
    this->addChild(exitDialog, 2); // 一个较高的z-order确保它在其他内容的顶部   

    // 创建一个TTF字体标签
    auto exitLabel = Label::createWithTTF("MAKE  SURE  YOUR  EXIT", "fonts/gill-sans-mt-condensed/Gill Sans MT Bold.ttf", 54);
    // 如果需要，设置标签的位置
    exitLabel->setPosition(Vec2(540, 520));
    // 设置颜色
    exitLabel->setTextColor(Color4B::BLACK);
    // 将标签添加到场景
    exitDialog->addChild(exitLabel,3);

    auto exitButton = cocos2d::ui::Button::create();
    exitButton->setTitleFontName("fonts/gill-sans-mt-condensed/Gill Sans MT Bold.ttf");
    exitButton->setTitleText("EXIT");
    exitButton->setTitleColor(Color3B::BLACK);
    exitButton->setTitleFontSize(50);
    exitButton->setPosition(Vec2(540, 400));
    exitButton->addTouchEventListener([](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
            Director::getInstance()->end();
        }
        });
    exitDialog->addChild(exitButton,3);

    auto returnButton = cocos2d::ui::Button::create();
    returnButton->setTitleFontName("fonts/gill-sans-mt-condensed/Gill Sans MT Bold.ttf");
    returnButton->setTitleText("RETURN");
    returnButton->setTitleColor(Color3B::BLACK);
    returnButton->setTitleFontSize(50);
    returnButton->setPosition(Vec2(540, 300));
    returnButton->addTouchEventListener([=](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
            exitDialog->setVisible(false);
        }
        });
    exitDialog->addChild(returnButton,3);

    //quit按钮触发事件

    quit->addTouchEventListener([=](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
            exitDialog->setVisible(true);
        }
        });

    // 设置鼠标监听事件
    auto mouseListener = EventListenerMouse::create();

    mouseListener->onMouseMove = [=](Event* event) {
        EventMouse* e = (EventMouse*)event;
        std::vector<std::pair<cocos2d::ui::Button*, Vec2>> buttons = {//此为雪山图标出现位置
            {newGame, Vec2(950, 470)},
            {continueButton, Vec2(950, 400)},
            {setting, Vec2(950, 330)},
            {staff, Vec2(950, 260)},
            {quit, Vec2(950, 190)},
            {exitButton, Vec2(500, 380)},
            {returnButton, Vec2(500, 310)}
        };

        bool isOverAnyButton = false;

        for (auto& pair : buttons) {
            cocos2d::ui::Button* button = pair.first;
            Vec2 iconPos = pair.second;

            if (button && getGlobalBoundingBox(button).containsPoint(Vec2(e->getCursorX(), e->getCursorY()))) {
                mountainSprite->setVisible(true);
                mountainSprite->setPosition(iconPos);
                mountainSprite->setLocalZOrder(button->getLocalZOrder() + 1); // 确保小图标出现在相应按钮的前面
                isOverAnyButton = true;
                break;  // 如果鼠标在某个按钮上，我们可以跳出循环
            }
        }

        if (!isOverAnyButton) {
            mountainSprite->setVisible(false);
        }
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

 
    return true;
}


void MainMenuScene::updateBackground(float dt)
{
    // 持续地移动两个背景图片
    bg1->setPositionX(bg1->getPositionX() - 1);
    bg2->setPositionX(bg2->getPositionX() - 1);

    // 如果bg1移出屏幕左侧，则重置它的位置到bg2的右侧
    if (bg1->getPositionX() + bg1->getContentSize().width * bg1->getScaleX() < 0)
    {
        bg1->setPositionX(bg2->getPositionX() + bg2->getContentSize().width * bg2->getScaleX());
    }

    // 如果bg2移出屏幕左侧，则重置它的位置到bg1的右侧
    if (bg2->getPositionX() + bg2->getContentSize().width * bg2->getScaleX() < 0)
    {
        bg2->setPositionX(bg1->getPositionX() + bg1->getContentSize().width * bg1->getScaleX());
    }
}

