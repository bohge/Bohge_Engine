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



/////////////////////
//		ตฦนโ	   //
/////////////////////
#include "FakeLights.h"
#include "FilePath.h"
#include "Vertex.h"
#include "RendBuffer.h"
#include "Texture.h"

using namespace std;


namespace BohgeEngine
{

	//-------------------------------------------------------------------------------------------------------
	FakePointLight::FakePointLight()
		:FakeLightNode( LightNode::LT_POINT )
	{
		ushort Indies[COUNT_OF_INDIES]={ 0,1,2,3 };
		m_pRendBuffer = NEW RendBuffer( Device::RM_TRIANGLE_STRIP );
		m_pVertexBuffer = NEW VertexBuffer( Device::MU_DYNAMIC );
		m_pIndicesBuffer = NEW IndicesBuffer( Device::MU_STATIC );
		m_pRendBuffer->SetVertexBuffer( m_pVertexBuffer );
		m_pRendBuffer->SetIndicesBuffer( m_pIndicesBuffer );
		m_pIndicesBuffer->FlushIndiesBuffer( COUNT_OF_INDIES, &Indies[0] );
		if ( !m_pTexture->isLoaded() )
		{
			m_pTexture->LoadTexture( Device::PF_A8, FILEPATH.TextureFolder() + "Smoke.png" );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void FakePointLight::_OnLightChangeRecaculater()
	{
		FakeLightVertex vertex[COUNT_OF_VERTEX];
		vector3f pos = GetWorldPosition();
		vector2f rectsize( GetRange(), GetRange() );
		vertex[0].Coordinate() = vector2f( 0, 0 );
		vertex[1].Coordinate() = vector2f( 0, 1 );
		vertex[2].Coordinate() = vector2f( 1, 0 );
		vertex[3].Coordinate() = vector2f( 1, 1 );
		vertex[0].SetVertex( vector4f( pos.m_x - rectsize.m_x, pos.m_y + rectsize.m_y, pos.m_z, 1.0 ) );
		vertex[1].SetVertex( vector4f( pos.m_x - rectsize.m_x, pos.m_y - rectsize.m_y, pos.m_z, 1.0 ) );
		vertex[2].SetVertex( vector4f( pos.m_x + rectsize.m_x, pos.m_y + rectsize.m_y, pos.m_z, 1.0 ) );
		vertex[3].SetVertex( vector4f( pos.m_x + rectsize.m_x, pos.m_y - rectsize.m_y, pos.m_z, 1.0 ) );
		for ( int i = 0 ; i < COUNT_OF_VERTEX; i ++ )
		{
			vertex[i].Normal() = pos;
		}
		m_pVertexBuffer->FlushVertexBuffer( COUNT_OF_VERTEX, vertex );
		LightNode::_OnLightChangeRecaculater();
	}
	//-------------------------------------------------------------------------------------------------------
}