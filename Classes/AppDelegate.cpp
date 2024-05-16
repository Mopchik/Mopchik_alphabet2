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
#include "AudioHelper.h"
#include "GameScene.h"
#include "NewMainMenuScene.h"
#include "Utils.h"

USING_NS_CC;
cocos2d::Size AppDelegate::designResolutionSize = cocos2d::Size(960, 540);
cocos2d::Size AppDelegate::smallResolutionSize = cocos2d::Size(960, 540);
cocos2d::Size AppDelegate::mediumResolutionSize = cocos2d::Size(1920, 1080);
cocos2d::Size AppDelegate::largeResolutionSize = cocos2d::Size(3840, 2160);

AppDelegate::AppDelegate() = default;

AppDelegate::~AppDelegate()
{
    AudioHelper::end();
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
#if (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX || CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
        glview = GLViewImpl::createWithRect("Mopchik", cocos2d::Rect(0.0f, 0.0f, 1125.0f/4.0f, 2436.0f/4.0f));
#else
        glview = GLViewImpl::createWithFullScreen("Mopchik");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
#if !defined(COCOS2D_DEBUG) || COCOS2D_DEBUG == 0
    director->setDisplayStats(false);
#else
    director->setDisplayStats(true);
#endif

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);
    
    bool isLanscape = (glview->getFrameSize().width > glview->getFrameSize().height);
    auto frameSize = glview->getFrameSize();

    // Set the design resolution
    if (isLanscape) {
        glview->setDesignResolutionSize(AppDelegate::designResolutionSize.width, AppDelegate::designResolutionSize.height, this->getResolutionPolicy());
    } else {
        glview->setDesignResolutionSize(AppDelegate::designResolutionSize.height, AppDelegate::designResolutionSize.width, this->getResolutionPolicy());
    }
    
    auto fileUtils = FileUtils::getInstance();
    std::vector<std::string> searchPaths;
    
    if ( (isLanscape && frameSize.width > AppDelegate::mediumResolutionSize.width) || (!isLanscape && frameSize.height > AppDelegate::mediumResolutionSize.width) )
    {
        director->setContentScaleFactor(4.0f);
        searchPaths.emplace_back("res/large");
    }
    else if ( (isLanscape && frameSize.width > AppDelegate::smallResolutionSize.width) || (!isLanscape && frameSize.height > AppDelegate::smallResolutionSize.width) )
    {
        director->setContentScaleFactor(2.0f);
        searchPaths.emplace_back("res/medium");
    }
    else
    {
        director->setContentScaleFactor(1.0f);
        searchPaths.emplace_back("res/small");
    }
    
    CCLOG("content scale factor: %f", director->getContentScaleFactor());
    CCLOG("device dpi: %d", Device::getDPI());
    CCLOG("frame size: %f %f", frameSize.width, frameSize.height);
    
    fileUtils->setSearchPaths(searchPaths);
    this->loadSpriteSheets();
    this->preloadAudio();

    register_all_packages();

    director->setClearColor(Color4F::WHITE);
    
    this->firstRun();

    director->runWithScene(NewMainMenuScene::createScene());

    return true;
}

void AppDelegate::firstRun()
{
//    AdmobHelper::getInstance()->addDelegate(this);
    
    AudioHelper::getBackgroundSoundVolume();
    AudioHelper::getEffectsSoundsVolume();
}

void AppDelegate::loadSpriteSheets()
{
    ZipUtils::setPvrEncryptionKeyPart(0, 0x619bfb25);
    ZipUtils::setPvrEncryptionKeyPart(1, 0xd6286ddf);
    ZipUtils::setPvrEncryptionKeyPart(2, 0x9fd621ab);
    ZipUtils::setPvrEncryptionKeyPart(3, 0x5d2d9a3e);
    
    Utils::printTime("AppDelegate", "read plist start");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui-1.plist");
    Utils::printTime("AppDelegate", "read plist end");
}

void AppDelegate::preloadAudio()
{
    AudioHelper::preloadEffect("sound/click.mp3");
    AudioHelper::preloadEffect("sound/hint.mp3");
    AudioHelper::preloadEffect("sound/reveal_hints.mp3");
    AudioHelper::preloadEffect("sound/right_pair.mp3");
    AudioHelper::preloadEffect("sound/select_letter.mp3");
    AudioHelper::preloadEffect("sound/select_word.mp3");
    AudioHelper::preloadEffect("sound/win.mp3");
    AudioHelper::preloadEffect("sound/wrong_word.mp3");
    AudioHelper::preloadEffect("sound/m-1.mp3");
    AudioHelper::preloadEffect("sound/m-2.mp3");
    AudioHelper::preloadEffect("sound/m-3.mp3");
    AudioHelper::preloadEffect("sound/m-4.mp3");
    AudioHelper::preloadEffect("sound/m-5.mp3");
    AudioHelper::preloadEffect("sound/m-6.mp3");
    AudioHelper::preloadEffect("sound/m-7.mp3");
    AudioHelper::preloadEffect("sound/m-8.mp3");
    AudioHelper::preloadEffect("sound/m-9.mp3");
    AudioHelper::preloadEffect("sound/m-loose.mp3");
    AudioHelper::preloadEffect("sound/m-win.mp3");
    AudioHelper::preloadEffect("sound/nohint.mp3");
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    auto scene = dynamic_cast<GameScene *>(Director::getInstance()->getRunningScene());
    if (scene) {
        scene->applicationDidEnterBackground();
    }

    Director::getInstance()->stopAnimation();
    if (!UserDefault::getInstance()->getBoolForKey("sound_in_background", false))
        AudioHelper::pauseAll();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (UserDefault::getInstance()->getBoolForKey("sound_in_background", false)) {
        Director::getInstance()->getEventDispatcher()->setEnabled(false);
    }
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    auto scene = dynamic_cast<GameScene *>(Director::getInstance()->getRunningScene());
    if (scene) {
        scene->applicationWillEnterForeground();
    }

    Director::getInstance()->startAnimation();
    if (!UserDefault::getInstance()->getBoolForKey("sound_in_background", false)) {
        AudioHelper::resumeAll();
    }

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (UserDefault::getInstance()->getBoolForKey("sound_in_background", false)) {
        Director::getInstance()->getEventDispatcher()->setEnabled(true);
    }
#endif
}

ResolutionPolicy AppDelegate::getResolutionPolicy()
{
    ResolutionPolicy policy;

    auto screenSize = Director::getInstance()->getOpenGLView()->getFrameSize();

    if (screenSize.width > screenSize.height) policy = ResolutionPolicy::FIXED_HEIGHT;
    else if (screenSize.height > screenSize.width) policy = ResolutionPolicy::FIXED_WIDTH;
    else policy = ResolutionPolicy::SHOW_ALL;
    
    return policy;
}
