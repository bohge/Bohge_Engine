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
#include "RenderTargetSource.h"
#include "RenderTargetResource.h"
#include "IGraphicSystem.h"
#include "TextureEntity.h"
#include "Utility.hpp"

#include <EASTL/string.h>
#include <sstream>

using namespace eastl;

namespace BohgeEngine
{
	//------------------------------------------------------------------------------------------------------
	RenderTargetSource::RenderTargetSource(void)
		:ISharedSource( IResourceProperty::SST_RENDERTARGET, IResourceProperty::DT_RENDER_THREAD, true ),
		m_eTargetType( RenderTargetProperty::RT_RENDER_TARGET_2D ),
		m_eSwapTarget( RenderTargetProperty::ST_SWAP_A )
	{
	}
	//------------------------------------------------------------------------------------------------------
	RenderTargetSource::~RenderTargetSource(void)
	{
		FOR_EACH( AttachmentMap, it, m_TextureBufferMap )
		{
			SAFE_DELETE( it->second );
		}
	}
	//------------------------------------------------------------------------------------------------------
	TextureEntity* RenderTargetSource::MakeTextureAttachment( RenderTargetProperty::RenderTargetAttachment rta )
	{
		TextureEntity* tex = NEW TextureEntity;
		tex->SetJobType( GetJobType() );
		tex->ConnectPushedEvent( DependencePushed() );
		tex->ConnectLoadedEvent( DependenceLoaded() );
		m_TextureBufferMap.insert( make_pair( rta,tex) );
		return tex;
	}
	//------------------------------------------------------------------------------------------------------
	void RenderTargetSource::MakeBufferAttachment( RenderTargetProperty::RenderTargetAttachment rta )
	{
		m_TextureBufferMap.insert( make_pair( rta, (TextureEntity*)NULL ) );
	}
	//------------------------------------------------------------------------------------------------------
	void RenderTargetSource::_DoLoadResource()
	{
		(*GetSourceMetadata().begin())->ProcessMetadata();
		RenderTargetProperty::RenderTargetMetadata* meta
			= (*GetSourceMetadata().begin())->GetMetadata<RenderTargetProperty::RenderTargetMetadata>();
		SetClearColor( meta->m_Color );
		SetViewPort( meta->m_ViewPort );
		SetSwapTarget( meta->m_eSwapTarget );
		SetSize( meta->m_Size );
		SetTargetType( meta->m_eTargetType );
		FOR_EACH( AttachmentMap, it, m_TextureBufferMap )
		{
			if ( NULL != it->second )
			{
				it->second->CreateResource();
			}
		}
	}
	//------------------------------------------------------------------------------------------------------
	uint RenderTargetSource::_GetResourceHashCode() const
	{
		eastl::vector<byte> bytes;
		(*GetSourceMetadata().begin())->GetIdentifier( bytes );
		FOR_EACH_CONST( AttachmentMap, it, m_TextureBufferMap )
		{
			bytes.push_back( it->first + 1);
			if ( NULL != it->second )
			{
				bytes.push_back( it->second->GetPixelFormat() + 1 );
			}
		}
		return Utility::HashCode( bytes );
	}
	//------------------------------------------------------------------------------------------------------
	ISharedResource* RenderTargetSource::MakeResource()
	{
		RenderTargetResource* res
			= static_cast<RenderTargetResource*>( IGraphicSystem::Instance()->CreateResource( IResourceProperty::SRT_RENDERTARGET ) );
		res->GenerateRenderTarget( m_eTargetType, m_Size );
		res->SetViewPort( m_ViewPort );
		res->SetClearColor( m_Color );
		FOR_EACH( AttachmentMap, it, m_TextureBufferMap )
		{
			if ( NULL != it->second )
			{
				ASSERT( it->second->GetSize() == m_Size );
				ASSERT( ( it->second->GetTextureType() == ( RenderTargetProperty::RT_RENDER_TARGET_2D == m_eTargetType ? TextureProperty::TT_TEXTURE2D : TextureProperty::TT_TEXTURECUBE ) ) );
			}
			res->SetAttachment( it->first, it->second );
		}
		return res;
	}
	//------------------------------------------------------------------------------------------------------
	void RenderTargetSource::_DoReleaseUnnecessaryDataAfterLoaded()
	{

	}

}