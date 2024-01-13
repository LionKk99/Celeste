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
	auto background = Sprite::create("mainMenu/quitWindow_00.png");
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

	auto png1 = Sprite::create("Setting/up.png");
	png1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	png1->setPosition(300, 400);
	png1->setScale(0.8f);
	this->addChild(png1);

	auto png2 = Sprite::create("Setting/left.png");
	png2->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	png2->setPosition(150, 270);
	png2->setScale(0.8f);
	this->addChild(png2);

	auto png3 = Sprite::create("Setting/down.png");
	png3->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	png3->setPosition(300, 270);
	png3->setScale(0.8f);
	this->addChild(png3);

	auto png4 = Sprite::create("Setting/right.png");
	png4->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	png4->setPosition(430, 270);
	png4->setScale(0.8f);
	this->addChild(png4);

	auto png5 = Sprite::create("Setting/shift.png");
	png5->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	png5->setPosition(600, 270);
	png5->setScale(0.8f);
	this->addChild(png5);

	auto png6 = Sprite::create("Setting/space.png");
	png6->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	png6->setPosition(725, 400);
	png6->setScale(0.8f);
	this->addChild(png6);

	auto png7 = Sprite::create("Setting/k.png");
	png7->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	png7->setPosition(950, 270);
	png7->setScale(0.8f);
	this->addChild(png7);

	auto label1 = Label::createWithTTF("UP", "fonts/gill-sans-mt-condensed/Gill Sans MT.ttf", 34);
	label1->setPosition(Vec2(370, 550));
	label1->setColor(Color3B::BLACK);
	this->addChild(label1);

	auto label2 = Label::createWithTTF("LEFT", "fonts/gill-sans-mt-condensed/Gill Sans MT.ttf", 34);
	label2->setPosition(Vec2(240, 250));
	label2->setColor(Color3B::BLACK);
	this->addChild(label2);

	auto label3 = Label::createWithTTF("DOWN", "fonts/gill-sans-mt-condensed/Gill Sans MT.ttf", 34);
	label3->setPosition(Vec2(385, 250));
	label3->setColor(Color3B::BLACK);
	this->addChild(label3);

	auto label4 = Label::createWithTTF("RIGHT", "fonts/gill-sans-mt-condensed/Gill Sans MT.ttf", 34);
	label4->setPosition(Vec2(525, 250));
	label4->setColor(Color3B::BLACK);
	this->addChild(label4);

	auto label5 = Label::createWithTTF("DASH", "fonts/gill-sans-mt-condensed/Gill Sans MT.ttf", 34);
	label5->setPosition(Vec2(725, 250));
	label5->setColor(Color3B::RED);
	this->addChild(label5);

	auto label6 = Label::createWithTTF("JUMP", "fonts/gill-sans-mt-condensed/Gill Sans MT.ttf", 34);
	label6->setPosition(Vec2(865, 560));
	label6->setColor(Color3B::RED);
	this->addChild(label6);

	auto label7 = Label::createWithTTF("CLIMB", "fonts/gill-sans-mt-condensed/Gill Sans MT.ttf", 34);
	label7->setPosition(Vec2(1030, 250));
	label7->setColor(Color3B::RED);
	this->addChild(label7);
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