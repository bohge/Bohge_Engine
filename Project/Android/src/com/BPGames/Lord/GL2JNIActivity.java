//////////////////////////////////////////////////////////////////////////////////////
//
//						The Bohge Engine License (BEL)
//
//	Copyright (c) 2011-2014 Peng Zhao
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in 
//	all copies or substantial portions of the Software. And the logo of 
//	Bohge Engine shall be displayed full screen for more than 3 seconds 
//	when the software is started. Copyright holders are allowed to develop 
//	game edit based on Bohge Engine, The edit must be released under the MIT 
//	open source license if it is going to be published. In no event shall 
//	copyright holders be prohibited from using any code of Bohge Engine 
//	to develop any other analogous game engines.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//
//////////////////////////////////////////////////////////////////////////////////////



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
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.location.Location;
import android.location.LocationManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.StatFs;
import android.view.Gravity;
import android.view.KeyEvent;
import android.widget.FrameLayout;
import android.util.DisplayMetrics;  
//import android.util.Log;

import com.BPGames.Lord.R;
import com.google.ads.*;


public class GL2JNIActivity extends Activity {

	private static GL2JNIActivity thisApp;
	private static FrameLayout layout;
	private static int m_lastSignal;	//之前的信号
	private static GL2JNIView mView;
    private static AdView adView; 
    private static int width;
    private static int height;
    private static ADCallBack callback;
    private boolean NotMemory;
    
    private static Handler mHandler = new Handler(){ 
        @Override
        public void handleMessage(Message msg)
        {
        	if( msg.what != m_lastSignal )
        	{
        		//Log.w("Bohge", "Ad message "+Integer.toString(msg.what) );
        		m_lastSignal = msg.what;
        		switch( msg.what )
        		{
        		case -1:
        			{
        				thisApp.finish();//退出
        			}break;
        		case 0:
        			{
        				adView.setEnabled(false);
        				layout.removeView(adView);
        			}break;
        		case 1: 
        			{
        				layout.removeView(adView);
        				FrameLayout.LayoutParams adParams = new FrameLayout.LayoutParams( width, height, Gravity.TOP | Gravity.CENTER );
        				adView.setEnabled(true);
        				layout.addView(adView, adParams);
        			}break;
        		case 2: 
    				{
    					layout.removeView(adView);
    					FrameLayout.LayoutParams adParams = new FrameLayout.LayoutParams( width, height, Gravity.TOP | Gravity.LEFT );
    					adView.setEnabled(true);
    					layout.addView(adView, adParams);
    				}break;
        		case 3: 
    				{
    					layout.removeView(adView);
    					FrameLayout.LayoutParams adParams = new FrameLayout.LayoutParams( width, height, Gravity.TOP | Gravity.RIGHT );
    					adView.setEnabled(true);
    					layout.addView(adView, adParams);
    				}break;
        		}
        	}
        }
    };

	@Override protected void onCreate(Bundle icicle) {
        super.onCreate(icicle);
        setContentView(R.layout.main);        
        thisApp = this;
        
        //计算空闲磁盘
        ApplicationInfo appInfo = null;
        PackageManager packMgmr = this.getPackageManager();
        String apkName = this.getPackageName();
        try {
			appInfo = packMgmr.getApplicationInfo(apkName, 0);
		} catch (NameNotFoundException e) {
			e.printStackTrace();
		}
        String dir = appInfo.dataDir;
        GL2JNILib.file(dir);
    	NotMemory = false;
        if( true == GL2JNILib.isFirstTime() )//如果第一次进入，则查看是否有足够空间 
        {
        	StatFs stat = new StatFs(appInfo.dataDir); //获取app安装路径空闲大小
        	double sdAvailSize = (double)stat.getAvailableBlocks() * (double)stat.getBlockSize() / 1024 /1024;//可用空间
        	//Log.i("Bohge", "Free size "+Double.toString(sdAvailSize) );
        	if( sdAvailSize < 25 ) //如果磁盘空闲小于25mb提示不能玩
        	{
        		NotMemory = true;
        		//this.finish();
        		AlertDialog.Builder dlgAlert  = new AlertDialog.Builder(this);                      
        		dlgAlert.setTitle("Error"); 
        		dlgAlert.setMessage("Requires 25MB free memory at least"); 
        		dlgAlert.setPositiveButton("OK",new DialogInterface.OnClickListener() {
        			public void onClick(DialogInterface dialog, int whichButton) {
        				thisApp.finish(); 
        			}
        		});
        		dlgAlert.setCancelable(false);
        		dlgAlert.create().show();
        		return;
        	}
        }
        //获取分辨率
        DisplayMetrics dm = new DisplayMetrics();     
        getWindowManager().getDefaultDisplay().getMetrics(dm);
        //初始化opengl
        if( !NotMemory )
        {
        	mView = new GL2JNIView(getApplication(), mHandler);
        }
        
        // 创建 adView
        //String ID = GL2JNILib.GetAdmobID();
        //adView = new AdView(this, AdSize.BANNER, "a14fdea136baacc");
        //String testID = "a14fdea136baacc";
        //if(  testID.equals(ID) )
        //{
        //	Log.w("Bohge", "done!" + ID );
        //}
        //else
        //{
        //	Log.w("Bohge", "error ID!!" + ID );
        //}
        
        adView = new AdView(this, AdSize.BANNER, GL2JNILib.GetAdmobID());
        // 查找 FrameLayout，假设其已获得
        // 属性 android:id="@+id/mainLayout"
        layout = (FrameLayout) findViewById(R.id.layout_main);
        // 在其中添加 adView
        FrameLayout.LayoutParams surfaceParams = new FrameLayout.LayoutParams( dm.widthPixels, dm.heightPixels,Gravity.TOP );
        layout.addView(mView, surfaceParams);
        width = AdSize.BANNER.getWidthInPixels(getBaseContext());
        height = AdSize.BANNER.getHeightInPixels(getBaseContext());
        
        AdRequest adRequest = new AdRequest();
        
        //得到最后一个可用的坐标信息
        LocationManager mgr = (LocationManager) getSystemService(Context.LOCATION_SERVICE); 
        String locationProvider = LocationManager.NETWORK_PROVIDER;
        Location lastKnownLocation = mgr.getLastKnownLocation(locationProvider);
        if( null != lastKnownLocation )
        {
        	//Log.i("Bohge", "Loaction" + lastKnownLocation.toString() );
        	adRequest.setLocation(lastKnownLocation);
        }
        
    	//adRequest.addTestDevice("PB3NTJ2QHS7MT");// 测试 Android 设备
    	
    	// 启动一般性请求并在其中加载广告
    	//adView.loadAd( adRequest );
    	callback = new ADCallBack();
    	adView.setAdListener(callback);
    	m_lastSignal = 0;//不显示广告
    }

    @Override protected void onPause() {
        super.onPause();
        if( !NotMemory )
        {
        	mView.onPause();
        }
    	this.finish();//直接销毁了吧，省得麻烦
    }
	@Override public boolean onKeyDown(int keyCode, KeyEvent event) {          
        //按下键盘上返回,搜索按钮  
        if(keyCode == KeyEvent.KEYCODE_BACK || keyCode == KeyEvent.KEYCODE_SEARCH || keyCode == KeyEvent.KEYCODE_HOME )
        { 
        	if( keyCode == KeyEvent.KEYCODE_BACK || keyCode == KeyEvent.KEYCODE_HOME)
        	{
                if( !NotMemory )
                {
                	mView.onKeyDown(1);//此处对应jni的响应
                }
        	}
        	return true;  
        }else
        {        
            return super.onKeyDown(keyCode, event);  
        }
    } 
    @Override protected void onResume() {
        super.onResume();
        if( !NotMemory )
        {
        	mView.onResume();
        }
    }
    @Override protected void onDestroy()
    {
    	super.onDestroy();
    	adView.destroy();
        if( !NotMemory )
        {
        	mView.Exit();
        }
		int pid = android.os.Process.myPid();
		android.os.Process.killProcess(pid);
    	
    }
}
