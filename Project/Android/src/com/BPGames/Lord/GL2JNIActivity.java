/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.BPGames.Lord;

import android.app.Activity;
import android.os.Bundle;
import android.view.Gravity;
import android.view.KeyEvent;
import android.widget.FrameLayout;
import android.util.DisplayMetrics;  
//import android.util.Log;

import com.BPGames.Lord.R;


public class GL2JNIActivity extends Activity {

	private static FrameLayout layout;
	private static GL2JNIView mView;

	@Override protected void onCreate(Bundle icicle) {
        super.onCreate(icicle);
        setContentView(R.layout.main);        
        //初始化opengl
       	mView = new GL2JNIView(getApplication());
        GL2JNILib.file("");
        layout = (FrameLayout) findViewById(R.id.layout_main);
        //获取分辨率,这个地方获取的是设备的分辨率，你们需要改为获取你们需要的layout的分辨率
        DisplayMetrics dm = new DisplayMetrics();     
        getWindowManager().getDefaultDisplay().getMetrics(dm);
        // 在其中添加 adView
        FrameLayout.LayoutParams surfaceParams = new FrameLayout.LayoutParams( dm.widthPixels, dm.heightPixels,Gravity.TOP );
        layout.addView(mView, surfaceParams);
    }

    @Override protected void onPause() {
        super.onPause();
       	mView.onPause();
    }
	@Override public boolean onKeyDown(int keyCode, KeyEvent event) {          
        //按下键盘上返回,搜索按钮  
        if(keyCode == KeyEvent.KEYCODE_BACK || keyCode == KeyEvent.KEYCODE_SEARCH || keyCode == KeyEvent.KEYCODE_HOME )
        { 
        	if( keyCode == KeyEvent.KEYCODE_BACK || keyCode == KeyEvent.KEYCODE_HOME)
        	{
               	mView.onKeyDown(1);//此处对应jni的响应
        	}
        	return true;  
        }else
        {        
            return super.onKeyDown(keyCode, event);  
        }
    } 
    @Override protected void onResume() {
        super.onResume();
       	mView.onResume();
    }
    @Override protected void onDestroy()
    {
    	super.onDestroy();
       	mView.Exit();
		int pid = android.os.Process.myPid();
		android.os.Process.killProcess(pid);
    	
    }
}
