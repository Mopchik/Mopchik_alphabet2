/****************************************************************************
 Copyright (c) 2015-2016 Chukong Technologies Inc.
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
package ru.mopchik.mopchik;

import android.content.SharedPreferences;
import android.content.Context;
import android.view.Surface;
import android.content.res.Configuration;
import android.content.pm.ActivityInfo;
import android.os.Bundle;
import org.cocos2dx.lib.Cocos2dxActivity;
import android.util.Log;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

//yandex ads
import com.yandex.mobile.ads.common.InitializationListener;
import com.yandex.mobile.ads.common.MobileAds;
import com.yandex.mobile.ads.rewarded.Reward;
import com.yandex.mobile.ads.rewarded.RewardedAdEventListener;
import com.yandex.mobile.ads.rewarded.RewardedAdLoadListener;
import com.yandex.mobile.ads.rewarded.RewardedAdLoader;
import com.yandex.mobile.ads.common.AdRequestConfiguration;
import com.yandex.mobile.ads.common.AdRequestError;
import com.yandex.mobile.ads.common.ImpressionData;

public class AppActivity extends Cocos2dxActivity {

    //private static AppActivity mainActivity;
    public static AppActivity mainActivity;
    public static String TAG = "MainAppActivity";

    //orientation lock
    final String ORIENTATION_KEY = "orientation_locked";
    final int ORIENTATION_DEFAULT = -1;

    //admob
    final String YA_RW_UNIT_ID = "R-M-8642780-1";

    private com.yandex.mobile.ads.rewarded.RewardedAd mRewardedAdY = null;

    boolean isActivityHidden = true;

    public native void rewardDidPresent();
    public native void rewardDidDismiss();
    public native void rewardUser();
    public native void rewardNotLoaded();


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        super.setEnableVirtualButton(false);
        mainActivity = this;
        if (!isTaskRoot()) {
            // Android launched another instance of the root mainActivity into an existing task
            //  so just quietly finish and go away, dropping the user back into the mainActivity
            //  at the top of the stack (ie: the last state of this task)
            // Don't need to finish it again since it's finished in super.onCreate .
            return;
        }
        // DO OTHER INITIALIZATION BELOW

        //orientation lock
//        final int orientation = loadIntForKey(ORIENTATION_KEY);
//        if (orientation != ORIENTATION_DEFAULT) {
//            super.setRequestedOrientation(orientation);
//        } else {
//            super.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_USER);
//        }
        super.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);

        loadRewardAdY();
    }

    //lock screen orientation
    public static void setAutoOrientationEnabled() {
        if (isOrientationLocked()) {
            mainActivity.unlockOrientation();
        } else {
            mainActivity.lockOrientation();
        }
    }

    private void lockOrientation() {
        Log.d(TAG, "lockOrientation");
        final int orientation = getResources().getConfiguration().orientation;
        final int rotation = getWindowManager().getDefaultDisplay().getOrientation();
        if (rotation == Surface.ROTATION_0 || rotation == Surface.ROTATION_90) {
            if (orientation == Configuration.ORIENTATION_PORTRAIT) {
                setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
                saveIntForKey(ORIENTATION_KEY, ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
            }
            else if (orientation == Configuration.ORIENTATION_LANDSCAPE) {
                setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
                saveIntForKey(ORIENTATION_KEY, ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
            }
        }
        else if (rotation == Surface.ROTATION_180 || rotation == Surface.ROTATION_270) {
            if (orientation == Configuration.ORIENTATION_PORTRAIT) {
                setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_REVERSE_PORTRAIT);
                saveIntForKey(ORIENTATION_KEY, ActivityInfo.SCREEN_ORIENTATION_REVERSE_PORTRAIT);
            }
            else if (orientation == Configuration.ORIENTATION_LANDSCAPE) {
                setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_REVERSE_LANDSCAPE);
                saveIntForKey(ORIENTATION_KEY, ActivityInfo.SCREEN_ORIENTATION_REVERSE_LANDSCAPE);
            }
        }
    }

    private void unlockOrientation() {
        Log.d(TAG, "unlockOrientation");
        super.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_USER);
        saveIntForKey(ORIENTATION_KEY, ORIENTATION_DEFAULT);
    }

    public static boolean isOrientationLocked() {
        return (mainActivity.loadIntForKey(mainActivity.ORIENTATION_KEY) != mainActivity.ORIENTATION_DEFAULT);
    }

    public void loadRewardAdY() {
        Log.d("yandex_ads", "loadRewardAdY");
        destroyRewardedAd();

        RewardedAdLoader loader = new RewardedAdLoader(this);
        loader.setAdLoadListener(new RewardedAdLoadListener() {
            @Override public void onAdLoaded(@NonNull com.yandex.mobile.ads.rewarded.RewardedAd rewardedAd) {
                Log.d("yandex_ads", "onAdLoaded(reward)");
                mRewardedAdY = rewardedAd;
                mRewardedAdY.setAdEventListener(new RewardedAdEventListener() {
                    @Override public void onAdShown() { Log.d("yandex_ads", "onAdShown(reward)");
                        rewardDidPresent();
                    }
                    @Override public void onAdFailedToShow(@NonNull com.yandex.mobile.ads.common.AdError adError) {
                        Log.d("yandex_ads", "onAdFailedToShow(reward)" + adError.getDescription());
                        loadRewardAdY();
                    }
                    @Override public void onAdDismissed() {
                        rewardDidDismiss();
                        mRewardedAdY = null;
                        Log.d("yandex_ads", "onAdDismissed(reward)");
                        loadRewardAdY();
                    }
                    @Override public void onAdClicked() { Log.d("yandex_ads", "onAdClicked(reward)"); }
                    @Override public void onAdImpression(@Nullable ImpressionData impressionData) { Log.d("yandex_ads", "onImpression(reward)"); }
                    @Override public void onRewarded(@NonNull Reward reward) {
                        Log.d("yandex_ads", "onRewarded(reward)");
                        mainActivity.rewardUser();
                    }
                });
            }
            @Override public void onAdFailedToLoad(@NonNull AdRequestError adRequestError) { Log.d("yandex_ads", "onAdFailedToLoad(reward)" + adRequestError.getDescription());
                mRewardedAdY = null;
                rewardNotLoaded();
            }
            public void onLeftApplication() { Log.d("yandex_ads", "onLeftApplication(reward)"); }
            public void onReturnedToApplication() { Log.d("yandex_ads", "onReturnedToApplication(reward)"); }
        });
        loader.loadAd(new AdRequestConfiguration.Builder(YA_RW_UNIT_ID).build());
    }

    static public void showRewardAd() {
        mainActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if(mainActivity.mRewardedAdY != null) {
                    mainActivity.mRewardedAdY.show(mainActivity);
                }else {
                    Log.d(TAG, "The rewarded ad wasn't ready yet.");
                    mainActivity.loadRewardAdY();
                    mainActivity.rewardNotLoaded();
                }
            }
        });
    }

    static public boolean isRewardLoaded() {
        return mainActivity.mRewardedAdY != null;
    }

    public static void makeToast(String message) {
        mainActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Toast.makeText(mainActivity, message, Toast.LENGTH_LONG).show();
            }
        });
    }

    //utils
    private void saveIntForKey(final String key, final int value) {
        SharedPreferences preferences = getSharedPreferences("MyPreferences", Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = preferences.edit();
        editor.putInt(key, value);
        editor.apply();
    }

    private int loadIntForKey(final String key) {
        SharedPreferences preferences = getSharedPreferences("MyPreferences", Context.MODE_PRIVATE);
        return preferences.getInt(key, -1);
    }

    private void saveFloatForKey(final String key, float value) {
        SharedPreferences preferences = getSharedPreferences("MyPreferences", Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = preferences.edit();
        editor.putFloat(key, value);
        editor.apply();
    }

    private float loadFloatForKey(final String key) {
        SharedPreferences preferences = getSharedPreferences("MyPreferences", Context.MODE_PRIVATE);
        return preferences.getFloat(key, -1.0f);
    }

    static public boolean isKeyUsed(final String key) {
        SharedPreferences preferences = mainActivity.getSharedPreferences("MyPreferences", Context.MODE_PRIVATE);
        return preferences.contains(key);
    }

    private void deleteKey(final String key) {
        SharedPreferences preferences = getSharedPreferences("MyPreferences", Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = preferences.edit();
        editor.remove(key);
        editor.apply();
    }

    //Activity
    @Override protected void onStart() {
        super.onStart();
    }

    @Override
    protected void onResume() {
        super.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
    }

    @Override protected void onStop() {
        super.onStop();
    }

    @Override
    protected void onDestroy() {
        destroyRewardedAd();
        super.onDestroy();
    }
    private void destroyRewardedAd() {
//        if (mRewardedAdY != null) {
//            mRewardedAdY.setAdEventListener(null);
//            mRewardedAdY = null;
//        }
    }
}