#import "AdmobHelper.h"
#import <cocos2d.h>

#import <Foundation/Foundation.h>
#import <YandexMobileAds/YMARewardedAd.h>

#define YA_REWARDVIDEO_ID @"R-M-8642801-1"

@interface AdmobController : NSObject <YMARewardedAdDelegate>

@property (nonatomic, assign) AdmobHelperDelegate *delegate;

@property (nonatomic, strong) YMARewardedAd *yRewardedAd;

@end

@implementation AdmobController

@synthesize delegate;

+ (AdmobController *)controller {
    AdmobController *controller = [[self alloc] init];
    return controller;
}

- (id)init
{
    if (self = [super init]) {
        delegate = nullptr;
        return self;
    }
    return nil;
}

#pragma mark YMARewardedAdDelegate
- (void)rewardedAd:(YMARewardedAd *)rewardedAd didReward:(id<YMAReward>)reward {
    if (self.delegate != nullptr) {
        self.delegate->rewardUser();
    }
    NSLog(@"yRewardBasedVideoAd reward user");
}
- (void)rewardedAdDidAppear:(YMARewardedAd *)rewardedAd {
    if (self.delegate != nullptr) {
        self.delegate->rewardVideoDidPresent();
    }
    NSLog(@"yRewardBasedVideoAd Did Appear");
}
- (void)rewardedAdDidDisappear:(YMARewardedAd *)rewardedAd {
    if (self.delegate != nullptr) {
        self.delegate->rewardVideoDidDismiss();
    }
    AdmobHelper::getInstance()->loadRewardVideo();
    NSLog(@"yRewardBasedVideoAd Did Dismiss");
}
- (void)rewardedAdDidFailToLoad:(YMARewardedAd *)rewardedAd error:(NSError *)error {
    NSLog(@"yRewardedAd failed to load with error: %@", [error localizedDescription]);
}
- (void)rewardedAdDidFailToPresent:(nonnull YMARewardedAd *)rewardedAd error:(nonnull NSError *)error {
    NSLog(@"yRewardBasedVideoAd ad did fail to present full screen content.");
}

@end

USING_NS_CC;

AdmobController *_controller;

AdmobHelper* AdmobHelper::getInstance()
{
    static AdmobHelper sm_pSharedInstance;
    return &sm_pSharedInstance;
}

AdmobHelper::AdmobHelper()
{
    _controller = [AdmobController controller];

    UIViewController *rootViewController = [UIApplication sharedApplication].keyWindow.rootViewController;

    this->loadRewardVideo();
}

AdmobHelper::~AdmobHelper()
{
    _controller.delegate = nullptr;
    _controller = nil;
}

void AdmobHelper::setDelegate(AdmobHelperDelegate *delegate)
{
    _controller.delegate = delegate;
}

void AdmobHelper::loadRewardVideo()
{
    _controller.yRewardedAd = [[YMARewardedAd alloc] initWithAdUnitID:YA_REWARDVIDEO_ID];
    _controller.yRewardedAd.delegate = _controller;
    [_controller.yRewardedAd load];
}

void AdmobHelper::showRewardVideo()
{
    if (_controller.yRewardedAd && _controller.yRewardedAd.loaded) {
        UIViewController *rootViewController = [UIApplication sharedApplication].keyWindow.rootViewController;
        [_controller.yRewardedAd presentFromViewController:rootViewController];
    } else {
        this->loadRewardVideo();

        if (_controller.delegate) {
            _controller.delegate->rewardVideoNotLoaded();
        }

        CCLOG("RewardBasedVideoAd is not loaded");
    }
}

bool AdmobHelper::isRewardLoaded()
{
    return _controller.yRewardedAd && _controller.yRewardedAd.loaded;
}
