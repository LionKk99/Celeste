/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "AppDelegate.h"
#include "MainMenuScene.h"
#include "Scene/Level1Scene.h"



// #define USE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#endif

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(1280, 720);//设置分辨率
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#endif
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil,multisamplesCount
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 0};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("Celeste", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("Celeste");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    // Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
    auto frameSize = glview->getFrameSize();
    // if the frame's height is larger than the height of medium size.
    if (frameSize.height > mediumResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is larger than the height of small size.
    else if (frameSize.height > smallResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is smaller than the height of medium size.
    else
    {        
        director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
    }

    register_all_packages();        
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("movement/crouch/crouch_00.plist", "movement/crouch/crouch_00.png");//蹲姿加载
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("movement/crouch/CrouchToIdle.plist", "movement/crouch/CrouchToIdle.png");//蹲姿到静止加载
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("movement/crouch/CrouchToMove.plist", "movement/crouch/CrouchToMove.png");//蹲姿到移动加载

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("movement/dash/dashdown.plist", "movement/dash/dashdown.png");//向下冲刺加载
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("movement/dash/dashmove.plist", "movement/dash/dashmove.png");//移动冲刺加载
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("movement/dash/dashup.plist", "movement/dash/dashup.png");//向上冲刺加载
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("movement/death/Bdeath.plist", "movement/death/Bdeath.png");//B死亡
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("movement/death/death.plist", "movement/death/death.png");//死亡
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("movement/death/Respawn.plist", "movement/death/Respawn.png");//重生

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("movement/drop/drop0.plist", "movement/drop/drop0.png");//向下坠落加载
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("movement/drop/drop1.plist", "movement/drop/drop1.png");//移动坠落加载
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("movement/drop/Bdrop0.plist", "movement/drop/Bdrop0.png");//B向下坠落加载
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("movement/drop/Bdrop1.plist", "movement/drop/Bdrop1.png");//B移动坠落加载

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("movement/holdwall/holdwall.plist", "movement/holdwall/holdwall.png");//爬墙加载
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("movement/holdwall/Bholdwall.plist", "movement/holdwall/Bholdwall.png");//B爬墙加载
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("movement/holdwall/holdwallup.plist", "movement/holdwall/holdwallup.png");//爬墙向上加载
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("movement/holdwall/Bholdwallup.plist", "movement/holdwall/Bholdwallup.png");//B爬墙向上加载
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("movement/holdwall/holdwalldown.plist", "movement/holdwall/holdwalldown.png");//爬墙向下加载
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("movement/holdwall/Bholdwalldown.plist", "movement/holdwall/Bholdwalldown.png");//B爬墙向下加载
    //可加爬墙turnface

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("movement/idle/idle_0.plist", "movement/idle/idle_0.png");//站立1加载
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("movement/idle/Idle1.plist", "movement/idle/Idle1.png");//站立2加载

    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("movement/jump/jumpup.plist", "movement/jump/jumpup.png");//跳跃加载
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("movement/jump/jumpmove.plist", "movement/jump/jumpmove.png");//跳跃移动加载
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("movement/top/Top0.plist", "movement/top/Top0.png");//跳跃补全

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("movement/jump/Bjumpup.plist", "movement/jump/Bjumpup.png");//B跳跃加载
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("movement/jump/Bjumpmove.plist", "movement/jump/Bjumpmove.png");//B跳跃移动加载

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("movement/landing/landing0.plist", "movement/landing/landing0.png");//落地加载
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("movement/landing/landing1.plist", "movement/landing/landing1.png");//移动落地加载

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("movement/lookup/LookUp.plist", "movement/lookup/LookUp.png");//向上看加载

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("movement/move/Move.plist", "movement/move/Move.png");//移动加载
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("movement/move/MoveToCrouch.plist", "movement/move/MoveToCrouch.png");//移动到蹲加载
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("movement/move/MoveTurn.plist", "movement/move/MoveTurn.png");//移动转向加载

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("movement/pushwall/pushwall.plist", "movement/pushwall/pushwall.png");//推墙加载

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("movement/shadow/shadow.plist", "movement/shadow/shadow.png");//残影加载

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("movement/slidingwall/slidingwall.plist", "movement/slidingwall/slidingwall.png");//滑落加载
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("movement/slidingwall/Bslidingwall.plist", "movement/slidingwall/Bslidingwall.png");//滑落加载

    
   //测试plist和png加载
    /*
    Vector<SpriteFrame*> idleFrames;
    auto cache = SpriteFrameCache::getInstance();
    for (int i = 0; i <= 5; i++) {
        std::string frameName = StringUtils::format("landing_00-%d.png", i);
        auto frame = cache->getSpriteFrameByName(frameName);
        if (frame) {
            idleFrames.pushBack(frame);
            CCLOG("Loaded frame: %s", frameName.c_str());
        }
        else {
            CCLOG("Error: Cannot find frame: %s", frameName.c_str());
        }
    }
*/
      
    
    
    
    

    // create a scene. it's an autorelease object//生成主界面场景
    //auto scene = MainMenuScene::create();
    //director->runWithScene(scene);

    //测试第一关
    auto scene = Level1Scene::createScene();
    Director::getInstance()->replaceScene(scene);

    

    // create a scene. it's an autorelease object
    //auto scene = HelloWorld::createScene();
    // run
    //director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#endif
}
