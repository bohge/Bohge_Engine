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
#include "3DMath.h"

#include <EASTL/string.h>

namespace BohgeEngine
{
	class BOHGE_FRAMEWORK Config
	{
	private:
		enum Constant
		{
			IO_MAX_LITE_PATH_SIZE = 255,
		};
	private:
		vector4d		m_ViewSize;//屏幕尺寸
		vector2d		m_Resolution;//分辨率
		eastl::string		m_RootPath;//资源系统根目录
		eastl::string		m_ResourcePath;//可写目录
		eastl::string		m_LitePath;//游戏目录
	public:
		Config(void);
		~Config(void);
	public:
		void SetViewSize( const vector4d& size );
		void SetResolution( const vector2d& res );
		void SetRootPath( const eastl::string& path );
		void SetResourcePath( const eastl::string& path );
		void SetLitePath( const eastl::string& path );
		const vector4d& GetViewSize() const;
		const vector2d& GetResolution() const;
		const eastl::string& GetRootPath() const;
		const eastl::string& GetResourcePath() const;
		const eastl::string& GetLitePath() const;
	};
	struct ViewResolution
	{
		vector4d		m_OldViewSize;//屏幕尺寸
		vector2d		m_OldResolution;//分辨率
		vector4d		m_NewViewSize;//屏幕尺寸
		vector2d		m_NewResolution;//分辨率
	};
}