#ifndef __MAIN_MENU_SCENE_H__
#define __MAIN_MENU_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"  // 为了使用UI组件
#include "AudioEngine.h"
#include "audio/include/AudioEngine.h"

class MainMenuScene : public cocos2d::Scene
{
public:
    virtual bool init();
    CREATE_FUNC(MainMenuScene);

    void updateBackground(float dt); // 更新背景的方法
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

    cocos2d::Sprite* titleSprite;//标题
    cocos2d::Sprite* mountainSprite;//雪山:表示选中状态
    cocos2d::Sprite* bg1;  // 背景1
    cocos2d::Sprite* bg2;  // 背景2
    cocos2d::Sprite* exitDialog;  //退出界面
    cocos2d::ui::Button* newGame;//新游戏
    cocos2d::ui::Button* continueButton;//读档继续游戏
    cocos2d::ui::Button* setting;//设置
    cocos2d::ui::Button* staff;//工作人员及致谢
    cocos2d::ui::Button* quit;//退出游戏 
    cocos2d::ui::Button* exitButton;//退出按钮
    cocos2d::ui::Button* returnButton;//返回主菜单按钮
};
void write_to_file(int number);
int read_from_file();
#endif // __MAIN_MENU_SCENE_H__
