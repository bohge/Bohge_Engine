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
//		场景属性	   //
/////////////////////////
#pragma once
#include "3DMath.h"
#include "SceneDataIndex.h"
#include "Serializer.h"

namespace BohgeEngine
{
#define TerrainSmooth 0.3f //地形平滑过渡占总段落的百分比

	struct TerrainProperty
	{
		int			m_TileSize;//地形的尺寸
		int			m_SplitLevel;//地形分割的深度，也就是树的深度
		int			m_Scale;//每一个顶点之间的间距

		int			m_TextureSize;//权重和阴影图的尺寸
		int			m_TextureRepeat;//纹理的重复数量
		TerrainProperty()
			:m_TileSize(0),
			m_SplitLevel(0),
			m_Scale(0.0f),
			m_TextureSize(0),
			m_TextureRepeat(0.0f)
		{
		}
		//--------------------------------------------------------------------------------------------------------------------------------------------------------
		void Save( Serializer& ser )
		{
			ser<<SceneDataIndex::Terrain_TileSize<<m_TileSize;
			ser<<SceneDataIndex::Terrain_Split<<m_SplitLevel;
			ser<<SceneDataIndex::Terrain_Scale<<m_Scale;
			ser<<SceneDataIndex::Terrain_TextureSize<<m_TextureSize;
			ser<<SceneDataIndex::Terrain_Repeat<<m_TextureRepeat;
			ser<<SceneDataIndex::Save_Done;
		}
		//--------------------------------------------------------------------------------------------------------------------------------------------------------
		void Load( Serializer& ser )
		{
			// TODO: 在此添加加载代码
			bool isContinue = true;
			while( isContinue )
			{
				int index;
				ser>>index;
				switch( index )
				{
				case SceneDataIndex::Terrain_TileSize: ser>>m_TileSize; break;
				case SceneDataIndex::Terrain_Split: ser>>m_SplitLevel; break;
				case SceneDataIndex::Terrain_Scale: ser>>m_Scale; break;
				case SceneDataIndex::Terrain_TextureSize: ser>>m_TextureSize; break;
				case SceneDataIndex::Terrain_Repeat: ser>>m_TextureRepeat; break;
				case SceneDataIndex::Save_Done: isContinue = false;
				}
			}
		}
	};

	struct WaterProperty
	{
		//下列数值都是与地形相同的
		int			m_TileSize;//地形的尺寸
		int			m_Scale;//每一个顶点之间的间距
	};

	struct ObjectProperty
	{
		//下列数值都是与地形相同的
		int			m_TileSize;//地形的尺寸
		int			m_Scale;//每一个顶点之间的间距
	};

	struct PlantProperty
	{
		//下列数值都是与地形相同的
		int			m_TileSize;//地形的尺寸
		int			m_SplitLevel;//地形分割的深度，也就是树的深度
		int			m_Scale;//每一个顶点之间的间距
	};

	struct MainProperty
	{
		vector3f		m_LightPos;
		vector3f		m_LightDir;
		vector3f		m_LightColor;
		vector3f		m_AmbieColor;
		vector3f		m_VolumeLightColor;
		float			m_FogDistance;
		vector3f		m_FogColor;
		vector3f		m_FogParam;
		bool			m_isCameraUnderWater;//摄像机是否可以在水下，涉及一些地面的剔除
		//--------------------------------------------------------------------------------------------------------------------------------------------------------
		MainProperty()
			:m_FogDistance(0.0f),
			m_VolumeLightColor(0.97,0.66,0.38),
			m_isCameraUnderWater(false)
		{

		}
		void Save( Serializer& ser )
		{
			ser<<SceneDataIndex::Main_LightDir<<m_LightDir;
			ser<<SceneDataIndex::Main_LightPos<<m_LightPos;
			ser<<SceneDataIndex::Main_LightClr<<m_LightColor;
			ser<<SceneDataIndex::Main_AmbieClr<<m_AmbieColor;
			ser<<SceneDataIndex::Main_FogDista<<m_FogDistance;
			ser<<SceneDataIndex::Main_FogColor<<m_FogColor;
			ser<<SceneDataIndex::Main_Unwater<<m_isCameraUnderWater;
			ser<<SceneDataIndex::Main_FogParam<<m_FogParam;
			ser<<SceneDataIndex::Save_Done;
		}
		//--------------------------------------------------------------------------------------------------------------------------------------------------------
		void Load( Serializer& ser )
		{
			// TODO: 在此添加加载代码
			bool isContinue = true;
			while( isContinue )
			{
				int index;
				ser>>index;
				switch( index )
				{
				case SceneDataIndex::Main_LightDir: ser>>m_LightDir; break;
				case SceneDataIndex::Main_LightPos: ser>>m_LightPos; break;
				case SceneDataIndex::Main_LightClr: ser>>m_LightColor; break;
				case SceneDataIndex::Main_AmbieClr: ser>>m_AmbieColor; break;
				case SceneDataIndex::Main_FogDista: ser>>m_FogDistance; break;
				case SceneDataIndex::Main_FogColor: ser>>m_FogColor; break;
				case SceneDataIndex::Main_Unwater: ser>>m_isCameraUnderWater; break;
				case SceneDataIndex::Main_FogParam: ser>>m_FogParam; break;
				case SceneDataIndex::Save_Done: isContinue = false;
				}
			}
		}
	};


	struct SecenProperty
	{
		MainProperty		m_Main;
		//地形
		TerrainProperty		m_Terrain;
		WaterProperty		m_Water;
		PlantProperty		m_Plant;
		ObjectProperty		m_Object;
		//--------------------------------------------------------------------------------------------------------------------------------------------------------
		void Save( Serializer& ser )
		{
			ser<<SceneDataIndex::Main_Property;
			m_Main.Save( ser );
			ser<<SceneDataIndex::Terrain_Property;
			m_Terrain.Save(ser);


			ser<<SceneDataIndex::Save_Done;
		}
		//--------------------------------------------------------------------------------------------------------------------------------------------------------
		void Load( Serializer& ser )
		{
			bool isContinue = true;
			while( isContinue )
			{
				int index;
				ser>>index;
				switch( index )
				{
				case SceneDataIndex::Main_Property: m_Main.Load( ser ); break;
				case SceneDataIndex::Terrain_Property: m_Terrain.Load( ser ); break;
				case SceneDataIndex::Save_Done:
					{
						m_Water.m_Scale = m_Terrain.m_Scale;
						m_Water.m_TileSize = m_Terrain.m_TileSize;
						m_Object.m_Scale = m_Terrain.m_Scale;
						m_Object.m_TileSize = m_Terrain.m_TileSize;
						m_Plant.m_Scale = m_Terrain.m_Scale;
						m_Plant.m_TileSize = m_Terrain.m_TileSize;
						m_Plant.m_SplitLevel = m_Terrain.m_SplitLevel;
						isContinue = false; 
					}break;
				}
			}
		}
	};

}