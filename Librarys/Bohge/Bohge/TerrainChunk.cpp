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
//		地形四叉树   //
/////////////////////////

#include "TerrainChunk.h"
#include "RendBuffer.h"
#include "Material.h"
#include "Plane.hpp"
#include "Camera.h"
#include "GameSetting.h"
#include "TerrainShaders.h"

//这种简化的做法由一个问题，就是当最小chunk的size大于检测范围的时候会有裂痕的产生，为了避免这种情况，尽量拉远可是距离，或者降低chunk的尺寸，降低scale等方法

namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	TerrainChunk::TerrainChunk( const vector2d& begin, const vector2d& end, int iterDepth, const TerrainProperty& ppy )
		:m_isFirstNode( 0 == iterDepth ),
		m_isLastNode( ppy.m_SplitLevel == iterDepth ),
		m_isSplit(false),
		m_isDraw(false),
		m_isCanSee(false),
		m_DrawLevel(0),
		m_LeafChunk_UL(NULL),
		m_LeafChunk_UR(NULL),
		m_LeafChunk_DL(NULL),
		m_LeafChunk_DR(NULL)
	{
		vector4f& range = GameSetting::Instance().TurningPoint();
		m_DetailDisance_Tile.m_x = 1.0f / (Engine::Instance().GetCamera()->GetFar() * range.m_w * 0.5f);
		m_DetailDisance_Tile.m_y = 2.0f;
		{//计算地形的一些变形属性
			float begin = ( iterDepth == ppy.m_SplitLevel ? 0 : range[iterDepth+1] ) * Engine::Instance().GetCamera()->GetFar();
			float end = range[iterDepth] * Engine::Instance().GetCamera()->GetFar();
			m_BeginEnd.m_x = begin;
			m_BeginEnd.m_y = end;
			m_SizeScaleInv_RepeatSizeInv.m_x = 1.0 / ( (ppy.m_TileSize-1) * ppy.m_Scale );
			m_SizeScaleInv_RepeatSizeInv.m_y = 1.0 / ( (ppy.m_TileSize-1) * ppy.m_Scale / ppy.m_TextureRepeat );
		}

		//计算DrawData
		int i = 0;
		do 
		{
			DrawData data;
			{//计算绘制属性
				data.m_RenderShader = iterDepth - i >= ppy.m_SplitLevel ? ShaderManage::TerrainCDLOD : ShaderManage::TerrainCDLODLow;
				float begin = ( iterDepth - i == ppy.m_SplitLevel ? 0 : range[iterDepth-i+1] ) * Engine::Instance().GetCamera()->GetFar();
				float end = range[iterDepth-i] * Engine::Instance().GetCamera()->GetFar();
				float morphEnd = end;
				float morphBegin = morphEnd - ( end - begin) * TerrainSmooth;
				data.m_MorphXY_SteperScaleInvers_DoubleChunkSize.m_x = morphEnd / (morphEnd-morphBegin);//制定了变形的开始与结束
				if( m_isFirstNode || iterDepth == i )
				{
					data.m_MorphXY_SteperScaleInvers_DoubleChunkSize.m_y = 0.0f;
				}
				else
				{
					data.m_MorphXY_SteperScaleInvers_DoubleChunkSize.m_y = 1.0f / (morphEnd-morphBegin);
				}
				data.m_DetailIndex = iterDepth - i;//这个数值不可能小于0，应为地0级别地形只有一层
				ASSERT( data.m_DetailIndex >= 0 );
			}
			data.m_IndicesBuffer = NEW IndicesBuffer( Device::MU_STATIC );
			{//计算本层的索引
				std::vector<ushort>	indices;
				int stepr = Math::Pow( 2.0f, float(ppy.m_SplitLevel - iterDepth + i ));//计算步长
				data.m_MorphXY_SteperScaleInvers_DoubleChunkSize.m_z = 1.0f / ( stepr * ppy.m_Scale ); //当前的递增距离与缩放的乘积的逆，用于确定当前是第几个mesh等级使用，将顶点还原到0,1,2,3,4,5这种1递增方式
				data.m_MorphXY_SteperScaleInvers_DoubleChunkSize.m_w = 2.0 * stepr*ppy.m_Scale; //当前chunk的尺寸

				vector2d size = end - begin;//计算索引数量
				size = ( size - vector2d(1,1) ) / stepr + vector2d(1,1);
				ASSERT( ((size.m_x - 1) % stepr ) % 2 == 0 );//必须被当前递增步长整除后为偶数
				indices.resize( size.m_x*(size.m_y-1)*2 );

				int i = 0;
				for ( int z = begin.m_y ; z < end.m_y-1; z+=stepr )
				{
					for ( int x = begin.m_x ; x < end.m_x; x+=stepr )
					{
						indices[i] = x + z * ppy.m_TileSize; 
						indices[i+1] = x + (z + stepr) * ppy.m_TileSize;
						i+=2;
					}
					z += stepr;
					if( z < end.m_y-1 )
					{
						for( int x = end.m_x-1; x >= begin.m_x ; x-=stepr )
						{
							indices[i] = x + (z + stepr) * ppy.m_TileSize;
							indices[i+1] = x + z * ppy.m_TileSize; 
							i+=2;
						}
					}
				}
				data.m_IndicesBuffer->FlushIndiesBuffer( indices.size(), &indices[0] );
			}
			i++;
			m_DrawData.push_back( data );
		} while ( m_DrawData.size() < DL_COUNT && !m_isFirstNode );

		//判断是否需要继续迭代
		if ( ppy.m_SplitLevel != iterDepth )
		{
			vector2d sizebe = ( end - begin - vector2d(1,1) ) / 2 + vector2d(1,1);
			vector2d b,e;
			vector3f center = m_BindBox.GetCenter();
			{
				b = begin;
				e = b+sizebe;
				m_LeafChunk_UL = NEW TerrainChunk( b, e,
					//aabbox3df( m_BindBox.GetMin(), vector3f( center.m_x, m_BindBox.GetMax().m_y, center.m_z ) ),
					iterDepth+1, ppy);
			}
			{
				b = begin;
				b.m_x = begin.m_x + sizebe.m_x - 1;
				e = b+sizebe;
				m_LeafChunk_UR = NEW TerrainChunk( b, e,
					//aabbox3df( vector3f(center.m_x, m_BindBox.GetMin().m_y, m_BindBox.GetMin().m_z ),
					//vector3f( m_BindBox.GetMax().m_x, m_BindBox.GetMax().m_y, center.m_z ) ),
					iterDepth+1, ppy);
			}
			{
				b = begin;
				b.m_y = begin.m_y + sizebe.m_y - 1;
				e = b+sizebe;
				m_LeafChunk_DL = NEW TerrainChunk( b, e,
					//aabbox3df( vector3f( m_BindBox.GetMin().m_x, m_BindBox.GetMin().m_y, center.m_z ),
					//vector3f( center.m_x, m_BindBox.GetMax().m_y, m_BindBox.GetMax().m_z ) ),
					iterDepth+1, ppy);
			}
			{
				b = begin+sizebe - vector2d(1,1);
				e = end;
				m_LeafChunk_DR = NEW TerrainChunk( b, e,
					//aabbox3df( vector3f( center.m_x, m_BindBox.GetMin().m_y, center.m_z ),	m_BindBox.GetMax() ),
					iterDepth+1, ppy);
			}
		}
	}
	//-------------------------------------------------------------------------------------------------------
	TerrainChunk::~TerrainChunk( )
	{
		for(int i = 0 ; i < m_DrawData.size(); i++ )
		{
			SAFE_DELETE( m_DrawData[i].m_IndicesBuffer );
		}
		SAFE_DELETE( m_LeafChunk_UL );
		SAFE_DELETE( m_LeafChunk_UR );
		SAFE_DELETE( m_LeafChunk_DL );
		SAFE_DELETE( m_LeafChunk_DR );
	}
	//-------------------------------------------------------------------------------------------------------
	void TerrainChunk::Update( const Frustumf& view, aabspheref& sphere, std::multimap< float, TerrainChunk* >& tmap )
	{
		m_isDraw = false;
		m_isSplit = false;
		if( m_isCanSee && view.Intersect( m_BindBox ) )//如果此节点在绑定盒子内
		{
			sphere.SetRadius( m_BeginEnd.m_x );//如果下一个细节等级与这个绑定盒子没有相交，则不需要分裂
			//if( (!m_BindBox.Intersection( sphere ) || m_isLastNode ) && !m_isFirstNode )
			if(!m_BindBox.Intersection( sphere ) || m_isLastNode )
			{
				if( m_isFirstNode )
				{
					m_DrawLevel = static_cast<int>(DL_HIGHT);
				}
				else
				{
					sphere.SetRadius( m_BeginEnd.m_y );//如果这个等级与绑定和相交了，就是用当前高级细节等级
					if ( m_BindBox.Intersection( sphere ) )
					{
						m_DrawLevel = static_cast<int>(DL_HIGHT);
					}
					else
					{
						m_DrawLevel = static_cast<int>(DL_LOW);
					}
				}
				m_isDraw = true;
				tmap.insert( std::make_pair( ( sphere.GetCenter() - m_BindBox.GetCenter() ).LengthPow(), this ) );
			}
			else
			{
				m_isSplit = true;
				m_LeafChunk_UL->Update( view, sphere, tmap );
				m_LeafChunk_UR->Update( view, sphere, tmap );
				m_LeafChunk_DL->Update( view, sphere, tmap );
				m_LeafChunk_DR->Update( view, sphere, tmap );
			}
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void TerrainChunk::AssignMaterial( const bool& isCast, const ICamera& camera )
	{
		if( m_isLastNode )
		{
			for ( int i = 0 ; i < DL_COUNT ; i ++ )
			{
				if ( m_DrawData[i].m_RenderShader != ShaderManage::TerrainCDLODLow )
				{
					if ( isCast && camera.GetFrustum().Intersect( m_BindBox ) )
					{
						m_DrawData[i].m_RenderShader = ShaderManage::TerrainCDLOD;
					}
					else
					{
						m_DrawData[i].m_RenderShader = ShaderManage::TerrainCDLODNoShadow;
					}
				}
			}
		}
		else if ( m_isSplit )
		{
			m_LeafChunk_UL->AssignMaterial( isCast, camera );
			m_LeafChunk_UR->AssignMaterial( isCast, camera );
			m_LeafChunk_DL->AssignMaterial( isCast, camera );
			m_LeafChunk_DR->AssignMaterial( isCast, camera );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void TerrainChunk::Render( Engine& engine) const
	{
		//if( m_isDraw )
		ASSERT( true == m_isDraw );
		{
			//DEBUGLOG( "shader is %s \n", m_DrawData[m_DrawLevel].m_RenderShader == ShaderManage::TerrainCDLOD ? "TerrainCDLOD" : m_DrawData[m_DrawLevel].m_RenderShader == ShaderManage::TerrainCDLODLow ? "TerrainCDLODLow" : "TerrainCDLODNoShadow" );
			m_Material->SetShader( Pipeline::PT_LIGHTING, m_DrawData[m_DrawLevel].m_RenderShader );
			m_Buffer->SetIndicesBuffer( m_DrawData[m_DrawLevel].m_IndicesBuffer );

			m_Material->SetNode( (IAttributeNode*)this );
			m_Buffer->SetRenderMode( Device::RM_TRIANGLE_STRIP );
			engine.GetDevice()->Draw( *m_Buffer, *m_Material);
		}
		//else if( !m_isLastNode && m_isSplit )
		//{
		//	m_LeafChunk_UL->Render( buffer, mater, engine );
		//	m_LeafChunk_UR->Render( buffer, mater, engine );
		//	m_LeafChunk_DL->Render( buffer, mater, engine );
		//	m_LeafChunk_DR->Render( buffer, mater, engine );
		//}
	}
	//-------------------------------------------------------------------------------------------------------
	const void * TerrainChunk::Attribute( const IAttributeNode::Node_Attribute &att ) const
	{
		switch ( att )
		{
		case IAttributeNode::ATT_USER_0: return m_SizeScaleInv_RepeatSizeInv.GetVectorArray(); break;
		case IAttributeNode::ATT_USER_1: return m_DrawData[m_DrawLevel].m_MorphXY_SteperScaleInvers_DoubleChunkSize.GetVectorArray(); break;
		case IAttributeNode::ATT_USER_2: return &m_DrawData[m_DrawLevel].m_DetailIndex; break;
		case IAttributeNode::ATT_USER_3: return m_OffsetScale.GetVectorArray(); break;
		case IAttributeNode::ATT_USER_4: return &m_DetailDisance_Tile; break;
		}
		return NULL;
	}
	//-------------------------------------------------------------------------------------------------------
	void TerrainChunk::LoadBindBox( Serializer& loader )
	{
		vector3f vmin, vmax;
		loader>>vmax>>vmin;
		loader>>m_isCanSee;
		m_BindBox = aabbox3df( vmin, vmax );
		if( !m_isLastNode )
		{
			m_LeafChunk_UL->LoadBindBox( loader );//读取的迭代顺序必须如此，如果修改要对应修改TerrainEditChunk的保存顺序
			m_LeafChunk_UR->LoadBindBox( loader );
			m_LeafChunk_DL->LoadBindBox( loader );
			m_LeafChunk_DR->LoadBindBox( loader );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void TerrainChunk::OffsetScale( const vector2f& os )
	{
		m_OffsetScale = os;
		if( !m_isLastNode )
		{
			m_LeafChunk_UL->OffsetScale( os );//读取的迭代顺序必须如此，如果修改要对应修改TerrainEditChunk的保存顺序
			m_LeafChunk_UR->OffsetScale( os );
			m_LeafChunk_DL->OffsetScale( os );
			m_LeafChunk_DR->OffsetScale( os );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void TerrainChunk::SetBuffer( RendBuffer* buffer )
	{
		m_Buffer = buffer;
		if( !m_isLastNode )
		{
			m_LeafChunk_UL->SetBuffer( buffer );
			m_LeafChunk_UR->SetBuffer( buffer );
			m_LeafChunk_DL->SetBuffer( buffer );
			m_LeafChunk_DR->SetBuffer( buffer );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void TerrainChunk::SetMaterial( Material* m )
	{
		m_Material= m;
		if( !m_isLastNode )
		{
			m_LeafChunk_UL->SetMaterial( m );
			m_LeafChunk_UR->SetMaterial( m );
			m_LeafChunk_DL->SetMaterial( m );
			m_LeafChunk_DR->SetMaterial( m );
		}
	}
}