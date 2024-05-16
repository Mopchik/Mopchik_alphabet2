#import "PlatformHelper.h"
#import "AppController.h"

void PlatformHelper::lockDeviceRotation(){
    [[NSUserDefaults standardUserDefaults] setInteger:[[UIApplication sharedApplication] statusBarOrientation] forKey:@"supported_orientations"];
    [[UIDevice currentDevice] setValue:@([[UIApplication sharedApplication] statusBarOrientation]) forKey:@"orientation"];
}
