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
#include "ISharedSource.h"
#include "ShaderParameter.h"




namespace BohgeEngine
{
	class ShaderSource : public ISharedSource
	{
	private:
		typedef eastl::list< eastl::string > StringList;
	private:
		StringList						m_Includes;
		StringList						m_VertexSegmentName;
		StringList						m_FragmentSegmentName;
		ShaderParameterInfomationList	m_UniformList;
		ShaderParameterInfomationList	m_AttributeList;
		eastl::string						m_IncludesPath;
		eastl::string						m_VertexCode;
		eastl::string						m_FragmentCode;
	public:
		ShaderSource(void);
		virtual ~ShaderSource(void);
	private:
		virtual void _DoLoadResource();//加载资源
		virtual void _DoReleaseUnnecessaryDataAfterLoaded();//资源被make后释放不需要的数据
	public:
		virtual ISharedResource* MakeResource();//将资源数据制作为实体资源，制作资源需要在主线程进行
	public:
		BOHGE_FORCEINLINE void PushVertexSegmentName( const eastl::string& name )
		{
			m_VertexSegmentName.push_back( name );
			PushSourceMetadata( PathMetadata(name) );
		}
		BOHGE_FORCEINLINE void PushFragmentSegmentName( const eastl::string& name )
		{
			m_FragmentSegmentName.push_back( name );
			PushSourceMetadata( PathMetadata(name) );
		}
		BOHGE_FORCEINLINE void PushIncludeFile( const eastl::string& name )
		{
			m_Includes.push_back( name );
			PushSourceMetadata( PathMetadata(name) );
		}
		BOHGE_FORCEINLINE void PushBackVertexCode( const eastl::string& code )
		{
			m_VertexCode += code;
		}
		BOHGE_FORCEINLINE void PushBackFragmentCode( const eastl::string& code )
		{
			m_FragmentCode += code;
		}
		BOHGE_FORCEINLINE const eastl::string& GetVertexCode()
		{
			return m_VertexCode;
		}
		BOHGE_FORCEINLINE const eastl::string& GetFragmentCode()
		{
			return m_FragmentCode;
		}
		BOHGE_FORCEINLINE void PushAttribute( const ShaderParameter* sp )
		{
			m_AttributeList.push_back( ShaderParameterInfomation( sp->GetVariableType(), sp->GetInputsType(), sp->GetName() ) );
		}
		BOHGE_FORCEINLINE void PushUniform( const ShaderParameter* sp )
		{
			m_UniformList.push_back( ShaderParameterInfomation( sp->GetVariableType(), sp->GetInputsType(), sp->GetName() ) );
		}
		BOHGE_FORCEINLINE void SetIncludePath( const eastl::string& path )
		{
			m_IncludesPath = path;
		}
	};
}
