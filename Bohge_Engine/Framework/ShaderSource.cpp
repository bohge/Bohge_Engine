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
#include "IMaterialSystem.h"
#include "IGraphicSystem.h"
#include "ShaderResource.h"
#include "ShaderSource.h"

#include <EASTL/vector.h>

namespace BohgeEngine
{
	//------------------------------------------------------------------------------------------------------
	ShaderSource::ShaderSource(void)
		:ISharedSource( IResourceProperty::SST_SHADERPROGRAM, IResourceProperty::DT_RENDER_THREAD, true )
	{
	}
	//------------------------------------------------------------------------------------------------------
	ShaderSource::~ShaderSource(void)
	{
	}
	//------------------------------------------------------------------------------------------------------
	void ShaderSource::_DoLoadResource()
	{
		IMaterialSystem::Instance()->Parser( m_IncludesPath, m_Includes, m_VertexSegmentName, m_FragmentSegmentName, *this );
		//优化shader代码
		IGraphicSystem::Instance()->ProcessShader( ShaderProperty::SC_VERTEX, m_VertexCode );
		IGraphicSystem::Instance()->ProcessShader( ShaderProperty::SC_FRAGMENT, m_FragmentCode );

		//去掉多余的参数
		for( ShaderParameterInfomationList::iterator it = m_AttributeList.begin();
			it != m_AttributeList.end();
			)
		{
			if ( -1 == m_VertexCode.find( it->m_ParamName ) )
			{
				it = m_AttributeList.erase( it );
			}
			else
			{
				++ it;
			}
		}
		for( ShaderParameterInfomationList::iterator it = m_UniformList.begin();
			it != m_UniformList.end();
			)
		{
			if ( -1 == m_VertexCode.find( it->m_ParamName )
				&& -1 == m_FragmentCode.find( it->m_ParamName ) )
			{
				it = m_AttributeList.erase( it );
			}
			else
			{
				++ it;
			}
		}
	}
	//------------------------------------------------------------------------------------------------------
	ISharedResource* ShaderSource::MakeResource()
	{
		ShaderResource* res = static_cast<ShaderResource*>( IGraphicSystem::Instance()->CreateResource( IResourceProperty::SRT_SHADERPROGRAM ) );
		res->CreateProgram( m_AttributeList, m_UniformList, m_VertexCode, m_FragmentCode );
		return res;
	}
	//------------------------------------------------------------------------------------------------------
	void ShaderSource::_DoReleaseUnnecessaryDataAfterLoaded()
	{
		m_Includes.clear();
		m_VertexSegmentName.clear();
		m_FragmentSegmentName.clear();
		m_UniformList.clear();
		m_AttributeList.clear();
		m_IncludesPath.clear();
		m_VertexCode.clear();
		m_FragmentCode.clear();
	}

}