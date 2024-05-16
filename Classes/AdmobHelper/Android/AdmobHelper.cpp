#include "AdmobHelper.h"
#include "cocos2d.h"

#include "platform/android/jni/JniHelper.h"
#include <jni.h>
//#include <android/log.h>

#define ACTIVITY_CLASS "ru/mopchik/mopchik/AppActivity"

AdmobHelperDelegate *_admobDelegate;

AdmobHelper* AdmobHelper::getInstance()
{
    static AdmobHelper sm_pSharedInstance;
    return &sm_pSharedInstance;
}

AdmobHelper::AdmobHelper()
{
    _admobDelegate = nullptr;
}

AdmobHelper::~AdmobHelper() = default;

void AdmobHelper::setDelegate(AdmobHelperDelegate *delegate)
{
    _admobDelegate = delegate;
}

void AdmobHelper::showRewardVideo()
{
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, ACTIVITY_CLASS, "showRewardAd", "()V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
}

void AdmobHelper::loadRewardVideo() {};

bool AdmobHelper::isRewardLoaded() {
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, ACTIVITY_CLASS, "isRewardLoaded", "()Z"))
    {
        jboolean returnValue = t.env->CallStaticBooleanMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
        return (returnValue == JNI_TRUE);
    }
    return false;
}

extern "C" {

void Java_ru_mopchik_mopchik_AppActivity_rewardDidPresent(JNIEnv * env , jobject thiz ) {
    if (_admobDelegate != nullptr) {
        _admobDelegate->rewardVideoDidPresent();
    }
}

void Java_ru_mopchik_mopchik_AppActivity_rewardDidDismiss(JNIEnv * env , jobject thiz ) {
    if (_admobDelegate != nullptr) {
        _admobDelegate->rewardVideoDidDismiss();
    }
}

void Java_ru_mopchik_mopchik_AppActivity_rewardUser(JNIEnv * env , jobject thiz ) {
    if (_admobDelegate != nullptr) {
        _admobDelegate->rewardUser();
    }
}

void Java_ru_mopchik_mopchik_AppActivity_rewardNotLoaded(JNIEnv * env , jobject thiz ) {
    if (_admobDelegate != nullptr) {
        _admobDelegate->rewardVideoNotLoaded();
    }
}
} //extern "C"
