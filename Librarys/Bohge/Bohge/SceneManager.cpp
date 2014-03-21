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
//		绘制管理	   //
/////////////////////////


#include "Predefine.h"
#include "SceneManager.h"
#include "Device.h"
#include "ShaderManager.h"
#include "ISceneNode.h"
#include "SkyBox.h"
#include "RendBuffer.h"
#include "Camera.h"
#include "SceneMain.h"
#include "Serializer.h"
#include "Environment.h"
#include "FilePath.h"


#include <string>

using namespace std;

namespace BohgeEngine
{
	//---------------------------------------------------------------
	SceneManager::SceneManager()
	{
		m_pSky = NEW SkyBox();
		m_pWorld = NEW SceneMain();
	}
	//---------------------------------------------------------------
	SceneManager::~SceneManager()
	{
		SAFE_DELETE(m_pSky);
		SAFE_DELETE(m_pWorld);
	}
	//---------------------------------------------------------------
	//void SceneManage::RenderShadow(engine& engine)
	//{
	//	if( !m_lShadowable.empty() )
	//	{
	//		ASSERT(false);//换花阴影的方式
	//		bool isJustShadowVolume = false;
	//		bool isCull = engine.isState( Device::CULL_FACE_TEST );
	//		if( isCull )
	//		{
	//			engine.DisableCullFace();
	//		}
	//		if(false == isJustShadowVolume)
	//		{		
	//			engine.GetDevice()->SetStencilMask( 0xFFFFFFFF );
	//			engine.GetDevice()->EnableStancil();//glEnable( GL_STENCIL_TEST );
	//			engine.GetDevice()->ColorMask(false, false, false, false);//glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );
	//			engine.GetDevice()->DepthMask(false);//glDepthMask( GL_FALSE );
	//			engine.GetDevice()->StencilFunction(Device::ALWAYS, 0, 0 );//glStencilFunc( GL_ALWAYS, 0, 0 );
	//			engine.GetDevice()->StencilOpSeparate(Device::FRONT,
	//				Device::KEEP,
	//				Device::INCR_WRAP,
	//				Device::KEEP);//glStencilOpSeparate( GL_FRONT, GL_KEEP, GL_INCR_WRAP, GL_KEEP );
	//			engine.GetDevice()->StencilOpSeparate(Device::BACK,
	//				Device::KEEP,
	//				Device::DECR_WRAP,
	//				Device::KEEP);//glStencilOpSeparate( GL_BACK, GL_KEEP, GL_DECR_WRAP, GL_KEEP );
	//			engine.DepthFunction(Device::GREATER);//glDepthFunc( GL_GREATER );
	//		}
	//
	//		//在模板上绘制阴影体
	//		{
	//			//这个是能用的，但是要使用SM了，改用线性深度，把不需要的shader都去掉了，这个也去掉了
	//			//ShadowVolume& sv = engine.GetShaderManage()->GetShader<ShadowVolume>(ShaderManage::ShadowVolume);
	//			//ShadowVolumeInstance& svi = engine.GetShaderManage()->GetShader<ShadowVolumeInstance>(ShaderManage::ShadowVolumeInstance);
	//			//ListShadowable::iterator ite = m_lShadowable.begin();
	//			//ListShadowable::iterator end = m_lShadowable.end();
	//			//for( ; ite != end ; ite ++ )
	//			//{
	//			//	RendBuffer& buff = (*ite)->GetMashEdgeBuffer();
	//			//	if( buff.hasInstance() )
	//			//	{
	//			//		engine.GetDevice()->Draw( buff, svi, NULL );
	//			//	}
	//			//	else
	//			//	{
	//			//		sv.SetParamTransform( (*ite)->GetTransform() );
	//			//		engine.GetDevice()->Draw( buff, sv, NULL );
	//			//	}
	//			//}
	//		}
	//		if(false == isJustShadowVolume)
	//		{
	//			engine.DepthFunction(Device::LEQUAL);//glDepthFunc( GL_LEQUAL );
	//			engine.GetDevice()->StencilFunction(Device::NOTEQUAL, 0, 0xFFFFFFFF );//glStencilFunc( GL_NOTEQUAL, 0, 0xFFFFFFFF );
	//			engine.GetDevice()->StencilOp(Device::ZERO, Device::KEEP, Device::KEEP);//glStencilOp( GL_ZERO, GL_KEEP, GL_KEEP );
	//			engine.GetDevice()->ColorMask(true, true, true, true);//glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
	//			engine.GetDevice()->DepthMask(true);//glDepthMask( GL_TRUE );
	//			{
	//				//这个是能用的，但是要使用SM了，改用线性深度，把不需要的shader都去掉了，这个也去掉了
	//				//绘制被影响的物体
	//				//Constant& cs = engine.GetShaderManage()->GetShader<Constant>(ShaderManage::Constant);
	//				//ListBeShadowable::iterator ite = m_lBeShadowable.begin();
	//				//ListBeShadowable::iterator end = m_lBeShadowable.end();
	//				//for( ; ite != end ; ite ++ )
	//				//{
	//				//	cs.SetParamTransform( (*ite)->GetTransform() );
	//				//	engine.GetDevice()->Draw((*ite)->GetRenderBuffer(),cs,NULL);
	//				//}
	//			}
	//			//clean up
	//			engine.GetDevice()->DisableStancil();
	//		}
	//		if( isCull )
	//		{
	//			engine.GetDevice()->EnableCullFace();
	//		}
	//	}
	//}
	//---------------------------------------------------------------
	void SceneManager::BeginUpdate( Engine& engine )
	{
		m_UpdataQueue.clear();
		for ( SceneNodeList::iterator it = m_SceneNodeList.begin();
			it != m_SceneNodeList.end();
			it ++ )
		{
			m_UpdataQueue.insert( make_pair( (*it)->NodeDepth(),*it ) );
		}

		//更新
		m_pWorld->BeginUpdate( engine );
		for ( NodeQueue::iterator it = m_UpdataQueue.begin();
			it != m_UpdataQueue.end();
			it ++ )
		{
			it->second->BeginUpdate( engine );
		}
	}
	//---------------------------------------------------------------
	void SceneManager::EndUpdate( Engine& engine )
	{
		for ( NodeQueue::iterator it = m_UpdataQueue.begin();
			it != m_UpdataQueue.end();
			it ++ )
		{
			it->second->EndUpdate( engine );
		}
		m_pWorld->EndUpdate( engine );

		for ( NeedReset::iterator it = m_AttributeMap.begin();
			it != m_AttributeMap.end();
			it ++ )
		{
			it->second->ResetFatherChanged();
		}
	}
	//---------------------------------------------------------------
	void SceneManager::Update( uint milisecond )
	{
		//更新世界
		m_pWorld->Update( milisecond );

		//处理更新
		for ( NodeQueue::iterator it = m_UpdataQueue.begin();
			it != m_UpdataQueue.end();
			it ++ )
		{
			it->second->Update( milisecond );
		}
	}
	//---------------------------------------------------------------
	void SceneManager::RenderSecen(Engine& engine)
	{
		const ICamera& maincame = *Engine::Instance().GetCamera();
		//计算更新全部其他的物体
		m_NeedRenders.clear();
		m_Transparent.clear();

		//处理是否绘制
		for ( NodeQueue::iterator it = m_UpdataQueue.begin();
			it != m_UpdataQueue.end();
			it ++ )
		{
			if( it->second->isShow() ) //计算需要绘制的物体
			{
				if( it->second->isStatus( ISceneNode::RS_NOCULL ) )
				{
					vector3f pos = it->second->WorldBindBox().GetCenter();
					float dir = ( pos - maincame.GetPosition() ).LengthPow();
					if ( it->second->isStatus( ISceneNode::RS_TRANSPARENT ) )
					{
						m_Transparent.insert( make_pair( dir, it->second ) );
					}
					else
					{
						m_NeedRenders.insert( make_pair( dir, it->second ) );
					}
				}
				else
				{
					if ( it->second->isInview( maincame.GetFrustum() ) )
					{
						vector3f pos = it->second->WorldBindBox().GetCenter();
						float dir = ( pos - maincame.GetPosition() ).LengthPow();
						if ( it->second->isStatus( ISceneNode::RS_TRANSPARENT ) )
						{
							m_Transparent.insert( make_pair( dir, it->second ) );
						}
						else
						{
							m_NeedRenders.insert( make_pair( dir, it->second ) );
						}
					}
				}
			}
		}

		//开始绘制

		//绘制天空盒
		m_pSky->RenderBackground(engine);
		m_pSky->RenderSkyBox(engine);

		for ( NodeMultimap::iterator it = m_NeedRenders.begin();
			it != m_NeedRenders.end();
			it++ )
		{
			it->second->BeginRender( engine);
		}

		//绘制
		for ( NodeMultimap::iterator it = m_NeedRenders.begin();
			it != m_NeedRenders.end();
			it++ )
		{
			it->second->Render( engine);
#ifdef DRAWAABBOX
			it->second->RenderBind( engine );
#endif
		}
		m_pWorld->Render( engine );

		//结束绘制
		for ( NodeMultimap::iterator it = m_NeedRenders.begin();
			it != m_NeedRenders.end();
			it++ )
		{
			it->second->EndRender(engine);
		}
		m_pWorld->EndRender( engine );

		//透明的物体
		//开始绘制
		for ( TransparentNodeMultimap::iterator it = m_Transparent.begin();
			it != m_Transparent.end();
			it++ )
		{
			it->second->BeginRender( engine);
		}
		//绘制
		for ( TransparentNodeMultimap::iterator it = m_Transparent.begin();
			it != m_Transparent.end();
			it++ )
		{
			it->second->Render( engine);
#ifdef DRAWAABBOX
			it->second->RenderBind( engine );
#endif
		}
		//结束绘制
		for ( TransparentNodeMultimap::iterator it = m_Transparent.begin();
			it != m_Transparent.end();
			it++ )
		{
			it->second->EndRender(engine);
		}
	}
	//---------------------------------------------------------------
	void SceneManager::RenderNormal( Engine& engine )
	{
		for ( NodeMultimap::iterator it = m_NeedRenders.begin();
			it != m_NeedRenders.end();
			it++ )
		{
			it->second->Render( engine);
		}
		m_pWorld->RenderNormal( engine );
	}
	//---------------------------------------------------------------
	bool SceneManager::OnCameraUpdate( ICamera& camera )
	{
		vector3f pos = camera.GetPosition();
		float nearondiYr = 0;
		vector3f downDir;
		vector3f roaxis =  Math::Cross( camera.GetForward(), camera.GetUp() );
		roaxis.NormalizeSelf();
		Quaternionf rota;
		rota.RotateAxis( roaxis, -camera.Fov() / camera.Aspect() * 0.8 );
		downDir = camera.GetForward() * rota;
		downDir.NormalizeSelf();
		if ( downDir.Dot( vector3f( 0,1,0 ) ) < 0.0 )
		{
			nearondiYr = -( downDir * camera.GetNear() ).m_y;
		}
		float wh_nearondiYr = m_pWorld->GetHeight( pos.m_x, pos.m_z ) + nearondiYr;
		//float wh_nearondiYr = m_pWorld->GetHeight( pos.m_x, pos.m_z ) + camera.GetNear();
		if ( pos.m_y < wh_nearondiYr )
		{
			pos.m_y = wh_nearondiYr;
			camera.SetPosition( pos );
			return true;
		}
		return false;
	}
	////---------------------------------------------------------------
	//void SceneManage::RemoveShadowObject(IShadowable* input)
	//{
	//	for ( ListShadowable::iterator it = m_lShadowable.begin();
	//		it != m_lShadowable.end();
	//		)
	//	{
	//		if( *it == input )
	//		{
	//			it = m_lShadowable.erase( it );
	//		}
	//		else
	//		{
	//			it ++;
	//		}
	//	}
	//}
	////---------------------------------------------------------------
	//void SceneManage::RemoveBeShadowObject(IBeShadowable* input)
	//{
	//	for ( ListBeShadowable::iterator it = m_lBeShadowable.begin();
	//		it != m_lBeShadowable.end();
	//		)
	//	{
	//		if( *it == input )
	//		{
	//			it = m_lBeShadowable.erase( it );
	//		}
	//		else
	//		{
	//			it ++;
	//		}
	//	}
	//}
	//---------------------------------------------------------------
	void SceneManager::LoadScene( const std::string& path )
	{
		string fullpath = FILEPATH.SceneFolder() + path;
		Serializer loader( fullpath );
		bool isExport;
		loader>>isExport;
		if ( isExport )
		{
			string folder;
			loader>>folder;
			fullpath = fullpath.substr( 0, fullpath.find_last_of("/\\") );
			m_pWorld->Load( fullpath + PLANTFORMDELIMITER + folder + PLANTFORMDELIMITER );
		}
		else
		{
			ASSERT(false);
		}
	}
	//---------------------------------------------------------------
	void SceneManager::AssignMaterial( const bool& isCast, const ICamera& camera )
	{
		m_pWorld->AssignMaterial( isCast, camera );
	}
	//---------------------------------------------------------------
	const LocatableObject* SceneManager::GetLoadedObject( const std::string& Tilename )
	{
		return m_pWorld->GetLoadedObject( Tilename );
	}

}