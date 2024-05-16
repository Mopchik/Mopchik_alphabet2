/****************************************************************************
 Copyright (c) 2013      cocos2d-x.org
 Copyright (c) 2013-2016 Chukong Technologies Inc.
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#import "RootViewController.h"
#import "cocos2d.h"
#import "platform/ios/CCEAGLView-ios.h"

@interface RootViewController ()

@property (nonatomic, assign) CGSize lastFrameSize;

@end

@implementation RootViewController

/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
    }
    return self;
}
*/

- (UIInterfaceOrientationMask)supportedInterfaceOrientations {
    return UIInterfaceOrientationMaskPortrait;
}

// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
    // Initialize the CCEAGLView
    CCEAGLView *eaglView = [CCEAGLView viewWithFrame: [UIScreen mainScreen].bounds
                                         pixelFormat: (__bridge NSString *)cocos2d::GLViewImpl::_pixelFormat
                                         depthFormat: cocos2d::GLViewImpl::_depthFormat
                                  preserveBackbuffer: NO
                                          sharegroup: nil
                                       multiSampling: cocos2d::GLViewImpl::_multisamplingCount > 0 ? YES : NO
                                     numberOfSamples: cocos2d::GLViewImpl::_multisamplingCount ];
    
    // Enable or disable multiple touches
    [eaglView setMultipleTouchEnabled:NO];
    
    // Set EAGLView as view of RootViewController
    self.view = eaglView;
    
    self.lastFrameSize = [UIScreen mainScreen].bounds.size;
}

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
}

- (void)viewDidDisappear:(BOOL)animated {
    [super viewDidDisappear:animated];
}

//- (NSUInteger) supportedInterfaceOrientations{
//    NSInteger orientation = [[NSUserDefaults standardUserDefaults] integerForKey:@"supported_orientations"];
//    
//    if (orientation == 1) {
//        return UIInterfaceOrientationMaskPortrait;
//    } else if (orientation == 2){
//        return UIInterfaceOrientationMaskPortraitUpsideDown;
//    } else if (orientation == 3){
//        return UIInterfaceOrientationMaskLandscapeRight;
//    } else if (orientation == 4){
//        return UIInterfaceOrientationMaskLandscapeLeft;
//    } else {
//        return UIInterfaceOrientationMaskAll;
//    }
//}

- (BOOL) shouldAutorotate {
    return ([[UIApplication sharedApplication] applicationState] == UIApplicationStateActive);
}

- (void)willRotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation duration:(NSTimeInterval)duration {
    [super willRotateToInterfaceOrientation:toInterfaceOrientation duration:duration];
    
    CGSize currentFrameSize = [UIScreen mainScreen].bounds.size;
    
    if SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(@"16") {
        if (CGSizeEqualToSize(self.lastFrameSize, currentFrameSize)) {
            return;
        } else {
            self.lastFrameSize = currentFrameSize;
        }
    }
    
    if (SYSTEM_VERSION_LESS_THAN(@"16")) {
        if (IS_IPAD && SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(@"13")) {
            if (@available(iOS 13.0, *)) {
                UIWindow *window = [[UIApplication sharedApplication] windows].firstObject;
                if (!window.isKeyWindow)
                    return;
                
                if (window.windowScene.interfaceOrientation == UIInterfaceOrientationLandscapeLeft || window.windowScene.interfaceOrientation == UIInterfaceOrientationLandscapeRight) {
                    if (toInterfaceOrientation == UIInterfaceOrientationPortrait || toInterfaceOrientation == UIInterfaceOrientationPortraitUpsideDown) {
                        return;
                    }
                } else {
                    if (toInterfaceOrientation == UIInterfaceOrientationLandscapeLeft || toInterfaceOrientation == UIInterfaceOrientationLandscapeRight) {
                        return;
                    }
                }
            } else {
                return;
            }
        } else {
            if ([[UIApplication sharedApplication] statusBarOrientation] == UIInterfaceOrientationLandscapeLeft || [[UIApplication sharedApplication] statusBarOrientation] == UIInterfaceOrientationLandscapeRight) {
                if (toInterfaceOrientation == UIInterfaceOrientationLandscapeLeft || toInterfaceOrientation == UIInterfaceOrientationLandscapeRight) {
                    return;
                }
            } else {
                if (toInterfaceOrientation == UIInterfaceOrientationPortrait || toInterfaceOrientation == UIInterfaceOrientationPortraitUpsideDown) {
                    return;
                }
            }
        }
    }
    
    auto glview = cocos2d::Director::getInstance()->getOpenGLView();
    
    if (glview)
    {
        CCEAGLView *eaglview = (__bridge CCEAGLView *)glview->getEAGLView();
        
        if (eaglview)
        {
            CGSize s = CGSizeMake([eaglview getHeight], [eaglview getWidth]);
            cocos2d::Application::getInstance()->applicationScreenSizeChanged((int) s.width, (int) s.height);
        }
    }
}

//fix not hide status on ios7
- (BOOL)prefersStatusBarHidden {
    return YES;
}

// Controls the application's preferred home indicator auto-hiding when this view controller is shown.
- (BOOL)prefersHomeIndicatorAutoHidden {
    return YES;
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];

    // Release any cached data, images, etc that aren't in use.
}

@end
