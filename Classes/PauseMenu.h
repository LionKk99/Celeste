#ifndef PAUSEMENU_H
#define PAUSEMENU_H
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "editor-support/cocostudio/SimpleAudioEngine.h"
#include "AudioEngine.h"
#include <fstream>
#include "Player/Player.h"
class PauseMenu :public cocos2d::Scene
{
public:
	virtual bool init();
	void ButtonCallBack(Ref* ref, cocos2d::ui::Widget::TouchEventType type);
	void SaveButtonCallBack(Ref* ref, cocos2d::ui::Widget::TouchEventType type);
	void ExitButtonCallBack(Ref* sender);
	void SaveFile(int k);
	CREATE_FUNC(PauseMenu);
};

class PauseOverlay :public cocos2d::Scene
{
public:
	virtual bool init();
	void onBackButtonClicked(Ref*);
	CREATE_FUNC(PauseOverlay);
};

#endif
/*class PauseOverlay : public cocos2d::Layer
{
public:
	virtual bool init() override;

	void setLabelText(const std::string& text);

	CREATE_FUNC(PauseOverlay);

private:
	void onConfirmButtonClicked(cocos2d::Ref* sender);
	void onCancelButtonClicked(cocos2d::Ref* sender);
};*/
