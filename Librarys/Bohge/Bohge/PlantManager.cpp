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
//		植被管理	   //
/////////////////////////
#include "SceneDataIndex.h"
#include "PlantManager.h"
#include "PlantTile.h"
#include "PlantChunk.h"
#include "Frustum.hpp"
#include "Camera.h"

using namespace std;



namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	PlantManager::PlantManager(const PlantProperty& ppy)
		:m_Property( ppy )
	{
	}
	//-------------------------------------------------------------------------------------------------------
	PlantManager::~PlantManager(void)
	{
		this->Unload();
	}
	//-------------------------------------------------------------------------------------------------------
	void PlantManager::Load( const std::string& path )
	{
		this->Unload();
		string PlantPath = path + PlantFolder;
		string PlantTexturePath = PlantPath + PlantTextureFolder;
		Serializer loader( PlantPath+PlantInfoList );
		bool isContinue = true;
		while( isContinue )
		{
			int index;
			loader>>index;
			switch( index )
			{
			case SceneDataIndex::Plant_Tile: 
				{
					string tilepath;
					vector2d offset;
					loader>>offset>>tilepath;
					tilepath += PLANTFORMDELIMITER;

					PlantTile* temp = new PlantTile( );
					temp->Load( PlantPath + tilepath, PlantTexturePath );
					m_PlantList.push_back( temp );
				}break;
			case  SceneDataIndex::Save_Done: isContinue = false; break;
			}
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void PlantManager::Unload()
	{
		for ( PlantTiles::iterator it = m_PlantList.begin();
			it != m_PlantList.end();
			it ++ )
		{
			SAFE_DELETE( *it );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void PlantManager::Update( uint milisecon )
	{
		m_PlantRendMap.clear();
		const Frustumf& frustum = Engine::Instance().GetCamera()->GetFrustum();
		for ( PlantTiles::iterator it = m_PlantList.begin();
			it != m_PlantList.end();
			it ++ )
		{
			if( IntersectedType::IT_NO != frustum.Intersect( (*it)->BindBox() ) )
			{
				(*it)->isInView() = true;
				(*it)->Update( milisecon, m_PlantRendMap );
			}
			else
			{
				(*it)->isInView() = false;
			}
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void PlantManager::RenderPlant( Engine& engine )
	{
		for ( PlantChunkMap::iterator it = m_PlantRendMap.begin();
			it != m_PlantRendMap.end();
			it ++ )
		{
			it->second->RenderOpaque( engine );
		}

		//他们两个材质不一样，还是分开画
		for ( PlantChunkMap::iterator it = m_PlantRendMap.begin();
			it != m_PlantRendMap.end();
			it ++ )
		{
			it->second->RenderTransparent( engine );
		}
	}

}