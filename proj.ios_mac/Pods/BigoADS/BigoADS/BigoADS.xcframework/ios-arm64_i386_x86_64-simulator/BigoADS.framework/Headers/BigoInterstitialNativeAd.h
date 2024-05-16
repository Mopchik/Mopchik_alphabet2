//
//  BigoInterstitialNativeAd.h
//  AFNetworking
//
//  Created by 李剑锋 on 2021/7/14.
//

#import <Foundation/Foundation.h>
#import "BigoInterstitialAd.h"

@class BGEndPageCompanion;
@class BGPlayableAdCompanion;
NS_ASSUME_NONNULL_BEGIN

@interface BigoInterstitialNativeAd : BigoInterstitialAd

@property (nonatomic, readonly) BGPlayableAdCompanion *playableAdCompanion;
@property (nonatomic, readonly) BGEndPageCompanion *endPageCompanion;

@end

NS_ASSUME_NONNULL_END
