#include "AudioHelper.h"
#include "AppDelegate.h"

#define GET_BACKGROUND_SOUND_VOLUME cocos2d::UserDefault::getInstance()->getFloatForKey("background_sound_volume", 1.0f)
#define SET_BACKGROUND_SOUND_VOLUME(x) cocos2d::UserDefault::getInstance()->setFloatForKey("background_sound_volume", x)

#define GET_BUTTON_SOUND_VOLUME cocos2d::UserDefault::getInstance()->getFloatForKey("button_sound_volume", 1.0f)
#define SET_BUTTON_SOUND_VOLUME(x) cocos2d::UserDefault::getInstance()->setFloatForKey("button_sound_volume", x)

float AudioHelper::backgroundSoundVolume = -1.0f;
float AudioHelper::buttonSoundsVolume = -1.0f;
std::map<std::string, int> AudioHelper::effects = {};

#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;

int backgroundSoundId = -1;

void AudioHelper::playBackgroundSound()
{
    std::string melodyPath = FILEUTILS->fullPathForFilename("sound/main_melody.mp3");
    if (!FILEUTILS->isFileExist(melodyPath)) {
        backgroundSoundId = -1;
        return;
    }
    
    backgroundSoundId = AudioEngine::play2d(melodyPath, true, AudioHelper::getBackgroundSoundVolume());
}

bool AudioHelper::isBackgroundSoundPlaying()
{
    return backgroundSoundId > -1;
}

void AudioHelper::playEffectSound(const std::string &path)
{
    auto soundId = AudioEngine::play2d(path, false, AudioHelper::getEffectsSoundsVolume());
    effects[path] = soundId;
}

void AudioHelper::stopEffectSound(const std::string &path)
{
    if (effects.find(path) == effects.end())
        return;

    auto soundId = effects[path];
    AudioEngine::stop(soundId);
}

bool AudioHelper::muteBackgroundSound()
{
    if (AudioHelper::getBackgroundSoundVolume() > 0) {
        AudioHelper::backgroundSoundVolume = 0.0f;
        SET_BACKGROUND_SOUND_VOLUME(AudioHelper::backgroundSoundVolume);
        AudioEngine::setVolume(backgroundSoundId, AudioHelper::backgroundSoundVolume);
        
        return true;
    } else {
        AudioHelper::backgroundSoundVolume = 1.0f;
        SET_BACKGROUND_SOUND_VOLUME(AudioHelper::backgroundSoundVolume);
        AudioEngine::setVolume(backgroundSoundId, AudioHelper::backgroundSoundVolume);
        
        return false;
    }
}

bool AudioHelper::muteEffectsSounds()
{
    if (AudioHelper::getEffectsSoundsVolume() > 0) {
        AudioHelper::buttonSoundsVolume = 0.0f;
        SET_BUTTON_SOUND_VOLUME(AudioHelper::buttonSoundsVolume);
        
        return true;
    } else {
        AudioHelper::buttonSoundsVolume = 1.0f;
        SET_BUTTON_SOUND_VOLUME(AudioHelper::buttonSoundsVolume);
        
        return false;
    }
}

const float AudioHelper::getBackgroundSoundVolume()
{
    if (AudioHelper::backgroundSoundVolume < 0.0f)
        AudioHelper::backgroundSoundVolume = GET_BACKGROUND_SOUND_VOLUME;

    return AudioHelper::backgroundSoundVolume;
}

const float AudioHelper::getEffectsSoundsVolume()
{
    if (AudioHelper::buttonSoundsVolume < 0.0f)
        AudioHelper::buttonSoundsVolume = GET_BUTTON_SOUND_VOLUME;

    return AudioHelper::buttonSoundsVolume;
}

void AudioHelper::preloadBackgroundMusic(const std::string &filePath)
{
    AudioEngine::preload(filePath);
}

void AudioHelper::preloadEffect(const std::string &filePath)
{
    AudioEngine::preload(filePath);
}

void AudioHelper::pauseAll()
{
    AudioEngine::pauseAll();
}

void AudioHelper::resumeAll()
{
    AudioEngine::resumeAll();
}

void AudioHelper::stopAll()
{
    backgroundSoundId = -1;
    
    AudioEngine::stopAll();
}

void AudioHelper::end()
{
    AudioEngine::end();
}
