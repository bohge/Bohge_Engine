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
#include "RenderObjectMetadata.h"
#include "RenderTargetMetadata.h"
#include "RenderObjectEntity.h"
#include "RenderTargetEntity.h"
#include "ShapeBufferUtility.h"
#include "TextureMetadata.h"
#include "IPipelineSystem.h"
#include "ScriptMetadata.h"
#include "TextureEntity.h"
#include "GlobalDefine.h"
#include "ScriptEntity.h"
#include "PostEffect.h"
#include "Framework.h"
#include "3DMath.h"




namespace BohgeEngine
{
	RTTI_TYPE_IMPLEMENT( PostEffect );
	//---------------------------------------------------------------------------------------------------------
	PostEffect::PostEffect(void)
		:m_ResourceCount(0),
		m_pScript( NEW ScriptEntity ),
		m_pProcess(NULL),
		m_pResize(NULL),
		m_ResizeFunction(NULL),
		m_Queue(0)
	{
		m_pScript->ConnectLoadedEvent( MEMBER_FUNC_PTR( &PostEffect::OnScriptLoaded ) );
	}
	//---------------------------------------------------------------------------------------------------------
	PostEffect::~PostEffect(void)
	{
		IPipelineSystem::Instance()->DetachPostEffect( this );
		SAFE_DELETE( m_pScript );
		FOR_EACH( RenderObjectList, rt, m_RenderObjectList )
		{
			SAFE_DELETE( *rt );
		}

		FOR_EACH( RenderTargetList, rt, m_RenderTargetList )
		{
			SAFE_DELETE( *rt );
		}
	}
	//---------------------------------------------------------------------------------------------------------
	RenderObjectEntity* PostEffect::CreateRenderObject( const eastl::string& path )
	{
		RenderObjectEntity* ro = NEW RenderObjectEntity();
		ro->PushMetadata(
			RenderObjectMeshMetadate( 
			RenderObjectProperty::RM_TRIANGLES,
			QuadVertexMetadata( BufferObjectProperty::MU_STATIC ),
			QuadIndicesMetadata( BufferObjectProperty::MU_STATIC ) )
			);
		ro->PushMetadata( RenderObjectMaterialMetadata( PathMetadata( path ) ) );
		++m_ResourceCount;
		ro->ConnectLoadedEvent( MEMBER_FUNC_PTR( &PostEffect::OnResourceLoaded ) );
		ro->CreateResource();
		m_RenderObjectList.push_back( ro );
		return ro;
	}
	//---------------------------------------------------------------------------------------------------------
	RenderTargetEntity* PostEffect::CreateRenderTarget( RenderTargetProperty::SwapTarget st, const vector2d& size )
	{
		RenderTargetEntity* rte = NEW RenderTargetEntity();
		rte->ListenResizeview();
		rte->SetJobType( IJob::JT_SYNCHRONOUS );
		rte->PushMetadata(
			RenderTargetDependResolutionMetadata(
			RenderTargetProperty::RT_RENDER_TARGET_2D,
			st,
			GlobalDefine::Color_Black,
			vector4d( 0,0,size.m_x,size.m_y ),size
			));
		TextureEntity* tex = rte->MakeTextureAttachment( RenderTargetProperty::TA_COLOR_0 );
		tex->PushMetadata( 
			TextureBufferMetadata( 
			size,
			TextureProperty::TT_TEXTURE2D,
			TextureProperty::TU_STATIC,
			TextureProperty::PF_R8G8B8A8, 1, true,
			TextureProperty::TW_CLAMP_TO_EDGE,
			TextureProperty::TW_CLAMP_TO_EDGE,
			TextureProperty::TF_LINEAR,
			TextureProperty::TF_LINEAR,
			NULL ) );
		++m_ResourceCount;
		rte->ConnectLoadedEvent( MEMBER_FUNC_PTR( &PostEffect::OnResourceLoaded ) );
		rte->CreateResource();
		m_RenderTargetList.push_back( rte );
		return rte;
	}
	//---------------------------------------------------------------------------------------------------------
	void PostEffect::OnScriptLoaded( const SmartPtr<IAsyncSource>& sou )
	{
		m_ResizeFunction = Framework::Instance()->RegistResizeviewListener( MEMBER_FUNC_PTR( &PostEffect::OnResizeview ) );
		m_Queue	= *static_cast<const float*>( m_pScript->GetMember( ScriptProperty::POST_EFFECT_QUEUE ) );
		IFunctionInvoker* init	= m_pScript->SignFunciton( ScriptProperty::POST_EFFECT_INIT, 2 );
		init->Invoke( this, &Framework::Instance()->GetConfig().GetResolution() );
		m_pProcess		= m_pScript->SignFunciton( ScriptProperty::POST_EFFECT_PROCESS, 3 );
		m_pResize		= m_pScript->SignFunciton( ScriptProperty::POST_EFFECT_ONRESIZE, 1 );
	}
	//---------------------------------------------------------------------------------------------------------
	void PostEffect::OnResourceLoaded( const SmartPtr<IAsyncSource>&  sou )
	{
		if ( 0 == -- m_ResourceCount )
		{
			IPipelineSystem::Instance()->AttachPostEffect( this );
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void PostEffect::CreatePostEffect( const eastl::string& path )
	{
		m_pScript->PushMetadata( ScriptMetadata( ScriptProperty::ST_RENDER, path) );
		m_pScript->CreateResource();
	}
	//---------------------------------------------------------------------------------------------------------
	void PostEffect::OnResizeview( const ViewResolution& size )
	{
		m_pResize->Invoke( &size.m_NewResolution );
	}

}