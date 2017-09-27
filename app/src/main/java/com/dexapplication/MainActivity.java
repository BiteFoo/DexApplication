package com.dexapplication;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import java.lang.reflect.Method;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
//    static {
//        System.loadLibrary("native-lib");
//    }
    TextView tv;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        // Example of a call to a native method
         tv = (TextView) findViewById(R.id.sample_text);
        tv.setText("before changeCode ret1()= "+ret1());
        Log.d("selfmodify","before changeCode"+ret1());
//        try
//        {
//            Method method =MainActivity.class.getMethod("ret1");
//            tv.setText("chang "+stringFromJNI(method));
//            tv.setText("call changeCode ret1() = " + ret1());
//            Log.d("selfmodify","call changeCode ret1()" + ret1());
//
////            tv.setText(" , ret value "+ret1());
//            Log.d("selfmodify","call ret1() finished");
//            /*
//            * 这里无论是采用的MainActivity来修改Test中的ret1()方法还是Application作为修改，都能完成修改。
//            *
//            *
//            *
//            * */
////            tv.setText("change value "+new Test().ret1());
//        } catch (NoSuchMethodException e) {
//            e.printStackTrace();
//        }
      //Log.d("selfmodify","before changeCode"+ret1());
    }

    public  void changeCode(View v){

        Toast.makeText(this,"toast",Toast.LENGTH_SHORT).show();

       try
        {
            Method method =MainActivity.class.getMethod("ret1");
            tv.setText(stringFromJNI(method)+ "\ncall changeCode ret1() = " + ret1());
            Log.d("mmdalvik","call changeCode ret1()" + ret1());
//            tv.setText(" , ret value "+ret1());
            Log.d("mmdalvik","call ret1() finished");
           /* * 这里无论是采用的MainActivity来修改Test中的ret1()方法还是Application作为修改，都能完成修改。
            */
//            tv.setText("change value "+new Test().ret1());
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
    public native String stringFromJNI();
}
