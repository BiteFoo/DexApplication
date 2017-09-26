package com.dexapplication;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

import java.lang.reflect.Method;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
        try
        {
            Method method =MainActivity.class.getMethod("ret1");
            tv.setText(stringFromJNI(method) +" , ret value "+ret1());

        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        }

    }

   public  int ret1(){
       return  1;
   }
   public int ret2(){
       return  2;
   }
    public void test(){}

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI(Method method);
}
