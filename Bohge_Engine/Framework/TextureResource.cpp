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
#include "SubTextureBuffer.h"
#include "TextureResource.h"
#include "IGraphicSystem.h"


namespace BohgeEngine
{
	handle TextureResource::s_CurrentBind = -1;

	//------------------------------------------------------------------------------------------------------
	TextureResource::TextureResource( void ) 
		:m_Anisotropic(0),
		m_eFormat( (TextureProperty::PixelFormat)-1 ),
		m_eTextureType( (TextureProperty::TextureType)-1 ),
		m_MemoryUseage( (TextureProperty::TextureUseage)-1 ),
		m_GPUResourceHandle(-1),
		m_isMipMap(false),
		m_SwitchIndex(0)
	{
		m_Warp.m_SWarp = (TextureProperty::TextureWarp)-1;
		m_Warp.m_TWarp = (TextureProperty::TextureWarp)-1;
		m_Filter.m_MagFilter = (TextureProperty::TextureFilter)-1;
		m_Filter.m_MinFilter = (TextureProperty::TextureFilter)-1;
	}
	//------------------------------------------------------------------------------------------------------
	TextureResource::~TextureResource( void )
	{

	}
	//------------------------------------------------------------------------------------------------------
	void TextureResource::ReleaseResource()
	{
		if ( -1 != m_GPUResourceHandle )
		{
			_DoDeleteTexture( m_GPUResourceHandle );
			s_CurrentBind = -1;
		}
	}
	//------------------------------------------------------------------------------------------------------
	void TextureResource::OnGraphicUpdata()
	{
		SubTextureList& sublist = m_BufferList[ m_SwitchIndex ];
		m_SwitchIndex = !m_SwitchIndex;
		FOR_EACH( SubTextureList, it, sublist )
		{
			_DoSubstituteTextureBuffer( (*it)->GetBegin(), (*it)->GetSize(), (*it)->GetBuffer() );
			SAFE_DELETE(*it);
		}
		sublist.clear();
	}
	//------------------------------------------------------------------------------------------------------
	void TextureResource::SubstituteTextureBuffer( const vector2d& begin, const vector2d& size, const byte* const buffer )
	{
		m_BufferList[m_SwitchIndex].push_back( NEW SubTextureBuffer( begin, size, buffer ) );
		if ( 0 != m_BufferList[m_SwitchIndex].size() )//第一次进入，推入回掉系统
		{
			IGraphicSystem::Instance()->HostedGraphicUpdata( MEMBER_FUNC_PTR(&TextureResource::OnGraphicUpdata) );
		}
	}
}



////////////////////////////////////////////////////////////////////////////////////////
////
////						The Bohge Engine License (BEL)
////
////	Copyright (c) 2011-2014 Peng Zhao
////
////	Permission is hereby granted, free of charge, to any person obtaining a copy
////	of this software and associated documentation files (the "Software"), to deal
////	in the Software without restriction, including without limitation the rights
////	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
////	copies of the Software, and to permit persons to whom the Software is
////	furnished to do so, subject to the following conditions:
////
////	The above copyright notice and this permission notice shall be included in 
////	all copies or substantial portions of the Software. And the logo of 
////	Bohge Engine shall be displayed full screen for more than 3 seconds 
////	when the software is started. Copyright holders are allowed to develop 
////	game edit based on Bohge Engine, The edit must be released under the MIT 
////	open source license if it is going to be published. In no event shall 
////	copyright holders be prohibited from using any code of Bohge Engine 
////	to develop any other analogous game engines.
////
////	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
////	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
////	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
////	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
////	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
////	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
////
////////////////////////////////////////////////////////////////////////////////////////
//#include "SubTextureBuffer.h"
//#include "TextureResource.h"
//#include "IGraphicSystem.h"
//#include "IThreadSystem.h"
//#include "IThreadMutex.h"
//
//
//namespace BohgeEngine
//{
//	handle TextureResource::s_CurrentBind = -1;
//
//	//------------------------------------------------------------------------------------------------------
//	TextureResource::TextureResource( void ) 
//		:m_Anisotropic(0),
//		m_eFormat( (TextureProperty::PixelFormat)-1 ),
//		m_eTextureType( (TextureProperty::TextureType)-1 ),
//		m_MemoryUseage( (TextureProperty::TextureUseage)-1 ),
//		m_GPUResourceHandle(-1),
//		m_isMipMap(false)
//	{
//		m_pFlushMutex = static_cast<IThreadMutex*>(IThreadSystem::Instance()->ThreadEntityFactory( ThreadProperty::ET_MUTEX ));
//		m_Warp.m_SWarp = (TextureProperty::TextureWarp)-1;
//		m_Warp.m_TWarp = (TextureProperty::TextureWarp)-1;
//		m_Filter.m_MagFilter = (TextureProperty::TextureFilter)-1;
//		m_Filter.m_MinFilter = (TextureProperty::TextureFilter)-1;
//	}
//	//------------------------------------------------------------------------------------------------------
//	TextureResource::~TextureResource( void )
//	{
//
//	}
//	//------------------------------------------------------------------------------------------------------
//	void TextureResource::ReleaseResource()
//	{
//		if ( -1 != m_GPUResourceHandle )
//		{
//			_DoDeleteTexture( m_GPUResourceHandle );
//			s_CurrentBind = -1;
//		}
//	}
//	//------------------------------------------------------------------------------------------------------
//	void TextureResource::OnGraphicUpdata()
//	{
//		m_pFlushMutex->Lock();
//		FOR_EACH( SubTextureList, it, m_BufferList )
//		{
//			_DoSubstituteTextureBuffer( (*it)->GetBegin(), (*it)->GetSize(), (*it)->GetBuffer() );
//			SAFE_DELETE(*it);
//		}
//		m_BufferList.clear();
//		m_pFlushMutex->Unlock();
//	}
//	//------------------------------------------------------------------------------------------------------
//	void TextureResource::SubstituteTextureBuffer( const vector2d& begin, const vector2d& size, const byte* const buffer )
//	{
//		m_pFlushMutex->Lock();
//		m_BufferList.push_back( NEW SubTextureBuffer( begin, size, buffer ) );
//		IGraphicSystem::Instance()->HostedGraphicUpdata( MEMBER_FUNC_PTR(&TextureResource::OnGraphicUpdata) );
//		m_pFlushMutex->Unlock();
//	}
//}