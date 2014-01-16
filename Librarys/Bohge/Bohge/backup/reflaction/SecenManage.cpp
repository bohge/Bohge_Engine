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
#include "SecenManage.h"
#include "IDriver.h"
#include "ShaderManage.h"
#include "ISecenNode.h"
#include "SkyBox.h"
#include "RendBuffer.h"
#include "Camera.h"
#include "SceneMain.h"
#include "Serializer.h"
#include "Environment.h"
#include "EmitterManager.h"
#include "Camera.h"


#include <string>

using namespace std;

//---------------------------------------------------------------
SecenManage::SecenManage()
{
	m_pSky = NEW SkyBox();
	m_pWorld = NEW SceneMain();
	m_pEmitters = NEW EmitterManager();
}
//---------------------------------------------------------------
SecenManage::~SecenManage()
{
	SAFE_DELETE(m_pSky);
	SAFE_DELETE(m_pWorld);
	SAFE_DELETE( m_pEmitters );
}
//---------------------------------------------------------------
//void SecenManage::RenderShadow(IDriver& driver)
//{
//	if( !m_lShadowable.empty() )
//	{
//		ASSERT(false);//换花阴影的方式
//		bool isJustShadowVolume = false;
//		bool isCull = driver.isState( IDriver::CULL_FACE_TEST );
//		if( isCull )
//		{
//			driver.DisableCullFace();
//		}
//		if(false == isJustShadowVolume)
//		{		
//			driver.SetStencilMask( 0xFFFFFFFF );
//			driver.EnableStancil();//glEnable( GL_STENCIL_TEST );
//			driver.ColorMask(false, false, false, false);//glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );
//			driver.DepthMask(false);//glDepthMask( GL_FALSE );
//			driver.StencilFunction(IDriver::ALWAYS, 0, 0 );//glStencilFunc( GL_ALWAYS, 0, 0 );
//			driver.StencilOpSeparate(IDriver::FRONT,
//				IDriver::KEEP,
//				IDriver::INCR_WRAP,
//				IDriver::KEEP);//glStencilOpSeparate( GL_FRONT, GL_KEEP, GL_INCR_WRAP, GL_KEEP );
//			driver.StencilOpSeparate(IDriver::BACK,
//				IDriver::KEEP,
//				IDriver::DECR_WRAP,
//				IDriver::KEEP);//glStencilOpSeparate( GL_BACK, GL_KEEP, GL_DECR_WRAP, GL_KEEP );
//			driver.DepthFunction(IDriver::GREATER);//glDepthFunc( GL_GREATER );
//		}
//
//		//在模板上绘制阴影体
//		{
//			//这个是能用的，但是要使用SM了，改用线性深度，把不需要的shader都去掉了，这个也去掉了
//			//ShadowVolume& sv = driver.GetShaderManage().GetShader<ShadowVolume>(ShaderManage::ShadowVolume);
//			//ShadowVolumeInstance& svi = driver.GetShaderManage().GetShader<ShadowVolumeInstance>(ShaderManage::ShadowVolumeInstance);
//			//ListShadowable::iterator ite = m_lShadowable.begin();
//			//ListShadowable::iterator end = m_lShadowable.end();
//			//for( ; ite != end ; ite ++ )
//			//{
//			//	RendBuffer& buff = (*ite)->GetMashEdgeBuffer();
//			//	if( buff.hasInstance() )
//			//	{
//			//		driver.Draw( buff, svi, NULL );
//			//	}
//			//	else
//			//	{
//			//		sv.SetParamTransform( (*ite)->GetTransform() );
//			//		driver.Draw( buff, sv, NULL );
//			//	}
//			//}
//		}
//		if(false == isJustShadowVolume)
//		{
//			driver.DepthFunction(IDriver::LEQUAL);//glDepthFunc( GL_LEQUAL );
//			driver.StencilFunction(IDriver::NOTEQUAL, 0, 0xFFFFFFFF );//glStencilFunc( GL_NOTEQUAL, 0, 0xFFFFFFFF );
//			driver.StencilOp(IDriver::ZERO, IDriver::KEEP, IDriver::KEEP);//glStencilOp( GL_ZERO, GL_KEEP, GL_KEEP );
//			driver.ColorMask(true, true, true, true);//glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
//			driver.DepthMask(true);//glDepthMask( GL_TRUE );
//			{
//				//这个是能用的，但是要使用SM了，改用线性深度，把不需要的shader都去掉了，这个也去掉了
//				//绘制被影响的物体
//				//Constant& cs = driver.GetShaderManage().GetShader<Constant>(ShaderManage::Constant);
//				//ListBeShadowable::iterator ite = m_lBeShadowable.begin();
//				//ListBeShadowable::iterator end = m_lBeShadowable.end();
//				//for( ; ite != end ; ite ++ )
//				//{
//				//	cs.SetParamTransform( (*ite)->GetTransform() );
//				//	driver.Draw((*ite)->GetRenderBuffer(),cs,NULL);
//				//}
//			}
//			//clean up
//			driver.DisableStancil();
//		}
//		if( isCull )
//		{
//			driver.EnableCullFace();
//		}
//	}
//}
//---------------------------------------------------------------
void SecenManage::RenderBackground(IDriver& driver)
{
	//driver.DepthMask( false );
	m_pSky->RenderBackground(driver);
	//driver.DepthMask( true );
}
//---------------------------------------------------------------
void SecenManage::RenderSkyBox(IDriver& driver)
{
	//driver.DepthMask( false );
	m_pSky->RenderSkyBox(driver);
	//driver.DepthMask( true );
}
//---------------------------------------------------------------
void SecenManage::OnBeginUpdate( IDriver& driver )
{
	m_pWorld->OnBeginUpdate( driver );
	for ( ListUpdateable::iterator it = m_lUpdateable.begin();
		it != m_lUpdateable.end();
		it ++ )
	{
		(*it)->OnBeginUpdate( driver );
	}
}
//---------------------------------------------------------------
void SecenManage::Update( uint milisecond )
{
	m_TransparentMap.clear();
	//更新世界
	m_pWorld->Update( milisecond );
	m_pWorld->GetTransparentNode( m_TransparentMap );
	//更新粒子
	m_pEmitters->Updata( milisecond );
	m_pEmitters->GetTransparentNode( m_TransparentMap );
	//计算更新全部其他的物体
	m_NeedRenders.clear();
	m_ShadowRenders.clear();

	//处理更新
	for ( ListUpdateable::iterator it = m_lUpdateable.begin();
		it != m_lUpdateable.end();
		it ++ )
	{
		(*it)->Update(milisecond);
	}
	//处理是否绘制
	for ( ListRenderable::iterator it = m_lRenderable.begin();
		it != m_lRenderable.end();
		it ++ )
	{
		if( (*it)->isRender() ) //计算需要绘制的物体
		{
			if( (*it)->isStatus( IRenderNode::RS_NOCULL ) )
			{
				m_NeedRenders.push_back( *it );
				if( (*it)->isStatus( IRenderNode::RS_SHADOW ) )
				{
					m_ShadowRenders.push_back( *it );
				}
			}
			else
			{
				if( NULL != (*it)->Attribute( IAttributeNode::ATT_TRANSFORM ) )
				{
					if( IT_NO != IDriver::Instance().GetCamera().GetFrustum().Intersect( (*it)->BindBox() * *(Matrix44f*)((*it)->Attribute( IAttributeNode::ATT_TRANSFORM )) ) )
					{
						m_NeedRenders.push_back( *it );
					}
					if( (*it)->isStatus( IRenderNode::RS_SHADOW ) )
					{
						if( IT_NO != IDriver::Instance().GetEnvironment().GetCamera().GetFrustum().Intersect( (*it)->BindBox() * *(Matrix44f*)((*it)->Attribute( IAttributeNode::ATT_TRANSFORM )) ) )
						{
								m_ShadowRenders.push_back( *it );
						}
					}
				}
				else
				{
					if( IT_NO != IDriver::Instance().GetCamera().GetFrustum().Intersect( (*it)->BindBox() ) )
					{
						m_NeedRenders.push_back( *it );
						if( (*it)->isStatus( IRenderNode::RS_SHADOW ) )
						{
							m_ShadowRenders.push_back( *it );
						}
					}
					if( (*it)->isStatus( IRenderNode::RS_SHADOW ) )
					{
						if( IT_NO != IDriver::Instance().GetEnvironment().GetCamera().GetFrustum().Intersect( (*it)->BindBox() ) )
						{
							m_ShadowRenders.push_back( *it );
						}
					}
				}
			}
		}
	}
}
//---------------------------------------------------------------
void SecenManage::Render(IDriver& driver)
{

	for ( NeedRenders::iterator it = m_NeedRenders.begin();
		it != m_NeedRenders.end();
		it++ )
	{
		(*it)->Render( driver);
	}
	m_pWorld->Render( driver );
    
	for ( NeedRenders::iterator it = m_NeedRenders.begin();
		it != m_NeedRenders.end();
		it++ )
	{
		(*it)->OnEndRender(driver);
	}
	m_pWorld->OnEndRender( driver );

	for ( TransparentMap::iterator it = m_TransparentMap.begin();
		it != m_TransparentMap.end();
		it++ )
	{
		it->second->RenderTransparent(driver);
	}
}
//---------------------------------------------------------------
void SecenManage::RenderDepth( IDriver& driver )
{
	for ( NeedRenders::iterator it = m_ShadowRenders.begin();
		it != m_ShadowRenders.end();
		it++ )
	{
		(*it)->Render( driver);
	}
}
//---------------------------------------------------------------
void SecenManage::RenderReflection( IDriver& driver )
{
	for ( NeedRenders::iterator it = m_NeedRenders.begin();
		it != m_NeedRenders.end();
		it++ )
	{
		(*it)->Render( driver);
	}
	m_pWorld->Render( driver );
	for ( TransparentMap::iterator it = m_TransparentMap.begin();
		it != m_TransparentMap.end();
		it++ )
	{
		it->second->RenderTransparent(driver);
	}
}
//---------------------------------------------------------------
float SecenManage::GetWaterMiddleHeight()
{
	return m_pWorld->GetWaterMiddleHeight();
}
////---------------------------------------------------------------
//void SecenManage::RemoveShadowObject(IShadowable* input)
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
//void SecenManage::RemoveBeShadowObject(IBeShadowable* input)
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
void SecenManage::LoadScene( const std::string& path )
{
	string fullpath = File::Instance().MakeFilePath( path );
	Serializer loader( fullpath, File::FILE_READ );
	bool isExport;
	loader>>isExport;
	if ( isExport )
	{
		string folder;
		loader>>folder;
		fullpath = fullpath.substr( 0, fullpath.find_last_of(PLANTFORMDELIMITER) );
		m_pWorld->Load( fullpath + PLANTFORMDELIMITER + folder + PLANTFORMDELIMITER );
	}
	else
	{
		ASSERT(false);
	}
}

