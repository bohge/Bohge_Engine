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



/////////////////////////
//		植被四叉树	   //
/////////////////////////
#include "PlantChunk.h"
#include "Material.h"
#include "RendBuffer.h"
#include "Serializer.h"
#include "Texture.h"
#include "SceneDataIndex.h"
#include "Camera.h"
#include "GameSetting.h"
#include "Log.h"
#include "FilePath.h"


using namespace std;


namespace BohgeEngine
{

	//-------------------------------------------------------------------------------------------------------
	PlantChunk::PlantChunk( int depth )
		:m_LeftTop(NULL),
		m_RightTop(NULL),
		m_LeftBottom(NULL),
		m_RightBottom(NULL),
		m_isDraw(false),
		m_Depth( depth ),
		m_CurrentDrawLevel(0)
	{
		float cf = Engine::Instance().GetCamera()->GetFar();
		m_VisableRadius = GameSetting::Instance().PlantVisableDistance() * cf;
		m_FadeVector.resize( vector4f::Size() );
	}
	//-------------------------------------------------------------------------------------------------------
	PlantChunk::~PlantChunk(void)
	{
		SAFE_DELETE(m_LeftTop);
		SAFE_DELETE(m_RightTop);
		SAFE_DELETE(m_LeftBottom);
		SAFE_DELETE(m_RightBottom);
		for ( int i = 0 ; i < m_BufferVector.size(); i ++ )
		{
			SAFE_DELETE( m_BufferVector[i].m_PlantBuffer );
			SAFE_DELETE( m_BufferVector[i].m_pPlantVertexBuffer );
			SAFE_DELETE( m_BufferVector[i].m_PlantMaterial );
			for ( int j = 0 ; j < m_BufferVector[i].m_LodIndices.size(); j ++ )
			{
				SAFE_DELETE( m_BufferVector[i].m_LodIndices[j] );
			}
			m_BufferVector[i].m_LodIndices.clear();
		}
		m_BufferVector.clear();
	}
	//-------------------------------------------------------------------------------------------------------
	void PlantChunk::LoadStruct( Serializer& loader )
	{
		bool isSplit = false;
		loader>>isSplit;
		if( isSplit )
		{
			m_LeftTop = NEW PlantChunk( m_Depth+1 );
			m_RightTop = NEW PlantChunk( m_Depth+1 );
			m_LeftBottom = NEW PlantChunk( m_Depth+1 );
			m_RightBottom = NEW PlantChunk( m_Depth+1 );
			m_LeftTop->LoadStruct( loader );
			m_RightTop->LoadStruct( loader );
			m_LeftBottom->LoadStruct( loader );
			m_RightBottom->LoadStruct( loader );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void PlantChunk::LoadBindBox( Serializer& loader )
	{
		vector3f min,max;
		loader>>min>>max;
		m_BindBox = aabbox3df( min, max );

		if ( NULL != m_LeftTop )
		{
			m_LeftTop->LoadBindBox( loader );
		}
		if ( NULL != m_RightTop )
		{
			m_RightTop->LoadBindBox( loader );
		}
		if ( NULL != m_LeftBottom )
		{
			m_LeftBottom->LoadBindBox( loader );
		}
		if ( NULL != m_RightBottom )
		{
			m_RightBottom->LoadBindBox( loader );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void PlantChunk::LoadBuffer( Serializer& loader, const std::string& texPath )
	{
		int buffersize = 0;
		loader>>buffersize;
		if( 0 != buffersize )
		{
			m_BufferVector.resize( buffersize );
			for ( int BufferIndex = 0 ; BufferIndex < buffersize ; BufferIndex ++ )
			{
				bool isContinue = true;
				while( isContinue )
				{
					int index;
					loader>>index;
					switch( index )
					{
					case SceneDataIndex::Plant_Materal:
						{
							m_BufferVector[BufferIndex].m_PlantMaterial = NEW Material;
							for ( bool isDone = false; !isDone; )
							{
								int att = 0;
								loader>> att;
								switch( att )
								{
								case Material::ATT_AMBIENT : loader>>m_BufferVector[BufferIndex].m_PlantMaterial->Ambient(); break;
								case Material::ATT_DIFFUSE : loader>>m_BufferVector[BufferIndex].m_PlantMaterial->Diffuse(); break;
								case Material::ATT_SPECULAR : loader>>m_BufferVector[BufferIndex].m_PlantMaterial->Specular(); break;
								case Material::ATT_EMIT : loader>>m_BufferVector[BufferIndex].m_PlantMaterial->Emit(); break;
								case Material::ATT_OPCITY : loader>>m_BufferVector[BufferIndex].m_PlantMaterial->Opacity(); break;
								case Material::ATT_SPECULAR_LEVEL : loader>>m_BufferVector[BufferIndex].m_PlantMaterial->SpeLevel(); break;
								case Material::ATT_SHININESS : loader>>m_BufferVector[BufferIndex].m_PlantMaterial->Shininess(); break;
								case Material::ATT_TEXTURES :
									{
										int texCount = 0;
										loader>> texCount;
										for ( int j = 0; j < texCount; j ++)
										{
											std::string name;
											int ID,pf;
											loader>>ID>>pf>>name;
											if( FILEPATH.isExist(texPath + name) )
											{
												Texture2D* temp = m_BufferVector[BufferIndex].m_PlantMaterial->LoadTexture( static_cast<Material::Material_Attribute>(ID), (Device::PixelFormat)pf, texPath + name );
												temp->SetAnisotropic( 2 );
												temp->SetWarp( Device::TW_REPEAT, Device::TW_REPEAT );
											}
										}
									}break;
								case static_cast<int>(Material::ATTRIBUTE_END) : isDone = true; break;
								}
							}
						}break;
					case SceneDataIndex::Plant_Shader:
						{
							int type = 0;
							loader>>type;
							m_BufferVector[BufferIndex].m_eShader = static_cast<PlantBaseInfo::Plant_Shader>(type);
							switch( m_BufferVector[BufferIndex].m_eShader )
							{
							case PlantBaseInfo::PS_TRUNK:
								{
									m_BufferVector[BufferIndex].m_PlantMaterial->SetShader( Pipeline::PT_LIGHTING, ShaderManage::PlantTrunk );
									m_BufferVector[BufferIndex].m_PlantMaterial->SetShader( Pipeline::PT_NORMAL, ShaderManage::PlantTrunkNormal );
								} break;
							case PlantBaseInfo::PS_LEAF: 
								{
									//m_BufferVector[BufferIndex].m_PlantMaterial->GetDrawState().m_isDepthMask = false;
									m_BufferVector[BufferIndex].m_PlantMaterial->GetDrawState( Pipeline::PT_LIGHTING).m_isCullFace = Device::CFM_NONE;
									m_BufferVector[BufferIndex].m_PlantMaterial->GetAlpahState( Pipeline::PT_LIGHTING ).m_isAlphaEnable = false;
									m_BufferVector[BufferIndex].m_PlantMaterial->GetDrawState( Pipeline::PT_NORMAL).m_isCullFace = Device::CFM_NONE;
									m_BufferVector[BufferIndex].m_PlantMaterial->GetAlpahState( Pipeline::PT_NORMAL ).m_isAlphaEnable = false;
									//m_BufferVector[BufferIndex].m_PlantMaterial->GetAlpahState().m_DesA = Device::BL_ONE;
									m_BufferVector[BufferIndex].m_PlantMaterial->SetShader( Pipeline::PT_LIGHTING, ShaderManage::PlantLeaf );
									m_BufferVector[BufferIndex].m_PlantMaterial->SetShader( Pipeline::PT_NORMAL, ShaderManage::PlantLeafNormal );
								} break;
							default:ASSERT(false);break;
							}
						}break;
					case SceneDataIndex::Plant_Vertex:
						{
							std::vector< VERTEXTYPE >	vertexsVector;
							int vercount;
							loader>> vercount;
							vertexsVector.resize( vercount );
							for ( int i = 0 ; i < vercount; i ++ )//使用这种方法保存时为了可能在app读取的时候就不要法线了，不做实时光照了
							{
								loader>>vertexsVector[i].Position();
								loader>>vertexsVector[i].Normal();
								loader>>vertexsVector[i].Coordinate();
							}
							m_BufferVector[BufferIndex].m_PlantBuffer = NEW RendBuffer( Device::RM_TRIANGLES );
							m_BufferVector[BufferIndex].m_pPlantVertexBuffer = NEW VertexBuffer( Device::MU_STATIC );
							m_BufferVector[BufferIndex].m_PlantBuffer->SetVertexBuffer( m_BufferVector[BufferIndex].m_pPlantVertexBuffer );
							m_BufferVector[BufferIndex].m_pPlantVertexBuffer->FlushVertexBuffer( vercount, &vertexsVector[0] );
						}break;
					case SceneDataIndex::Plant_Indices:
						{
							int VectorSize;
							loader>>VectorSize;
							m_BufferVector[BufferIndex].m_LodIndices.resize( VectorSize );
							for ( int index = 0 ; index< VectorSize ; index ++ )
							{
								int indicesSize = 0;
								loader>>indicesSize;
								vector<ushort> indices;
								indices.resize( indicesSize );
								loader.Read( sizeof(ushort)*indicesSize, &indices[0] );
								m_BufferVector[BufferIndex].m_LodIndices[index] = NEW IndicesBuffer( Device::MU_STATIC );
								m_BufferVector[BufferIndex].m_LodIndices[index]->FlushIndiesBuffer( indices.size(), &indices[0] );
							}
						}break;
					case SceneDataIndex::Plant_Buffer_End:
						{ 
							isContinue = false;
						}break;
					}
				}
			}
		}

		if ( NULL != m_LeftTop )
		{
			m_LeftTop->LoadBuffer( loader, texPath );
		}
		if ( NULL != m_RightTop )
		{
			m_RightTop->LoadBuffer( loader, texPath );
		}
		if ( NULL != m_LeftBottom )
		{
			m_LeftBottom->LoadBuffer( loader, texPath );
		}
		if ( NULL != m_RightBottom )
		{
			m_RightBottom->LoadBuffer( loader, texPath );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void PlantChunk::Update( const Frustumf& view, aabspheref& sphere, float fadetimespan, std::multimap< float, PlantChunk* >& plantmap  )
	{
		if( !view.Intersect( m_BindBox ) )//如果此节点不在绑定盒子内
		{
			m_isDraw = false;
		}
		else//如果此节点不在绑定盒子内
		{
			bool l_wasDraw = m_isDraw;//曾经是否可绘制
			if( !m_BufferVector.empty() )
			{
				{//处理淡入淡出
					//DEBUGLOG( "Fade ", 1 );
					for( int i = 0 ; i < vector4f::Size() ; i ++ )
					{
						if ( FS_FADEOUT == m_FadeVector[i].m_InOut || FS_FADEIN == m_FadeVector[i].m_InOut )
						{
							m_FadeVector[i].m_Alpha += fadetimespan * m_FadeVector[i].m_InOut;
							if ( m_FadeVector[i].m_Alpha <= 0.0 && FS_FADEOUT == m_FadeVector[i].m_InOut )
							{
								m_FadeVector[i].m_InOut = FS_HIDE;
							}
							else if ( m_FadeVector[i].m_Alpha >= 1.0 && FS_FADEIN == m_FadeVector[i].m_InOut )
							{
								m_FadeVector[i].m_InOut = FS_SHOW;
							}
							m_FadeVector[i].m_Alpha = Math::Clamp0to1( m_FadeVector[i].m_Alpha );
						}
						//DEBUGLOG( "%d -- %d -- %f", i, m_FadeVector[i].m_InOut, m_FadeVector[i].m_Alpha );
					}
					//DEBUGLOG( "\n", 1 );
				}
				int DrawLevel = 0;
				m_isDraw = false;
				for ( int i = vector4f::Size()-1; i >= 0; i -- )
				{
					sphere.SetRadius( m_VisableRadius[i] );//如果下一个细节等级与这个绑定盒子没有相交，则不需要分裂
					if ( m_BindBox.Intersection( sphere ) )
					{
						m_DistancePow = ( sphere.GetCenter() - m_BindBox.GetCenter() ).LengthPow();
						DrawLevel = i;
						m_isDraw = true;
						plantmap.insert( std::make_pair( ( sphere.GetCenter() - m_BindBox.GetCenter() ).LengthPow() , this ) );
						break;
					}
				}
				//开始计算淡入淡出
				for( int i = 0 ; i < vector4f::Size() ; i ++ )
				{
					if ( false == l_wasDraw ) //如果曾经是不可见，则立马变为可见
					{
						if ( i <= DrawLevel ) //如果是可见的细致等级，就设置为稳定的
						{
							m_FadeVector[i].m_InOut = FS_SHOW;
							m_FadeVector[i].m_Alpha = 1.0;
						}
						else//不可见得就设置为out
						{
							m_FadeVector[i].m_InOut = FS_HIDE;
							m_FadeVector[i].m_Alpha = 0.0;
						}
					}
					else//当物体一直在可见状态就需要alpha的变化了
					{
						if ( i <= DrawLevel )//如果是可见的等级
						{
							if( FS_SHOW != m_FadeVector[i].m_InOut )//如果以前是out，则变化为in，稳定的和正在in的不处理
							{
								m_FadeVector[i].m_InOut = FS_FADEIN;
							}
						}
						else//不可见的
						{
							if( FS_HIDE != m_FadeVector[i].m_InOut )//如果以前不是不可见的，就变为不可见
							{
								m_FadeVector[i].m_InOut = FS_FADEOUT;
							}
						}
					}
				}
			}
			if ( NULL != m_LeftTop )
			{
				m_LeftTop->Update( view, sphere, fadetimespan, plantmap );
			}
			if ( NULL != m_RightTop )
			{
				m_RightTop->Update( view, sphere, fadetimespan, plantmap );
			}
			if ( NULL != m_LeftBottom )
			{
				m_LeftBottom->Update( view, sphere, fadetimespan, plantmap );
			}
			if ( NULL != m_RightBottom )
			{
				m_RightBottom->Update( view, sphere, fadetimespan, plantmap );
			}
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void PlantChunk::RenderOpaque( Engine& engine )
	{
		//if( m_isDraw )
		ASSERT( true == m_isDraw );
		{
			for ( RenderbufferVector::const_iterator buffer = m_BufferVector.begin();
				buffer != m_BufferVector.end();
				buffer ++ )
			{
				if( PlantBaseInfo::PS_TRUNK == buffer->m_eShader )
				{
					for( m_CurrentDrawLevel = 0; m_CurrentDrawLevel < vector4f::Size(); m_CurrentDrawLevel ++ )
					{
						if ( FS_HIDE != m_FadeVector[m_CurrentDrawLevel].m_InOut && m_CurrentDrawLevel < buffer->m_LodIndices.size() )
						{
							buffer->m_PlantMaterial->SetNode( this );
							buffer->m_PlantBuffer->SetIndicesBuffer( buffer->m_LodIndices[m_CurrentDrawLevel] );
							engine.GetDevice()->Draw( *buffer->m_PlantBuffer, *buffer->m_PlantMaterial );
						}
					}
				}
			}
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void PlantChunk::RenderTransparent( Engine& engine )
	{
		//if( m_isDraw )
		ASSERT( true == m_isDraw );
		{
			for ( RenderbufferVector::const_iterator buffer = m_BufferVector.begin();
				buffer != m_BufferVector.end();
				buffer ++ )
			{
				if( PlantBaseInfo::PS_LEAF == buffer->m_eShader )
				{
					for( m_CurrentDrawLevel = 0; m_CurrentDrawLevel < vector4f::Size(); m_CurrentDrawLevel ++ )
					{
						if ( FS_HIDE != m_FadeVector[m_CurrentDrawLevel].m_InOut && m_CurrentDrawLevel < buffer->m_LodIndices.size() )
						{
							buffer->m_PlantMaterial->SetNode( this );
							buffer->m_PlantBuffer->SetIndicesBuffer( buffer->m_LodIndices[m_CurrentDrawLevel] );
							engine.GetDevice()->Draw( *buffer->m_PlantBuffer, *buffer->m_PlantMaterial );
						}
					}
				}
			}
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//void PlantChunk::RenderTransparent( engine& engine )
	//{
	//	//if( m_isDraw ) 得这个数据的时候就判断了
	//	{
	//		for ( RenderbufferVector::const_iterator buffer = m_BufferVector.begin();
	//			buffer != m_BufferVector.end();
	//			buffer ++ )
	//		{
	//			if( PlantBaseInfo::PS_LEAF == buffer->m_eShader ) //只有树干是带有深度的
	//			{
	//				for( m_CurrentDrawLevel = 0; m_CurrentDrawLevel < vector4f::Size(); m_CurrentDrawLevel ++ )
	//				{
	//					if ( FS_HIDE != m_FadeVector[m_CurrentDrawLevel].m_InOut && m_CurrentDrawLevel < buffer->m_LodIndices.size() )
	//					{
	//						buffer->m_PlantMaterial->SetNode( this );
	//						buffer->m_PlantBuffer->TempIndices( buffer->m_LodIndices[m_CurrentDrawLevel] );
	//						engine.GetDevice()->Draw( *buffer->m_PlantBuffer, *buffer->m_PlantMaterial );
	//					}
	//				}
	//			}
	//		}
	//	}
	//}
	//-------------------------------------------------------------------------------------------------------
	//void PlantChunk::GetTransparentNode( TransparentMap& nodemap )
	//{
	//	if ( m_isDraw )
	//	{
	//		nodemap.insert( std::make_pair( m_DistancePow, this) );
	//	}
	//	if ( NULL != m_LeftTop )
	//	{
	//		m_LeftTop->GetTransparentNode( nodemap );
	//	}
	//	if ( NULL != m_RightTop )
	//	{
	//		m_RightTop->GetTransparentNode( nodemap );
	//	}
	//	if ( NULL != m_LeftBottom )
	//	{
	//		m_LeftBottom->GetTransparentNode( nodemap );
	//	}
	//	if ( NULL != m_RightBottom )
	//	{
	//		m_RightBottom->GetTransparentNode( nodemap );
	//	}
	//}
	//-------------------------------------------------------------------------------------------------------
	const void* PlantChunk::Attribute( const Node_Attribute& name ) const
	{
		switch( name )
		{
		case IAttributeNode::ATT_USER_0: return &m_FadeVector[m_CurrentDrawLevel].m_Alpha;
		}
		return NULL;
	}

}