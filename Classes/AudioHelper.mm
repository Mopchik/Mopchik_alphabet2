#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import "RootViewController.h"

#include "AudioHelper.h"

float AudioHelper::backgroundSoundVolume = -1.0f;
float AudioHelper::buttonSoundsVolume = -1.0f;
std::map<std::string, int> AudioHelper::effects = {};

#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;

#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;

int backgroundSoundId = -1;

void AudioHelper::playBackgroundSound()
{
    std::string melodyPath = FILEUTILS->fullPathForFilename("sound/main_melody.mp3");
    if (!FILEUTILS->isFileExist(melodyPath)) {
        backgroundSoundId = -1;
        return;
    }
    
    if (SYSTEM_VERSION_GREATER_THAN(@"11.0") && SYSTEM_VERSION_LESS_THAN(@"12.4")) {
        backgroundSoundId = AudioEngine::play2d(melodyPath, true, AudioHelper::getBackgroundSoundVolume());
    } else {
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(AudioHelper::getBackgroundSoundVolume());
        SimpleAudioEngine::getInstance()->playBackgroundMusic(melodyPath.c_str(), true);
    }
    
    [[AVAudioSession sharedInstance] setCategory:AVAudioSessionCategoryPlayback error:nil];
}
bool AudioHelper::isBackgroundSoundPlaying()
{
    if (SYSTEM_VERSION_GREATER_THAN(@"11.0") && SYSTEM_VERSION_LESS_THAN(@"12.4")) {
        return backgroundSoundId > -1;
    } else {
        return SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying();
    }
}

void AudioHelper::playEffectSound(const std::string &path)
{
    if (SYSTEM_VERSION_GREATER_THAN(@"11.0") && SYSTEM_VERSION_LESS_THAN(@"12.4")) {
        auto soundId = AudioEngine::play2d(path, false, AudioHelper::getEffectsSoundsVolume());
        effects[path] = soundId;
    } else {
        auto soundId = SimpleAudioEngine::getInstance()->playEffect(path.c_str(), false, 1.0f, 0.0f,
                                                     AudioHelper::getEffectsSoundsVolume());
        effects[path] = soundId;
    }
}

void AudioHelper::stopEffectSound(const std::string &path)
{
    if (SYSTEM_VERSION_GREATER_THAN(@"11.0") && SYSTEM_VERSION_LESS_THAN(@"12.4")) {
        if (effects.find(path) == effects.end())
            return;

        auto soundId = effects[path];
        AudioEngine::stop(soundId);
    } else {
        if (effects.find(path) == effects.end())
            return;

        auto soundId = effects[path];
        SimpleAudioEngine::getInstance()->stopEffect(soundId);
    }
}

bool AudioHelper::muteBackgroundSound()
{
    if (SYSTEM_VERSION_GREATER_THAN(@"11.0") && SYSTEM_VERSION_LESS_THAN(@"12.4")) {
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
    } else {
        if (AudioHelper::getBackgroundSoundVolume() > 0) {
            AudioHelper::backgroundSoundVolume = 0.0f;
            SET_BACKGROUND_SOUND_VOLUME(AudioHelper::backgroundSoundVolume);
            SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(AudioHelper::backgroundSoundVolume);
            
            return true;
        } else {
            AudioHelper::backgroundSoundVolume = 1.0f;
            SET_BACKGROUND_SOUND_VOLUME(AudioHelper::backgroundSoundVolume);
            SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(AudioHelper::backgroundSoundVolume);
            
            return false;
        }
    }
}

bool AudioHelper::muteEffectsSounds()
{
    if (SYSTEM_VERSION_GREATER_THAN(@"11.0") && SYSTEM_VERSION_LESS_THAN(@"12.4")) {
        if (AudioHelper::getEffectsSoundsVolume() > 0) {
            AudioHelper::buttonSoundsVolume = 0.0f;
            SET_BUTTON_SOUND_VOLUME(AudioHelper::buttonSoundsVolume);
            
            return true;
        } else {
            AudioHelper::buttonSoundsVolume = 1.0f;
            SET_BUTTON_SOUND_VOLUME(AudioHelper::buttonSoundsVolume);
            
            return false;
        }
    } else {
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
    if (SYSTEM_VERSION_GREATER_THAN(@"11.0") && SYSTEM_VERSION_LESS_THAN(@"12.4")) {
        AudioEngine::preload(filePath);
    } else {
        SimpleAudioEngine::getInstance()->preloadBackgroundMusic(filePath.c_str());
    }
}

void AudioHelper::preloadEffect(const std::string &filePath)
{
    if (SYSTEM_VERSION_GREATER_THAN(@"11.0") && SYSTEM_VERSION_LESS_THAN(@"12.4")) {
        AudioEngine::preload(filePath);
    } else {
        SimpleAudioEngine::getInstance()->preloadEffect(filePath.c_str());
    }
}

void AudioHelper::pauseAll()
{
    if (SYSTEM_VERSION_GREATER_THAN(@"11.0") && SYSTEM_VERSION_LESS_THAN(@"12.4")) {
        AudioEngine::pauseAll();
    } else {
        SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
        SimpleAudioEngine::getInstance()->pauseAllEffects();
    }
}

void AudioHelper::resumeAll()
{
    if (SYSTEM_VERSION_GREATER_THAN(@"11.0") && SYSTEM_VERSION_LESS_THAN(@"12.4")) {
        AudioEngine::resumeAll();
    } else {
        SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
        SimpleAudioEngine::getInstance()->resumeAllEffects();
    }
}

void AudioHelper::stopAll()
{
    if (SYSTEM_VERSION_GREATER_THAN(@"11.0") && SYSTEM_VERSION_LESS_THAN(@"12.4")) {
        backgroundSoundId = -1;
        AudioEngine::stopAll();
    } else {
        SimpleAudioEngine::getInstance()->stopBackgroundMusic();
        SimpleAudioEngine::getInstance()->stopAllEffects();
    }
}

void AudioHelper::end()
{
    if (SYSTEM_VERSION_GREATER_THAN(@"11.0") && SYSTEM_VERSION_LESS_THAN(@"12.4")) {
        AudioEngine::end();
    } else {
        SimpleAudioEngine::getInstance()->end();
    }
}
