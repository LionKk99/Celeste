#ifndef __MAIN_MENU_SCENE_H__
#define __MAIN_MENU_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"  // 为了使用UI组件

class MainMenuScene : public cocos2d::Scene
{
public:
    virtual bool init();
    CREATE_FUNC(MainMenuScene);

    void updateBackground(float dt); // 更新背景的方法

private:
    cocos2d::Sprite* titleSprite;//标题
    cocos2d::Sprite* mountainSprite;//雪山:表示选中状态
    cocos2d::Sprite* bg1;  // 背景1
    cocos2d::Sprite* bg2;  // 背景2
    cocos2d::ui::Button* newGame;//新游戏
    cocos2d::ui::Button* continueButton;//读档继续游戏
    cocos2d::ui::Button* setting;//设置
    cocos2d::ui::Button* staff;//工作人员及致谢
    cocos2d::ui::Button* quit;//退出游戏   
    float scaleX = 1280.0f / 1280.0f; // 宽度缩放比例
    float scaleY = 720.0f / 720.0f;  // 高度缩放比例
};

#endif // __MAIN_MENU_SCENE_H__
