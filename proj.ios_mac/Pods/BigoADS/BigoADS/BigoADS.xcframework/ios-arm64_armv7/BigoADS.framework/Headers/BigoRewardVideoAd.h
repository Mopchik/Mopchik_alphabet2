//
//  BigoRewardVideoAd.h
//  BigoADS
//
//  Created by lijianfeng on 2021/7/27.
//

#import "BigoInterstitialNativeAd.h"

NS_ASSUME_NONNULL_BEGIN

@class BigoRewardVideoAd;
@protocol BigoRewardVideoAdInteractionDelegate <BigoAdInteractionDelegate>

@optional
//激励视频激励回调
- (void)onAdRewarded:(BigoRewardVideoAd *)ad;

@end

@interface BigoRewardVideoAd : BigoInterstitialNativeAd

- (void)setRewardVideoAdInteractionDelegate:(nullable id<BigoRewardVideoAdInteractionDelegate>)delegate;

- (void)setAdInteractionDelegate:(nullable id<BigoAdInteractionDelegate>)delegate NS_UNAVAILABLE;

@end

NS_ASSUME_NONNULL_END
