#include "NewMainMenuScene.h"
#include "AppDelegate.h"
#include "AudioHelper.h"
#include "WordGameScene.h"

USING_NS_CC;
using namespace ui;

#define LETTERS_COLOR Color3B(226U, 250U, 215U)
#define GUESSED_LETTER_COLOR Color3B(89U, 77U, 77U)
#define OPENED_LETTERS_COLOR Color3B(76U, 76U, 76U)
#define GUESSED_LETTER_ON_SETKA_COLOR Color3B(131U, 131U, 131U)

Scene* NewMainMenuScene::createScene()
{
    auto instance = new (std::nothrow) NewMainMenuScene();

    if (instance->init()) {
        instance->autorelease();
        return instance;
    }

    CC_SAFE_DELETE(instance);
    return nullptr;
}

// on "init" you need to initialize your instance
bool NewMainMenuScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    AdmobHelper::getInstance()->setDelegate(this);
    
    _wordGameController = WordGameController::getInstance();
    if (!_wordGameController)
        return false;
    
    //fix background scale
    auto background = Sprite::createWithSpriteFrameName("main_background.png");
    if (!background) {
        return false;
    }
    //fix _background scale
    auto backgroundOriginalSize = background->getContentSize();
    auto widthScale = (VIEW_SIZE).width / backgroundOriginalSize.width;
    auto heightScale = (VIEW_SIZE).height / backgroundOriginalSize.height;
    auto backgroundScale = std::max(widthScale, heightScale);
    background->setScale(backgroundScale);
    background->setLocalZOrder(0);
    this->addChild(background, -1);
    this->setContentSize(VIEW_SIZE);
    background->setPosition(VIEW_SIZE.width * 0.5f, VIEW_SIZE.height * 0.5f);
    
    bool isSoundOn = AudioHelper::getEffectsSoundsVolume() > 0;
    _soundOn = Sprite::createWithSpriteFrameName("sound_on.png");
    _soundOn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->addChild(_soundOn, 1);
    _soundOff = Sprite::createWithSpriteFrameName("sound_off.png");
    _soundOff->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->addChild(_soundOff, 1);
    _soundOn->setVisible(isSoundOn);
    _soundOff->setVisible(!isSoundOn);
    setClickListener(_soundOn, [this]()->void{
        if(_soundOn->isVisible()){
            _soundOn->setVisible(false);
            _soundOff->setVisible(true);
        } else {
            _soundOff->setVisible(false);
            _soundOn->setVisible(true);
        }
        AudioHelper::muteEffectsSounds();
    });

    bool isMusicOn = AudioHelper::getBackgroundSoundVolume() > 0;
    _musicOn = Sprite::createWithSpriteFrameName("music_on.png");
    _musicOn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->addChild(_musicOn, 1);
    _musicOff = Sprite::createWithSpriteFrameName("music_off.png");
    _musicOff->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->addChild(_musicOff, 1);
    _musicOn->setVisible(isMusicOn);
    _musicOff->setVisible(!isMusicOn);
    setClickListener(_musicOn, [this]()->void{
        if(_musicOn->isVisible()){
            _musicOn->setVisible(false);
            _musicOff->setVisible(true);
        } else {
            _musicOff->setVisible(false);
            _musicOn->setVisible(true);
        }
        AudioHelper::muteBackgroundSound();
    });
    
    _videoButton = Sprite::createWithSpriteFrameName("video.png");
    _videoButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->addChild(_videoButton, 1);
    setClickListener(_videoButton, [this]()->void{
        if(_videoButton->isVisible()){
            AdmobHelper::getInstance()->showRewardVideo();
        }
    });
    
    _playButton = ui::Button::create("button_continue.png", "button_continue_tapped.png", "button_continue.png", ui::Widget::TextureResType::PLIST);
    _playButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _playButton->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _playButton->addClickEventListener(
        [this](Ref* sender) {
            AudioHelper::playEffectSound("sound/click.mp3");
            Director::getInstance()->pushScene(WordGameScene::createScene());
        }
    );
    this->addChild(_playButton);
    
    _playButtonLabel = Label::createWithTTF("ИГРАТЬ", "fonts/NotoSans-Bold.ttf", 25);
    _playButtonLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _playButtonLabel->setIgnoreAnchorPointForPosition(false);
    _playButtonLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _playButtonLabel->setColor(LETTERS_COLOR);
    _playButton->addChild(_playButtonLabel);
    
    _pearlsButton = ui::Button::create("pearls_big.png", "pearls_big.png", "pearls_big.png", ui::Widget::TextureResType::PLIST);
    _pearlsButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->addChild(_pearlsButton, 10);

    _availablePearls = UserDefault::getInstance()->getIntegerForKey("available_pearls", 100);
    _pearlsButtonLabel = Label::createWithTTF(std::to_string(_availablePearls), "fonts/NotoSans-Bold.ttf", 20);
    _pearlsButtonLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _pearlsButtonLabel->setIgnoreAnchorPointForPosition(false);
    _pearlsButtonLabel->setNormalizedPosition(Vec2(0.05f, 0.5f));
    _pearlsButtonLabel->setColor(LETTERS_COLOR);
    _pearlsButton->addChild(_pearlsButtonLabel);
    _pearlsButton->addClickEventListener([](Ref* pSender) {
        AudioHelper::playEffectSound("sound/click.mp3");
        // Director::getInstance()->pushScene(NewShopMenuScene::createScene());
        });
    
    _guessedLetterBackground = Sprite::createWithSpriteFrameName("circles_next_letter.png");
    _guessedLetterBackground->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _guessedLetterBackground->setLocalZOrder(10);
    this->addChild(_guessedLetterBackground);
    
    _guessedLetter = Label::createWithTTF("",
                                          "fonts/NotoSans-Bold.ttf",
                                          45,
                                          Size::ZERO,
                                          TextHAlignment::CENTER,
                                          TextVAlignment::CENTER);
    _guessedLetter->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _guessedLetter->setIgnoreAnchorPointForPosition(false);
    _guessedLetter->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _guessedLetter->setColor(GUESSED_LETTER_COLOR);
    _guessedLetterBackground->addChild(_guessedLetter);
    
    _percent = Label::createWithTTF(std::to_string(23), "fonts/NotoSans-Bold.ttf", 25);
    _percent->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _percent->setIgnoreAnchorPointForPosition(false);
    _percent->setColor(Color3B::WHITE);
    this->addChild(_percent);
    
    _alphabetBackground = Sprite::createWithSpriteFrameName("background_alphabet.png");
    _alphabetBackground->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _alphabetBackground->setLocalZOrder(1);
    this->addChild(_alphabetBackground);
    
    auto length = _alphabet.length();
    for(size_t i = 0; i < length; i++) {
        auto cell = Sprite::createWithSpriteFrameName("cell.png");
        cell->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        cell->setColor(Color3B::WHITE);
        cell->setLocalZOrder(1);
        _alphabetBackground->addChild(cell);
        
        auto label = Label::createWithTTF(_convert.to_bytes(_alphabet[i]), "fonts/NotoSans-Bold.ttf", 24);
        label->setHorizontalAlignment(TextHAlignment::CENTER);
        label->setVerticalAlignment(TextVAlignment::CENTER);
        label->setName("label");
        label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        label->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
        label->setIgnoreAnchorPointForPosition(false);
        label->setColor(OPENED_LETTERS_COLOR);
        label->setVisible(false);
        cell->addChild(label);
        _alphabetCells.emplace_back(cell);
    }
    
    setDimensions();
    
    return true;
}

void NewMainMenuScene::setDimensions() {
    auto topButtonsSize = Size(65.0f, 65.0f);
    auto topButtonsIndent = 7.0f;
    _pearlsButton->setContentSize(cocos2d::Size(topButtonsSize.width * 2.0f, topButtonsSize.height));
    auto pearlsButtonPosition = Vec2(VIEW_SIZE.width - topButtonsIndent - topButtonsSize.width, VIEW_SIZE.height - topButtonsIndent - topButtonsSize.height * 0.5f);
    _pearlsButton->setPosition(pearlsButtonPosition);
    
    auto topButtonsBetweenIndent = 35.f;
    auto soundButtonPosition = Vec2(topButtonsIndent + topButtonsSize.width * 0.5f, VIEW_SIZE.height - topButtonsIndent - topButtonsSize.height * 0.5f);
    auto musicButtonPosition = Vec2(soundButtonPosition.x + topButtonsSize.width / 2 + topButtonsBetweenIndent, soundButtonPosition.y);
    _soundOn->setContentSize(topButtonsSize);
    _soundOn->setPosition(soundButtonPosition);
    _soundOff->setContentSize(topButtonsSize);
    _soundOff->setPosition(soundButtonPosition);
    _musicOn->setContentSize(topButtonsSize);
    _musicOn->setPosition(musicButtonPosition);
    _musicOff->setContentSize(topButtonsSize);
    _musicOff->setPosition(musicButtonPosition);
    
    auto videoButtonPosition = Vec2(musicButtonPosition.x + topButtonsSize.width / 2 + topButtonsBetweenIndent, soundButtonPosition.y);
    _videoButton->setContentSize(topButtonsSize);
    _videoButton->setPosition(videoButtonPosition);
    
    auto cellOutsideIndent = 18.f;
    auto cellInnerIndent = 5.f;
    auto cellBackgroundLeftIndent = 30.f;
    auto cellBackgroundWidth = VIEW_SIZE.width - 2 * cellBackgroundLeftIndent;
    auto cellSize = (cellBackgroundWidth - 2 * cellOutsideIndent - 7 * cellInnerIndent) / 8;
    auto cellBackgroundHeight = 2 * cellOutsideIndent + 3 * cellInnerIndent + 4 * cellSize;
    if(cellBackgroundHeight > VIEW_SIZE.height / 3.6f) {
        cellBackgroundHeight = VIEW_SIZE.height / 3.6f;
        cellSize = (cellBackgroundHeight - 2 * cellOutsideIndent - 3 * cellInnerIndent) / 4;
        cellBackgroundWidth = 2 * cellOutsideIndent + 7 * cellInnerIndent + 8 * cellSize;
    }
    
    auto guessedLetterBackgroundSize = cellBackgroundHeight / 1.5f;
    auto guessedAlphabetIndent = 55.f;
    auto guessedPercentIndent = 10.f;
    auto playButtonHeight = 80.f;
    auto playButtonWidth = cellBackgroundWidth / 2;
    
    auto cellBackgroundPosY = VIEW_SIZE.height * 0.35f;
    auto guessedBacgkroundPosY = cellBackgroundPosY + cellBackgroundHeight / 2 + guessedAlphabetIndent + guessedLetterBackgroundSize / 2;
    
    _alphabetBackground->setPosition(Vec2(VIEW_SIZE.width / 2, cellBackgroundPosY));
    _alphabetBackground->setContentSize(Size(cellBackgroundWidth, cellBackgroundHeight));
    
    _playButton->setPosition(Vec2(VIEW_SIZE.width / 2, (cellBackgroundPosY - cellBackgroundHeight / 2) / 2));
    _playButton->setScale9Enabled(true);
    _playButton->Node::setContentSize(Size(playButtonWidth, playButtonHeight));
    
    _guessedLetterBackground->setContentSize(Size(guessedLetterBackgroundSize, guessedLetterBackgroundSize));
    _guessedLetterBackground->setPosition(Vec2(VIEW_SIZE.width / 2, guessedBacgkroundPosY));
    _percent->setPosition(Vec2(VIEW_SIZE.width / 2, guessedBacgkroundPosY + guessedLetterBackgroundSize / 2 + guessedPercentIndent));
    
    for(int i = 0; i < _alphabetCells.size(); ++i) {
        auto x = i % 8;
        auto y = i / 8;
        auto pos_x = cellOutsideIndent + x * cellSize + x * cellInnerIndent;
        auto pos_y = cellBackgroundHeight - cellOutsideIndent - y * cellSize - y * cellInnerIndent;
        _alphabetCells.at(i)->setContentSize(Size(cellSize, cellSize));
        _alphabetCells.at(i)->setPosition(Vec2(pos_x, pos_y));
    }
}

void NewMainMenuScene::setOpenLetters(){
    auto openedLetters = _wordGameController->getOpenLetters();
    auto guessedLetter = _convert.to_bytes(_wordGameController->getGuessedLetter());
    auto length = _alphabet.length();
    auto levelPassed = _wordGameController->levelPassed();
    
    if(_isFirstScreen) {
        _percent->setString(std::to_string(_wordGameController->getPercent()) + "%");
        _isFirstScreen = false;
    } else {
        _playButtonLabel->setString("ПРОДОЛЖИТЬ");
    }
    if(levelPassed) {
        auto changeString = CallFunc::create([this]() {
            _percent->setString(std::to_string(_wordGameController->getPercent()) + "%");
        });
        auto fadeOut = FadeOut::create(0.5f);
        auto fadeIn = FadeIn::create(0.5f);
        auto sequence = Sequence::create(fadeOut, changeString, fadeIn, nullptr);
        _percent->runAction(sequence);
    }
    if(_wordGameController->stagePassed()) {
        auto lastGuessedLetter = openedLetters.at(openedLetters.size() - 1);
        for(size_t i = 0; i < length; i++) {
            auto label = _alphabetCells.at(i)->getChildByName("label");
            auto findInd = openedLetters.find(_alphabet[i]);
            if(findInd != std::u16string::npos && _alphabet[i] != lastGuessedLetter) {
                label->setVisible(true);
            } else if(_alphabet[i] == lastGuessedLetter) {
                auto fadeIn = FadeIn::create(1.f);
                label->setOpacity(0);
                label->setVisible(true);
                label->runAction(fadeIn);
            }
        }
        auto changeString = CallFunc::create([this, guessedLetter]() {
            _guessedLetter->setString(guessedLetter);
        });
        auto fadeOut = FadeOut::create(0.5f);
        auto fadeIn = FadeIn::create(0.5f);
        auto sequence = Sequence::create(fadeOut, changeString, fadeIn, nullptr);
        _guessedLetter->runAction(sequence);
        
        AudioHelper::playEffectSound("sound/winning_awards.mp3");
    } else {
        for(size_t i = 0; i < length; i++) {
            auto label = _alphabetCells.at(i)->getChildByName("label");
            label->setVisible(openedLetters.find(_alphabet[i]) != std::u16string::npos);
        }
        _guessedLetter->setString(guessedLetter);
        if (levelPassed) {
            AudioHelper::playEffectSound("sound/win.mp3");
        }
    }
}


void NewMainMenuScene::setClickListener(cocos2d::Sprite* sprite, std::function<void()> func) {
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [sprite, func](Touch* touch, Event* event) ->bool {
        if (sprite->getBoundingBox().containsPoint(touch->getLocation())) {
            func();
            return true;
        }
        return false;
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, sprite);
}

void NewMainMenuScene::rewardVideoDidPresent()
{
    Director::getInstance()->stopAnimation();
    AudioHelper::pauseAll();
}

void NewMainMenuScene::rewardVideoDidDismiss()
{
    Director::getInstance()->startAnimation();
    AudioHelper::resumeAll();
}

void NewMainMenuScene::rewardVideoNotLoaded() {};

void NewMainMenuScene::rewardUser() {
    _availablePearls += 50;
    UserDefault::getInstance()->setIntegerForKey("available_pearls", _availablePearls);

    auto changeString = CallFunc::create([this]() {
        _pearlsButtonLabel->setString(std::to_string(_availablePearls));
    });
    auto fadeOut = FadeOut::create(0.5f);
    auto fadeIn = FadeIn::create(0.5f);
    auto sequence = Sequence::create(fadeOut, changeString, fadeIn, nullptr);
    _pearlsButtonLabel->runAction(sequence);
    AudioHelper::playEffectSound("sound/shell_shake.mp3");
    _videoButton->setVisible(AdmobHelper::getInstance()->isRewardLoaded());
}

void NewMainMenuScene::onEnter()
{
    Scene::onEnter();
    setOpenLetters();
    if (!AudioHelper::isBackgroundSoundPlaying()) {
        AudioHelper::playBackgroundSound();
    }
    _videoButton->setVisible(AdmobHelper::getInstance()->isRewardLoaded());
}

void NewMainMenuScene::onExit()
{
    Scene::onExit();
}

NewMainMenuScene::NewMainMenuScene()
        :  _playButton(nullptr) {}


