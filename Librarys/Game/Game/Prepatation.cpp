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
//		游戏开始之前android释放资源	   //
/////////////////////////////////////////

#include "Preparation.h"
#include "EngineResourceList.h"
#include "GameResourceList.h"
#include "Unpack.h"
#include "Log.h"

#include <string>
#include <pthread.h>


using namespace std;


pthread_mutex_t Mutex = PTHREAD_MUTEX_INITIALIZER;//锁
std::string inPack = "assets/";


//--------------------------------------------------------------------------------------------------------------------------------------
void* ReleaseThread(void* Param)
{
	/*bool* isDone = static_cast<bool*>(Param);
	//font
	if( ! File::Instance().isExist(FontName) )
	{
		Unpack::Instance().Decompress(
			(inPack + FontFolder + FontName ),
			(File::Instance().WritePath() + FontFolder + FontName ) );
	}
	else
	{
		DEBUGLOG("%s, is Exist\n", FontName );
	}
	//游戏资源
	//文本
	for (int i = 0 ; i < String_Count ; i++)
	{
		if( ! File::Instance().isExist(StringFileName[i]) )
		{
			Unpack::Instance().Decompress(
				(inPack + FontFolder + StringFileName[i] ),
				(File::Instance().WritePath() + FontFolder + StringFileName[i] ) );
		}
		else
		{
			DEBUGLOG("%s, is Exist\n", StringFileName[i] );
		}
	}
	//sound
	for (int i = 0 ; i < Sound_Count ; i++)
	{
		if( ! File::Instance().isExist(SoundFileName[i]) )
		{
			Unpack::Instance().Decompress(
				(inPack + SoundFolder + SoundFileName[i] ),
				(File::Instance().WritePath() + SoundFolder + SoundFileName[i] ) );
		}
		else
		{
			DEBUGLOG("%s, is Exist\n", SoundFileName[i] );
		}
	}
	//textrue
	for (int i = 0 ; i < Texture_Count ; i++)
	{
		if( ! File::Instance().isExist(TextureFileName[i]) )
		{
			Unpack::Instance().Decompress(
				(inPack + TextureFolder + TextureFileName[i] ),
				(File::Instance().WritePath() + TextureFolder + TextureFileName[i] ) );
		}
		else
		{
			DEBUGLOG("%s, is Exist\n", TextureFileName[i] );
		}
	}
	//UI
	for (int i = 0 ; i < UI_Count ; i++)
	{
		if( ! File::Instance().isExist(UIFileName[i]) )
		{
			Unpack::Instance().Decompress(
				(inPack + UIFolder + UIFileName[i] ),
				(File::Instance().WritePath() + UIFolder + UIFileName[i] ) );
		}
		else
		{
			DEBUGLOG("%s, is Exist\n", UIFileName[i] );
		}
	}

	//模型
	//for (int i = 0 ; i < Model_Count ; i++)
	//{
	//	if( ModelFileName[i].find('.') != -1 )
	//	{
	//		if( ! File::Instance().isExist( ModelFileName[i] ) )
	//		{
	//			Unpack::Instance().Decompress(
	//				(inPack + ModelFolder + ModelFileName[i] ),
	//				(File::Instance().WritePath() + ModelFolder + ModelFileName[i] ) );
	//		}
	//		else
	//		{
	//			DEBUGLOG("%s, is Exist\n", ModelFileName[i].c_str() );
	//		}
	//	}
	//}
	//timespec time;
	//time.tv_sec = 15;
	//pthread_delay_np(&time);
	//pthread_mutex_lock( &m_Mutex );
	*isDone = true;
	//pthread_mutex_unlock( &m_Mutex );
	DEBUGLOG("Release done!\n",1);
	pthread_exit(NULL);*/
	return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------
ReleaseResource::ReleaseResource()
:m_isFinish(false)
{

}
//--------------------------------------------------------------------------------------------------------------------------------------
void ReleaseResource::ReleaseNecessary() //释放进入logo界面必须的资源
{
	//引擎必须资源
	//shader
	/*for (int i = 0 ; i < ShaderResource::SHADER_COUNT ; i++)
	{
		if( ! File::Instance().isExist( ShaderResource::Instance().GetShaderVS(i) ) )
		{
			Unpack::Instance().Decompress( //vs
				(inPack + ShaderFolder + ShaderResource::Instance().GetShaderVS(i) ),
				(File::Instance().WritePath() + ShaderFolder + ShaderResource::Instance().GetShaderVS(i) ) );
		}
		else
		{
			DEBUGLOG("%s, is Exist\n", ShaderResource::Instance().GetShaderVS(i).c_str() );
		}
		if( ! File::Instance().isExist( ShaderResource::Instance().GetShaderPS(i) ) )
		{
			Unpack::Instance().Decompress( //ps
				(inPack + ShaderFolder + ShaderResource::Instance().GetShaderPS(i) ),
				(File::Instance().WritePath() + ShaderFolder + ShaderResource::Instance().GetShaderPS(i) ) );
		}
		else
		{
			DEBUGLOG("%s, is Exist\n", ShaderResource::Instance().GetShaderPS(i).c_str() );
		}
	}
	//logo 必须资源
	//tex
	if( ! File::Instance().isExist(TextureFileName[tex_Logo]) )
	{
		Unpack::Instance().Decompress(
			(inPack + TextureFolder + TextureFileName[tex_Logo] ),
			(File::Instance().WritePath() + TextureFolder + TextureFileName[tex_Logo] ) );
	}
	else
	{
		DEBUGLOG("%s, is Exist\n", TextureFileName[tex_Logo] );
	}
	if( ! File::Instance().isExist(TextureFileName[tex_Loading]) ) //tex_Loading
	{
		Unpack::Instance().Decompress(
			(inPack + TextureFolder + TextureFileName[tex_Loading] ),
			(File::Instance().WritePath() + TextureFolder + TextureFileName[tex_Loading] ) );
	}
	else
	{
		DEBUGLOG("%s, is Exist\n", TextureFileName[tex_Loading] );
	}
	//UI
	if( ! File::Instance().isExist(UIFileName[ui_StudioLogo]) )
	{
		Unpack::Instance().Decompress(
			(inPack + UIFolder + UIFileName[ui_StudioLogo] ),
			(File::Instance().WritePath() + UIFolder + UIFileName[ui_StudioLogo] ) );
	}
	else
	{
		DEBUGLOG("%s, is Exist\n", UIFileName[ui_StudioLogo] );
	}
	if( ! File::Instance().isExist(UIFileName[ui_EngineLogo]) )
	{
		Unpack::Instance().Decompress(
			(inPack + UIFolder + UIFileName[ui_EngineLogo] ),
			(File::Instance().WritePath() + UIFolder + UIFileName[ui_EngineLogo] ) );
	}
	else
	{
		DEBUGLOG("%s, is Exist\n", UIFileName[ui_EngineLogo] );
	}
	if( ! File::Instance().isExist(UIFileName[ui_Loading]) )
	{
		Unpack::Instance().Decompress(
			(inPack + UIFolder + UIFileName[ui_Loading] ),
			(File::Instance().WritePath() + UIFolder + UIFileName[ui_Loading] ) );
	}
	else
	{
		DEBUGLOG("%s, is Exist\n", UIFileName[ui_Loading] );
	}*/
}
//--------------------------------------------------------------------------------------------------------------------------------------
void ReleaseResource::CreateReleaseTread()
{
	pthread_t pid;
	pthread_create(&pid,NULL, ReleaseThread, &m_isFinish);
}
//--------------------------------------------------------------------------------------------------------------------------------------
bool ReleaseResource::isFinished()
{
	//pthread_mutex_lock( &m_Mutex );
	bool result = m_isFinish;
	//pthread_mutex_unlock( &m_Mutex );
	return result;
	//return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------