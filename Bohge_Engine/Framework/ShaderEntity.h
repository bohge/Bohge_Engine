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
#include "ShaderResource.h"
#include "ShaderSource.h"
#include "SmartPtr.hpp"
#include "IAsyncEntity.h"



#include <EASTL/list.h>

namespace BohgeEngine
{
	class Node;
	class MaterialEntity;
	class ShaderResource;
	class VertexBufferEntity;
	class ShaderEntity : public IAsyncEntity< ShaderResource, ShaderSource >
	{
		RTTI_DRIVER_TYPE_DECLEAR( ShaderEntity, IAsyncEntity );
	public:
		ShaderEntity(void);
		~ShaderEntity(void);
	private:
		virtual void _OnCreateResource();//准备加载资源的时候调用
		virtual void _OnResourceCreated();//资源加载完毕的回掉
	public:
		BOHGE_FORCEINLINE void ActiveShader( const MaterialEntity& material, const Node& node )
		{
			_GetResourcePtr()->BindProgram( material, node );
		}
		BOHGE_FORCEINLINE const Attributes& GetShaderAttributes()
		{
			return _GetResourcePtr()->GetShaderAttributes();
		}
		BOHGE_FORCEINLINE void PushVertexSegmentName( const eastl::string& name )
		{
			_GetSourcePtr()->PushVertexSegmentName( name );
		}
		BOHGE_FORCEINLINE void PushFragmentSegmentName( const eastl::string& name )
		{
			_GetSourcePtr()->PushFragmentSegmentName( name );
		}
		BOHGE_FORCEINLINE void PushInclude( const eastl::string& name )
		{
			_GetSourcePtr()->PushIncludeFile( name );
		}
		BOHGE_FORCEINLINE void SetIncludePath( const eastl::string& path )
		{
			_GetSourcePtr()->SetIncludePath( path );
		}

#ifdef _DEBUG
	private:
		eastl::string		m_ShaderName;
	public:
		BOHGE_FORCEINLINE void SetShaderName( const eastl::string& name )
		{
			m_ShaderName = name;
		}
#endif
	};
}

