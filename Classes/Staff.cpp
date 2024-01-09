#include"Staff.h"
#include"MainMenuScene.h"
Scene* Credits::createScene()
{
	return Credits::create();
}
bool Credits::init()
{
	if (!Scene::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//´´½¨±³¾°
	Sprite* bgLayer = Sprite::create("staff/quitWindow_00.png");
	bgLayer->setScale(1.0f);
	bgLayer->setAnchorPoint(Vec2(0.0, 0.0));
	bgLayer->setPosition(Vec2(100, 0));
	this->addChild(bgLayer);

	//back°´Å¥
	auto BackToMenuButton = ui::Button::create();
	BackToMenuButton->setTitleText("Back");
	BackToMenuButton->setTitleFontName("fonts/gill-sans-mt-condensed/Gill Sans MT.ttf");
	BackToMenuButton->setTitleFontSize(50);
	BackToMenuButton->setTitleColor(Color3B::BLACK);
	BackToMenuButton->setScale(1.0f);
	BackToMenuButton->addTouchEventListener(CC_CALLBACK_2(Credits::buttonCallBack, this));
	BackToMenuButton->setPosition(Vec2(1100,100));
	this->addChild(BackToMenuButton);

	auto label1 = Label::createWithTTF("Staff", "fonts/arial.ttf", 50);
	label1->setPosition(Vec2(640, 630));
	label1->setColor(Color3B::BLACK);
	this->addChild(label1);

	auto label2 = Label::createWithTTF("Specially thanks to", "fonts/gill-sans-mt-condensed/Gill Sans MT.ttf", 50);
	label2->setPosition(Vec2(600, 300));
	label2->setColor(Color3B::BLACK);
	this->addChild(label2);

	auto staff1 = Sprite::create("staff/Staff1.png");
	staff1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	staff1->setPosition(200,500);
	staff1->setScale(0.4f);
	this->addChild(staff1);

	auto staff2 = Sprite::create("staff/Staff2.png");
	staff2->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	staff2->setPosition(200, 415);
	staff2->setScale(0.4f);
	this->addChild(staff2);

	auto staff3 = Sprite::create("staff/Staff3.png");
	staff3->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	staff3->setPosition(200,330);
	staff3->setScale(0.4f);
	this->addChild(staff3);

	auto thanks1 = Sprite::create("staff/Thank1.jpg");
	thanks1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	thanks1->setPosition(200,150);
	thanks1->setScale(0.25f);
	this->addChild(thanks1);

	auto thanks2 = Sprite::create("staff/Thank2.jpg");
	thanks2->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	thanks2->setPosition(600, 150);
	thanks2->setScale(0.06f);
	this->addChild(thanks2);
	/*
	auto label = Label::createWithTTF("Junan Huo", "fonts/Marker Felt.ttf", 24);
	label->setPosition(Vec2(240, 160));
	this->addChild(label);

	auto label1 = Label::createWithTTF("Yuzhe Zhou", "fonts/arial.ttf", 24);
	label1->setPosition(Vec2(260, 180));
	this->addChild(label1);

	auto label2 = Label::createWithTTF("Sijia Gong", "fonts/Marker Felt.ttf", 24);
	label2->setPosition(Vec2(260, 180));
	this->addChild(label2);

	auto label3 = Label::createWithTTF("Staff", "fonts/Marker Felt.ttf", 50);
	label3->setPosition(Vec2(600, 400));
	this->addChild(label3);*/

}

void Credits::buttonCallBack(Ref* ref, cocos2d::ui::Widget::TouchEventType type)
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