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
	//创建背景
	Sprite* bgLayer = Sprite::create("Staff/quitWindow_00.png");
	bgLayer->setScale(1.0f);
	bgLayer->setAnchorPoint(Vec2(0.0, 0.0));
	bgLayer->setPosition(Vec2(100, 0));
	this->addChild(bgLayer);

	//back按钮
	auto BackToMenuButton = ui::Button::create();
	BackToMenuButton->setTitleText("Next Page");
	BackToMenuButton->setTitleFontName("fonts/gill-sans-mt-condensed/Gill Sans MT.ttf");
	BackToMenuButton->setTitleFontSize(45);
	BackToMenuButton->setTitleColor(Color3B::BLACK);
	BackToMenuButton->setScale(1.0f);
	BackToMenuButton->addTouchEventListener(CC_CALLBACK_2(Credits::buttonCallBack, this));
	BackToMenuButton->setPosition(Vec2(1000, 100));
	this->addChild(BackToMenuButton);

	/*auto label1 = Label::createWithTTF("Staff", "fonts/arial.ttf", 50);
	label1->setPosition(Vec2(400, 500));
	label1->setColor(Color3B::BLACK);
	this->addChild(label1);*/

	/*auto label2 = Label::createWithTTF("Specially thanks to", "fonts/gill-sans-mt-condensed/Gill Sans MT.ttf", 50);
	label2->setPosition(Vec2(600, 300));
	label2->setColor(Color3B::BLACK);
	this->addChild(label2);*/

	auto staff1 = Sprite::create("Staff/Staff1.png");
	staff1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	staff1->setPosition(200, 450);
	staff1->setScale(0.7f);
	this->addChild(staff1);

	auto staff2 = Sprite::create("Staff/Staff2.png");
	staff2->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	staff2->setPosition(200, 270);
	staff2->setScale(0.7f);
	this->addChild(staff2);

	auto staff3 = Sprite::create("Staff/Staff3.png");
	staff3->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	staff3->setPosition(200, 90);
	staff3->setScale(0.7f);
	this->addChild(staff3);

	// Staff 1的姓名和学号
	auto staff1Label = Label::createWithTTF("name", "fonts/gill-sans-mt-condensed/Gill Sans MT.ttf", 24);
	staff1Label->setColor(Color3B::BLACK);
	staff1Label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	staff1Label->setPosition(300, 450);
	this->addChild(staff1Label);

	// Staff 2的姓名和学号
	auto staff2Label = Label::createWithTTF("姓名 学号", "fonts/gill-sans-mt-condensed/Gill Sans MT.ttf", 24);
	staff2Label->setColor(Color3B::BLACK);
	staff2Label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	staff2Label->setPosition(300, 270);
	this->addChild(staff2Label);

	// Staff 3的姓名和学号
	auto staff3Label = Label::createWithTTF("姓名 学号", "fonts/gill-sans-mt-condensed/Gill Sans MT.ttf", 24);
	staff3Label->setColor(Color3B::BLACK);
	staff3Label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	staff3Label->setPosition(300,90);
	this->addChild(staff3Label);






	/*auto label3 = Label::createWithTTF("Staff", "fonts/Marker Felt.ttf", 50);
	label3->setPosition(Vec2(600, 400));
	this->addChild(label3);*/

}

void Credits::buttonCallBack(Ref* ref, cocos2d::ui::Widget::TouchEventType type)
{
	switch (type) {
	case Widget::TouchEventType::ENDED:
	{
		auto scene = CreditsOverlay::create();
		Director::getInstance()->replaceScene(TransitionFade::create(1.0, scene));
		break;
	}
	default:
		break;
	}


}

bool CreditsOverlay::init()
{
	if (!Scene::init())
	{
		return false;
	}
	Sprite* bgLayer = Sprite::create("Staff/quitWindow_00.png");
	bgLayer->setScale(1.0f);
	bgLayer->setAnchorPoint(Vec2(0.0, 0.0));
	bgLayer->setPosition(Vec2(100, 0));
	this->addChild(bgLayer);


	auto BackToMenuButton = ui::Button::create();
	BackToMenuButton->setTitleText("Back");
	BackToMenuButton->setTitleFontName("fonts/gill-sans-mt-condensed/Gill Sans MT.ttf");
	BackToMenuButton->setTitleFontSize(45);
	BackToMenuButton->setTitleColor(Color3B::BLACK);
	BackToMenuButton->setScale(1.0f);
	BackToMenuButton->addTouchEventListener(CC_CALLBACK_2(CreditsOverlay::buttonCallBack, this));
	BackToMenuButton->setPosition(Vec2(1100, 100));
	this->addChild(BackToMenuButton);

	auto thanks1 = Sprite::create("Staff/Thank1.jpg");
	thanks1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	thanks1->setPosition(200, 430);
	thanks1->setScale(0.25f);
	this->addChild(thanks1);

	auto thanks2 = Sprite::create("Staff/Thank2.jpg");
	thanks2->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	thanks2->setPosition(200, 200);
	thanks2->setScale(0.06f);
	this->addChild(thanks2);

	auto label2 = Label::createWithTTF("Specially thanks to", "fonts/gill-sans-mt-condensed/Gill Sans MT.ttf", 50);
	label2->setPosition(Vec2(600, 550));
	label2->setColor(Color3B::BLACK);
	this->addChild(label2);
}

void CreditsOverlay::buttonCallBack(Ref* ref, cocos2d::ui::Widget::TouchEventType type)
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