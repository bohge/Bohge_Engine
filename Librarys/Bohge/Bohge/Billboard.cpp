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
//			¹ã¸æ°æ		   //
/////////////////////////////
#include "Predefine.h"
#include "Billboard.h"
#include "Vertex.h"
#include "ShaderManage.h"
#include "3DMath.h"
#include "Camera.h"
#include "Texture.h"
#include "RendBuffer.h"
#include "Shaders2D.h"


namespace BohgeEngine
{
	Billboard::	Billboard( )
	{
		m_VertexArray[0].Coordinates() = vector4f( 0, 0, 0, 0 );
		m_VertexArray[1].Coordinates() = vector4f( 0, 1, 0, 1 );
		m_VertexArray[2].Coordinates() = vector4f( 1, 0, 1, 0 );
		m_VertexArray[3].Coordinates() = vector4f( 1, 1, 1, 1 );
	}
	//-------------------------------------------------------------------------------------
	Billboard::Billboard( const vector2f& b, const vector2f& e )
	{
		SetBillCoordnate( b, e );
	}
	//-------------------------------------------------------------------------------------
	Billboard::~Billboard()
	{
	}
	//-------------------------------------------------------------------------------------
	void Billboard::GetVertexBuffer(SharedBuffer& input) const 
	{
		const static ushort Indies[COUNT_OF_INDIES]={0,1,2, 2,1,3};
		input.AddVertex( m_VertexArray, Indies );
	}
	//-------------------------------------------------------------------------------------
	void Billboard::SetBillBorad( const vector3f& pos, const vector2f& rectsize )
	{
		//m_VertexArray[0].SetVertex( vector4f( pos.m_x - rectsize.m_x, pos.m_y + rectsize.m_y, pos.m_z, 1.0 ) );
		//m_VertexArray[1].SetVertex( vector4f( pos.m_x - rectsize.m_x, pos.m_y - rectsize.m_y, pos.m_z, 1.0 ) );
		//m_VertexArray[2].SetVertex( vector4f( pos.m_x + rectsize.m_x, pos.m_y + rectsize.m_y, pos.m_z, 1.0 ) );
		//m_VertexArray[3].SetVertex( vector4f( pos.m_x + rectsize.m_x, pos.m_y - rectsize.m_y, pos.m_z, 1.0 ) );
		m_VertexArray[0].SetVertex( vector4f( pos.m_x - rectsize.m_x, pos.m_y + 2*rectsize.m_y, pos.m_z, 1.0 ) );
		m_VertexArray[1].SetVertex( vector4f( pos.m_x - rectsize.m_x, pos.m_y, pos.m_z, 1.0 ) );
		m_VertexArray[2].SetVertex( vector4f( pos.m_x + rectsize.m_x, pos.m_y + 2*rectsize.m_y, pos.m_z, 1.0 ) );
		m_VertexArray[3].SetVertex( vector4f( pos.m_x + rectsize.m_x, pos.m_y, pos.m_z, 1.0 ) );
		for ( int i = 0 ; i < COUNT_OF_VERTEX; i ++ )
		{
			m_VertexArray[i].Normal() = pos;
		}
	}
	//-------------------------------------------------------------------------------------
	void Billboard::SetBillCoordnate( const vector2f& lt, const vector2f& rb )
	{
		m_VertexArray[0].Coordinates() = vector4f( lt.m_x, lt.m_y, 0, 0 );
		m_VertexArray[1].Coordinates() = vector4f( lt.m_x, rb.m_y, 0, 1 );
		m_VertexArray[2].Coordinates() = vector4f( rb.m_x, lt.m_y, 1, 0 );
		m_VertexArray[3].Coordinates() = vector4f( rb.m_x, rb.m_y, 1, 1 );
	}
	//-------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------







	//////////////////////////////
	//		BillboardNode		//
	/////////////////////////////
	//-------------------------------------------------------------------------------------
	BillboardNode::BillboardNode()
		:m_pTexture(NULL)
	{
		ushort Indies[Billboard::COUNT_OF_INDIES]={0,1,2, 2,1,3};
		m_pRendBuffer = NEW RendBuffer( Billboard::GetRenderMode() );
		m_pVertexBuffer = NEW VertexBuffer( Device::MU_DYNAMIC );
		m_pIndicesBuffer = NEW IndicesBuffer( Device::MU_STATIC );
		m_pRendBuffer->SetVertexBuffer( m_pVertexBuffer );
		m_pRendBuffer->SetIndicesBuffer( m_pIndicesBuffer );
		m_pIndicesBuffer->FlushIndiesBuffer( Billboard::COUNT_OF_INDIES, &Indies[0] );
	}
	//-------------------------------------------------------------------------------------
	BillboardNode::~BillboardNode()
	{
		SAFE_DELETE( m_pRendBuffer );
		SAFE_DELETE( m_pVertexBuffer );
		SAFE_DELETE( m_pIndicesBuffer );
		//SAFE_DELETE( m_pTexture );
	}
	//-------------------------------------------------------------------------------------
	void BillboardNode::EndRender( Engine& engine )
	{
		engine.GetDevice()->DisableDepthTest();
		engine.GetDevice()->EnableAlphaBlend( Device::BL_SRC_ALPHA, Device::BL_ONE_MINUS_SRC_ALPHA, Device::BL_ONE, Device::BL_ONE );
		BillboardShader& bs = engine.GetShaderManage()->GetShader<BillboardShader>(ShaderManage::BillboardShader);
		engine.GetDevice()->Draw( *m_pRendBuffer, bs, m_pTexture );
	}
	//-------------------------------------------------------------------------------------
	bool BillboardNode::isInview( const Frustumf& cFru )
	{
		return IntersectedType::IT_NO != cFru.Intersect( m_BindBox );
	}
	//-------------------------------------------------------------------------------------
	void BillboardNode::SetBillBorad( const vector3f& pos, const vector2f& rectsize )
	{
		Billboard::SetBillBorad( pos, rectsize );
		m_pVertexBuffer->FlushVertexBuffer( COUNT_OF_VERTEX, m_VertexArray );
		m_BindBox.Reset();
		m_BindBox.Contains( vector3f(pos.m_x - rectsize.m_x, pos.m_y - rectsize.m_y, pos.m_z) );
		m_BindBox.Contains( vector3f(pos.m_x + rectsize.m_x, pos.m_y + rectsize.m_y, pos.m_z) );
	}
	//-------------------------------------------------------------------------------------
	void BillboardNode::SetBillCoordnate( const vector2f& lt, const vector2f& rb )
	{
		Billboard::SetBillCoordnate( lt, rb );
		m_pVertexBuffer->FlushVertexBuffer( COUNT_OF_VERTEX, m_VertexArray );
	}
}