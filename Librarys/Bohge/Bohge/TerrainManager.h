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

#pragma once
#include "3DMath.h"
#include "SecenProperty.h"
#include "IPostEffect.h"
#include "TerrainTile.h"
#include <list>
#include <map>
#include <string>


namespace BohgeEngine
{
	class ICamera;
	class TerrainChunk;
	class TerrainManager
	{
	private:
		typedef std::multimap< float, TerrainChunk* > TerrainChunkMap;//用于排序绘制的map
		typedef std::list< TerrainTile* >	TerrainTiles;//网格地形链表
	private:
		TerrainProperty				m_Property;//地形属性
		TerrainTiles				m_TerrainList;//地形链表
		TerrainChunkMap				m_TerrainRendMap;
	public:
		TerrainManager( const TerrainProperty& ppy );
		~TerrainManager();
		void Update( uint milisecon );
		void Render(Engine& engine);
	public:
		void Load( const std::string& path );
		void Unload();
		float GetHeight( float x, float y) const;
		void AssignMaterial( const bool& isCast, const ICamera& camera );//通过阴影摄像机确定渲染每一块的材质
	public:
		BOHGE_FORCEINLINE float GetHeightForWater( float x, float y, float maxdepth) const
		{
			float depth = - maxdepth;
			for ( TerrainTiles::const_iterator it = m_TerrainList.begin();
				it != m_TerrainList.end();
				it ++ )
			{
				if( (*it)->GetHeightForWater( x, y, depth ) )
				{
					break;
				}
			}
			return depth;
		}
	};
}