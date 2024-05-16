#include "AdmobHelper.h"

AdmobHelper* AdmobHelper::getInstance()
{
    static AdmobHelper sm_pSharedInstance;
    return &sm_pSharedInstance;
}

AdmobHelper::AdmobHelper() = default;

AdmobHelper::~AdmobHelper() = default;

void AdmobHelper::setDelegate(AdmobHelperDelegate *delegate) {};

void AdmobHelper::showRewardVideo() {};
void AdmobHelper::loadRewardVideo() {};
bool AdmobHelper::isRewardLoaded() { return false; }
