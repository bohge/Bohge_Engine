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



/////////////////////////////////
//		注册engine和管理	   //
/////////////////////////////////
#pragma once
#include "Engine.h"
#include "SceneManage.h"
#include "SoundManage.h"
#include "UIManager.h"
#include "PhysicManager.h"
#include "NetHost.h"



namespace BohgeEngine
{
	static void RegisterEngine( ScriptBase* s, const char* classname )
	{
		//注册一个类，实际是table
		s->RegClass<Engine >( classname );
		//注册table成员函数
		s->RegClassFunc< Engine >( "GetCamera", &Engine::GetCamera );
		s->RegClassFunc< Engine >( "GetSceneManage", &Engine::GetSceneManage );
		s->RegClassFunc< Engine >( "GetSoundManage", &Engine::GetSoundManage );
		s->RegClassFunc< Engine >( "GetUIManager", &Engine::GetUIManager );
		s->RegClassFunc< Engine >( "GetPhysicManager", &Engine::GetPhysicManager );
		s->RegClassFunc< Engine >( "GetNet", &Engine::GetNet );
		//添加一个全局的engine
		s->SetGlobal( "EngineInstance", &Engine::Instance() );
	}



	static void RegisterUIManager( ScriptBase* s, const char* classname )
	{
		//注册一个类，实际是table
		s->RegClass<UIManager >( classname );
		//注册table成员函数
		s->RegClassFunc< UIManager >( "CreateUIGroup", &UIManager::CreateUIGroup );
		s->RegClassFunc< UIManager >( "DeleteUIGroup", &UIManager::DeleteUIGroup );
	}
}