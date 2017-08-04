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
#include "Framework/RenderObjectProperty.h"
#include "Framework/Serializer.h"
#include "Framework/IIOSystem.h"
#include "Framework/IFile.h"
#include "MeshLoader.h"



using namespace BohgeEngine;
namespace ResourceSystemPlugin
{
	//------------------------------------------------------------------------------------------------------
	MeshLoader::MeshLoader(void)
		:ILoader( IResourceProperty::SLT_MESH )
	{
	}
	//------------------------------------------------------------------------------------------------------
	MeshLoader::~MeshLoader(void)
	{
	}
	//------------------------------------------------------------------------------------------------------
	void* MeshLoader::_DoLoad( const eastl::string& path )
	{
		RenderObjectProperty::MeshStreamData* pReslut = NEW RenderObjectProperty::MeshStreamData();
		Serializer loader( path );
		for ( bool isDone = false; !isDone ; )
		{
			int chunk = -1;
			loader>>chunk;//读取chunk
			switch ( chunk )
			{
			case RenderObjectProperty::MC_MESHES_DATA: _LoadMesh( loader, *pReslut ); break;
			case RenderObjectProperty::MC_MESHES_INDICES: _LoadIndices( loader, *pReslut ); break;
			case RenderObjectProperty::MC_MESHES_RENDERMODE: _LoadRenderMode( loader, *pReslut ); break;
			case RenderObjectProperty::MC_MESHES_END: isDone = true; break;
			default: ASSERT(false);
			}
		}
		return pReslut;
	}
	//------------------------------------------------------------------------------------------------------
	void MeshLoader::_LoadMesh( BohgeEngine::Serializer& file, BohgeEngine::RenderObjectProperty::MeshStreamData& Stream )
	{
		int size;
		for ( bool isDone = false; !isDone ; )
		{
			int chunk = -1;
			file>>chunk;//读取chunk
			switch ( chunk )
			{
			case RenderObjectProperty::MC_MD_VERTEX:
				{
					Stream.m_Vertex.SetVertexType<vector4f>
						( Attributes::ATTRIBUTE_POSITION, BufferObjectProperty::DT_FLOAT, BufferObjectProperty::DT_HALF_FLOAT);
				}break;
			case RenderObjectProperty::MC_MD_NORMAL:
				{
					Stream.m_Vertex.SetVertexType<vector3f>
						( Attributes::ATTRIBUTE_NORMAL, BufferObjectProperty::DT_FLOAT, BufferObjectProperty::DT_HALF_FLOAT);
				}break;
			case RenderObjectProperty::MC_MD_BINORMAL:
				{
					Stream.m_Vertex.SetVertexType<vector3f>
						( Attributes::ATTRIBUTE_BINORMAL, BufferObjectProperty::DT_FLOAT, BufferObjectProperty::DT_HALF_FLOAT);
				}break;
			case RenderObjectProperty::MC_MD_TANGENT:
				{
					Stream.m_Vertex.SetVertexType<vector3f>
						( Attributes::ATTRIBUTE_TANGENT, BufferObjectProperty::DT_FLOAT, BufferObjectProperty::DT_HALF_FLOAT);
				}break;
			case RenderObjectProperty::MC_MD_TEX_COORD:
				{
					Stream.m_Vertex.SetVertexType<vector2f>
						( Attributes::ATTRIBUTE_TEXCOORD0, BufferObjectProperty::DT_FLOAT, BufferObjectProperty::DT_HALF_FLOAT);
				}break;
			case RenderObjectProperty::MC_MD_BONEWEIGHT:
				{
					Stream.m_Vertex.SetVertexType<vector4f>
						( Attributes::ATTRIBUTE_BONEWEIGHTS, BufferObjectProperty::DT_FLOAT, BufferObjectProperty::DT_HALF_FLOAT);
					Stream.m_Vertex.SetVertexType<vector4f>
						( Attributes::ATTRIBUTE_BONEINDICES, BufferObjectProperty::DT_FLOAT, BufferObjectProperty::DT_UINT_8_8_8_8);
				}break;
			case RenderObjectProperty::MC_MD_END:
				{
					file>>size;
					isDone = true;
				}break;
			default: ASSERT(false);
			}
		}

		//创建
		Stream.m_Vertex.ReserveBuffer( size );

		//加载数据
		vector4f vec4;
		vector3f vec3;
		vector2f vec2;
		for ( int i = 0 ; i < size ; ++ i )
		{
			if ( Stream.m_Vertex.hasVertex( Attributes::ATTRIBUTE_POSITION ) )
			{
				file>>vec4;
				Stream.m_Vertex.PushVertexDataFast( Attributes::ATTRIBUTE_POSITION, vec4 );
			}
			if ( Stream.m_Vertex.hasVertex( Attributes::ATTRIBUTE_NORMAL ) )
			{
				file>>vec3;
				Stream.m_Vertex.PushVertexDataFast( Attributes::ATTRIBUTE_NORMAL, vec3 );
			}
			if ( Stream.m_Vertex.hasVertex( Attributes::ATTRIBUTE_BINORMAL ) )
			{
				file>>vec3;
				Stream.m_Vertex.PushVertexDataFast( Attributes::ATTRIBUTE_BINORMAL, vec3 );
			}
			if ( Stream.m_Vertex.hasVertex( Attributes::ATTRIBUTE_TANGENT ) )
			{
				file>>vec3;
				Stream.m_Vertex.PushVertexDataFast( Attributes::ATTRIBUTE_TANGENT, vec3 );
			}
			if ( Stream.m_Vertex.hasVertex( Attributes::ATTRIBUTE_TEXCOORD0 ) )
			{
				file>>vec2;
				Stream.m_Vertex.PushVertexDataFast( Attributes::ATTRIBUTE_TEXCOORD0, vec2 );
			}
			if ( Stream.m_Vertex.hasVertex( Attributes::ATTRIBUTE_BONEWEIGHTS ) )
			{
				file>>vec4;
				Stream.m_Vertex.PushVertexDataFast( Attributes::ATTRIBUTE_BONEINDICES, vec4 );
				file>>vec4;
				Stream.m_Vertex.PushVertexDataFast( Attributes::ATTRIBUTE_BONEWEIGHTS, vec4 );
			}
		}
	}
	//------------------------------------------------------------------------------------------------------
	void MeshLoader::_LoadIndices( BohgeEngine::Serializer& file, BohgeEngine::RenderObjectProperty::MeshStreamData& Stream )
	{
		int size, data;
		file>>size;
		Stream.m_Indices.SetIndicesType( BufferObjectProperty::IT_UINT16 );
		Stream.m_Indices.ReserveBuffer( size );
		for ( int i = 0 ; i < size ; ++ i )
		{
			file>>data;
			Stream.m_Indices.PushIndicesDataFast( data );
		}
	}
	//------------------------------------------------------------------------------------------------------
	void MeshLoader::_LoadRenderMode( BohgeEngine::Serializer& file, BohgeEngine::RenderObjectProperty::MeshStreamData& Stream )
	{
		int renderMode;
		file>>renderMode;
		Stream.m_eMode = static_cast< RenderObjectProperty::RenderMode >( renderMode );
	}

}
