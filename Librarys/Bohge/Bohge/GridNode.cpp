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
//			地板网格	   //
/////////////////////////////
#include "GridNode.h"
#include "Vertex.h"
#include "ShaderManage.h"
#include "RendBuffer.h"
#include "Shaders2D.h"
#include <vector>

using namespace std;


namespace BohgeEngine
{
	//------------------------------------------------------------------------------------------------------
	GridNode::GridNode(void)
		:m_pRendBuffer(NULL)
	{
		m_pRendBuffer = NEW RendBuffer( Device::RM_LINES );
		m_pVertexBuffer = NEW VertexBuffer( Device::MU_DYNAMIC );
		m_pIndicesBuffer = NEW IndicesBuffer( Device::MU_DYNAMIC );
		m_pRendBuffer->SetVertexBuffer( m_pVertexBuffer );
		m_pRendBuffer->SetIndicesBuffer( m_pIndicesBuffer );
	}
	//------------------------------------------------------------------------------------------------------
	GridNode::~GridNode(void)
	{
		SAFE_DELETE( m_pRendBuffer );
		SAFE_DELETE( m_pVertexBuffer );
		SAFE_DELETE( m_pIndicesBuffer );
	}
	//------------------------------------------------------------------------------------------------------
	void GridNode::MakeGrid( const vector3f& center, float size, int grids )
	{
		int Lines = (grids+1)*2;
		float addtion = size / grids;
		float begin = - size / 2;
		m_BindBox.Reset();
		m_BindBox.Contains( vector3f( center.m_x + begin, center.m_y, center.m_z + begin ) );
		m_BindBox.Contains( vector3f( center.m_x - begin, center.m_y, center.m_z - begin ) );
		VertexColor* linever = NEW VertexColor[Lines*2];
		for ( int i = 0 ; i < Lines / 2; i ++ )//纵线
		{
			if ( i == Lines / 4 )
			{
				linever[i*2].SetColor( Color(0,0,1,1) );
				linever[i*2+1].SetColor( Color(0,0,1,1) );
			}
			else
			{
				linever[i*2].SetColor( Color::White );
				linever[i*2+1].SetColor( Color::White );
			}
			linever[i*2].SetVertex( vector4f( 
				center.m_x+begin+addtion*i,
				center.m_y,
				center.m_z+begin
				) );
			linever[i*2+1].SetVertex( vector4f( 
				center.m_x+begin+addtion*i,
				center.m_y,
				center.m_z+begin+size
				) );
		}
		for ( int i = Lines / 2, j = 0 ; i < Lines ; i ++, j ++ )//纵线
		{
			if ( i == 3* Lines / 4 )
			{
				linever[i*2].SetColor( Color(1,0,0,1) );
				linever[i*2+1].SetColor( Color(1,0,0,1) );
			}
			else
			{
				linever[i*2].SetColor( Color::White );
				linever[i*2+1].SetColor( Color::White );
			}
			linever[i*2].SetVertex( vector4f( 
				center.m_x+begin,
				center.m_y,
				center.m_z+begin+addtion*j
				) );
			linever[i*2+1].SetVertex( vector4f( 
				center.m_x+begin+size,
				center.m_y,
				center.m_z+begin+addtion*j
				) );
		}
		vector<ushort> Indies;
		for ( int i = 0; i < Lines * 2; i ++ )
		{
			Indies.push_back(i);
		}
		m_pVertexBuffer->FlushVertexBuffer( Lines*2, linever );
		m_pIndicesBuffer->FlushIndiesBuffer( Indies.size(), &Indies[0] );
		SAFE_DELETE_ARRAY(linever);
	}
	//------------------------------------------------------------------------------------------------------
	bool GridNode::isInview( const Frustumf& cFru )
	{
		//return IntersectedType::IT_NO != cFru.Intersect( m_BindBox * m_Transfrom );
		return IntersectedType::IT_NO != cFru.Intersect( m_BindBox );
	}
	//------------------------------------------------------------------------------------------------------
	void GridNode::Render( Engine& engine )
	{
		WorldShapes& ss = engine.GetShaderManage()->GetShader<WorldShapes>(ShaderManage::WorldShapes);
		ss.SetParamTransform( &m_Transform );
		engine.GetDevice()->Draw( *m_pRendBuffer, ss, NULL );
	}
	//------------------------------------------------------------------------------------------------------
	const void* GridNode::Attribute( const Node_Attribute& name ) const
	{
		switch( name )
		{
		case IAttributeNode::ATT_WORLDTRANSFORM: return &m_Transform;
		}
		return NULL;
	}

}