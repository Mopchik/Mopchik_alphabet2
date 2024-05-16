#ifndef MOPCHIK_WORDGAMESCENE_H
#define MOPCHIK_WORDGAMESCENE_H

#include <cocos2d.h>
#include "GameScene.h"
#include "AdmobHelper.h"
#include "WordGameController.h"
#include "ui/CocosGUI.h"

class WordGameScene : public GameScene {
public:
    static cocos2d::Scene* createScene();
    virtual bool initWord();

    void applicationDidEnterBackground() final;
    void applicationWillEnterForeground() final {};

CC_CONSTRUCTOR_ACCESS:
    WordGameScene();

protected:
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual ~WordGameScene();

private:
    cocos2d::Node *_background;
    cocos2d::ui::Button* _backButton;
    cocos2d::ui::Button *_pearlsButton;
    cocos2d::Label *_pearlsButtonLabel;

    int _availablePearls;

    cocos2d::Size _topButtonsSize;
    float _topButtonsIndent;

    WordGameController *_wordGameController;

    cocos2d::Sprite* _wordGameBackground;

    std::vector<cocos2d::Sprite*> _circlesVector;

    cocos2d::Sprite* _buildingLine;
    std::vector<cocos2d::Sprite*> _lines;
    std::vector<cocos2d::Sprite*> _currentSequence;

    std::vector<std::vector<cocos2d::Sprite*>> _setka;

    cocos2d::Sprite* _enterRectangle;

    cocos2d::ui::Button* _reshuffleButton;
    cocos2d::ui::Button* _hintButton;

    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> _convert;

    float _fontSize;
    float _enterRectangleHeight;
    float _verticalIndent;
    float _letterCircleRadius;
    float _setkaOutsideIndent;
    float _setkaInnerIndent;
    float _setkaHeight;

    bool _isControllerAvailable;

    void setDimensionsAndPositions();
    cocos2d::Sprite* createEmptyLine(const cocos2d::Vec2& coordinatesLineBegin);
    void setLineCoordinates(cocos2d::Sprite* line, const cocos2d::Vec2& coordinatesLineEnd);
    bool isInsideCircle(cocos2d::Sprite* circle, const cocos2d::Vec2& point);
    cocos2d::Sprite* createEnterRectangle(const cocos2d::Vec2& coordinates);
    void updateEnterRectangleText();
    std::string getCurrentSequenceText();
    cocos2d::Sprite* createWordCell(const cocos2d::Vec2& coordinates, float cellSize, std::string letter);
    void initSetka();
//    void setWordToBackground(WordGameWord* word, cocos2d::Sprite* background);
    void initHintButton();
    void initReshuffleButton();
    std::vector<int> createRandomVector(int size);
//    void initWordCards();
//    void updateBackgroundCardsSpaces();
//    void wordCompleted();
//    void win();
};


#endif //MOPCHIK_WORDGAMESCENE_H
