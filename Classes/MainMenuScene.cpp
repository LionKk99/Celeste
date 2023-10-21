#include "MainMenuScene.h"
#include "/Celeste/Classes/Scene/Level1Scene.h"

USING_NS_CC;
//��ȡ��ȷ�ı߽��
Rect getGlobalBoundingBox(cocos2d::ui::Widget* widget) {
    Size s = widget->getContentSize() * widget->getScale();
    return Rect(widget->getWorldPosition().x - s.width * widget->getAnchorPoint().x,
        widget->getWorldPosition().y - s.height * widget->getAnchorPoint().y,
        s.width, s.height);
}
// ��ʼ������
bool MainMenuScene::init()
{
    float scaleFactor = 0.9f; // �������������ӣ����Ը�����Ҫ����
    // 1. ���ȣ����ø���ĳ�ʼ������
    if (!Scene::init())
    {
        return false;
    }

    // 2. ������Ʒֱ���
    Director::getInstance()->getOpenGLView()->setDesignResolutionSize(1280, 720, ResolutionPolicy::NO_BORDER);     
        

    // 3. ���ز����ñ���ͼƬ
    
    // ����һ���µı�����
    Sprite * bgLayer = Sprite::create("mainMenu/Start_BG.png"); // ��������ı���ͼ��·��
    bgLayer->setScale(scaleFactor); // ʹ�ú�����������ͬ������
    bgLayer->setAnchorPoint(Vec2(0, 0));
    bgLayer->setPosition(Vec2(0, 0)); // ����ͼƬͨ������Ļ�����½ǿ�ʼ
    this->addChild(bgLayer); // �������������㣬�������ͻ���������ĺ���

    bg1 = Sprite::create("mainMenu/sky_02.png");
    bg2 = Sprite::create("mainMenu/sky_02.png");

    bg1->setScale(scaleFactor);
    bg2->setScale(scaleFactor);

    // ��Yλ�����ơ����磬����5%�ĸ߶�
    float yOffset = Director::getInstance()->getVisibleSize().height * 0.05;

    bg1->setAnchorPoint(Vec2(0, 0.5));
    bg1->setPosition(Vec2(0, 360 - yOffset)); // 720��һ����360

    bg2->setAnchorPoint(Vec2(0, 0.5));
    bg2->setPosition(Vec2(bg1->getContentSize().width * scaleFactor, 360 - yOffset));
    

    // ��������ͼ
    titleSprite = Sprite::create("mainMenu/title_00.png"); // ����ʹ����ı���ͼ��·��
    // �������Ա���ͼ�������ţ�����ʹ��titleSprite->setScale(scaleFactor); 
    titleSprite->setAnchorPoint(Vec2(0.5, 1)); // ����ê��Ϊ����
    titleSprite->setPosition(Vec2(1280 / 2, 650)); // �����������Ļ�����Ϸ�
    titleSprite->setScale(scaleFactor); // ʹ�ú�����������ͬ������
    this->addChild(titleSprite, 1); // ʹ��z-orderΪ1ȷ��������������б���֮��    
   

    this->addChild(bg1);
    this->addChild(bg2);

    // 4. ���ø��·���ʹ����ͼƬ������
    this->schedule(CC_CALLBACK_1(MainMenuScene::updateBackground, this), "bg_update_key");

    
    // 5. ��Ӱ�ť
    
    // ��ʼ����ʾѡ��״̬��ѩɽСͼ�꣬����������ӵ�����
    mountainSprite = Sprite::create("mainMenu/mountain_00.png");
    mountainSprite->setScale(0.0317);  // ������������
    mountainSprite->setVisible(false);  // ��ʼʱ�����Сͼ�������ص�
    this->addChild(mountainSprite);   
  

    double buttonFontSize = 50;

  // ʾ����ʼ����һ����ť
    newGame = cocos2d::ui::Button::create();
    newGame->setTitleText("New Game");
    newGame->setTitleFontName("fonts/gill-sans-mt-condensed/Gill Sans MT.ttf"); // ����ѡ������Ҫ������
    newGame->setTitleFontSize(buttonFontSize);
    newGame->setPosition(Vec2(1000, 470));
    newGame->setAnchorPoint(Vec2(0, 0.5));
    this->addChild(newGame);
    newGame->addTouchEventListener([](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
            // �л��� Level1Scene����ʹ�ý������
            auto scene = Level1Scene::createScene();
            Director::getInstance()->replaceScene(scene);
            //Director::getInstance()->replaceScene(TransitionFade::create(1.0, scene)); // ʹ��һ���������ֵĹ��ɶ���������1��(δ֪����)
        }
        });   

    // ʾ����ʼ���ڶ�����ť
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

    //����˳�ʱ��ʾ�Ľ���
    auto visibleSize = Director::getInstance()->getVisibleSize();

    exitDialog = Sprite::create("mainMenu/quitWindow_00.png");
    exitDialog->setPosition(Director::getInstance()->getVisibleSize() / 2);
    exitDialog->setScale(0.7);  // ������������
    exitDialog->setVisible(false); // ��ʼʱ������
    this->addChild(exitDialog, 2); // һ���ϸߵ�z-orderȷ�������������ݵĶ���   

    // ����һ��TTF�����ǩ
    auto exitLabel = Label::createWithTTF("MAKE  SURE  YOUR  EXIT", "fonts/gill-sans-mt-condensed/Gill Sans MT Bold.ttf", 54);
    // �����Ҫ�����ñ�ǩ��λ��
    exitLabel->setPosition(Vec2(540, 520));
    // ������ɫ
    exitLabel->setTextColor(Color4B::BLACK);
    // ����ǩ��ӵ�����
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

    //quit��ť�����¼�

    quit->addTouchEventListener([=](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
            exitDialog->setVisible(true);
        }
        });

    // �����������¼�
    auto mouseListener = EventListenerMouse::create();

    mouseListener->onMouseMove = [=](Event* event) {
        EventMouse* e = (EventMouse*)event;
        std::vector<std::pair<cocos2d::ui::Button*, Vec2>> buttons = {//��Ϊѩɽͼ�����λ��
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
                mountainSprite->setLocalZOrder(button->getLocalZOrder() + 1); // ȷ��Сͼ���������Ӧ��ť��ǰ��
                isOverAnyButton = true;
                break;  // ��������ĳ����ť�ϣ����ǿ�������ѭ��
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
    // �������ƶ���������ͼƬ
    bg1->setPositionX(bg1->getPositionX() - 1);
    bg2->setPositionX(bg2->getPositionX() - 1);

    // ���bg1�Ƴ���Ļ��࣬����������λ�õ�bg2���Ҳ�
    if (bg1->getPositionX() + bg1->getContentSize().width * bg1->getScaleX() < 0)
    {
        bg1->setPositionX(bg2->getPositionX() + bg2->getContentSize().width * bg2->getScaleX());
    }

    // ���bg2�Ƴ���Ļ��࣬����������λ�õ�bg1���Ҳ�
    if (bg2->getPositionX() + bg2->getContentSize().width * bg2->getScaleX() < 0)
    {
        bg2->setPositionX(bg1->getPositionX() + bg1->getContentSize().width * bg1->getScaleX());
    }
}

