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
//		地形		   //
/////////////////////////
#include "3DMath.h"
#include "TerrainTile.h"
#include "RendBuffer.h"
#include "Material.h"
#include "Texture.h"
#include "TerrainChunk.h"
#include "Camera.h"
#include "FilePath.h"


namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	TerrainTile::TerrainTile( const TerrainProperty& ppy )
		:m_Property(ppy),
		m_QTNode(NULL),
		m_isInView(true),
		m_ScaleInv( 1.0 / ppy.m_Scale )
	{
		ASSERT( m_Property.m_TileSize % 2 == 1 );
		ASSERT( m_Property.m_TextureSize % 2 == 0 );
		ASSERT( m_Property.m_TileSize*m_Property.m_TileSize < ushort(-1) );

		m_TerrainBuffer = NEW RendBuffer( Device::RM_TRIANGLE_STRIP	);//构造一个渲染buffer
		m_pVertexBuffer = NEW VertexBuffer( Device::MU_STATIC );
		m_TerrainBuffer->SetVertexBuffer( m_pVertexBuffer );
		//材质
		m_TerrainMaterial = new Material;
		m_TerrainMaterial->SetShader( Pipeline::PT_LIGHTING, ShaderManage::TerrainCDLODNoShadow );

		//构造树结构
		m_QTNode = NEW TerrainChunk( vector2d(0,0), vector2d(m_Property.m_TileSize, m_Property.m_TileSize), 0, m_Property );
	}
	//-------------------------------------------------------------------------------------------------------
	TerrainTile::~TerrainTile( )
	{
		SAFE_DELETE( m_QTNode );
		SAFE_DELETE( m_TerrainBuffer );
		SAFE_DELETE( m_pVertexBuffer );
		SAFE_DELETE( m_TerrainMaterial );
	}
	//-------------------------------------------------------------------------------------------------------
	void TerrainTile::Load( const std::string& path, const std::string& TexPath )
	{
		Serializer loader( path + TerrainMeshName );

		std::vector<Vertex2TextureNormal>	Vertexs;//顶点
		std::vector<ushort> indices;//索引
		Vertexs.resize( m_Property.m_TileSize*m_Property.m_TileSize );
		m_Hegiht.resize( m_Property.m_TileSize*m_Property.m_TileSize );
		indices.resize( m_Property.m_TileSize*(m_Property.m_TileSize-1)*2 );

		bool isContinue = true;
		while( isContinue )
		{
			int index;
			loader>>index;
			switch( index )
			{
			case SceneDataIndex::Terrain_Offset: 
				{
					loader>>m_Offset.m_x>>m_Offset.m_y;
					int xoff = ( m_Offset.m_x * ( m_Property.m_TileSize - 1 )  - ( m_Property.m_TileSize - 1 ) / 2.0f ) * m_Property.m_Scale ;
					int yoff = ( m_Offset.m_y * ( m_Property.m_TileSize - 1 )  - ( m_Property.m_TileSize - 1 ) / 2.0f ) * m_Property.m_Scale ;
					m_QTNode->OffsetScale( vector2f( xoff, yoff ) );

					vector2f center( m_Offset.m_x * (m_Property.m_TileSize - 1 ) * m_Property.m_Scale,
						m_Offset.m_y * (m_Property.m_TileSize - 1 ) * m_Property.m_Scale);
					float halfsize = (m_Property.m_TileSize-1) * m_Property.m_Scale * 0.5;
					m_WorldRange.Contains( center + vector2f(halfsize, halfsize) );
					m_WorldRange.Contains( center - vector2f(halfsize, halfsize) );
				}break;
			case SceneDataIndex::Terrain_Bind:
				{
					vector3f vmin, vmax;
					loader>>vmax>>vmin;
					m_BindBox = aabbox3df( vmin, vmax );
					m_QTNode->LoadBindBox( loader );
				}break;
			case SceneDataIndex::Terrain_Vertex: 
				{
					{//读取顶点
						for ( int z = 0 ; z < m_Property.m_TileSize ; z ++ )
						{
							for ( int x = 0 ; x < m_Property.m_TileSize ; x ++ )
							{
								int index = z*m_Property.m_TileSize+x;
								float height;
								loader>>height;
								m_Hegiht[index] = height;
								Vertexs[index].Position() = vector4f( x*m_Property.m_Scale, height , z*m_Property.m_Scale );
								loader>>Vertexs[index].Normal();
							}
						}
					}
				}break;
			case SceneDataIndex::Terrain_Materal:
				{
					for ( bool isDone = false; !isDone; )
					{
						int att = 0;
						loader>> att;
						switch( att )
						{
						case static_cast<int>(Material::ATT_AMBIENT) : loader>>m_TerrainMaterial->Ambient().m_x >>m_TerrainMaterial->Ambient().m_y >>m_TerrainMaterial->Ambient().m_z; break;
						case static_cast<int>(Material::ATT_DIFFUSE) : loader>>m_TerrainMaterial->Diffuse().m_x >>m_TerrainMaterial->Diffuse().m_y >>m_TerrainMaterial->Diffuse().m_z; break;
						case static_cast<int>(Material::ATT_SPECULAR) : loader>>m_TerrainMaterial->Specular().m_x >>m_TerrainMaterial->Specular().m_y >>m_TerrainMaterial->Specular().m_z; break;
						case static_cast<int>(Material::ATT_EMIT) : loader>>m_TerrainMaterial->Emit().m_x >>m_TerrainMaterial->Emit().m_y >>m_TerrainMaterial->Emit().m_z; break;
						case static_cast<int>(Material::ATT_OPCITY) : loader>>m_TerrainMaterial->Opacity(); break;
						case static_cast<int>(Material::ATT_SPECULAR_LEVEL) : loader>>m_TerrainMaterial->SpeLevel(); break;
						case static_cast<int>(Material::ATT_SHININESS) : loader>>m_TerrainMaterial->Shininess(); break;
						case static_cast<int>(Material::ATT_TEXTURES) :
							{
								int texCount = 0;
								loader>> texCount;
								for ( int j = 0; j < texCount; j ++)
								{
									std::string name;
									int ID;
									loader>>ID>>name;
									if( FILEPATH.isExist(TexPath + name) )
									{
										Texture2D* temp = m_TerrainMaterial->LoadTexture( static_cast<Material::Material_Attribute>(ID), TexPath + name );
										temp->SetWarp( Device::TW_REPEAT, Device::TW_REPEAT );
									}
								}
							}break;
						case static_cast<int>(Material::ATTRIBUTE_END):
							{
								m_TerrainMaterial->GetAlpahState( Pipeline::PT_LIGHTING ).m_isAlphaEnable = false;
								m_TerrainMaterial->GetAlpahState( Pipeline::PT_NORMAL ).m_isAlphaEnable = false;
								m_TerrainMaterial->SetShader( Pipeline::PT_NORMAL, ShaderManage::TerrainCDLODNormal );
								isDone = true;
								m_QTNode->SetMaterial( m_TerrainMaterial );
							}break;
						}
					}
				}break;
			case SceneDataIndex::Terrain_Weight:
				{
					Texture2D* tex = m_TerrainMaterial->LoadTexture( Material::ATT_TEX_TERRAIN_WEIGHT, path + TerrainWeight );
					tex->SetWarp( Device::TW_CLAMP_TO_EDGE, Device::TW_CLAMP_TO_EDGE );
				}break;
			case SceneDataIndex::Terrain_Diffuse:
				{
					Texture2D* tex = m_TerrainMaterial->LoadTexture( Material::ATT_TEX_DIFFUSE, path + TerrainDiffuse );
					tex->SetWarp( Device::TW_CLAMP_TO_EDGE, Device::TW_CLAMP_TO_EDGE );
				}break;
			case SceneDataIndex::Terrain_CDLOD://用于平滑过渡
				{
					for ( int z = 0 ; z < m_Property.m_TileSize ; z ++ )
					{
						for ( int x = 0 ; x < m_Property.m_TileSize ; x ++ )
						{
							loader>>Vertexs[z*m_Property.m_TileSize+x].Coordinates();
						}
					}
				}break;
			case SceneDataIndex::Save_Done:
				{
					m_pVertexBuffer->FlushVertexBuffer( Vertexs.size(), &Vertexs[0] );
					isContinue = false;
					m_QTNode->SetBuffer( m_TerrainBuffer );
				}break;
			}
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void TerrainTile::Update( uint milisecon, std::multimap< float, TerrainChunk* >& tmap )
	{
		aabspheref sphere( Engine::Instance().GetCamera()->GetPosition(), Engine::Instance().GetCamera()->GetFar() );
		m_QTNode->Update( Engine::Instance().GetCamera()->GetFrustum(), sphere, tmap );
	}
	//-------------------------------------------------------------------------------------------------------
	void TerrainTile::AssignMaterial( const bool& isCast, const ICamera& camera )
	{
		m_QTNode->AssignMaterial( isCast, camera );
	}
}