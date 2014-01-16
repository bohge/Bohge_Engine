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
#include "WaterManager.h"
#include "WaterGrid.h"
#include "RenderTarget.h"
#include "Pipeline.h"
#include "CopyCurrentScreen.h"
#include "WaterTile.h"
#include "Serializer.h"
#include "SceneDataIndex.h"
#include "Camera.h"
#ifndef COPYSCREEN
#include "TerrainManager.h"
#endif

using namespace std;

//-------------------------------------------------------------------------------------------------------
WaterManager::WaterManager(void)
:m_pWater(NULL),
m_isRender(false),
m_LastTileIndex(-1)
{
}
//-------------------------------------------------------------------------------------------------------
WaterManager::~WaterManager(void)
{
	SAFE_DELETE( m_pWater );
#ifndef COPYSCREEN
	SAFE_DELETE( m_Refraction );
#endif
	this->Unload();
}
//-------------------------------------------------------------------------------------------------------
void WaterManager::OnBeginUpdate( IDriver& driver )
{
	m_isRender = false;
	Frustumf frustum = driver.GetCamera().GetFrustum();
	for ( WaterMeshes::iterator it = m_WaterDatas.begin();
		it != m_WaterDatas.end();
		it ++ )
	{
		if( frustum.Intersect( (*it)->BindBox() ) )
		{
			m_isRender = true;
			if( m_LastTileIndex != (*it)->GetIndex() )
			{
				m_LastTileIndex = (*it)->GetIndex();
				m_MiddleHeight = (*it)->GetWaterParameter().get_float( p_fWaterBaseHeight );
				m_pWater->WaterSurface( (*it)->GetMaterial(), (*it)->GetWaterParameter() );
			}
			break;
		}
	}
	if( m_isRender )
	{
		m_pWater->OnBeginUpdate( driver );
	}
}
//-------------------------------------------------------------------------------------------------------
#ifdef COPYSCREEN
void WaterManager::Render( IDriver& driver )
{
	if( m_isRender )
	{
		driver.GetPipeline().MakeCopyScreen();
		m_pWater->Render( driver );
	}
}
#else
void WaterManager::Render( IDriver& driver, TerrainManager* world )
{
	if( m_isRender )
	{
		RenderTarget* now = driver.PopRenderTarget( );
		driver.PushRenderTarget( m_Refraction );
		driver.Clear( IDriver::COLOR_BUFFER | IDriver::DEPTH_BUFFER | IDriver::STENCIL_BUFFER );
		world->Render( driver );
		driver.PopRenderTarget( );
		driver.PushRenderTarget( now );
	}
}
#endif
//-------------------------------------------------------------------------------------------------------
#ifdef TERRAINMANAGER
void WaterManager::Load( const std::string& path, const TerrainManager& terrain )
#else ifdef TERRAINEDIT
void WaterManager::Load( const std::string& path, const TerrainEdit& terrain )
#endif
{
	if( NULL == m_pWater )//如果没有构造grid，就new一个
	{
		vector2d size = IDriver::Instance().GetViewprot();
#ifdef COPYSCREEN
		m_pWater = NEW WaterGrid( *((Texture2D*)IDriver::Instance().GetPipeline().GetCopyColorTexture()), *((Texture2D*)IDriver::Instance().GetPipeline().GetCopyDepthTexture()), terrain );
#else
		m_Refraction = NEW RenderTarget(size , IDriver::FBO_texColor_texDepth);
		m_pWater = NEW WaterGrid( *((Texture2D*)m_Refraction->GetColorBuffer()), *((Texture2D*)m_Refraction->GetDepthBuffer()), terrain );
#endif
	}
	this->Unload();
	//开始读取
 	string LocalPath = path + WaterFolder;
	Serializer loader( LocalPath + WaterInfoList, File::FILE_READ );
	bool isContinue = true;
	while( isContinue )
	{
		int index;
		loader>>index;
		switch( index )
		{
		case SceneDataIndex::Water_Tile: 
			{
				string teerainpath;
				vector2f offset;
				loader>>offset.m_x>>offset.m_y>>teerainpath;
				teerainpath += PLANTFORMDELIMITER;
				WaterTile* temp = new WaterTile( );
				temp->Load( LocalPath + teerainpath, LocalPath );
				m_WaterDatas.push_back( temp );
			}break;
		case SceneDataIndex::Save_Done: isContinue = false; break;
		}
	}
}
//-------------------------------------------------------------------------------------------------------
void WaterManager::Unload()
{
	for ( WaterMeshes::iterator it = m_WaterDatas.begin();
		it != m_WaterDatas.end();
		it ++ )
	{
		SAFE_DELETE( *it );
	}
}
//-------------------------------------------------------------------------------------------------------
float WaterManager::GetHeight( float x, float y ) const
{
	float depth = -100000000;
	for ( WaterMeshes::const_iterator it = m_WaterDatas.begin();
		it != m_WaterDatas.end();
		it ++ )
	{
		if( (*it)->GetHeight( x, y, depth ) )
		{
			break;
		}
	}
	return depth;
}