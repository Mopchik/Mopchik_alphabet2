#ifndef  __ADMOB_HELPER_H_
#define  __ADMOB_HELPER_H_

#include <cocos2d.h>

class AdmobHelperDelegate
{
public:

    virtual void rewardVideoDidPresent() {};
    virtual void rewardVideoDidDismiss() {};
    virtual void rewardVideoNotLoaded() {};
    virtual void rewardUser() {};
    
protected:
    
    virtual ~AdmobHelperDelegate() {};
};

class AdmobHelper
{
public:
    static AdmobHelper * getInstance();

    //reward video
    void showRewardVideo();
    void loadRewardVideo();
    bool isRewardLoaded();
    
    void setDelegate(AdmobHelperDelegate *delegate);
    
CC_CONSTRUCTOR_ACCESS:
    
    AdmobHelper();
    virtual ~AdmobHelper();
};


#endif // __ADMOB_HELPER_H_
