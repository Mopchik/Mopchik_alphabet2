#include "WordGameScene.h"
#include "AppDelegate.h"
#include "AudioHelper.h"

USING_NS_CC;

#define SELECT_COLOR Color3B(56U, 144U, 59U)
#define CIRCLE_COLOR Color3B(69U, 203U, 74U)
#define LETTERS_COLOR Color3B(226U, 250U, 215U)
#define CELL_COLOR Color3B(226U, 250U, 215U)
#define ENTER_COLOR Color3B(56U, 144U, 59U)
#define ENTER_LETTERS_COLOR Color3B(226U, 250U, 215U)
#define SETKA_LETTERS_COLOR Color3B(89U, 77U, 77U)

WordGameScene::WordGameScene() :
        _background(nullptr),
        _backButton(nullptr),
        _pearlsButton(nullptr),
        _pearlsButtonLabel(nullptr),
        _availablePearls(0),
        _topButtonsSize(cocos2d::Size(65.0f, 65.0f)),
//        _topButtonsIndent(5.0f),
        _topButtonsIndent(7.0f),
        _buildingLine(nullptr),
        _enterRectangle(nullptr),
        _reshuffleButton(nullptr),
        _hintButton(nullptr),
        _fontSize(0.0f),
        _isControllerAvailable(true){}

WordGameScene::~WordGameScene()
{
    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this, true);
}

Scene* WordGameScene::createScene()
{
    auto instance = new (std::nothrow) WordGameScene();

    if (instance->initWord()) {
        instance->autorelease();
        return instance;
    }

    CC_SAFE_DELETE(instance);
    return nullptr;
}

// on "init" you need to initialize your instance
bool WordGameScene::initWord() {
    if (!Scene::init()) {
        return false;
    }

    _wordGameController = WordGameController::getInstance();
    if (!_wordGameController)
        return false;

    //set background
    _background = Sprite::createWithSpriteFrameName("main_background.png");
    if (!_background) {
        return false;
    }
    //fix _background scale
    auto backgroundOriginalSize = _background->getContentSize();
    auto widthScale = (VIEW_SIZE).width / backgroundOriginalSize.width;
    auto heightScale = (VIEW_SIZE).height / backgroundOriginalSize.height;
    auto backgroundScale = std::max(widthScale, heightScale);
    _background->setScale(backgroundScale);
    _background->setLocalZOrder(0);
    this->addChild(_background, -1);

    _availablePearls = UserDefault::getInstance()->getIntegerForKey("available_pearls", 100);

    _backButton = ui::Button::create("back.png", "back.png", "back.png", ui::Widget::TextureResType::PLIST);
    _backButton->addClickEventListener([this] (Ref *pSender) {
        AudioHelper::playEffectSound("sound/click.mp3");
        Director::getInstance()->popScene();
    });
    _backButton->setContentSize(_topButtonsSize);
    _backButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _backButton->setPosition(Vec2(_topButtonsIndent + _topButtonsSize.width * 0.5f, VIEW_SIZE.height - _topButtonsIndent - _topButtonsSize.height * 0.5f));
    this->addChild(_backButton, 10);

    _pearlsButton = ui::Button::create("pearls_big.png", "pearls_big.png", "pearls_big.png", ui::Widget::TextureResType::PLIST);
    _pearlsButton->setContentSize(cocos2d::Size(_topButtonsSize.width * 2.0f, _topButtonsSize.height));
    _pearlsButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _pearlsButton->setPosition(Vec2(VIEW_SIZE.width - _topButtonsIndent - _topButtonsSize.width, VIEW_SIZE.height - _topButtonsIndent - _topButtonsSize.height * 0.5f));
    this->addChild(_pearlsButton, 10);

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

    _wordGameBackground = Sprite::createWithSpriteFrameName("white_circle.png");
    _wordGameBackground->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _wordGameBackground->setColor(CIRCLE_COLOR);
    _wordGameBackground->setLocalZOrder(1);
    this->addChild(_wordGameBackground);
    
    // initWordCards();
    initHintButton();
    initReshuffleButton();
    
    auto lettersCount = _wordGameController->getLettersCount();
    for (size_t i = 0; i < lettersCount; ++i) {
        auto circle = Sprite::createWithSpriteFrameName("white_circle.png");
        circle->setTag(1);
        circle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        circle->setIgnoreAnchorPointForPosition(false);
        circle->setColor(CIRCLE_COLOR);
        circle->setLocalZOrder(1);
        circle->setOpacity(0.0f);
        _wordGameBackground->addChild(circle);
        auto label = Label::createWithTTF(_wordGameController->getLetterAt(i), "fonts/NotoSans-Bold.ttf", _fontSize);
        CCLOG("Letter:%s", _wordGameController->getLetterAt(i).c_str());
        label->setHorizontalAlignment(TextHAlignment::CENTER);
        label->setVerticalAlignment(TextVAlignment::CENTER);
        label->setName("letter_label");
        label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        label->setIgnoreAnchorPointForPosition(false);
        label->setColor(LETTERS_COLOR);
        circle->addChild(label);
        _circlesVector.emplace_back(circle);
    }

    setDimensionsAndPositions();
//    setWordToBackground(_wordGameController->getWord(), _wordBackgrounds.at(0));
    initSetka();

    auto circleWordListener = EventListenerTouchOneByOne::create();

    circleWordListener->onTouchBegan = [this](Touch* touch, Event* event) ->bool {
        if (!_isControllerAvailable) {
            return false;
        }
        std::for_each(_circlesVector.begin(), _circlesVector.end(), [this, touch](cocos2d::Sprite* firstCircle) {
            if (isInsideCircle(firstCircle, _wordGameBackground->convertToNodeSpace(touch->getLocation()))) {
                _isControllerAvailable = false;
                _reshuffleButton->setEnabled(false);
                _hintButton->setEnabled(false);

                auto firstCircleCenter = firstCircle->getPosition();
                _buildingLine = createEmptyLine(firstCircleCenter);
                _wordGameBackground->addChild(_buildingLine);
                _currentSequence.push_back(firstCircle);
                firstCircle->setColor(SELECT_COLOR);
                firstCircle->setOpacity(255.0f);

                auto enterRectangleX = VIEW_SIZE.width / 2;
                auto enterRectangleY = _wordGameBackground->getPosition().y + _wordGameBackground->getContentSize().height / 2 + _verticalIndent;
                _enterRectangle = createEnterRectangle(Vec2(enterRectangleX, enterRectangleY));
                updateEnterRectangleText();
                this->addChild(_enterRectangle);

                auto fadeOut = FadeOut::create(0.2f);
                _reshuffleButton->runAction(fadeOut);

                AudioHelper::playEffectSound("sound/select_letter.mp3");
            }
            });
        return true;
    };

    circleWordListener->onTouchMoved = [this](Touch* touch, Event* event) {
        if (!_buildingLine) {
            return;
        }
        setLineCoordinates(_buildingLine, _wordGameBackground->convertToNodeSpace(touch->getLocation()));
        std::for_each(_circlesVector.begin(), _circlesVector.end(), [this, touch](cocos2d::Sprite* nextCircle) {
            if (isInsideCircle(nextCircle, _wordGameBackground->convertToNodeSpace(touch->getLocation()))) {
                auto it = std::find(_currentSequence.begin(), _currentSequence.end(), nextCircle);
                if (it == _currentSequence.end()) {
                    auto nextCircleCenter = nextCircle->getPosition();
                    setLineCoordinates(_buildingLine, nextCircleCenter);
                    _lines.push_back(_buildingLine);
                    _buildingLine = createEmptyLine(nextCircleCenter);
                    _wordGameBackground->addChild(_buildingLine);
                    _currentSequence.push_back(nextCircle);
                    nextCircle->setColor(SELECT_COLOR);
                    nextCircle->setOpacity(255.0f);
                    updateEnterRectangleText();
                    AudioHelper::playEffectSound("sound/select_letter.mp3");
                }
            }
            });
    };

    circleWordListener->onTouchEnded = [this](Touch* touch, Event* event) {
//        auto word = _wordGameController->getWord();
        _isControllerAvailable = true;
        auto first = getCurrentSequenceText();
        auto words = _wordGameController->getWords();
        bool any_good = false;
        for(int i = 0; i < words.size(); ++i){
            if(first == words[i]->getWordAnswer() and not _wordGameController->isWordSolved(i)) {
                auto word_x = words[i]->getPosX();
                auto word_y = words[i]->getPosY();
                for(int j = 0; j < words[i]->getWordAnswerLength(); j++) {
                    _setka[word_x][word_y]->getChildByName("label")->setVisible(true);
                    if(words[i]->isHorizontal()) {
                        word_x++;
                    } else {
                        word_y++;
                    }
                }
                any_good = true;
                _wordGameController->wordSolved(i);
                if(_wordGameController->allWordsSolved()) {
                    _wordGameController->onRoundCompleted();
                    Director::getInstance()->popScene();
                }
            }
        }
        if(any_good) {
            AudioHelper::playEffectSound("sound/right_pair.mp3");
        } else {
            AudioHelper::playEffectSound("sound/wrong_word.mp3");
        }
        _isControllerAvailable = true;
        _reshuffleButton->setEnabled(true);
        _hintButton->setEnabled(true);
        _lines.push_back(_buildingLine);
        std::for_each(_lines.begin(), _lines.end(), [this](cocos2d::Sprite* line) {
            _wordGameBackground->removeChild(line, true);
            });
        std::for_each(_currentSequence.begin(), _currentSequence.end(),
            [](cocos2d::Sprite* circle) {
                circle->setColor(Color3B(255U, 255U, 255U));
                circle->setOpacity(0.0f);
            }
        );
        _buildingLine = nullptr;
        _lines.clear();
        _currentSequence.clear();
        this->removeChild(_enterRectangle, true);
        _enterRectangle = nullptr;
        auto fadeIn = FadeIn::create(0.2f);
        _reshuffleButton->runAction(fadeIn);
    };

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(circleWordListener, _wordGameBackground);

    return true;
}

void WordGameScene::applicationDidEnterBackground()
{
    // _wordGameController->saveProgressToFile();
}

void WordGameScene::setDimensionsAndPositions()
{
    this->setContentSize(VIEW_SIZE);
    _background->setPosition(VIEW_SIZE.width * 0.5f, VIEW_SIZE.height * 0.5f);

    auto backButtonPosition = Vec2(_topButtonsIndent + _topButtonsSize.width * 0.5f, VIEW_SIZE.height - _topButtonsIndent - _topButtonsSize.height * 0.5f);
    _backButton->setPosition(backButtonPosition);

    auto pearlsButtonPosition = Vec2(VIEW_SIZE.width - _topButtonsIndent - _topButtonsSize.width, VIEW_SIZE.height - _topButtonsIndent - _topButtonsSize.height * 0.5f);
    _pearlsButton->setPosition(pearlsButtonPosition);

    auto lettersCount = _wordGameController->getLettersCount();

    auto minBetweenIndent = 20.0f;
    auto outsideIndent = 15.0f;
    auto indent = 80.0f;
    auto centerIndent = 15.0f;
    _setkaOutsideIndent = 30.0f;
    _verticalIndent = 20.0f;
    _setkaInnerIndent = 3.0f;

    auto wordGameBackgroundSize = VIEW_SIZE.width - indent * 2;
    auto setkaHeightCoef = 0.7f;
    _setkaHeight = wordGameBackgroundSize / setkaHeightCoef;

    _enterRectangleHeight = wordGameBackgroundSize / 6.24f;

    auto wordGameBackgroundPosition = Vec2(VIEW_SIZE.width * 0.5, VIEW_SIZE.height * 0.3);

    auto minIndentBetweenTopAndWordCards = _topButtonsSize.height + _topButtonsIndent + _verticalIndent;

    if (wordGameBackgroundPosition.y + wordGameBackgroundSize / 2 + _verticalIndent + _enterRectangleHeight +
        _verticalIndent + _setkaHeight > VIEW_SIZE.height - minIndentBetweenTopAndWordCards) {
        wordGameBackgroundSize = (VIEW_SIZE.height - minIndentBetweenTopAndWordCards - 2 * _verticalIndent - wordGameBackgroundPosition.y) / (1 / 2.0f + 1 / 6.24f + 1 / setkaHeightCoef);
        _enterRectangleHeight = wordGameBackgroundSize / 6.24f;
        _setkaHeight = wordGameBackgroundSize / setkaHeightCoef;
    }

//    auto buttonSize = wordGameBackgroundSize / 3.5f;
    auto buttonSize = _topButtonsSize.width;
    auto hintButtonPosition = Vec2(
        2 * _setkaOutsideIndent,
        wordGameBackgroundPosition.y - wordGameBackgroundSize / 2 - _verticalIndent
    );

    auto reshuffleSize = wordGameBackgroundSize / 5;
    _letterCircleRadius = (wordGameBackgroundSize / 2 - outsideIndent - centerIndent - reshuffleSize * std::sqrtf(2) / 2) / 2;
    auto distanceBetweenBigAndLetterCenters = wordGameBackgroundSize / 2 - outsideIndent - _letterCircleRadius;
    auto L = 2 * M_PI / lettersCount;
    auto distanceBetweenLetterCenters = std::sqrtf(2 * std::powf(distanceBetweenBigAndLetterCenters, 2) * (1 - std::cosf(L)));
    if (distanceBetweenLetterCenters - 2 * _letterCircleRadius < minBetweenIndent) {
        _letterCircleRadius = (2 * (wordGameBackgroundSize / 2 - outsideIndent) * (std::sqrtf(2 * (1 - std::cosf(L))) + std::cosf(L) - 1) - minBetweenIndent * (2 - 2 * std::sqrtf(1 - std::cosf(L)))) / (2 * (1 + std::cosf(L)));
        distanceBetweenBigAndLetterCenters = wordGameBackgroundSize / 2 - outsideIndent - _letterCircleRadius;
    }

    _fontSize = _letterCircleRadius * 5.0f / 3;

    _wordGameBackground->setContentSize(cocos2d::Size(wordGameBackgroundSize, wordGameBackgroundSize));
    _wordGameBackground->setPosition(wordGameBackgroundPosition);
    _reshuffleButton->setScale9Enabled(true);
    _reshuffleButton->setScale(reshuffleSize / _reshuffleButton->getContentSize().width);

    for (size_t i = 0; i < lettersCount; ++i) {
        auto x = std::cosf(M_PI / 2 + L * i) * distanceBetweenBigAndLetterCenters;
        auto y = std::sinf(M_PI / 2 + L * i) * distanceBetweenBigAndLetterCenters;
        x += wordGameBackgroundSize * 0.5f;
        y += wordGameBackgroundSize * 0.5f;
        _circlesVector.at(i)->setContentSize(cocos2d::Size(_letterCircleRadius * 2, _letterCircleRadius * 2));
        _circlesVector.at(i)->setPosition(Vec2(x, y));
        auto label = dynamic_cast<Label*>(_circlesVector.at(i)->getChildByName("letter_label"));
        label->setTTFConfig(TTFConfig("fonts/NotoSans-Bold.ttf", _fontSize));
        label->setPosition(Vec2(_circlesVector.at(i)->getContentSize().width / 2, _circlesVector.at(i)->getContentSize().height / 2 * 1.04f));
    }
    _hintButton->setScale9Enabled(true);
    _hintButton->setScale(buttonSize / _hintButton->getContentSize().width);
    // _hintButton->getRendererNormal()->setContentSize(_hintButton->getContentSize());
    _hintButton->setPosition(hintButtonPosition);
}

Sprite* WordGameScene::createEmptyLine(const Vec2& coordinatesLineBegin)
{
    auto line = Sprite::createWithSpriteFrameName("line.png");
    line->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    line->setPosition(coordinatesLineBegin);
    line->setContentSize(Size(0.0f, _letterCircleRadius / 4));
    line->setColor(SELECT_COLOR);
    line->setLocalZOrder(0);
    return line;
}

void WordGameScene::setLineCoordinates(
    cocos2d::Sprite* line,
    const cocos2d::Vec2& coordinatesLineEnd
) {
    auto coordinatesLineBegin = line->getPosition();
    auto length = std::sqrtf((coordinatesLineEnd.x - coordinatesLineBegin.x) * (coordinatesLineEnd.x - coordinatesLineBegin.x) +
        (coordinatesLineEnd.y - coordinatesLineBegin.y) * (coordinatesLineEnd.y - coordinatesLineBegin.y));
    auto angle = 0.0f;
    if (std::abs(coordinatesLineEnd.x - coordinatesLineBegin.x) <= std::numeric_limits<float>::epsilon()) {
        if (coordinatesLineEnd.y >= coordinatesLineBegin.y) {
            angle = 90.0f;
        }
        else {
            angle = -90.0f;
        }
    }
    else {
        angle = std::atan((coordinatesLineEnd.y - coordinatesLineBegin.y) / (coordinatesLineEnd.x - coordinatesLineBegin.x)) * 180.0f / M_PI;
        if (coordinatesLineEnd.x < coordinatesLineBegin.x) {
            angle += 180.0f;
        }
    }
    line->setRotation(-angle);
    line->setContentSize(Size(length, line->getContentSize().height));
}

bool WordGameScene::isInsideCircle(cocos2d::Sprite* circle, const Vec2& point)
{
    auto center = circle->getPosition();
    return (point.x - center.x) * (point.x - center.x) +
        (point.y - center.y) * (point.y - center.y) <= _letterCircleRadius * _letterCircleRadius;
}

cocos2d::Sprite* WordGameScene::createEnterRectangle(const cocos2d::Vec2& coordinates)
{
    auto sprite = Sprite::createWithSpriteFrameName("enter_rectangle_center.png");
    sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    sprite->setPosition(coordinates);
    sprite->setContentSize(Size(0.0f, _enterRectangleHeight));
    sprite->setLocalZOrder(1);
    sprite->setColor(ENTER_COLOR);

    auto left = Sprite::createWithSpriteFrameName("enter_rectangle_left.png");
    left->setName("left");
    auto width = left->getContentSize().width * _enterRectangleHeight / left->getContentSize().height;
    left->setContentSize(Size(width, _enterRectangleHeight));
    left->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    left->setPosition(Vec2(0, 0));
    left->setColor(ENTER_COLOR);

    auto right = Sprite::createWithSpriteFrameName("enter_rectangle_right.png");
    right->setName("right");
    right->setContentSize(Size(width, _enterRectangleHeight));
    right->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    right->setPosition(Vec2(sprite->getContentSize().width, 0));
    right->setColor(ENTER_COLOR);

    auto label = Label::createWithTTF("", "fonts/NotoSans-Bold.ttf", _enterRectangleHeight * 4.0f / 6);
    label->setHorizontalAlignment(TextHAlignment::CENTER);
    label->setVerticalAlignment(TextVAlignment::CENTER);
    label->setName("label");
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    label->setIgnoreAnchorPointForPosition(false);
    label->setPosition(Vec2(-left->getContentSize().width / 2, sprite->getContentSize().height / 2 * 1.04f));
    label->setColor(ENTER_LETTERS_COLOR);

    sprite->addChild(left);
    sprite->addChild(right);
    sprite->addChild(label);

    return sprite;
}

void WordGameScene::updateEnterRectangleText()
{
    if (!_enterRectangle) return;
    auto label = dynamic_cast<Label*>(_enterRectangle->getChildByName("label"));
    auto right = _enterRectangle->getChildByName("right");

    auto text = getCurrentSequenceText();
    label->setString(text);
    auto height = _enterRectangle->getContentSize().height;
    auto width = label->getContentSize().width - right->getContentSize().width;
    _enterRectangle->setContentSize(Size(width, height));
    right->setPosition(Vec2(width, 0));
}

std::string WordGameScene::getCurrentSequenceText()
{
    auto str = std::string();
    for (size_t i = 0; i < _currentSequence.size(); i++) {
        auto circle = _currentSequence.at(i);
        auto label = dynamic_cast<Label*>(circle->getChildByName("letter_label"));
        str += label->getString();
    }
    return str;
}

cocos2d::Sprite* WordGameScene::createWordCell(const cocos2d::Vec2& coordinates, float cellSize, std::string letter)
{
    auto cell = Sprite::createWithSpriteFrameName("cell.png");
    cell->setContentSize(Size(cellSize, cellSize));
    cell->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    cell->setPosition(coordinates);
    cell->setColor(CELL_COLOR);
    
    auto label = Label::createWithTTF(letter, "fonts/NotoSans-Bold.ttf", cellSize * 4.0f / 6);
    label->setHorizontalAlignment(TextHAlignment::CENTER);
    label->setVerticalAlignment(TextVAlignment::CENTER);
    label->setName("label");
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    label->setIgnoreAnchorPointForPosition(false);
    label->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    label->setColor(SETKA_LETTERS_COLOR);
    label->setVisible(false);
    cell->addChild(label);
    
    this->addChild(cell);

    return cell;
}

void WordGameScene::initSetka()
{
    auto x_count = _wordGameController->getSetkaWidth();
    auto y_count = _wordGameController->getSetkaHeight();
    auto width = VIEW_SIZE.width - _setkaOutsideIndent * 2;
    auto height = _setkaHeight;
    auto cellSize = (height - (y_count - 1) * _setkaInnerIndent) / y_count;
    auto xCellSize = (width - (x_count - 1) * _setkaInnerIndent) / x_count;
    auto leftIndent = _setkaOutsideIndent;
    auto topIndent = _topButtonsSize.height + _topButtonsIndent + _verticalIndent;
    if (xCellSize < cellSize) {
        cellSize = xCellSize;
        auto newHeight = (y_count - 1) * _setkaInnerIndent + y_count * cellSize;
        topIndent += (height - newHeight) / 2;
        height = newHeight;
    } else {
        auto newWidth = (x_count - 1) * _setkaInnerIndent + x_count * cellSize;
        leftIndent += (width - newWidth) / 2;
        width = newWidth;
    }
    
    for (int i = 0; i < x_count; i++) {
        std::vector<cocos2d::Sprite*> row;
        for (int j = 0; j < y_count; j++) {
            row.push_back(nullptr);
        }
        _setka.push_back(row);
    }
    
    auto words = _wordGameController->getWords();
    for(int i = 0; i < words.size(); i++) {
        auto word_x = words[i]->getPosX();
        auto word_y = words[i]->getPosY();
        for(int j = 0; j < words[i]->getWordAnswerLength(); j++) {
            if(_setka[word_x][word_y] == nullptr) {
                auto cellX = leftIndent + _setkaInnerIndent * word_x + cellSize * word_x;
                auto cellY = VIEW_SIZE.height - (topIndent + _setkaInnerIndent * word_y + cellSize * word_y);
                _setka[word_x][word_y] = createWordCell(Vec2(cellX, cellY), cellSize, words[i]->getAnswerLetterAt(j));
                _setka[word_x][word_y]->getChildByName("label")->setVisible(_wordGameController->isOpenedOnSetka(word_x, word_y));
            }
            if(words[i]->isHorizontal()) {
                word_x++;
            } else {
                word_y++;
            }
        }
    }
}

void WordGameScene::initHintButton()
{
    _hintButton = ui::Button::create("hint_button.png", "hint_button.png", "hint_button.png", ui::Widget::TextureResType::PLIST);
    _hintButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    // _hintButton->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
    // _hintButton->setLocalZOrder(10);

    _hintButton->addClickEventListener(
        [this](Ref* sender) {
            if (_availablePearls < 50) {
                AudioHelper::playEffectSound("sound/nohint.mp3");
//                Director::getInstance()->pushScene(NewShopMenuScene::createScene());
//                UserDefault::getInstance()->setIntegerForKey("available_pearls", 200);
//                _availablePearls = 200;
//                _pearlsButtonLabel->setString(std::to_string(200));
            }
            else {
                auto hintPos = _wordGameController->hintUsed();
                if (hintPos.first == -1 || hintPos.second == -1) {
                    return;
                }
                AudioHelper::playEffectSound("sound/hint.mp3");
                _availablePearls -= 50;
                UserDefault::getInstance()->setIntegerForKey("available_pearls", _availablePearls);
                _pearlsButtonLabel->setString(std::to_string(_availablePearls));
                _setka[hintPos.first][hintPos.second]->getChildByName("label")->setVisible(true);
                if (_wordGameController->allWordsSolved()) {
                    AudioHelper::playEffectSound("sound/right_pair.mp3");
                    _wordGameController->onRoundCompleted();
                    Director::getInstance()->popScene();
                }
            }
        }
    );

    this->addChild(_hintButton, 10);
}

void WordGameScene::initReshuffleButton()
{
    _reshuffleButton = ui::Button::create("reshuffle.png", "reshuffle.png", "reshuffle.png", ui::Widget::TextureResType::PLIST);
    _reshuffleButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _reshuffleButton->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _reshuffleButton->setLocalZOrder(10);

    _reshuffleButton->addClickEventListener(
        [this](Ref* sender) {
            AudioHelper::playEffectSound("sound/reshuffle.mp3");
            _reshuffleButton->setEnabled(false);
            _isControllerAvailable = false;
            this->scheduleOnce([this](float dt) {
                _reshuffleButton->setEnabled(true);
                _isControllerAvailable = true;
                },
                0.21f,
                    "makeAvailable"
                    );
            auto randomVector = createRandomVector(_circlesVector.size());
            for (int i = 0; i < _circlesVector.size(); ++i) {
                auto moveTo = MoveTo::create(0.2f, _circlesVector.at(randomVector.at(i))->getPosition());
                _circlesVector.at(i)->runAction(moveTo);
            }
        }
    );

    _wordGameBackground->addChild(_reshuffleButton, 10);
}

std::vector<int> WordGameScene::createRandomVector(int size) {
    std::vector<int> randomVector;
    std::vector<int> numVector;
    for (int i = 0; i < size; ++i) {
        numVector.push_back(i);
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    for (int i = size - 1; i >= 0; --i) {
        std::uniform_int_distribution<int> dist(0, i);
        int randomNum = dist(gen);
        randomVector.push_back(numVector.at(randomNum));
        numVector.erase(numVector.begin() + randomNum);
    }
    return randomVector;
}

void WordGameScene::onEnter()
{
    Scene::onEnter();
    
    auto newAvailablePearls = UserDefault::getInstance()->getIntegerForKey("available_pearls", 100);
    if (_availablePearls != newAvailablePearls) {
        _availablePearls = newAvailablePearls;
        UserDefault::getInstance()->setIntegerForKey("available_pearls", _availablePearls);
        _pearlsButtonLabel->setString(std::to_string(_availablePearls));
    }
}

void WordGameScene::onExit()
{
    Scene::onExit();
}
