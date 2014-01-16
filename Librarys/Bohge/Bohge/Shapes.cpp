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



/////////////////////////////////////
//			线,矩形等图形		   //
/////////////////////////////////////

#include "Shapes.h"
#include "Vertex.h"
#include "ShaderManage.h"
#include "RendBuffer.h"
#include "Camera.h"
#include "Shaders2D.h"



namespace BohgeEngine
{
	//------------------------------------------------------------------------------------------------------
	void Rect2D::NewRect2D()
	{
		m_pVertexArray = NEW VertexColor[COUNT_OF_VERTEX];

		static ushort Indies[COUNT_OF_INDIES]={0,1, 1,2, 2,3, 3,0};
		m_pRendBuffer = NEW RendBuffer( Device::RM_LINES );
		m_pVertexBuffer = NEW VertexBuffer( Device::MU_DYNAMIC );
		m_pIndicesBuffer = NEW IndicesBuffer( Device::MU_STATIC );
		m_pRendBuffer->SetVertexBuffer( m_pVertexBuffer );
		m_pRendBuffer->SetIndicesBuffer( m_pIndicesBuffer );
		m_pIndicesBuffer->FlushIndiesBuffer( COUNT_OF_INDIES, &Indies[0] );
	}
	//------------------------------------------------------------------------------------------------------
	Rect2D::Rect2D()
	{
		NewRect2D();
	}
	//------------------------------------------------------------------------------------------------------
	Rect2D::Rect2D(const Rect2D& input)
	{
		NewRect2D();
		for( uint i = 0 ; i < COUNT_OF_VERTEX ; i ++ )
		{
			m_pVertexArray[i] = input.m_pVertexArray[i];
		}
		m_pVertexBuffer->FlushVertexBuffer( COUNT_OF_VERTEX, m_pVertexArray );
	}
	//------------------------------------------------------------------------------------------------------
	Rect2D::Rect2D(const vector2f& leftTop, const vector2f& rightBottom, const Color& color)
	{
		NewRect2D();
		m_pVertexArray[0].SetVertex( vector4f( leftTop.m_x ,		leftTop.m_y ,		0.0 ) );
		m_pVertexArray[1].SetVertex( vector4f( leftTop.m_x ,		rightBottom.m_y ,	0.0 ) );
		m_pVertexArray[2].SetVertex( vector4f( rightBottom.m_x ,	rightBottom.m_y ,	0.0 ) );
		m_pVertexArray[3].SetVertex( vector4f( rightBottom.m_x ,	leftTop.m_y ,		0.0 ) );
		m_pVertexArray[0].SetColor( color );
		m_pVertexArray[1].SetColor( color );
		m_pVertexArray[2].SetColor( color );
		m_pVertexArray[3].SetColor( color );

		m_pVertexBuffer->FlushVertexBuffer( COUNT_OF_VERTEX, m_pVertexArray );
	}
	//------------------------------------------------------------------------------------------------------
	Rect2D::~Rect2D()
	{
		SAFE_DELETE_ARRAY(m_pVertexArray);
		SAFE_DELETE(m_pRendBuffer);
		SAFE_DELETE(m_pVertexBuffer);
		SAFE_DELETE(m_pIndicesBuffer);
	}
	//------------------------------------------------------------------------------------------------------
	void Rect2D::SetRect(const vector2f& leftTop, const vector2f& rightBottom)
	{
		m_pVertexArray[0].SetVertex( vector4f( leftTop.m_x ,		leftTop.m_y ,		0.0 ) );
		m_pVertexArray[1].SetVertex( vector4f( leftTop.m_x ,		rightBottom.m_y ,	0.0 ) );
		m_pVertexArray[2].SetVertex( vector4f( rightBottom.m_x ,	rightBottom.m_y ,	0.0 ) );
		m_pVertexArray[3].SetVertex( vector4f( rightBottom.m_x ,	leftTop.m_y ,		0.0 ) );
		m_pVertexArray[0].SetColor( Color::BloodRed );
		m_pVertexArray[1].SetColor( Color::BloodRed );
		m_pVertexArray[2].SetColor( Color::BloodRed );
		m_pVertexArray[3].SetColor( Color::BloodRed );
		m_pVertexBuffer->FlushVertexBuffer( COUNT_OF_VERTEX, m_pVertexArray );
	}
	//------------------------------------------------------------------------------------------------------
	void Rect2D::SetColor(const Color &color)
	{
		for( uint i = 0 ; i < COUNT_OF_VERTEX ; i ++ )
		{
			m_pVertexArray[i].SetColor( color );
		}
		m_pVertexBuffer->FlushVertexBuffer( COUNT_OF_VERTEX, m_pVertexArray );
	}
	//------------------------------------------------------------------------------------------------------
	void Rect2D::GetVertexBuffer(SharedBuffer &input) const
	{
		const static ushort Indies[COUNT_OF_INDIES]={0,1, 1,2, 2,3, 3,0};
		input.AddVertex(m_pVertexArray, Indies );
	}
	//------------------------------------------------------------------------------------------------------
	void Rect2D::GetRect(vector2f& leftTop, vector2f& rightBottom) const
	{
		vector4f lt = m_pVertexArray[0].Position();
		leftTop.m_x = lt.m_x; 
		leftTop.m_y = lt.m_y;

		vector4f rb = m_pVertexArray[2].Position();
		rightBottom.m_x = rb.m_x; 
		rightBottom.m_y = rb.m_y;
	}
	//------------------------------------------------------------------------------------------------------
	void Rect2D::Render(Engine& engine) const 
	{
		ScreenShapes& ss = engine.GetShaderManage()->GetShader<ScreenShapes>(ShaderManage::ScreenShapes);
		engine.GetDevice()->Draw( *m_pRendBuffer, ss, NULL );
	}
	//------------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------------








	//------------------------------------------------------------------------------------------------------
	Rect3D::Rect3D(const vector3f& ulb, const vector3f& drf, const Color& color)
	{
		m_pVertexArray = NEW VertexColor[COUNT_OF_VERTEX];
		m_pVertexArray[0] = VertexColor( vector4f(ulb.m_x, ulb.m_y, drf.m_z), color ) ;
		m_pVertexArray[1] = VertexColor( vector4f(drf.m_x, ulb.m_y, drf.m_z), color ) ;
		m_pVertexArray[2] = VertexColor( vector4f(drf.m_x, drf.m_y, drf.m_z), color ) ;
		m_pVertexArray[3] = VertexColor( vector4f(ulb.m_x, drf.m_y, drf.m_z), color ) ;
		m_pVertexArray[4] = VertexColor( vector4f(ulb.m_x, ulb.m_y, ulb.m_z), color ) ;
		m_pVertexArray[5] = VertexColor( vector4f(drf.m_x, ulb.m_y, ulb.m_z), color ) ;
		m_pVertexArray[6] = VertexColor( vector4f(drf.m_x, drf.m_y, ulb.m_z), color ) ;
		m_pVertexArray[7] = VertexColor( vector4f(ulb.m_x, drf.m_y, ulb.m_z), color ) ;

		static ushort Indies[COUNT_OF_INDIES]={	0,1, 1,2, 2,3, 3,0,
			0,4, 1,5, 2,6, 3,7,
			4,5, 5,6, 6,7, 7,4};

		m_pRendBuffer = NEW RendBuffer( Device::RM_LINES );
		m_pVertexBuffer = NEW VertexBuffer( Device::MU_DYNAMIC );
		m_pIndicesBuffer = NEW IndicesBuffer( Device::MU_STATIC );
		m_pRendBuffer->SetVertexBuffer( m_pVertexBuffer );
		m_pRendBuffer->SetIndicesBuffer( m_pIndicesBuffer );
		m_pVertexBuffer->FlushVertexBuffer( COUNT_OF_VERTEX, m_pVertexArray );
		m_pIndicesBuffer->FlushIndiesBuffer( COUNT_OF_INDIES, &Indies[0] );
	}
	//------------------------------------------------------------------------------------------------------
	Rect3D::~Rect3D()
	{
		SAFE_DELETE_ARRAY(m_pVertexArray);
		SAFE_DELETE(m_pRendBuffer);
		SAFE_DELETE(m_pVertexBuffer);
		SAFE_DELETE(m_pIndicesBuffer);
	}
	//------------------------------------------------------------------------------------------------------
	void Rect3D::GetVertexBuffer(SharedBuffer &input) const
	{
		const static ushort Indies[COUNT_OF_INDIES]={	0,1, 1,2, 2,3, 3,0,
			0,4, 1,5, 2,6, 3,7,
			4,5, 5,6, 6,7, 7,4};
		input.AddVertex(m_pVertexArray, Indies );
	}
	//------------------------------------------------------------------------------------------------------
	void Rect3D::Render(Engine& engine) const 
	{
		WorldShapes& ss = engine.GetShaderManage()->GetShader<WorldShapes>(ShaderManage::WorldShapes);
		ss.SetParamTransform( &m_Transform );
		engine.GetDevice()->Draw( *m_pRendBuffer, ss, NULL );
	}
	//------------------------------------------------------------------------------------------------------
	void Rect3D::ResetRect3D( const vector3f& ulb, const vector3f& drf )
	{
		m_pVertexArray[0].Position() = vector4f(ulb.m_x, ulb.m_y, drf.m_z);
		m_pVertexArray[1].Position() = vector4f(drf.m_x, ulb.m_y, drf.m_z);
		m_pVertexArray[2].Position() = vector4f(drf.m_x, drf.m_y, drf.m_z);
		m_pVertexArray[3].Position() = vector4f(ulb.m_x, drf.m_y, drf.m_z);
		m_pVertexArray[4].Position() = vector4f(ulb.m_x, ulb.m_y, ulb.m_z);
		m_pVertexArray[5].Position() = vector4f(drf.m_x, ulb.m_y, ulb.m_z);
		m_pVertexArray[6].Position() = vector4f(drf.m_x, drf.m_y, ulb.m_z);
		m_pVertexArray[7].Position() = vector4f(ulb.m_x, drf.m_y, ulb.m_z);
		m_pVertexBuffer->FlushVertexBuffer( COUNT_OF_VERTEX, m_pVertexArray );
	}
	//------------------------------------------------------------------------------------------------------
	void Rect3D::SetTransform( const Matrix44f& trf )
	{
		m_Transform = trf;
	}





	//------------------------------------------------------------------------------------------------------
	LineNode::LineNode( const vector4f& b, const vector4f& e, const Color& c )
	{
		m_BindBox.Contains( vector3f(b) );
		m_BindBox.Contains( vector3f(e) );
		m_pVertexArray = NEW VertexColor[COUNT_OF_VERTEX];
		m_pVertexArray[0].SetVertex( b );
		m_pVertexArray[0].SetColor( c );
		m_pVertexArray[1].SetVertex( e );
		m_pVertexArray[1].SetColor( c );
		static ushort Indies[COUNT_OF_INDIES]={0,1};
		m_pRendBuffer = NEW RendBuffer( Device::RM_LINE_STRIP );
		m_pVertexBuffer = NEW VertexBuffer( Device::MU_DYNAMIC );
		m_pIndicesBuffer = NEW IndicesBuffer( Device::MU_STATIC );
		m_pRendBuffer->SetVertexBuffer( m_pVertexBuffer );
		m_pRendBuffer->SetIndicesBuffer( m_pIndicesBuffer );
		m_pVertexBuffer->FlushVertexBuffer( COUNT_OF_VERTEX, m_pVertexArray );
		m_pIndicesBuffer->FlushIndiesBuffer( COUNT_OF_INDIES, &Indies[0] );
	}
	//------------------------------------------------------------------------------------------------------
	LineNode::~LineNode()
	{
		SAFE_DELETE_ARRAY(m_pVertexArray);
		SAFE_DELETE(m_pRendBuffer);
		SAFE_DELETE(m_pVertexBuffer);
		SAFE_DELETE(m_pIndicesBuffer);
	}
	//------------------------------------------------------------------------------------------------------
	void LineNode::Render( Engine& engine )
	{
		WorldShapes& ss = engine.GetShaderManage()->GetShader<WorldShapes>(ShaderManage::WorldShapes);
		ss.SetParamTransform( &m_Transfrom );
		engine.GetDevice()->Draw( *m_pRendBuffer, ss, NULL );
	}
	//------------------------------------------------------------------------------------------------------
	void LineNode::SetLine( const vector4f& b, const vector4f& e )
	{
		m_BindBox.Reset();
		m_BindBox.Contains( vector3f(b) );
		m_BindBox.Contains( vector3f(e) );
		m_pVertexArray[0].SetVertex( b );
		m_pVertexArray[1].SetVertex( e );
		m_pVertexBuffer->FlushVertexBuffer( COUNT_OF_VERTEX, m_pVertexArray );
	}

	//------------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------------

}