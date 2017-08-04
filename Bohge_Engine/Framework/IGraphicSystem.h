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
#pragma once
#include "IResourceProperty.h"
#include "ISharedResource.h"
#include "SystemSingleton.h"
#include "ShaderProperty.h"
#include "Delegate.hpp"
#include "IPlugin.h"


namespace BohgeEngine
{
	class BOHGE_FRAMEWORK IGraphicSystem : public IPlugin
	{
		SYSTEM_SINGLETON_DECLEAR(IGraphicSystem);
		RTTI_ABSTRACT_DRIVER_TYPE_DECLEAR( IGraphicSystem, IPlugin )
	private:
		Delegate<void>		m_GPUUpdataEvent;
	public:
		IGraphicSystem(void){SYSTEM_SINGLETON_INITIALIZE;}
		virtual ~IGraphicSystem(void){SYSTEM_SINGLETON_DESTROY;}
	private:
		virtual void _CreateContext( void* hWindows ) = 0;//创建设备
		virtual void _SwapBuffer() = 0;
	public:
		virtual void CheckError() = 0;
		virtual ISharedResource* CreateResource( IResourceProperty::SharedResourceType type ) = 0;
		virtual void ProcessShader( ShaderProperty::ShaderCode sc, eastl::string& code ) = 0;
		virtual uint GetMaxUniformArraySize() = 0;
	public:
		virtual void RegistEvent( Framework& framework ); //注册事件
	public:
		void OnFrameworkEntry( float time, Framework& framework );
		void OnBeginRender( float time, Framework& framework );
		void OnSwapBuffer( float time, Framework& framework );
	public:
		BOHGE_FORCEINLINE FunctionPtr HostedGraphicUpdata( Delegate<void>::FunctionType* const ptr )//托管更新动作
		{
			return m_GPUUpdataEvent.Connect( ptr );
		}
	};
}



#ifdef _DEBUG
#define CHECK_GRAPHIC_SYSTEM_ERROR ( BohgeEngine::IGraphicSystem::Instance()->CheckError() )
#else
#define CHECK_GRAPHIC_SYSTEM_ERROR
#endif