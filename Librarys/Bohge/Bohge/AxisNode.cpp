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



//////////////////////////////
//			坐标轴		   //
/////////////////////////////

#include "AxisNode.h"
#include "Vertex.h"
#include "ShaderManager.h"
#include "RendBuffer.h"
#include "Shaders2D.h"
#include "Billboard.h"
#include "Bfont.h"
#include "Texture.h"

#include <map>
#include <sstream>

using namespace std;


namespace BohgeEngine
{
	//------------------------------------------------------------------------------------------------------
	AxisNode::AxisNode(void)
		:m_BillboardSize(0),
		m_pBillboardArray(NULL),
		m_pLineArray(NULL),
		m_pLineBuffer(NULL),
		m_pLineVertex(NULL),
		m_pLineIndices(NULL),
		m_pBillboardString(NULL),
		m_pBillboardBuffer(NULL),
		m_pBillboardBufferVertex(NULL),
		m_pBillboardBufferIndices(NULL),
		m_AsixLength(0),
		m_TextureCount(0),
		m_isMaked(false)
	{
		static ushort Indies[COUNT_OF_INDIES]={0,1,2,3,4,5};
		m_pLineArray = NEW VertexColor[COUNT_OF_VERTEX];
		m_pLineArray[0].SetColor( Color(1,0,0,1) );//01x
		m_pLineArray[1].SetColor( Color(1,0,0,1) );
		m_pLineArray[2].SetColor( Color(0,1,0,1) );//23y
		m_pLineArray[3].SetColor( Color(0,1,0,1) );
		m_pLineArray[4].SetColor( Color(0,0,1,1) );//45z
		m_pLineArray[5].SetColor( Color(0,0,1,1) );
		m_pLineBuffer = NEW RendBuffer( Device::RM_LINES );
		m_pLineVertex = NEW VertexBuffer( Device::MU_DYNAMIC );
		m_pLineIndices = NEW IndicesBuffer( Device::MU_STATIC );
		m_pLineBuffer->SetVertexBuffer( m_pLineVertex );
		m_pLineBuffer->SetIndicesBuffer( m_pLineIndices );
		m_pLineIndices->FlushIndiesBuffer( COUNT_OF_INDIES, &Indies[0] );

		//广告版
		m_pBillboardArray = NEW Billboard[MAX_BILLBOARD_SIZE];
		m_pBillboardSharedBuffer = NEW SharedBuffer( 
			Billboard::GetElementCount(),
			Billboard::GetCountOfVertex(),
			Billboard::GetCountOfIndes(), 
			MAX_BILLBOARD_SIZE );
		m_pBillboardBuffer = NEW RendBuffer( Billboard::GetRenderMode() );
		m_pBillboardBufferVertex = NEW VertexBuffer( Device::MU_DYNAMIC );
		m_pBillboardBufferIndices = NEW IndicesBuffer( Device::MU_DYNAMIC );
		m_pBillboardBuffer->SetVertexBuffer( m_pBillboardBufferVertex );
		m_pBillboardBuffer->SetIndicesBuffer( m_pBillboardBufferIndices );
	}
	//------------------------------------------------------------------------------------------------------
	AxisNode::~AxisNode(void)
	{
		Engine::Instance().GetStringManager()->DestroyString( m_pBillboardString );
		SAFE_DELETE_ARRAY(m_pLineArray);
		SAFE_DELETE(m_pLineBuffer);
		SAFE_DELETE(m_pLineVertex);
		SAFE_DELETE(m_pLineIndices);
		SAFE_DELETE_ARRAY(m_pBillboardArray);
		SAFE_DELETE( m_pBillboardSharedBuffer );
		SAFE_DELETE( m_pBillboardBuffer );
		SAFE_DELETE( m_pBillboardBufferVertex );
		SAFE_DELETE( m_pBillboardBufferIndices );
	}
	//------------------------------------------------------------------------------------------------------
	void AxisNode::SetCenter( const vector3f& center )
	{
		m_Center = center;
		vector3f c = center;
		//const Matrix44f* pRootMat = (const Matrix44f*)FatherAttribute( IAttributeNode::ATT_WORLDTRANSFORM );
		//if ( NULL != pRootMat )
		//{
		//	float inv = 1.0 / pRootMat->a44;
		//	vector3f RootPos( pRootMat->a41*inv, pRootMat->a42*inv, pRootMat->a43*inv ) ;
		//	c += RootPos;
		//}
		const vector3f* pRootPosition = (const vector3f*)FatherAttribute( IAttributeNode::ATT_WORLDPOSITION );
		if ( NULL != pRootPosition )
		{
			c += *pRootPosition;
		}
		float l = m_AsixLength;
		int countText = m_TextureCount;

		m_pLineArray[0].SetVertex( vector4f(c.m_x,c.m_y,c.m_z,1) );//01x
		m_pLineArray[1].SetVertex( vector4f(c.m_x+l,c.m_y,c.m_z,1) );
		m_pLineArray[2].SetVertex( vector4f(c.m_x,c.m_y,c.m_z,1) );//23y
		m_pLineArray[3].SetVertex( vector4f(c.m_x,c.m_y+l,c.m_z,1) );
		m_pLineArray[4].SetVertex( vector4f(c.m_x,c.m_y,c.m_z,1) );//45z
		m_pLineArray[5].SetVertex( vector4f(c.m_x,c.m_y,c.m_z+l,1) );
		m_BindBox.Reset();
		m_BindBox.Contains( vector3f(c.m_x,c.m_y,c.m_z) );
		m_BindBox.Contains( vector3f(c.m_x+l,c.m_y,c.m_z) );
		m_BindBox.Contains( vector3f(c.m_x,c.m_y+l,c.m_z) );
		m_BindBox.Contains( vector3f(c.m_x,c.m_y,c.m_z+l) );
		m_pLineVertex->FlushVertexBuffer( COUNT_OF_VERTEX, m_pLineArray );

		float addtion = l / countText;
		float coord = 1.0 / countText;
		vector2f size( l * 0.08,  l * 0.03 );
		{//重置广告版
			for( int i = 0, index = 0 ; i < countText; i ++, index++ )//x轴
			{
				m_pBillboardArray[i].SetBillBorad( vector3f( c.m_x+addtion * (index+1), c.m_y, c.m_z ), size );
			}
			for( int i = countText, index = 0 ; i < countText * 2; i ++, index++ )//y轴
			{
				m_pBillboardArray[i].SetBillBorad( vector3f( c.m_x, c.m_y+addtion * (index+1), c.m_z ), size );}
			for( int i =  countText * 2, index = 0 ; i < countText * 3; i ++, index++ )//x轴
			{
				m_pBillboardArray[i].SetBillBorad( vector3f( c.m_x, c.m_y, c.m_z+addtion * (index+1)), size );
			}
		}
		{//打包buffer
			m_pBillboardSharedBuffer->CleanUp();
			for ( int i = 0; i < m_BillboardSize; i ++ )
			{
				m_pBillboardArray[i].GetVertexBuffer( *m_pBillboardSharedBuffer );
			}
			m_pBillboardBufferVertex->FlushVertexBuffer( 
				m_pBillboardSharedBuffer->CountOfAllVertex(),
				static_cast<Billboard::VertexType*>(m_pBillboardSharedBuffer->VertexPtr()) );
			m_pBillboardBufferIndices->FlushIndiesBuffer( m_pBillboardSharedBuffer->CountOfAllIndies(), m_pBillboardSharedBuffer->IndiesPtr() );
		}
	}
	//------------------------------------------------------------------------------------------------------
	void AxisNode::CreateAxis( const vector3f& center, float l, int countText  )
	{
		m_Center = center;
		vector3f c = center;
		const Matrix44f* pRootMat = (const Matrix44f*)FatherAttribute( IAttributeNode::ATT_WORLDTRANSFORM );
		if ( NULL != pRootMat )
		{
			float inv = 1.0 / pRootMat->a44;
			vector3f RootPos( pRootMat->a41*inv, pRootMat->a42*inv, pRootMat->a43*inv ) ;
			c += RootPos;
		}
		m_isMaked = true;
		m_AsixLength = l;
		m_TextureCount = countText;

		m_pLineArray[0].SetVertex( vector4f(c.m_x,c.m_y,c.m_z,1) );//01x
		m_pLineArray[1].SetVertex( vector4f(c.m_x+l,c.m_y,c.m_z,1) );
		m_pLineArray[2].SetVertex( vector4f(c.m_x,c.m_y,c.m_z,1) );//23y
		m_pLineArray[3].SetVertex( vector4f(c.m_x,c.m_y+l,c.m_z,1) );
		m_pLineArray[4].SetVertex( vector4f(c.m_x,c.m_y,c.m_z,1) );//45z
		m_pLineArray[5].SetVertex( vector4f(c.m_x,c.m_y,c.m_z+l,1) );
		m_BindBox.Reset();
		m_BindBox.Contains( vector3f(c.m_x,c.m_y,c.m_z) );
		m_BindBox.Contains( vector3f(c.m_x+l,c.m_y,c.m_z) );
		m_BindBox.Contains( vector3f(c.m_x,c.m_y+l,c.m_z) );
		m_BindBox.Contains( vector3f(c.m_x,c.m_y,c.m_z+l) );
		m_pLineVertex->FlushVertexBuffer( COUNT_OF_VERTEX, m_pLineArray );

		//计算文字
		m_BillboardSize = countText * 3;
		m_BillboardSize = m_BillboardSize <= MAX_BILLBOARD_SIZE ? m_BillboardSize : MAX_BILLBOARD_SIZE;

		float addtion = l / countText;
		float coord = 1.0 / countText;
		vector2f size( l * 0.15,  l * 0.6 );

		vector4f coordArray[MAX_TEXTURE_SIZE];
		{//纹理
			wstringstream wsstr;
			//wsstr.precision( 2 );
			for ( int i = 0 ; i < countText; i ++ )
			{
				wsstr<<addtion * (i+1);
			}
			Engine::Instance().GetStringManager()->DestroyString( m_pBillboardString );
			m_pBillboardString = Engine::Instance().GetStringManager()->MakeString( vector2f(0.2,0.2), wsstr.str() );
			m_pBillboardString->SetManaged( false );
			for ( int i = 0 ; i < countText; i ++ )
			{
				wstringstream wsstr2;
				wsstr2<<addtion * (i+1);
				coordArray[i] = m_pBillboardString->GetStringCoordnate( wsstr2.str() );
			}
		}
		{//重置广告版
			for( int i = 0, index = 0 ; i < countText; i ++, index++ )//x轴
			{
				m_pBillboardArray[i].SetBillBorad( vector3f( c.m_x+addtion * (index+1), c.m_y, c.m_z ), size );
				m_pBillboardArray[i].SetBillCoordnate( 
					vector2f( coordArray[index].m_x, coordArray[index].m_y ),
					vector2f( coordArray[index].m_z, coordArray[index].m_w ) );
			}
			for( int i = countText, index = 0 ; i < countText * 2; i ++, index++ )//y轴
			{
				m_pBillboardArray[i].SetBillBorad( vector3f( c.m_x, c.m_y+addtion * (index+1), c.m_z ), size );
				m_pBillboardArray[i].SetBillCoordnate( 
					vector2f( coordArray[index].m_x, coordArray[index].m_y ),
					vector2f( coordArray[index].m_z, coordArray[index].m_w ) );		}
			for( int i =  countText * 2, index = 0 ; i < countText * 3; i ++, index++ )//x轴
			{
				m_pBillboardArray[i].SetBillBorad( vector3f( c.m_x, c.m_y, c.m_z+addtion * (index+1)), size );
				m_pBillboardArray[i].SetBillCoordnate( 
					vector2f( coordArray[index].m_x, coordArray[index].m_y ),
					vector2f( coordArray[index].m_z, coordArray[index].m_w ) );		}
		}
		{//打包buffer
			m_pBillboardSharedBuffer->CleanUp();
			for ( int i = 0; i < m_BillboardSize; i ++ )
			{
				m_pBillboardArray[i].GetVertexBuffer( *m_pBillboardSharedBuffer );
			}
			m_pBillboardBufferVertex->FlushVertexBuffer( 
				m_pBillboardSharedBuffer->CountOfAllVertex(),
				static_cast<Billboard::VertexType*>(m_pBillboardSharedBuffer->VertexPtr()) );
			m_pBillboardBufferIndices->FlushIndiesBuffer( m_pBillboardSharedBuffer->CountOfAllIndies(), m_pBillboardSharedBuffer->IndiesPtr() );
		}
	}
	//------------------------------------------------------------------------------------------------------
	bool AxisNode::isInview( const Frustumf& cFru )
	{
		//return IntersectedType::IT_NO != cFru.Intersect( m_BindBox * m_Transfrom );
		return IntersectedType::IT_NO != cFru.Intersect( m_BindBox );
	}
	//------------------------------------------------------------------------------------------------------
	void AxisNode::EndRender( Engine& engine )
	{
		if ( m_isMaked )
		{
			engine.GetDevice()->DisableDepthTest();
			engine.GetDevice()->DepthMask( true );
			WorldShapes& ss = engine.GetShaderManager()->GetShader<WorldShapes>(ShaderManager::WorldShapes);
			ss.SetParamTransform( &m_Transfrom );
			engine.GetDevice()->Draw( *m_pLineBuffer, ss, NULL );

			engine.GetDevice()->EnableAlphaBlend( Device::BL_SRC_ALPHA, Device::BL_ONE_MINUS_SRC_ALPHA, Device::BL_SRC_ALPHA, Device::BL_ONE_MINUS_SRC_ALPHA);
			BillboardShader& bs = engine.GetShaderManager()->GetShader<BillboardShader>(ShaderManager::BillboardShader);
			engine.GetDevice()->Draw( *m_pBillboardBuffer, bs, &m_pBillboardString->GetTexture() );
		}
	}
	//------------------------------------------------------------------------------------------------------
	const void* AxisNode::Attribute( const Node_Attribute& name ) const
	{
		switch( name )
		{
		case IAttributeNode::ATT_WORLDTRANSFORM: return &m_Transfrom;
		}
		return NULL;
	}
	//------------------------------------------------------------------------------------------------------
	void AxisNode::Update( uint milisecon )
	{
		if ( isFatherChange() )
		{
			SetCenter( m_Center );
			CastChangedMessage();
		}
	}










	//////////////////////////////
	//		SelectableAxisNode	//
	/////////////////////////////
	//------------------------------------------------------------------------------------------------------
	void SelectableAxisNode::CreateAxis( const vector3f& center, float l, int countText )
	{
		AxisNode::CreateAxis( center, l, countText  );
		float wh = l * 0.05;
		vector3f c = center;
		const vector3f* pRootPos = (const vector3f*)FatherAttribute( IAttributeNode::ATT_WORLDPOSITION );
		if ( NULL != pRootPos )
		{
			c += *pRootPos;
		}
		m_XBind = aabbox3df( vector3f( c.m_x, c.m_y-wh, c.m_z-wh ), vector3f( c.m_x+l, c.m_y+wh, c.m_z+wh ) );
		m_YBind = aabbox3df( vector3f( c.m_x-wh, c.m_y, c.m_z-wh ), vector3f( c.m_x+wh, c.m_y+l, c.m_z+wh ) );
		m_ZBind = aabbox3df( vector3f( c.m_x-wh, c.m_y-wh, c.m_z ), vector3f( c.m_x+wh, c.m_y+wh, c.m_z+l ) );
	}
	//------------------------------------------------------------------------------------------------------
	void SelectableAxisNode::SetCenter( const vector3f& center )
	{
		AxisNode::SetCenter( center );
		float l = GetAsixLength();
		float wh = l * 0.05;
		vector3f c = center;
		const vector3f* pRootPos = (const vector3f*)FatherAttribute( IAttributeNode::ATT_WORLDPOSITION );
		if ( NULL != pRootPos )
		{
			c += *pRootPos;
		}
		m_XBind = aabbox3df( vector3f( c.m_x, c.m_y-wh, c.m_z-wh ), vector3f( c.m_x+l, c.m_y+wh, c.m_z+wh ) );
		m_YBind = aabbox3df( vector3f( c.m_x-wh, c.m_y, c.m_z-wh ), vector3f( c.m_x+wh, c.m_y+l, c.m_z+wh ) );
		m_ZBind = aabbox3df( vector3f( c.m_x-wh, c.m_y-wh, c.m_z ), vector3f( c.m_x+wh, c.m_y+wh, c.m_z+l ) );
	}
	//------------------------------------------------------------------------------------------------------
	SelectableAxisNode::Selected SelectableAxisNode::IsSelected( const Rayf& r )
	{
		if ( m_BindBox.Intersection( r ) )
		{
			map< float, std::pair< aabbox3df* , Selected > > boxmap;
			boxmap.insert( std::make_pair( (r.GetRayOrigin() - m_XBind.GetCenter()).LengthPow(), std::make_pair( &m_XBind, SA_X_AXIS ) ) );
			boxmap.insert( std::make_pair( (r.GetRayOrigin() - m_YBind.GetCenter()).LengthPow(), std::make_pair( &m_YBind, SA_Y_AXIS ) ) );
			boxmap.insert( std::make_pair( (r.GetRayOrigin() - m_ZBind.GetCenter()).LengthPow(), std::make_pair( &m_ZBind, SA_Z_AXIS ) ) );
			for ( map< float, std::pair< aabbox3df* , Selected > >::iterator it = boxmap.begin();
				it != boxmap.end();
				it ++ )
			{
				if ( it->second.first->Intersection( r ) )
				{
					return it->second.second;
				}
			}
		}
		return SA_NON;
	}
	//------------------------------------------------------------------------------------------------------
	void SelectableAxisNode::EndRender( Engine& engine )
	{
		AxisNode::EndRender( engine );
#ifdef DRAWAABBOX
		m_XBind.Render( engine );
		m_YBind.Render( engine );
		m_ZBind.Render( engine );
#endif
	}

}