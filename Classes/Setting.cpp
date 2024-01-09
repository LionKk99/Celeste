#include"Setting.h"
#include"MainMenuScene.h"


bool Setting::init()
{
	if (!Scene::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//´´½¨±³¾°
	auto background = Sprite::create("mainMenu/setting.png");
	background->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	background->setPosition(origin.x, origin.y);
	auto contentSize = background->getContentSize();
	background->setScale(visibleSize.width / contentSize.width, visibleSize.height / contentSize.height);
	this->addChild(background);

	//back°´Å¥
	auto BackToMenuButton = cocos2d::ui::Button::create();
	BackToMenuButton->setTitleText("Back");
	BackToMenuButton->setTitleFontName("fonts/arial.ttf");
	BackToMenuButton->setTitleFontSize(50);
	BackToMenuButton->setTitleColor(Color3B::BLACK);
	BackToMenuButton->setScale(1.0f);
	BackToMenuButton->addTouchEventListener(CC_CALLBACK_2(Setting::buttonCallBack, this));
	BackToMenuButton->setPosition(Vec2(origin.x + visibleSize.width * 2222 / 2560, origin.y + 1440 - 1292));
	this->addChild(BackToMenuButton);

}

void Setting::buttonCallBack(Ref* ref, cocos2d::ui::Widget::TouchEventType type)
{
	switch (type) {
	case Widget::TouchEventType::ENDED:
	{
		auto scene = MainMenuScene::create();
		Director::getInstance()->replaceScene(TransitionFade::create(1.0, scene));
		break;
	}
	default:
		break;
	}
}