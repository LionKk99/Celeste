#ifndef __MAIN_MENU_SCENE_H__
#define __MAIN_MENU_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"  // Ϊ��ʹ��UI���
#include "AudioEngine.h"
#include "audio/include/AudioEngine.h"

class MainMenuScene : public cocos2d::Scene
{
public:
    virtual bool init();
    CREATE_FUNC(MainMenuScene);

    void updateBackground(float dt); // ���±����ķ���
    void onEnter();
    void onExit();
    Scene* createScene();
    void initKeyboardListener();
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event);

private:

    int _backgroundMusicId;
    cocos2d::AudioEngine::AudioState _backgroundMusicState;
    int _firstinputMusicId;
    cocos2d::AudioEngine::AudioState _firstinputMusicState;

    cocos2d::Sprite* titleSprite;//����
    cocos2d::Sprite* mountainSprite;//ѩɽ:��ʾѡ��״̬
    cocos2d::Sprite* bg1;  // ����1
    cocos2d::Sprite* bg2;  // ����2
    cocos2d::Sprite* exitDialog;  //�˳�����
    cocos2d::ui::Button* newGame;//����Ϸ
    cocos2d::ui::Button* continueButton;//����������Ϸ
    cocos2d::ui::Button* setting;//����
    cocos2d::ui::Button* staff;//������Ա����л
    cocos2d::ui::Button* quit;//�˳���Ϸ 
    cocos2d::ui::Button* exitButton;//�˳���ť
    cocos2d::ui::Button* returnButton;//�������˵���ť
};
void write_to_file(int number);
int read_from_file();
#endif // __MAIN_MENU_SCENE_H__
