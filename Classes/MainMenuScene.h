#ifndef __MAIN_MENU_SCENE_H__
#define __MAIN_MENU_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"  // Ϊ��ʹ��UI���

class MainMenuScene : public cocos2d::Scene
{
public:
    virtual bool init();
    CREATE_FUNC(MainMenuScene);

    void updateBackground(float dt); // ���±����ķ���

private:
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

#endif // __MAIN_MENU_SCENE_H__
