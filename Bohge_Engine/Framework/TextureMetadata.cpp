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
#include "TextureMetadata.h"
#include "IResourceSystem.h"
#include "ByteHelper.hpp"
#include "ILoader.h"




using namespace eastl;
namespace BohgeEngine
{
	RTTI_TYPE_IMPLEMENT( TextureFileMetadata );
	//---------------------------------------------------------------------------------------------
	TextureFileMetadata::TextureFileMetadata( 
		TextureProperty::TextureUseage mu, TextureProperty::PixelFormat pf,
		uint x, bool mip,
		TextureProperty::TextureWarp s, TextureProperty::TextureWarp t, 
		TextureProperty::TextureFilter mag, TextureProperty::TextureFilter min, 
		const eastl::string& path )
		:m_Path( path ),
		m_pTextureData(NULL),
		m_eUsage( mu ),
		m_ePixelFormat( pf ),
		m_eSWarp( s ),
		m_eTWarp( s ),
		m_eMagFilter( mag ),
		m_eMinFilter( min ),
		m_uAnisotropic( x ),
		m_isMipMap( mip )
	{

	}
	//---------------------------------------------------------------------------------------------
	TextureFileMetadata::~TextureFileMetadata()
	{
		SAFE_DELETE( m_pTextureData );
	}
	//---------------------------------------------------------------------------------------------
	void* TextureFileMetadata::_ReturnMetadata()
	{
		return m_pTextureData;
	}
	//---------------------------------------------------------------------------------------------
	void TextureFileMetadata::GetIdentifier( eastl::vector<byte>& bytes ) const
	{
		bytes.insert( bytes.end(), m_Path.begin(), m_Path.end() );
		bytes.push_back( m_eSWarp );
		bytes.push_back( m_eTWarp );
		bytes.push_back( m_eMagFilter );
		bytes.push_back( m_eMinFilter );
		bytes.push_back( m_ePixelFormat );
		bytes.push_back( m_uAnisotropic );
	}
	//---------------------------------------------------------------------------------------------
	void TextureFileMetadata::ProcessMetadata()
	{
		ILoader* loader = IResourceSystem::Instance()->LoaderFactory( IResourceProperty::SLT_TEXTURE );
		m_pTextureData = loader->Load<TextureProperty::TextrueSourceData>( m_Path );
		IResourceSystem::Instance()->RecycleBin( loader );
		m_pTextureData->SetAnisotropic( m_uAnisotropic );
		m_pTextureData->SetFilter( m_eMagFilter, m_eMinFilter );
		m_pTextureData->SetPixelFormat( m_ePixelFormat );
		m_pTextureData->SetWarp( m_eSWarp, m_eTWarp );
		m_pTextureData->SetMipMap( m_isMipMap );
	}
	//---------------------------------------------------------------------------------------------
	void TextureFileMetadata::ReleaseMetadate()
	{
		SAFE_DELETE( m_pTextureData );
	}





	RTTI_TYPE_IMPLEMENT( TextureBufferMetadata );
	//------------------------------------------------------------------------------------------------------
	//TextureBufferMetadata::TextureBufferMetadata( 
	//	const vector2d& size, TextureProperty::TextureType type, 
	//	TextureProperty::PixelFormat pf, uint anisotropic, 
	//	TextureProperty::TextureWarp s, TextureProperty::TextureWarp t, 
	//	TextureProperty::TextureFilter mag, TextureProperty::TextureFilter min, 
	//	byte pixelsize, byte* buf )
	//	:m_Size(size),
	//	m_Channel(pixelsize),
	//	m_eType(type),
	//	m_pData(buf),
	//	m_ePixelFormat( pf ),
	//	m_eSWarp( s ),
	//	m_eTWarp( s ),
	//	m_eMagFilter( mag ),
	//	m_eMinFilter( min ),
	//	m_uAnisotropic( anisotropic )
	//{

	//}
	//------------------------------------------------------------------------------------------------------
	byte _ChannelMapping( TextureProperty::PixelFormat pf )
	{
		switch(pf)
		{
		case TextureProperty::PF_A8: return 1;
		case TextureProperty::PF_L8: return 1;
		case TextureProperty::PF_R8G8B8: return 3;
		case TextureProperty::PF_R5G6B5: return 3;
		case TextureProperty::PF_R8G8B8A8: return 4;
		case TextureProperty::PF_R4G4B4A4: return 4;
		case TextureProperty::PF_DEPTH16: return 2;
		case TextureProperty::PF_DEPTH32: return 4;
		case TextureProperty::PF_DEPTH24_STENCIL8: return 4;
		default: ASSERT(false);
		}
		return 0;
	}
	TextureBufferMetadata::TextureBufferMetadata( const vector2d& size, TextureProperty::TextureType type, TextureProperty::TextureUseage mu, TextureProperty::PixelFormat pf, uint anisotropic, bool mip, TextureProperty::TextureWarp s, TextureProperty::TextureWarp t, TextureProperty::TextureFilter mag, TextureProperty::TextureFilter min, byte* buf )
		:m_Size(size),
		m_Channel(_ChannelMapping(pf)),
		m_Usage( mu ),
		m_eType(type),
		m_pData(buf),
		m_ePixelFormat( pf ),
		m_eSWarp( s ),
		m_eTWarp( s ),
		m_eMagFilter( mag ),
		m_eMinFilter( min ),
		m_uAnisotropic( anisotropic ),
		m_isMipMap( mip )
	{

	}
	//------------------------------------------------------------------------------------------------------
	TextureBufferMetadata::~TextureBufferMetadata()
	{

	}
	//------------------------------------------------------------------------------------------------------
	void* TextureBufferMetadata::_ReturnMetadata()
	{
		return m_pTextureData;
	}
	//------------------------------------------------------------------------------------------------------
	void TextureBufferMetadata::GetIdentifier( eastl::vector<byte>& bytes ) const
	{
		Utility::SingleT2Byte( intptr_t( this ), bytes );
	}
	//------------------------------------------------------------------------------------------------------
	void TextureBufferMetadata::ProcessMetadata()
	{
		m_pTextureData = NEW TextureProperty::TextrueSourceData( m_Size, m_eType, m_Channel, m_pData );
		m_pTextureData->SetMemoryUseage( m_Usage );
		m_pTextureData->SetAnisotropic( m_uAnisotropic );
		m_pTextureData->SetFilter( m_eMagFilter, m_eMinFilter );
		m_pTextureData->SetPixelFormat( m_ePixelFormat );
		m_pTextureData->SetWarp( m_eSWarp, m_eTWarp );
		m_pTextureData->SetMipMap( m_isMipMap );
		if ( NULL == m_pTextureData->m_pData )
		{
			uint size = m_pTextureData->m_Size.m_x * m_pTextureData->m_Size.m_y * m_pTextureData->m_Channel;
			m_pTextureData->m_pData	= NEW byte[ size ];
			memset( m_pTextureData->m_pData, 1, size);
		}
	}
	//------------------------------------------------------------------------------------------------------
	void TextureBufferMetadata::ReleaseMetadate()
	{
		SAFE_DELETE( m_pTextureData );
	}

}