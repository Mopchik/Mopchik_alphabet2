#ifndef MOPCHIK_AUDIOHELPER_H
#define MOPCHIK_AUDIOHELPER_H

#include "AppDelegate.h"

#define GET_BACKGROUND_SOUND_VOLUME cocos2d::UserDefault::getInstance()->getFloatForKey("background_sound_volume", 1.0f)
#define SET_BACKGROUND_SOUND_VOLUME(x) cocos2d::UserDefault::getInstance()->setFloatForKey("background_sound_volume", x)

#define GET_BUTTON_SOUND_VOLUME cocos2d::UserDefault::getInstance()->getFloatForKey("button_sound_volume", 1.0f)
#define SET_BUTTON_SOUND_VOLUME(x) cocos2d::UserDefault::getInstance()->setFloatForKey("button_sound_volume", x)

class AudioHelper {
public:
    static void preloadBackgroundMusic(const std::string &filePath);
    static void preloadEffect(const std::string &filePath);
    static void pauseAll();
    static void resumeAll();
    static void stopAll();
    static void end();
    static void playBackgroundSound();
    static bool isBackgroundSoundPlaying();
    static void playEffectSound(const std::string &path);
    static void stopEffectSound(const std::string &path);
    static bool muteBackgroundSound();
    static bool muteEffectsSounds();
    static const float getBackgroundSoundVolume();
    static const float getEffectsSoundsVolume();

private:
    static float backgroundSoundVolume;
    static float buttonSoundsVolume;
    static std::map<std::string, int> effects;
};


#endif //MOPCHIK_AUDIOHELPER_H
