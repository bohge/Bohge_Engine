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



/////////////////////////////
//			包装层		   //
/////////////////////////////
#include "Engine.h"
#include "IOSystem.h"
#include "Environment.h"
#include "Bfont.h"
#include "UIManager.h"
#include "ActionManager.h"
#include "Camera.h"
#include "ShaderManager.h"
#include "SceneManager.h"
#include "UIManager.h"
#include "Pipeline.h"
#include "Log.h"
#include "ResourceManager.h"
#include "NetHost.h"
#include "SkyBox.h"
#include "PhysicManager.h"
#include "ScriptManager.h"
#include "Environment.h"
#include "RendBuffer.h"
#include "Language.h"
#include "MaterialManager.h"
#include "SoundManager.h"


#ifdef IOS
#include <sys/time.h>
#endif
#include <time.h>


using namespace BohgeNet;




namespace BohgeEngine
{
	//-------------------------------------------------------------------
	Engine::Engine()
		:m_pDevice(NULL),
		m_pEnvironment(NULL),
		m_uFPS(0),
		m_isUseLightCamera(false),
		m_pViewCamera(NULL),
		m_pActivCamera(NULL),
		m_pPhysic(NULL)
	{
#ifdef NEON_MATH
		InitializationNeonMath();
#endif
	}
	//-------------------------------------------------------------------
	Engine::~Engine()
	{
	}
	//-------------------------------------------------------------------
	void Engine::Initialization( const vector2d& viewport, const vector2d& resolution, DeviceLevel l, const std::string& root, const std::string& wirte )
	{
		//构造其他引擎成员必须在engine构造之后
		//应为其他成员的构造可能会调用engine
		IOSystem::Create();
		IOSystem::Instance().SetRootPath( root );
		IOSystem::Instance().SetWritePath( wirte );
		SharedBuffer::CreatSharedMemeryInstance();
		LanguageControl::Create( );
		m_Sound = SoundManager::Create( );
		m_eDeviceLevel = l;
		m_pDevice = NEW Device();
		m_pDevice->OnDeviceCreate();
		m_pDevice->SetViewport(vector4d(0,0,viewport.m_x,viewport.m_y));
		m_pDevice->SetResolutionSize(resolution);
		m_pAction = NEW ActionManager();
		//	m_pCamera = NEW Camera();
		m_pShader = NEW ShaderManager();
		m_pMaterial = NEW MaterialManager;
		m_pUI = NEW UIManager();
		m_pString = NEW StringManager();
		//m_pParticle = NEW EmitterManager();
		m_pPipeline = NEW Pipeline();
		m_pResource = NEW ResourceManager();
		m_pSecen = NEW SceneManager();
		m_pPhysic = NEW PhysicManager();
		m_pScript = NEW ScriptManager();
		m_pNet = NEW NetHost();
		m_pEnvironment = NEW Environment( vector3f(0,100,0), vector3f(0,-1,0), vector3f(0.2,0.2,0.2), vector3f(1,1,1), vector3f(0.99,0.66,0.38), vector3f(0,0,0), vector3f(0,0,0) );
		m_pNet->Initialization();//初始化网络
		m_pMaterial->Initialization( );
		//初始化随机数
		srand(unsigned(time(0)));
		//得到时间
#ifdef WIN32
		m_tBeginTiem = clock();
#endif
#ifdef ANDROID
		timespec time1;
		clock_gettime(CLOCK_MONOTONIC, &time1);
		m_tBeginTiem = time1.tv_sec * 1000 + time1.tv_nsec / 1000000;
#endif
#ifdef IOS
		struct timeval tv;
		gettimeofday(&tv, NULL);
		m_tBeginTiem = 1000* tv.tv_sec + tv.tv_usec / 1000;
#endif
		DEBUGLOG("begin time is %d\n", m_tBeginTiem);
		m_uTimeSpan = 0;
		m_tLastTime = 0;
	}
	//-------------------------------------------------------------------
	void Engine::Drop()
	{
		SharedBuffer::DestorySharedMemeryInstance();
		LanguageControl::Destroy();
		SoundManager::Destroy();
		m_Sound = NULL;
		m_pNet->Destry();//消灭网络
		SAFE_DELETE( m_pAction );
		m_pViewCamera = NULL;
		//SAFE_DELETE( m_pCamera );//外部给进来的，外部删除
		SAFE_DELETE( m_pMaterial );
		SAFE_DELETE( m_pShader );
		SAFE_DELETE( m_pUI );
		SAFE_DELETE( m_pString );
		//SAFE_DELETE( m_pParticle );
		SAFE_DELETE( m_pPipeline );
		SAFE_DELETE( m_pNet );
		SAFE_DELETE( m_pPhysic );
		SAFE_DELETE( m_pScript );
		SAFE_DELETE( m_pSecen );
		SAFE_DELETE( m_pEnvironment );
		SAFE_DELETE( m_pResource );
		m_pDevice->OnDeviceDestroy();
		SAFE_DELETE( m_pDevice );
		IOSystem::Destroy();
	}
	//-------------------------------------------------------------------
	void Engine::_AttachCamera( ICamera* cptr )
	{
		m_pViewCamera = cptr;
		if ( !m_isUseLightCamera )
		{
			m_pActivCamera = cptr;
		}
	}
	//-------------------------------------------------------------------
	void Engine::OnFramebegin()
	{
		m_pNet->OnFarmeBegin();//每帧开始前处理网络输入
	}
	//-------------------------------------------------------------------
	void Engine::Update()	//更新engine中的各种manage
	{
		m_pDevice->Update();
		m_pAction->Update( m_uTimeSpan );
		m_pViewCamera->Update();	//更新摄像机
		m_pPhysic->Update( m_uTimeSpan );//更新物理
		if ( m_pViewCamera->isNearFarChange() )
		{
			m_pEnvironment->RecaculateFogParam();
		}
		m_pSecen->BeginUpdate( *this );
		m_Sound->Update();//检查声音的播放状态
		//m_pParticle->Update(*this, m_uTimeSpan);
		m_pUI->Update(m_uTimeSpan);
		m_pSecen->Update( m_uTimeSpan);//调用Update更新场景
		m_pEnvironment->Update( *m_pViewCamera, m_uTimeSpan );
		m_pSecen->EndUpdate( *this );
	}
	//-------------------------------------------------------------------
	void Engine::Render()
	{
		m_pNet->OnFarmeEnd();//发送本帧内需要发送的网络讯息
		m_pAction->Reset();//特殊放在此处，重置动作mask
		m_pPipeline->Render(*this);

#ifdef DRAWACTIONLINE
		m_pAction->Render( *this );
#endif

		static uint time = 0;//计时工作
		static uint frame = 0;
		ulong CurrentTime;
#ifdef WIN32
		CurrentTime = clock();
#endif
#ifdef ANDROID
		timespec time1;
		clock_gettime(CLOCK_MONOTONIC, &time1);
		CurrentTime = time1.tv_sec * 1000 + time1.tv_nsec / 1000000;
#endif
#ifdef IOS
		struct timeval tv;
		gettimeofday(&tv, NULL);
		CurrentTime = 1000* tv.tv_sec + tv.tv_usec / 1000;
#endif
		CurrentTime -= m_tBeginTiem;
		m_uTimeSpan = CurrentTime - m_tLastTime;
		ASSERT( m_uTimeSpan >= 0 );
		m_tLastTime = CurrentTime;
		time += m_uTimeSpan;
		frame ++;
		if( time >= 5000 )
		{
			m_uFPS = frame / 5;
			time = 0;
			frame = 0;
			DEBUGLOG("FPS %d\n", m_uFPS);
			//printf("FPS %d\n", m_uFPS);
		}
	}
	//-------------------------------------------------------------------
	uint Engine::CurrentTime()
	{
		ulong CurrentTime;
#ifdef WIN32
		CurrentTime = clock();
#endif
#ifdef ANDROID
		timespec time1;
		clock_gettime(CLOCK_MONOTONIC, &time1);
		CurrentTime = time1.tv_sec * 1000 + time1.tv_nsec / 1000000;
#endif
#ifdef IOS
		struct timeval tv;
		gettimeofday(&tv, NULL);
		CurrentTime = 1000* tv.tv_sec + tv.tv_usec / 1000;
#endif
		return CurrentTime;
	}
	//-------------------------------------------------------------------
	Engine* Engine::m_pEngine = NULL;
	Engine* Engine::CreateEngine(const vector2d& viewport, const vector2d& resolution, DeviceLevel l, const std::string& root, const std::string& wirte )
	{
		m_pEngine = NEW Engine();
		m_pEngine->Initialization( viewport, resolution, l, root, wirte );
		return m_pEngine;
	}
	//-------------------------------------------------------------------
	void Engine::DropEngine(Engine** engine)
	{
		DEBUGLOG("engine Exit!!\n",1);
		m_pEngine->Drop();
		SAFE_DELETE(m_pEngine);
		if( NULL != engine)
		{
			*engine = NULL;
		}
	}
	//-------------------------------------------------------------------
	void Engine::ResizeViewport( const vector2d& size )
	{
		m_pDevice->SetViewport( vector4d( 0, 0, size.m_x, size.m_y ) );
		m_pDevice->SetResolutionSize( size );
		m_pPipeline->OnSizeChange( size );
		m_pViewCamera->CreatePerspectiveProjection( m_pViewCamera->GetNear(), m_pViewCamera->GetFar() );
		m_pString->OnSizeChange( size );
	}

}