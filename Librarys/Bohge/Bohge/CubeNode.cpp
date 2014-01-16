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



/////////////////////////////////////////
//			State Manage class		   //
/////////////////////////////////////////

#include "Predefine.h"
#include "CubeNode.h"
#include "Vertex.h"
#include "3DMath.h"
#include "Vertex.h"
#include "RendBuffer.h"


namespace BohgeEngine
{
	//--------------------------------------------------------------------------------------------------------------------------------------
	CubeNode::CubeNode()
	{
		m_pVertexArray = NEW VertexTextureNormal[COUNT_OF_VERTEX];
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	CubeNode::CubeNode( const CubeNode& in)
	{
		m_pVertexArray = NEW VertexTextureNormal[COUNT_OF_VERTEX];
		for ( int i = 0; i < COUNT_OF_VERTEX; i ++ )
		{
			m_pVertexArray[i] = in.m_pVertexArray[i];
		}
		m_vPosition = in.m_vPosition;
		m_vSize = in.m_vSize;
		m_BeginCoord = in.m_BeginCoord;
		m_EndCoord = in.m_EndCoord;
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	CubeNode::CubeNode (const vector3f& size, const vector2f& beginCoord, const vector2f& endCoord)
		:m_vSize(size),
		m_BeginCoord(beginCoord),
		m_EndCoord(endCoord)
	{
		vector3f halfSize = size / 2;

		m_pVertexArray = NEW VertexTextureNormal[COUNT_OF_VERTEX];
		m_pVertexArray[0] = VertexTextureNormal( vector4f(-halfSize.m_x,+halfSize.m_y,+halfSize.m_z), vector2f( beginCoord.m_u, endCoord.m_v ), vector3f(0,0,1) );
		m_pVertexArray[1] = VertexTextureNormal( vector4f(-halfSize.m_x,-halfSize.m_y,+halfSize.m_z), beginCoord, vector3f(0,0,1) );
		m_pVertexArray[2] = VertexTextureNormal( vector4f(+halfSize.m_x,+halfSize.m_y,+halfSize.m_z), endCoord, vector3f(0,0,1) );
		m_pVertexArray[3] = VertexTextureNormal( vector4f(+halfSize.m_x,-halfSize.m_y,+halfSize.m_z), vector2f( endCoord.m_u, beginCoord.m_v ), vector3f(0,0,1) );

		m_pVertexArray[4] = VertexTextureNormal( vector4f(+halfSize.m_x,+halfSize.m_y,+halfSize.m_z), vector2f( beginCoord.m_u, endCoord.m_v ), vector3f(1,0,0) );
		m_pVertexArray[5] = VertexTextureNormal( vector4f(+halfSize.m_x,-halfSize.m_y,+halfSize.m_z), beginCoord, vector3f(1,0,0) );
		m_pVertexArray[6] = VertexTextureNormal( vector4f(+halfSize.m_x,+halfSize.m_y,-halfSize.m_z), endCoord, vector3f(1,0,0) );
		m_pVertexArray[7] = VertexTextureNormal( vector4f(+halfSize.m_x,-halfSize.m_y,-halfSize.m_z), vector2f( endCoord.m_u, beginCoord.m_v ), vector3f(1,0,0) );

		m_pVertexArray[8] = VertexTextureNormal( vector4f(+halfSize.m_x,+halfSize.m_y,-halfSize.m_z), vector2f( beginCoord.m_u, endCoord.m_v ), vector3f(0,0,-1) );
		m_pVertexArray[9] = VertexTextureNormal( vector4f(+halfSize.m_x,-halfSize.m_y,-halfSize.m_z), beginCoord, vector3f(0,0,-1) );
		m_pVertexArray[10] = VertexTextureNormal( vector4f(-halfSize.m_x,+halfSize.m_y,-halfSize.m_z), endCoord, vector3f(0,0,-1) );
		m_pVertexArray[11] = VertexTextureNormal( vector4f(-halfSize.m_x,-halfSize.m_y,-halfSize.m_z),  vector2f( endCoord.m_u, beginCoord.m_v ), vector3f(0,0,-1) );

		m_pVertexArray[12] = VertexTextureNormal( vector4f(-halfSize.m_x,+halfSize.m_y,-halfSize.m_z), vector2f( beginCoord.m_u, endCoord.m_v ), vector3f(-1,0,0) );
		m_pVertexArray[13] = VertexTextureNormal( vector4f(-halfSize.m_x,-halfSize.m_y,-halfSize.m_z), beginCoord, vector3f(-1,0,0) );
		m_pVertexArray[14] = VertexTextureNormal( vector4f(-halfSize.m_x,+halfSize.m_y,+halfSize.m_z), endCoord, vector3f(-1,0,0) );
		m_pVertexArray[15] = VertexTextureNormal( vector4f(-halfSize.m_x,-halfSize.m_y,+halfSize.m_z), vector2f( endCoord.m_u, beginCoord.m_v ), vector3f(-1,0,0) );

		m_pVertexArray[16] = VertexTextureNormal( vector4f(-halfSize.m_x,+halfSize.m_y,-halfSize.m_z),vector2f( beginCoord.m_u, endCoord.m_v ), vector3f(0,1,0) );
		m_pVertexArray[17] = VertexTextureNormal( vector4f(-halfSize.m_x,+halfSize.m_y,+halfSize.m_z),beginCoord, vector3f(0,1,0) );
		m_pVertexArray[18] = VertexTextureNormal( vector4f(+halfSize.m_x,+halfSize.m_y,-halfSize.m_z),endCoord, vector3f(0,1,0) );
		m_pVertexArray[19] = VertexTextureNormal( vector4f(+halfSize.m_x,+halfSize.m_y,+halfSize.m_z), vector2f( endCoord.m_u, beginCoord.m_v ), vector3f(0,1,0) );

		m_pVertexArray[20] = VertexTextureNormal( vector4f(-halfSize.m_x,-halfSize.m_y,+halfSize.m_z),vector2f( beginCoord.m_u, endCoord.m_v ), vector3f(0,-1,0) );
		m_pVertexArray[21] = VertexTextureNormal( vector4f(-halfSize.m_x,-halfSize.m_y,-halfSize.m_z),beginCoord, vector3f(0,-1,0) );
		m_pVertexArray[22] = VertexTextureNormal( vector4f(+halfSize.m_x,-halfSize.m_y,+halfSize.m_z),endCoord, vector3f(0,-1,0) );
		m_pVertexArray[23] = VertexTextureNormal( vector4f(+halfSize.m_x,-halfSize.m_y,-halfSize.m_z),vector2f( endCoord.m_u, beginCoord.m_v ), vector3f(0,-1,0) );

	}
	//-------------------------------------------------------------------
	CubeNode::CubeNode(const vector3f& pos, const vector3f& size, const vector2f& beginCoord, const vector2f& endCoord)
		:m_vSize(size),
		m_vPosition(pos),
		m_BeginCoord(beginCoord),
		m_EndCoord(endCoord)
	{
		vector3f halfSize = size / 2;
		m_pVertexArray = NEW VertexTextureNormal[COUNT_OF_VERTEX];
		m_pVertexArray[0] = VertexTextureNormal( vector4f(pos.m_x-halfSize.m_x,pos.m_y+halfSize.m_y,pos.m_z+halfSize.m_z), vector2f( beginCoord.m_u, endCoord.m_v ), vector3f(0,0,1) );
		m_pVertexArray[1] = VertexTextureNormal( vector4f(pos.m_x-halfSize.m_x,pos.m_y-halfSize.m_y,pos.m_z+halfSize.m_z), beginCoord, vector3f(0,0,1) );
		m_pVertexArray[2] = VertexTextureNormal( vector4f(pos.m_x+halfSize.m_x,pos.m_y+halfSize.m_y,pos.m_z+halfSize.m_z), endCoord, vector3f(0,0,1) );
		m_pVertexArray[3] = VertexTextureNormal( vector4f(pos.m_x+halfSize.m_x,pos.m_y-halfSize.m_y,pos.m_z+halfSize.m_z), vector2f( endCoord.m_u, beginCoord.m_v ), vector3f(0,0,1) );

		m_pVertexArray[4] = VertexTextureNormal( vector4f(pos.m_x+halfSize.m_x,pos.m_y+halfSize.m_y,pos.m_z+halfSize.m_z), vector2f( beginCoord.m_u, endCoord.m_v ), vector3f(1,0,0) );
		m_pVertexArray[5] = VertexTextureNormal( vector4f(pos.m_x+halfSize.m_x,pos.m_y-halfSize.m_y,pos.m_z+halfSize.m_z), beginCoord, vector3f(1,0,0) );
		m_pVertexArray[6] = VertexTextureNormal( vector4f(pos.m_x+halfSize.m_x,pos.m_y+halfSize.m_y,pos.m_z-halfSize.m_z), endCoord, vector3f(1,0,0) );
		m_pVertexArray[7] = VertexTextureNormal( vector4f(pos.m_x+halfSize.m_x,pos.m_y-halfSize.m_y,pos.m_z-halfSize.m_z), vector2f( endCoord.m_u, beginCoord.m_v ), vector3f(1,0,0) );

		m_pVertexArray[8] = VertexTextureNormal( vector4f(pos.m_x+halfSize.m_x,pos.m_y+halfSize.m_y,pos.m_z-halfSize.m_z), vector2f( beginCoord.m_u, endCoord.m_v ), vector3f(0,0,-1) );
		m_pVertexArray[9] = VertexTextureNormal( vector4f(pos.m_x+halfSize.m_x,pos.m_y-halfSize.m_y,pos.m_z-halfSize.m_z), beginCoord, vector3f(0,0,-1) );
		m_pVertexArray[10] = VertexTextureNormal( vector4f(pos.m_x-halfSize.m_x,pos.m_y+halfSize.m_y,pos.m_z-halfSize.m_z), endCoord, vector3f(0,0,-1) );
		m_pVertexArray[11] = VertexTextureNormal( vector4f(pos.m_x-halfSize.m_x,pos.m_y-halfSize.m_y,pos.m_z-halfSize.m_z),  vector2f( endCoord.m_u, beginCoord.m_v ), vector3f(0,0,-1) );

		m_pVertexArray[12] = VertexTextureNormal( vector4f(pos.m_x-halfSize.m_x,pos.m_y+halfSize.m_y,pos.m_z-halfSize.m_z), vector2f( beginCoord.m_u, endCoord.m_v ), vector3f(-1,0,0) );
		m_pVertexArray[13] = VertexTextureNormal( vector4f(pos.m_x-halfSize.m_x,pos.m_y-halfSize.m_y,pos.m_z-halfSize.m_z), beginCoord, vector3f(-1,0,0) );
		m_pVertexArray[14] = VertexTextureNormal( vector4f(pos.m_x-halfSize.m_x,pos.m_y+halfSize.m_y,pos.m_z+halfSize.m_z), endCoord, vector3f(-1,0,0) );
		m_pVertexArray[15] = VertexTextureNormal( vector4f(pos.m_x-halfSize.m_x,pos.m_y-halfSize.m_y,pos.m_z+halfSize.m_z), vector2f( endCoord.m_u, beginCoord.m_v ), vector3f(-1,0,0) );

		m_pVertexArray[16] = VertexTextureNormal( vector4f(pos.m_x-halfSize.m_x,pos.m_y+halfSize.m_y,pos.m_z-halfSize.m_z),vector2f( beginCoord.m_u, endCoord.m_v ), vector3f(0,1,0) );
		m_pVertexArray[17] = VertexTextureNormal( vector4f(pos.m_x-halfSize.m_x,pos.m_y+halfSize.m_y,pos.m_z+halfSize.m_z),beginCoord, vector3f(0,1,0) );
		m_pVertexArray[18] = VertexTextureNormal( vector4f(pos.m_x+halfSize.m_x,pos.m_y+halfSize.m_y,pos.m_z-halfSize.m_z),endCoord, vector3f(0,1,0) );
		m_pVertexArray[19] = VertexTextureNormal( vector4f(pos.m_x+halfSize.m_x,pos.m_y+halfSize.m_y,pos.m_z+halfSize.m_z), vector2f( endCoord.m_u, beginCoord.m_v ), vector3f(0,1,0) );

		m_pVertexArray[20] = VertexTextureNormal( vector4f(pos.m_x-halfSize.m_x,pos.m_y-halfSize.m_y,pos.m_z+halfSize.m_z),vector2f( beginCoord.m_u, endCoord.m_v ), vector3f(0,-1,0) );
		m_pVertexArray[21] = VertexTextureNormal( vector4f(pos.m_x-halfSize.m_x,pos.m_y-halfSize.m_y,pos.m_z-halfSize.m_z),beginCoord, vector3f(0,-1,0) );
		m_pVertexArray[22] = VertexTextureNormal( vector4f(pos.m_x+halfSize.m_x,pos.m_y-halfSize.m_y,pos.m_z+halfSize.m_z),endCoord, vector3f(0,-1,0) );
		m_pVertexArray[23] = VertexTextureNormal( vector4f(pos.m_x+halfSize.m_x,pos.m_y-halfSize.m_y,pos.m_z-halfSize.m_z),vector2f( endCoord.m_u, beginCoord.m_v ), vector3f(0,-1,0) );
	}
	//-------------------------------------------------------------------
	void CubeNode::SetPostion(const vector3f& pos, const vector3f& size)
	{
		m_vPosition = pos;
		m_vSize = size;
		vector3f halfSize = size / 2;
		m_pVertexArray[0].SetVertex( vector4f(pos.m_x-halfSize.m_x,pos.m_y+halfSize.m_y,pos.m_z+halfSize.m_z) );
		m_pVertexArray[1].SetVertex( vector4f(pos.m_x-halfSize.m_x,pos.m_y-halfSize.m_y,pos.m_z+halfSize.m_z) );
		m_pVertexArray[2].SetVertex( vector4f(pos.m_x+halfSize.m_x,pos.m_y+halfSize.m_y,pos.m_z+halfSize.m_z) );
		m_pVertexArray[3].SetVertex( vector4f(pos.m_x+halfSize.m_x,pos.m_y-halfSize.m_y,pos.m_z+halfSize.m_z) );

		m_pVertexArray[4].SetVertex( vector4f(pos.m_x+halfSize.m_x,pos.m_y+halfSize.m_y,pos.m_z+halfSize.m_z) );
		m_pVertexArray[5].SetVertex( vector4f(pos.m_x+halfSize.m_x,pos.m_y-halfSize.m_y,pos.m_z+halfSize.m_z) );
		m_pVertexArray[6].SetVertex( vector4f(pos.m_x+halfSize.m_x,pos.m_y+halfSize.m_y,pos.m_z-halfSize.m_z) );
		m_pVertexArray[7].SetVertex( vector4f(pos.m_x+halfSize.m_x,pos.m_y-halfSize.m_y,pos.m_z-halfSize.m_z) );

		m_pVertexArray[8].SetVertex( vector4f(pos.m_x+halfSize.m_x,pos.m_y+halfSize.m_y,pos.m_z-halfSize.m_z) );
		m_pVertexArray[9].SetVertex( vector4f(pos.m_x+halfSize.m_x,pos.m_y-halfSize.m_y,pos.m_z-halfSize.m_z) );
		m_pVertexArray[10].SetVertex( vector4f(pos.m_x-halfSize.m_x,pos.m_y+halfSize.m_y,pos.m_z-halfSize.m_z) );
		m_pVertexArray[11].SetVertex( vector4f(pos.m_x-halfSize.m_x,pos.m_y-halfSize.m_y,pos.m_z-halfSize.m_z) );

		m_pVertexArray[12].SetVertex( vector4f(pos.m_x-halfSize.m_x,pos.m_y+halfSize.m_y,pos.m_z-halfSize.m_z) );
		m_pVertexArray[13].SetVertex( vector4f(pos.m_x-halfSize.m_x,pos.m_y-halfSize.m_y,pos.m_z-halfSize.m_z) );
		m_pVertexArray[14].SetVertex( vector4f(pos.m_x-halfSize.m_x,pos.m_y+halfSize.m_y,pos.m_z+halfSize.m_z) );
		m_pVertexArray[15].SetVertex( vector4f(pos.m_x-halfSize.m_x,pos.m_y-halfSize.m_y,pos.m_z+halfSize.m_z) );

		m_pVertexArray[16].SetVertex( vector4f(pos.m_x-halfSize.m_x,pos.m_y+halfSize.m_y,pos.m_z-halfSize.m_z) );
		m_pVertexArray[17].SetVertex( vector4f(pos.m_x-halfSize.m_x,pos.m_y+halfSize.m_y,pos.m_z+halfSize.m_z) );
		m_pVertexArray[18].SetVertex( vector4f(pos.m_x+halfSize.m_x,pos.m_y+halfSize.m_y,pos.m_z-halfSize.m_z) );
		m_pVertexArray[19].SetVertex( vector4f(pos.m_x+halfSize.m_x,pos.m_y+halfSize.m_y,pos.m_z+halfSize.m_z) );

		m_pVertexArray[20].SetVertex( vector4f(pos.m_x-halfSize.m_x,pos.m_y-halfSize.m_y,pos.m_z+halfSize.m_z) );
		m_pVertexArray[21].SetVertex( vector4f(pos.m_x-halfSize.m_x,pos.m_y-halfSize.m_y,pos.m_z-halfSize.m_z) );
		m_pVertexArray[22].SetVertex( vector4f(pos.m_x+halfSize.m_x,pos.m_y-halfSize.m_y,pos.m_z+halfSize.m_z) );
		m_pVertexArray[23].SetVertex( vector4f(pos.m_x+halfSize.m_x,pos.m_y-halfSize.m_y,pos.m_z-halfSize.m_z) );
	}
	//-------------------------------------------------------------------
	CubeNode::~CubeNode ()
	{
		SAFE_DELETE_ARRAY(m_pVertexArray);
	}
	//-------------------------------------------------------------------
	CubeNode& CubeNode::operator = (const CubeNode& in)
	{
		for ( int i = 0; i < COUNT_OF_VERTEX; i ++ )
		{
			m_pVertexArray[i] = in.m_pVertexArray[i];
		}
		m_vPosition = in.m_vPosition;
		m_vSize = in.m_vSize;
		m_BeginCoord = in.m_BeginCoord;
		m_EndCoord = in.m_EndCoord;
		return *this;
	}
	//-------------------------------------------------------------------
	void CubeNode::CopyCoordinate (const CubeNode& input)
	{
		m_BeginCoord = input.m_BeginCoord;
		m_EndCoord = input.m_EndCoord;
		for(int i = 0 ; i < COUNT_OF_VERTEX ; i ++)
		{
			m_pVertexArray[i].Coordinate() = input.m_pVertexArray[i].Coordinate();
		}
	}
	//-------------------------------------------------------------------
	void CubeNode::GetVertexBuffer(SharedBuffer& input) const
	{
		const static ushort Indies[COUNT_OF_INDIES] = {0,1,2,   2,1,3,
			4,5,6,   6,5,7,
			8,9,10,  10,9,11,
			12,13,14,  14,13,15,
			16,17,18,  18,17,19,
			20,21,22, 22,21,23};//全局唯一的静态索引，同时降低一定的内存开销
		input.AddVertex( m_pVertexArray, &Indies[0]);
	}
	//-------------------------------------------------------------------
	void CubeNode::GetMashEdge(SharedBuffer& input)
	{
		//加入盖子顶点和他位置相同的四个周边顶点
		VertexTextureNormal pVA[COUNT_OF_EDGE_VERTEX] =
		{
			m_pVertexArray[16],
			m_pVertexArray[17],
			m_pVertexArray[18],
			m_pVertexArray[19],

			m_pVertexArray[0],
			m_pVertexArray[2],
			m_pVertexArray[8],
			m_pVertexArray[10]
		};

		//索引要对应以上顶点的添加顺序
		const static ushort Indies[COUNT_OF_EDGE_INDIES] = {1,4,3,   3,4,5,
			3,5,2,   2,5,6,
			2,6,0,   0,6,7,	
			0,7,1,   1,7,4,
			0,1,2,   2,1,3
		};

		input.AddVertex( pVA, Indies);

		//const static uint Indies[COUNT_OF_EDGE_INDIES] = {
		//	17,0,19,	19,0,2,
		//	19,4,18,	18,4,6,
		//	18,8,16,	16,8,10,
		//	16,12,17,	17,12,14,

		//	//3,5,2,		2,5,4,
		//	//6,4,7,		7,4,8,
		//	//11,13,10,	10,13,12,
		//	//15,1,14,	14,1,0,

		//	//1,20,3,		3,20,22,
		//	//5,22,6,		6,22,23,
		//	//4,23,11,	11,23,21,
		//	//13,21,15,	15,21,20,

		//	//0,1,2,   2,1,3,
		//	//4,5,6,   6,5,7,
		//	//8,9,10,  10,9,11,
		//	//12,13,14,  14,13,15,
		//	//16,17,18,  18,17,19,
		//	//20,21,22, 22,21,23
		//};
		//input.AddVertex(m_pVertexArray, &Indies[0]);
	}
	//-------------------------------------------------------------------
	Vertex& CubeNode::GetVertex( int index )
	{
		ASSERT( index < COUNT_OF_VERTEX );
		return m_pVertexArray[index];
	}
	//-------------------------------------------------------------------
	uint CubeNode::GetElementCountPerVertex()
	{
		return  VertexTextureNormal::GetElementCount();
	}
	//-------------------------------------------------------------------
	//-------------------------------------------------------------------
}