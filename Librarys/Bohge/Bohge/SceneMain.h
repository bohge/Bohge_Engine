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
//		场景主管			//
/////////////////////////
#pragma once
#include "SecenProperty.h"

#include <string>
#include <map>



namespace BohgeEngine
{

	class Engine;
	class Texture2D;
	class PlantManager;
	class WaterManager;
	class TerrainManager;
	class ObjectManager;
	class ITransparent;
	class LocatableObject;
	class SceneMain
	{
		bool					m_isLoaded;
	private:
		SecenProperty			m_Property;//场景的属性
		WaterManager*			m_pWater;
		TerrainManager*			m_pWorld;
		PlantManager*			m_pPlant;
		ObjectManager*			m_pObject;
	public:
		SceneMain(void);
		~SceneMain(void);
	public:
		void BeginUpdate( Engine& engine );
		void Update( uint milisecon );
		void EndUpdate( Engine& engine );
		void Render(Engine& engine);
		void EndRender( Engine& engine );
		void RenderNormal( Engine& engine );//绘制法线
		float GetHeight( float x, float y) const;//得到高度
		void AssignMaterial( const bool& isCast, const ICamera& camera );//通过阴影摄像机确定渲染每一块的材质
		//void GetTransparentNode( std::map<float, ITransparent*, std::greater<float> >& nodemap );
		const LocatableObject* GetLoadedObject( const std::string& Tilename );
		void Load( const std::string& path );
	};

}