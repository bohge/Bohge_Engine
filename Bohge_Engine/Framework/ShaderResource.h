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
#include "MaterialProperty.h"
#include "ShaderAttributes.h"
#include "ShaderParameter.h"
#include "ISharedResource.h"
#include "3DMath.h"



namespace BohgeEngine
{
	class Node;
	class TextureEntity;
	class MaterialEntity;
	class VertexBufferEntity;
	class BOHGE_FRAMEWORK ShaderResource : public ISharedResource
	{
	private:
		typedef void (*UnifromFunction)( ShaderResource&, handle, const MaterialEntity&, const Node& );
	private:
		struct ShaderUniform
		{
			handle				m_hUniform;
			UnifromFunction		m_Function;
		};
	private:
		typedef eastl::list< ShaderUniform* >			UnifromFunctionList;
	private:
		static uint					s_CurrentBind;
	private:
		typedef eastl::vector<handle>	UniformHandleMapping;
		typedef eastl::vector<void*> UniformDataCache;
	private:
		Attributes					m_Attributes;
		UnifromFunctionList			m_UnifromFunctionList;//属性设置队列
		UniformHandleMapping		m_HandleMapping;//句柄映射机制
		UniformDataCache			m_DataCache;//数值缓存，决定是否调用赋值函数
		uint						m_GPUResourceHandle;
	public:
		ShaderResource(void);
		virtual ~ShaderResource(void);
	private:
		virtual void _DoBindProgram(handle h) = 0;
		virtual void _OnShaderBegin() = 0;
		virtual void _DoDeleteProgram(handle h) = 0;
		virtual handle _DoCreateProgram( const Attributes& attr, const eastl::string& vscode, const eastl::string& pscode ) = 0;
		virtual handle _DoGetParameterHandle(handle hPragma, const eastl::string& name) = 0;
		virtual void _DoSetParameter(handle h, const vector2arrayf& input) = 0;
		virtual void _DoSetParameter(handle h, const vector3arrayf& input) = 0;
		virtual void _DoSetParameter(handle h, const vector4arrayf& input) = 0;
		virtual void _DoSetParameter(handle h, const int input) = 0;
		virtual void _DoSetParameter(handle h, const uint input) = 0;
		virtual void _DoSetParameter(handle h, const float input) = 0;
		virtual void _DoSetParameter(handle h, const eastl::vector<float>& input) = 0;
		virtual void _DoSetParameter(handle h, const vector2f& input) = 0;
		virtual void _DoSetParameter(handle h, const vector3f& input) = 0;
		virtual void _DoSetParameter(handle h, const vector4f& input) = 0;
		virtual void _DoSetParameter(handle h, const Matrix33f& input) = 0;
		virtual void _DoSetParameter(handle h, const Matrix44f& input) = 0;
		virtual void _DoSetParameter(handle h, const TextureEntity& input) = 0;
	public:
		virtual void ReleaseResource();
	private:
		UnifromFunction	_MappingUniformFunction(  const ShaderParameter::InputsType& it  );
		Attributes::ShaderAttributes _MappingAttributes( const ShaderParameter::InputsType& it );
	public:
		void CreateProgram( const ShaderParameterInfomationList& attrs, const ShaderParameterInfomationList& unifs, const eastl::string& vscode, const eastl::string& pscode );
	public:
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE const Attributes& GetShaderAttributes()
		{
			return m_Attributes;
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void BindProgram( const MaterialEntity& material, const Node& node )
		{
			_OnShaderBegin();
			if ( s_CurrentBind != m_GPUResourceHandle )
			{
				s_CurrentBind = m_GPUResourceHandle;
				_DoBindProgram( m_GPUResourceHandle );
			}
			FOR_EACH( UnifromFunctionList, it, m_UnifromFunctionList )
			{
				(*it)->m_Function( *this, (*it)->m_hUniform, material, node );
			}
		}
		//------------------------------------------------------------------------------------------------------
		template<typename T>
		BOHGE_FORCEINLINE handle GetParameterHandle( const eastl::string& name, bool noCache, bool isInit )
		{
			handle real = _DoGetParameterHandle( m_GPUResourceHandle, name );//得到实际句柄
			ASSERT( -1 != real );
			m_HandleMapping.push_back( real );//放到数组队列
			if ( noCache )
			{
				m_DataCache.push_back(NULL);
			}
			else
			{
				T* cache = NEW T;
				m_DataCache.push_back( cache );//构建一个cache，用于比对
				if ( isInit )
				{
					_DoSetParameter( real, *cache );//初始化
				}
			}
			return m_HandleMapping.size()-1;//返回映射值
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void SetParameter( handle h, MaterialProperty::UserParameterType spt, const void* data )
		{
			switch( spt )
			{
			case MaterialProperty::UPT_VEC1: SetParameter<float,vector1f>(h, *static_cast<const vector1f*>(data) ); break;
			case MaterialProperty::UPT_VEC2: SetParameter<vector2f,vector2f>(h, *static_cast<const vector2f*>(data) ); break;
			case MaterialProperty::UPT_VEC3: SetParameter<vector3f,vector3f>(h, *static_cast<const vector3f*>(data) ); break;
			case MaterialProperty::UPT_VEC4: SetParameter<vector4f,vector4f>(h, *static_cast<const vector4f*>(data) ); break;
			case MaterialProperty::UPT_MAT3: SetParameter<Matrix33f,Matrix33f>(h, *static_cast<const Matrix33f*>(data) ); break;
			case MaterialProperty::UPT_MAT4: SetParameter<Matrix44f,Matrix44f>(h, *static_cast<const Matrix44f*>(data) ); break;
			case MaterialProperty::UPT_VEC2_ARRAY: SetParameter< vector2arrayf, vector2arrayf >(h, *static_cast<const vector2arrayf*>(data) ); break;
			case MaterialProperty::UPT_VEC3_ARRAY: SetParameter< vector3arrayf, vector3arrayf >(h, *static_cast<const vector3arrayf*>(data) ); break;
			case MaterialProperty::UPT_VEC4_ARRAY: SetParameter< vector4arrayf, vector4arrayf >(h, *static_cast<const vector4arrayf*>(data) ); break;
			default: ASSERT(false);
			}
		}
		//------------------------------------------------------------------------------------------------------
		template<typename T, typename U>
		BOHGE_FORCEINLINE void SetParameter( handle h, const T& data )
		{
			if ( NULL == m_DataCache[h]//短路求值
			|| *(static_cast<U*>(m_DataCache[h])) != (U)data )
			{
				if( m_DataCache[h] ) *(static_cast<U*>(m_DataCache[h])) = (U)data;
				_DoSetParameter( m_HandleMapping[h], data );
			}
		}
	};
}
