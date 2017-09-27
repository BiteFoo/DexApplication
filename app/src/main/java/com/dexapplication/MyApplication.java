package com.dexapplication;

import android.app.Application;
import android.content.Context;

/**
 * Created by John.Lu on 2017/9/27.
 */

public class MyApplication extends Application {
    @Override
    protected void attachBaseContext(Context base) {
        super.attachBaseContext(base);
        System.loadLibrary("native-lib");
    }

    @Override
    public void onCreate() {
        super.onCreate();
      //  initChange();
    }
    public native void initChange();//

}
