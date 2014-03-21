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



/////////////////////////////////////////
//			enter point of game		   //
/////////////////////////////////////////

#include "Game.h"
#include "Log.h"
#include "Device.h"
#include "FilePath.h"
#include "GameSetting.h"
#include "AdHelper.h"
#include <jni.h>
#include <EGL/egl.h>
#include <GLES/gl.h>

using namespace BohgeGame;

extern "C" {
	JNIEXPORT void JNICALL Java_com_BPGames_Lord_GL2JNILib_file(JNIEnv * env, jobject obj, jstring dir);
	JNIEXPORT jboolean JNICALL Java_com_BPGames_Lord_GL2JNILib_isFirstTime(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_com_BPGames_Lord_GL2JNILib_init(JNIEnv * env, jobject obj, jint width, jint height, jboolean isHigh, jstring path);
	JNIEXPORT jboolean JNICALL Java_com_BPGames_Lord_GL2JNILib_step(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_com_BPGames_Lord_GL2JNILib_exit(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_com_BPGames_Lord_GL2JNILib_KeyDown(JNIEnv * env, jobject obj, jint key);
	JNIEXPORT void JNICALL Java_com_BPGames_Lord_GL2JNILib_TouchPoint(JNIEnv * env, jobject obj, jint length, jfloatArray jPoints );
	JNIEXPORT void JNICALL Java_com_BPGames_Lord_GL2JNILib_TouchMove(JNIEnv * env, jobject obj, jint length, jfloatArray jPoints );
	JNIEXPORT void JNICALL Java_com_BPGames_Lord_GL2JNILib_TouchRelease(JNIEnv * env, jobject obj, jint length, jfloatArray jPoints );
	JNIEXPORT jint JNICALL Java_com_BPGames_Lord_GL2JNILib_isShowAd(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_com_BPGames_Lord_GL2JNILib_ADConnect(JNIEnv * env, jobject obj);
	JNIEXPORT jstring JNICALL Java_com_BPGames_Lord_GL2JNILib_GetAdmobID(JNIEnv * env, jobject obj);
};

static vector2d g_Rsize;
JNIEXPORT void JNICALL Java_com_BPGames_Lord_GL2JNILib_file(JNIEnv * env, jobject obj, jstring dir)
{
	const char* cdir;
	jboolean isCopy;
	cdir = env->GetStringUTFChars(dir, &isCopy);
	//File::Instance().SetRootPath(string(cdir) + "/");
	//File::Instance().SetWritePath(string(cdir) + "/");
	FILEPATH.SetRootPath("/sdcard/Lord/");
	FILEPATH.SetWritePath("/sdcard/Lord/");
	env->ReleaseStringUTFChars( dir, cdir );
	env->DeleteLocalRef( dir );
}

JNIEXPORT void JNICALL Java_com_BPGames_Lord_GL2JNILib_init(JNIEnv * env, jobject obj, jint width, jint height, jboolean isHigh, jstring path)//, jstring dir)
{
	bool isds = static_cast<bool>(isHigh);
	const char* str;
	//const char* cdir;
	jboolean isCopy;
	str = env->GetStringUTFChars(path, &isCopy);
	//cdir = env->GetStringUTFChars(dir, &isCopy);
	//Unpack apk;
	//File::Instance().SetRootPath(string(cdir) + "/");
	//File::Instance().SetWritePath(string(cdir) + "/");
	//File::Instance().SetRootPath("/data/data/com.BPGames.Lord/files/");
	//File::Instance().SetRootPath("/sdcard/BreakTetris/");
	Engine::DeviceLevel dl = isds ? Engine::LEL_ULTRA : Engine::LEL_BASE;
	vector2d size( width, height );
	g_Rsize = size;
	Game::Instance().Initialization(size, size, dl, false, str, "111111111111111");
	env->ReleaseStringUTFChars( path, str );
	env->DeleteLocalRef( path );
}

JNIEXPORT jboolean JNICALL Java_com_BPGames_Lord_GL2JNILib_step(JNIEnv * env, jobject obj)
{
	bool isRunning = Game::Instance().Update();
	Game::Instance().Paint();
	return static_cast<jboolean>(isRunning);
}

JNIEXPORT void JNICALL Java_com_BPGames_Lord_GL2JNILib_exit(JNIEnv * env, jobject obj)
{
	Game::Instance().Exit();
}

static ActionManager::InputAction input;
JNIEXPORT void JNICALL Java_com_BPGames_Lord_GL2JNILib_TouchPoint(JNIEnv * env, jobject obj, jint length, jfloatArray jPoints )
{
	Engine& engine = Engine::Instance();

	float* arr;
	arr = env->GetFloatArrayElements( jPoints, NULL );
	input.Begin();
	input.SetType( ActionManager::INPUT_DOWN );
	for ( int i = 0; i < length; i ++ )
	{		
		float xPos = arr[i*2];;
		float yPos = g_Rsize.m_y - arr[i*2+1];//转换
		xPos /= g_Rsize.m_x;
		xPos = 2.0 * xPos - 1.0;
		yPos /= g_Rsize.m_y;
		yPos = 2.0 * yPos - 1.0;
		input.SetPoint( vector2f(xPos,yPos) );
	}
	Game::Instance().TouchPoint(input);
	env->ReleaseFloatArrayElements( jPoints, arr, 0 );
	env->DeleteLocalRef( jPoints );
}
JNIEXPORT void JNICALL Java_com_BPGames_Lord_GL2JNILib_TouchMove(JNIEnv * env, jobject obj, jint length, jfloatArray jPoints )
{
	Engine& engine = Engine::Instance();

	float* arr;
	arr = env->GetFloatArrayElements( jPoints, NULL );
	input.Begin();
	input.SetType( ActionManager::INPUT_MOVE );
	for ( int i = 0; i < length; i ++ )
	{		
		float xPos = arr[i*2];;
		float yPos = g_Rsize.m_y - arr[i*2+1];//转换
		xPos /= g_Rsize.m_x;
		xPos = 2.0 * xPos - 1.0;
		yPos /= g_Rsize.m_y;
		yPos = 2.0 * yPos - 1.0;
		input.SetPoint( vector2f(xPos,yPos) );
	}
	Game::Instance().TouchPoint(input);
	env->ReleaseFloatArrayElements( jPoints, arr, 0 );
	env->DeleteLocalRef( jPoints );
}
JNIEXPORT void JNICALL Java_com_BPGames_Lord_GL2JNILib_TouchRelease(JNIEnv * env, jobject obj, jint length, jfloatArray jPoints )
{
	Engine& engine = Engine::Instance();

	float* arr;
	arr = env->GetFloatArrayElements( jPoints, NULL );
	input.Begin();
	input.SetType( ActionManager::INPUT_UP );
	for ( int i = 0; i < length; i ++ )
	{		
		float xPos = arr[i*2];;
		float yPos = g_Rsize.m_y - arr[i*2+1];//转换
		xPos /= g_Rsize.m_x;
		xPos = 2.0 * xPos - 1.0;
		yPos /= g_Rsize.m_y;
		yPos = 2.0 * yPos - 1.0;
		input.SetPoint( vector2f(xPos,yPos) );
	}
	Game::Instance().TouchPoint(input);
	env->ReleaseFloatArrayElements( jPoints, arr, 0 );
	env->DeleteLocalRef( jPoints );
}

JNIEXPORT jint JNICALL Java_com_BPGames_Lord_GL2JNILib_isShowAd(JNIEnv * env, jobject obj)
{
	//此处-1被占用，不能回传-1应为-1在java中是退出游戏的信号
	if( Game::Instance().isShowAd() )
	{
		AdHelper::On_Screen os = AdHelper::Instance().Where();
		int r = 0;
		switch( os )
		{
		case AdHelper::OS_TOPCENTER : r=1;break;
		case AdHelper::OS_TOPLEFT : r=2;break;
		case AdHelper::OS_TOPRIGHT : r=3;break;
		}
		return r;
	}
	return 0;
}

JNIEXPORT void JNICALL Java_com_BPGames_Lord_GL2JNILib_KeyDown(JNIEnv * env, jobject obj, jint key)
{
	switch(key) 
	{
	case 1:
		{
			Game::Instance().SetActionState(ActionManager::ACTION_EXIT);
		}break;
	}
}


JNIEXPORT void JNICALL Java_com_BPGames_Lord_GL2JNILib_ADConnect(JNIEnv * env, jobject obj)
{
	Game::Instance().ADConnect();
}

JNIEXPORT jstring JNICALL Java_com_BPGames_Lord_GL2JNILib_GetAdmobID(JNIEnv * env, jobject obj)
{
	std::string str = Game::Instance().GetAdmobID(AdHelper::PF_ANDROID);
	return env->NewStringUTF(str.c_str());  
	//jclass strClass = env->FindClass("Ljava/lang/String;");
	//jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
	//jbyteArray bytes = env->NewByteArray(str.size());
	//env->SetByteArrayRegion(bytes, 0, str.size(), (jbyte*)str.c_str());
	//jstring encoding = env->NewStringUTF("utf-8");
	//return (jstring)env->NewObject(strClass, ctorID, bytes, encoding);
}

JNIEXPORT jboolean JNICALL Java_com_BPGames_Lord_GL2JNILib_isFirstTime(JNIEnv * env, jobject obj)
{
	return static_cast<jboolean>( !GameSetting::Instance().isSettingExist() );
}

void IAPBuyItem( int index )
{

}

void RefreshItem()
{

}

void Keyborad(const vector4f& rect, uint maxsize, const std::wstring& was, bool isNumberOnly  )
{

}

void HideKeyBorad()
{

}