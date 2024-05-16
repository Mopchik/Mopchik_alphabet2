#include "Utils.h"
#include <cocos2d.h>
#include <iomanip>
#include <sstream>

void Utils::printTime(const std::string &className, const std::string &message) {
    std::stringstream ss;
    auto tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm tm = *std::gmtime(&tt);
    ss << std::put_time( &tm, "%Z %Y-%m-%d %H:%M:%S.");
    CCLOG("%s %s : %s", className.c_str(), message.c_str(), ss.str().c_str());
}

void Utils::makeToast(const std::string &message) {
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, "ru/mopchik/mopchik/AppActivity", "makeToast",
                                                "(Ljava/lang/String;)V")) {
        jstring jMessage = t.env->NewStringUTF(message.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, jMessage);
        t.env->DeleteLocalRef(jMessage);
        t.env->DeleteLocalRef(t.classID);
    }
}
