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
//		Ö²±»Çø¿é	   //
/////////////////////////
#include "PlantTile.h"
#include "Material.h"
#include "RendBuffer.h"
#include "PlantChunk.h"
#include "Serializer.h"
#include "Camera.h"
#include "SceneDataIndex.h"


using namespace std;



namespace BohgeEngine
{
#define FADE_TIME 800


	//-------------------------------------------------------------------------------------------------------
	PlantTile::PlantTile(void)
		:m_isInView(false),
		m_QTNode(NULL)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	PlantTile::~PlantTile(void)
	{
		SAFE_DELETE( m_QTNode );
	}
	//-------------------------------------------------------------------------------------------------------
	void PlantTile::Load( const std::string& path, const std::string& TexPath )
	{
		Serializer loader( path + PlantMeshName );

		int BufferIndex = 0;
		bool isContinue = true;
		while( isContinue )
		{
			int index;
			loader>>index;
			switch( index )
			{
			case SceneDataIndex::Plant_Offset: 
				{
					loader>>m_Offset.m_x>>m_Offset.m_y;
				}break;
			case SceneDataIndex::Plant_Quda_Struct:
				{
					m_QTNode = NEW PlantChunk( 0 );
					m_QTNode->LoadStruct( loader );
				}break;
			case SceneDataIndex::Plant_BindBox: 
				{
					vector3f bmin,bmax;
					loader>>bmin>>bmax;
					m_BindBox = aabbox3df( bmin, bmax );
					m_QTNode->LoadBindBox( loader );
				}break;
			case SceneDataIndex::Plant_Quda_Buffer:
				{
					m_QTNode->LoadBuffer( loader, TexPath );
				}break;
			case SceneDataIndex::Save_Done:
				{
					isContinue = false;
				}break;
			}
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void PlantTile::Update( uint milisecon, std::multimap< float, PlantChunk* >& plantmap )
	{
		aabspheref sphere( Engine::Instance().GetCamera()->GetPosition(), Engine::Instance().GetCamera()->GetFar() );
		m_QTNode->Update( Engine::Instance().GetCamera()->GetFrustum(), sphere, static_cast<float>( milisecon ) / FADE_TIME, plantmap );
	}

}