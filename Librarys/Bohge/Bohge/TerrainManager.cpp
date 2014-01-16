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
//		世界管理器	   //
/////////////////////////
#include "TerrainManager.h"
#include "Camera.h"
#include "Frustum.hpp"
#include "RendBuffer.h"
#include "TerrainChunk.h"



using namespace std;

namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	TerrainManager::TerrainManager( const TerrainProperty& ppy  )
		:m_Property( ppy )
	{
	}
	//-------------------------------------------------------------------------------------------------------
	TerrainManager::~TerrainManager()
	{
		this->Unload();
	}
	//-------------------------------------------------------------------------------------------------------
	void TerrainManager::Load( const string& path )
	{
		this->Unload();
		string LocalPath = path + TerrainFolder;
		string texPath = LocalPath + TerrainTextureFolder;
		Serializer loader( LocalPath + TerrainInfoList );
		bool isContinue = true;
		while( isContinue )
		{
			int index;
			loader>>index;
			switch( index )
			{
			case SceneDataIndex::Terrain_Tile: 
				{
					string teerainpath;
					vector2f offset;
					loader>>offset.m_x>>offset.m_y>>teerainpath;
					teerainpath += PLANTFORMDELIMITER;
					TerrainTile* temp = new TerrainTile( m_Property );
					temp->Load( LocalPath + teerainpath, texPath );
					m_TerrainList.push_back( temp );
				}break;
			case SceneDataIndex::Save_Done: isContinue = false; break;
			}
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void TerrainManager::Update( uint milisecon )
	{
		m_TerrainRendMap.clear();
		const Frustumf& frustum = Engine::Instance().GetCamera()->GetFrustum();
		for ( TerrainTiles::iterator it = m_TerrainList.begin();
			it != m_TerrainList.end();
			it ++ )
		{
			if( IntersectedType::IT_NO != frustum.Intersect( (*it)->BindBox() ) )
			{
				(*it)->isInView() = true;
				(*it)->Update( milisecon, m_TerrainRendMap );
			}
			else
			{
				(*it)->isInView() = false;
			}
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void TerrainManager::Render( Engine& engine )
	{
		for ( TerrainChunkMap::iterator it = m_TerrainRendMap.begin();
			it != m_TerrainRendMap.end();
			it ++ )
		{
			it->second->Render( engine );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void TerrainManager::Unload()
	{
		for ( TerrainTiles::iterator it = m_TerrainList.begin();
			it != m_TerrainList.end();
			it ++ )
		{
			SAFE_DELETE( *it );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	float TerrainManager::GetHeight( float x, float y ) const
	{
		float depth = -100000000;
		for ( TerrainTiles::const_iterator it = m_TerrainList.begin();
			it != m_TerrainList.end();
			it ++ )
		{
			if( (*it)->GetHeight( x, y, depth ) )
			{
				break;
			}
		}
		return depth;
	}
	//-------------------------------------------------------------------------------------------------------
	void TerrainManager::AssignMaterial( const bool& isCast, const ICamera& camera )
	{
		for ( TerrainTiles::const_iterator it = m_TerrainList.begin();
			it != m_TerrainList.end();
			it ++ )
		{
			(*it)->AssignMaterial( isCast, camera );
		}
	}

}