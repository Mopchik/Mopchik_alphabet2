#include "PlatformHelper.h"
#include <cocos2d.h>

#define ACTIVITY_CLASS "ru/mopchik/mopchik/AppActivity"

USING_NS_CC;

void PlatformHelper::lockDeviceRotation()
{
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, ACTIVITY_CLASS, "setAutoOrientationEnabled", "()V"))
    {
        jstring javaText = t.env->NewStringUTF("lock");
        t.env->CallStaticVoidMethod(t.classID, t.methodID, javaText);
        t.env->DeleteLocalRef(t.classID);
    }
}
