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
#include "Framework/SystemSingleton.h"
#include "Framework/KeyboardAction.h"
#include "Framework/TouchAction.h"
#include "Framework/3DMath.h"

#include <EASTL/string.h>

namespace BohgeEngine{class IPlugin;}
namespace BohgeApp
{
	class ILiteapp;
	class IPluginLoader;
	class IApplication : public BohgeEngine::Object
	{
		SYSTEM_SINGLETON_DECLEAR( IApplication );
		RTTI_ABSTRACT_TYPE_DECLEAR( IApplication );
	private:
		IPluginLoader*	m_pLoader;
		ILiteapp*		m_pLiteapp;
		ILiteapp*		m_pNextLite;
	public:
		IApplication(void);
		~IApplication(void);
	private:
		virtual IPluginLoader* _CreateLoader() = 0;
		virtual void _ReleaseLoader( IPluginLoader* loder ) = 0;
	public:
		void Initialize( void* hWindows, const BohgeEngine::vector4d& view, const BohgeEngine::vector2d& resolution, const eastl::string& root, const eastl::string& wirte );
		void Resizeview( const BohgeEngine::vector4d& view );
		void PushTouchAction( const BohgeEngine::TouchAction& ta );
		void PushKeyboardAction( const BohgeEngine::KeyboardAction& ka );
		void Update();
		void Exit();
		void SwapLiteapp( ILiteapp* lite );
	};
}
