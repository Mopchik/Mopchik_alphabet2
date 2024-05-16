#ifndef MOPCHIK_NEWMAINMENUSCENE_H
#define MOPCHIK_NEWMAINMENUSCENE_H


#include <cocos2d.h>
#include "ui/CocosGUI.h"
#include "WordGameController.h"
#include "AdmobHelper.h"

class NewMainMenuScene : public cocos2d::Scene, private AdmobHelperDelegate
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;

CC_CONSTRUCTOR_ACCESS:
    NewMainMenuScene();

protected:
    virtual void onEnter() override;
    virtual void onExit() override;
    
private:
    cocos2d::ui::Button* _playButton;
    cocos2d::Label* _playButtonLabel;
    cocos2d::Sprite* _alphabetBackground;
    std::vector<cocos2d::Sprite*> _alphabetCells;
    cocos2d::Sprite* _guessedLetterBackground;
    cocos2d::Label* _guessedLetter;
    cocos2d::ui::Button* _pearlsButton;
    cocos2d::Label* _pearlsButtonLabel;
    cocos2d::Label* _percent;
    cocos2d::Sprite* _soundOn;
    cocos2d::Sprite* _soundOff;
    cocos2d::Sprite* _musicOn;
    cocos2d::Sprite* _musicOff;
    cocos2d::Sprite* _videoButton;
    int _availablePearls;
    WordGameController* _wordGameController;
    std::u16string _alphabet = u"АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> _convert;
    bool _isFirstScreen = true;
    
    void setDimensions();
    void setOpenLetters();
    void setClickListener(cocos2d::Sprite* sprite, std::function<void()> func);

    //AdmobHelperDelegate
    virtual void rewardVideoDidPresent() override;
    virtual void rewardVideoDidDismiss() override;
    virtual void rewardVideoNotLoaded() override;
    virtual void rewardUser() override;
};


#endif //MOPCHIK_NEWMAINMENUSCENE_H
