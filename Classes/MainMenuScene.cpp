#include "MainMenuScene.h"
#include "Scene/Level1Scene.h"
#include "Scene/Level2Scene.h"
#include "Scene/Level3Scene.h"
#include "Scene/Level4Scene.h"
#include "Scene/Level5Scene.h"
#include "Scene/Level6Scene.h"
#include "Scene/Level7Scene.h"
#include "Staff.h"
#include "PauseMenu.h"
#include "Setting.h"
#include <iostream>
USING_NS_CC;
//��ȡ��ȷ�ı߽��
Rect getGlobalBoundingBox(cocos2d::ui::Widget* widget) {
    Size s = widget->getContentSize() * widget->getScale();
    return Rect(widget->getWorldPosition().x - s.width * widget->getAnchorPoint().x,
        widget->getWorldPosition().y - s.height * widget->getAnchorPoint().y,
        s.width, s.height);
}

//int level = read_from_file();
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
        
    initKeyboardListener();

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
            //��մ浵�ļ�
            std::ofstream outFile1("Save.txt", std::ios::trunc);
            outFile1.close();
            //
            write_to_file(1);
            // �л��� Level1Scene
            auto scene = Level1Scene::createScene(); // ��������Level1Scene����һ����̬�� createScene �����������������
            Director::getInstance()->replaceScene(scene);// ʹ��һ���������ֵĹ��ɶ���������1��
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
    continueButton->addTouchEventListener([](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
            //CCLOG("Level: %s !!!", level);
            int level = read_from_file();
            switch (level)
            {
            case(1):
            {
                auto scene = Level1Scene::createScene();
                Director::getInstance()->replaceScene(scene);
                break;
            }
            case(2):
            {
                auto scene = Level2Scene::createScene();
                Director::getInstance()->replaceScene(scene);
                break;
            }
            case(3):
            {
                auto scene = Level3Scene::createScene();
                Director::getInstance()->replaceScene(scene);
                break;
            }
            case(4):
            {
                auto scene = Level4Scene::createScene();
                Director::getInstance()->replaceScene(scene);
                break;
            }
            case(5):
            {
                auto scene = Level5Scene::createScene();
                Director::getInstance()->replaceScene(scene);
                break;
            }
            case(6):
            {
                auto scene = Level6Scene::createScene();
                Director::getInstance()->replaceScene(scene);
                break;
            }
            case(7):
            {
                auto scene = Level7Scene::createScene();
                Director::getInstance()->replaceScene(scene);
                break;
            }
            default: {
                CCLOG("save file error!! ");
                auto scene = Level1Scene::createScene();
                Director::getInstance()->replaceScene(scene);                
            }

            }
        }
        });
        
    //
    setting = cocos2d::ui::Button::create();
    setting->setTitleText("Setting");
    setting->setTitleFontName("fonts/gill-sans-mt-condensed/Gill Sans MT.ttf");
    setting->setTitleFontSize(buttonFontSize);
    setting->setPosition(Vec2(1000, 330));
    setting->setAnchorPoint(Vec2(0, 0.5));
    this->addChild(setting);
    setting->addTouchEventListener([](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
            auto scene = Setting::create();
            Director::getInstance()->replaceScene(TransitionFade::create(1.0, scene));
        }
        });

    //
    staff = cocos2d::ui::Button::create();
    staff->setTitleText("Staff");
    staff->setTitleFontName("fonts/gill-sans-mt-condensed/Gill Sans MT.ttf");
    staff->setTitleFontSize(buttonFontSize);
    staff->setPosition(Vec2(1000, 260));
    staff->setAnchorPoint(Vec2(0, 0.5));
    this->addChild(staff);
    staff->addTouchEventListener([](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
            // �л��� Level1Scene
            auto scene = Credits::create(); // ��������Level1Scene����һ����̬�� createScene �����������������
            Director::getInstance()->replaceScene(TransitionFade::create(1.0, scene)); // ʹ��һ���������ֵĹ��ɶ���������1��
        }
        });
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

void MainMenuScene::initKeyboardListener()
{
    EventListenerKeyboard* listenerkeyPad = EventListenerKeyboard::create();
    listenerkeyPad->onKeyReleased = CC_CALLBACK_2(MainMenuScene::onKeyPressed, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);
}

void MainMenuScene::onKeyPressed(EventKeyboard::KeyCode keycode, Event* event) {
    if (keycode == EventKeyboard::KeyCode::KEY_ESCAPE) {
        // ESC �����£��л�����һ������
        auto pauseLayer = PauseMenu::create();
        Director::getInstance()->getRunningScene()->pause();
        Director::getInstance()->pushScene(pauseLayer);
    }
}
/*
char getLastCharacter(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate); // Open file at the end

    if (file.is_open()) {
        auto size = file.tellg(); // Get file size
        if (size > 0) {
            file.seekg(-1, std::ios_base::end); // Move one char before the EOF
            char lastChar;
            file >> lastChar; // Read the last character
            file.close();
            return lastChar;
        }
        else {
            std::cerr << "File is empty." << std::endl;
            file.close();
            return '\0'; // Return a default value
        }
    }
    else {
        std::cerr << "Unable to open the file: " << filename << std::endl;
        return '\0'; // Return a default value
    }
}
*/


int read_from_file() {
    std::ifstream file("save.txt");
    int number;
    if (file.is_open()) {
        file >> number;
        file.close();
        return number;
    }
    else {
        std::cerr << "�޷����ļ����ж�ȡ" << std::endl;
        return -1;  // ����һ�������ֵ
    }
}

void write_to_file(int number) {
    std::ofstream file("save.txt");
    if (file.is_open()) {
        file << number;
        file.close();
    }
    else {
        std::cerr << "�޷����ļ�����д��" << std::endl;
    }
}

void MainMenuScene::onEnter() {
    cocos2d::Scene::onEnter();  // ȷ�����û����onEnter

    _firstinputMusicId = cocos2d::AudioEngine::play2d("music/ui_main_title_firstinput.mp3", false);

    // ������ֵ�״̬
    _backgroundMusicState = cocos2d::AudioEngine::getState(_backgroundMusicId);

    // �������û�в��Ż��߲����Ѿ���ɣ���ô��ʼ��������(mp3��ʽ)
    if (_backgroundMusicState != cocos2d::AudioEngine::AudioState::PLAYING) {
        _backgroundMusicId = cocos2d::AudioEngine::play2d("music/mainmenu_background_loop.mp3", true);
    }
}

void MainMenuScene::onExit() {

    cocos2d::Scene::onExit();  // ȷ�����û����onExit

    cocos2d::AudioEngine::stop(_firstinputMusicId);
    // ֹͣ��������
    AudioEngine::stop(_backgroundMusicId);  // ע������ĸ���
}
