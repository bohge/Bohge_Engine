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
#include "Framework/ShaderParameter.h"

#include <EASTL/map.h>
#include <EASTL/string.h>
#include <EASTL/vector.h>


namespace LuaMaterialSystemPlugin
{
	class ShaderLibrary;
	//这个类传递进来没有处理的片断类型，当外部需要使用的时候
	//交由ShaderLibrary处理为可识别的数据，如果不使用就是原始文本
	class ShaderSegment
	{
	private:
		bool							m_isOriginal;//片断是否是原始的美被处理过的
		eastl::string						m_SegmentName;//片断名字
		eastl::string						m_SegmentCode;//片断代码
		eastl::string						m_Function;//Lua函数段
		BohgeEngine::ParameterMap		m_InputParameter;//输入的参数
		BohgeEngine::ParameterMap		m_OutputParameter;//输出的参数
	public:
		ShaderSegment( const eastl::string& name, const eastl::string& code );
		ShaderSegment( const ShaderSegment& ss );
		~ShaderSegment(void);
	private:
		void _Docopy( const ShaderSegment& ss );
		void _ReadParameter( ShaderLibrary& host, BohgeEngine::ParameterMap& into, const char* which );
	public:
		void operator = ( const ShaderSegment& ss );
		void ReleaseResource();//释放全部以加载的shader段
		void ProcessSegment( ShaderLibrary& host );//当片断为原始数据的时候，进行处理
		void RenameInputParameter( const BohgeEngine::ShaderParameter::InputsType& ty, const eastl::string& to );
		void RenameOutputParameter( const BohgeEngine::ShaderParameter::InputsType& ty, const eastl::string& to );
		bool isNeedInput( const BohgeEngine::ShaderParameter::InputsType& ty );
		bool isProvider( const BohgeEngine::ShaderParameter::InputsType& ty );
	public:
		BOHGE_FORCEINLINE bool isOriginal() const
		{
			return m_isOriginal;
		}
		BOHGE_FORCEINLINE const eastl::string& GetName() const
		{
			return m_SegmentName;
		}
		BOHGE_FORCEINLINE const eastl::string& GetCode() const
		{
			return m_SegmentCode;
		}
		BOHGE_FORCEINLINE const BohgeEngine::ParameterMap& GetSegmentInput() const
		{
			return m_InputParameter;
		}
		BOHGE_FORCEINLINE const BohgeEngine::ParameterMap& GetSegmentOutput() const
		{
			return m_OutputParameter;
		}
	};

	typedef eastl::vector< ShaderSegment* > Segments;
}

