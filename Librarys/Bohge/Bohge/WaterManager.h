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
//			水面统一管理		   //
/////////////////////////////////////
#pragma once
#include <string>
#include <list>


#define COPYSCREEN


class TerrainEdit;
namespace BohgeEngine
{
	class Engine;
	class WaterGrid;
	class WaterTile;
	class RenderTarget;
	class TerrainManager;
	class WaterManager
	{
	private:
		typedef std::list< WaterTile* >		WaterMeshes;//网格水面链表
	private:
#ifndef COPYSCREEN
		RenderTarget*			m_Refraction;//折射与深度
#endif
		int						m_LastTileIndex;
		bool					m_isRender;//是否能看到，是否需要绘制
		WaterMeshes				m_WaterDatas;//全部的水体信息
		WaterGrid*				m_pWater;//投射网格水体
	public:
		WaterManager(void);
		~WaterManager(void);
	public:
		void BeginUpdate( Engine& engine );
		void EndUpdate( Engine& engine );
		void RenderNormal( Engine& engine );//绘制法线
#ifdef COPYSCREEN
		void Render(Engine& engine);
#else
		void Render( Engine& engine, TerrainManager* world );
#endif
#ifdef TERRAINMANAGER
		void Load( const std::string& path, const TerrainManager& terrain );
#endif
#ifdef TERRAINEDIT
		void Load( const std::string& path, const TerrainEdit& terrain );
#endif
		void Unload();
		float GetHeight( float x, float y) const;
	};
}