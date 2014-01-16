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
//		场景主管	   //
/////////////////////////
#include "IDriver.h"
#include "SceneMain.h"
#include "TerrainManager.h"
#include "WaterManager.h"
#include "PlantManager.h"

using namespace std;
//-------------------------------------------------------------------------------------------------------
SceneMain::SceneMain(void)
:m_pWorld(NULL),
m_pWater(NULL),
m_pPlant(NULL),
m_isLoaded(false)
{
}
//-------------------------------------------------------------------------------------------------------
SceneMain::~SceneMain(void)
{
	SAFE_DELETE( m_pPlant );
	SAFE_DELETE(m_pWorld);
	SAFE_DELETE(m_pWater);
}
//-------------------------------------------------------------------------------------------------------
void SceneMain::Load( const string& path )
{
#ifdef TERRAINMANAGER
	SAFE_DELETE(m_pWorld);
	SAFE_DELETE(m_pWater);

	m_isLoaded = true;
	string name = SceneName;
	Serializer loader( path + name, File::FILE_READ );
	bool isContinue = true;
	while( isContinue )
	{
		int index;
		loader>>index;
		switch( index )
		{
		case SceneDataIndex::Scene_Property: m_Property.Load( loader ); break;
		case SceneDataIndex::Terrain_List: m_pWorld = NEW TerrainManager( m_Property.m_Terrain ); m_pWorld->Load( path );break;
		case SceneDataIndex::Water_List:
			{
				ASSERT( NULL != m_pWorld );
				m_pWater = NEW WaterManager();
				m_pWater->Load( path, *m_pWorld );
			}break;
		case SceneDataIndex::Plant_List: m_pPlant = NEW PlantManager( m_Property.m_Plant ); m_pPlant->Load( path );break;
		case SceneDataIndex::Save_Done: isContinue = false; break;
		}
	}
	IDriver::Instance().SetEnvironment( m_Property.m_Main.m_LightPos, m_Property.m_Main.m_LightDir, m_Property.m_Main.m_AmbieColor, m_Property.m_Main.m_LightColor, m_Property.m_Main.m_FogColor, m_Property.m_Main.m_FogParam );
#else ifdef TERRAINEDIT
	ASSERT(false);//EDIT不靠这个东西去读取场景，保留的话会有new water的问题
#endif
}
//-------------------------------------------------------------------------------------------------------
void SceneMain::OnBeginUpdate( IDriver& driver )
{
	if( NULL!= m_pWater )
	{
		m_pWater->OnBeginUpdate( driver );
	}
}
//-------------------------------------------------------------------------------------------------------
void SceneMain::Update( uint milisecon )
{
	if( m_isLoaded )
	{
		m_pWorld->Update( milisecon );
		m_pPlant->Update( milisecon );
	}
}
//-------------------------------------------------------------------------------------------------------
void SceneMain::Render( IDriver& driver )
{
	if( m_isLoaded )
	{
        m_pPlant->RenderOpaque( driver ); 
		m_pWorld->Render( driver );
	}
}
//-------------------------------------------------------------------------------------------------------
void SceneMain::OnEndRender( IDriver& driver )
{
	if( NULL!= m_pWater && m_isLoaded )
	{
#ifdef COPYSCREEN
		m_pWater->Render( driver );
#else
		m_pWater->Render( driver, m_pWorld );
#endif
	}
}
//-------------------------------------------------------------------------------------------------------
float SceneMain::GetHeight( float x, float y ) const
{
	float th = m_pWorld->GetHeight( x, y );
	float wh = m_pWater->GetHeight( x, y );
	return th > wh ? th : wh;
}
//-------------------------------------------------------------------------------------------------------
void SceneMain::GetTransparentNode( std::map<float, ITransparent*, std::greater<float> >& nodemap )
{
	if( NULL != m_pPlant && m_isLoaded )
	{
		m_pPlant->GetTransparentNode( nodemap );
	}
}
//-------------------------------------------------------------------------------------------------------
float SceneMain::GetWaterMiddleHeight() const
{
	if ( NULL != m_pWater )
	{
		return m_pWater->GetWaterMiddleHeight();
	}
	return 0.0f;
}
