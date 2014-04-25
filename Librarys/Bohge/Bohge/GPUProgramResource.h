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
#include "IResource.h"
#include "BaseShader.h"



namespace BohgeEngine
{
	class TextureTemp;
	class GPUProgramResource : public IResource
	{
	private:
		static uint					s_CurrentBind;
	private:
		typedef std::vector<handle>	UniformHandleMapping;
		typedef std::vector<void*> UniformDataCache;
	private:
		Attributes					m_Attributes;
		UniformHandleMapping		m_HandleMapping;//句柄映射机制
		UniformDataCache			m_DataCache;//数值缓存，决定是否调用赋值函数
		handle						m_hShaderProgram;
	public:
		GPUProgramResource(void);
		virtual ~GPUProgramResource(void);
	private:
		virtual void _DoBindProgram(handle h) = 0;
		virtual void _DoDeleteProgram(handle h) = 0;
		virtual handle _DoCreateProgram( const Attributes& attr, const std::string& vscode, const std::string& pscode ) = 0;
		virtual handle _DoGetParameterHandle(handle hPragma, const std::string& name) = 0;
		virtual void _DoSetParameter(handle h, const std::vector<Quaternionf>& input) = 0;
		virtual void _DoSetParameter(handle h, const int input) = 0;
		virtual void _DoSetParameter(handle h, const float input) = 0;
		virtual void _DoSetParameter(handle h, const std::vector<float>& input) = 0;
		virtual void _DoSetParameter(handle h, const vector2f& input) = 0;
		virtual void _DoSetParameter(handle h, const vector3f& input) = 0;
		virtual void _DoSetParameter(handle h, const vector4f& input) = 0;
		virtual void _DoSetParameter(handle h, const Matrix33f& input) = 0;
		virtual void _DoSetParameter(handle h, const Matrix44f& input) = 0;
		virtual void _DoSetParameter(handle h, const TextureTemp& input) = 0;
	public:
		virtual void ReleaseResource();
	public:
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void BindProgram()
		{
			if ( s_CurrentBind != m_hShaderProgram )
			{
				s_CurrentBind = m_hShaderProgram;
				_DoBindProgram( m_hShaderProgram );
			}
		}
		//------------------------------------------------------------------------------------------------------
		template<typename T>
		BOHGE_FORCEINLINE handle GetParameterHandle( const std::string& name, bool noCache )
		{
			handle real = _DoGetParameterHandle( m_hShaderProgram, name );//得到实际句柄
			m_HandleMapping.push_back( real );//放到数组队列
			if ( noCache )
			{
				m_DataCache.push_back(NULL);
			}
			else
			{
				T* cache = NEW T;
				m_DataCache.push_back( cache );//构建一个cache，用于比对
				_DoSetParameter( real, *cache );//初始化
			}
			return m_HandleMapping.size()-1;//返回映射值
		}
		//------------------------------------------------------------------------------------------------------
		template<typename T>
		BOHGE_FORCEINLINE void SetParameter( handle h, const T& data )
		{
			if ( NULL == m_DataCache[h]//短路求值
			|| m_DataCache[h] != data )
			{
				_DoSetParameter( m_HandleMapping[h], data );
			}
		}
	};
}
