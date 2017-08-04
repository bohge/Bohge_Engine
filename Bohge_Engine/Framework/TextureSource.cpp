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
#include "TextureResource.h"
#include "TextureMetadata.h"
#include "IGraphicSystem.h"
#include "TextureSource.h"
#include "Utility.hpp"


using namespace eastl;

namespace BohgeEngine
{
	//------------------------------------------------------------------------------------------------------
	TextureSource::TextureSource( void )
		:ISharedSource( IResourceProperty::SST_TEXTURE, IResourceProperty::DT_RENDER_THREAD, true ),
		m_Anisotropic( 0 ),
		m_eTextureType( TextureProperty::TT_TEXTURE2D ),
		m_eFormat( TextureProperty::PF_AUTO ),
		m_isLoaded(false),
		m_isMipMap(false)
	{
	}
	//------------------------------------------------------------------------------------------------------
	TextureSource::~TextureSource(void)
	{
	}
	//------------------------------------------------------------------------------------------------------
	uint TextureSource::_GetResourceHashCode() const
	{
		eastl::vector<byte> bytes;
		const ISourceMetadataList& metas = GetSourceMetadata();
		FOR_EACH_CONST( ISourceMetadataList, it, metas )
		{
			(*it)->GetIdentifier( bytes );
		}
		return Utility::HashCode( bytes );
	}
	//------------------------------------------------------------------------------------------------------
	byte* TextureSource::_FitFormat( int width, int height, int channel, byte* input )
	{
		switch( m_eFormat )
		{
		case TextureProperty::PF_AUTO:
			{
				switch( channel )
				{
				case 1: m_eFormat = TextureProperty::PF_A8; break;
				case 3: m_eFormat = TextureProperty::PF_R8G8B8; break;
				case 4: m_eFormat = TextureProperty::PF_R8G8B8A8; break;
				default: ASSERT(false);
				}
				return input;
			}
		case TextureProperty::PF_A8 :
			{
				switch( channel )
				{
				case 1:
					{
						return input;
					}
				case 3:
					{
						byte* newData = NEW byte[width*height];
						for ( int i = 0 ; i < height ; i ++  )
						{
							for ( int j = 0 ; j < width ; j ++ )
							{
								newData[j + i * width] = input[ ( j + i * width ) * channel + 0 ] * 0.3
									+ input[ ( j + i * width ) * channel + 1 ] * 0.59
									+ input[ ( j + i * width ) * channel + 2 ] * 0.11;//转换到灰度
							}
						}
						SAFE_DELETE_ARRAY(input);
						return newData;
					}
				case 4:
					{
						byte* newData = NEW byte[width*height];
						for ( int i = 0 ; i < height ; i ++  )
						{
							for ( int j = 0 ; j < width ; j ++ )
							{

								newData[j + i * width] = input[ ( j + i * width ) * channel + 3 ]; //只要alpha通道
							}
						}
						SAFE_DELETE_ARRAY(input);
						return newData;
					}
				}
			}
		case TextureProperty::PF_L8 :
			{
				switch( channel )
				{
				case 1:
					{
						return input;
					}
				default:
					{
						byte* newData = NEW byte[width*height];
						for ( int i = 0 ; i < height ; i ++  )
						{
							for ( int j = 0 ; j < width ; j ++ )
							{
								newData[j + i * width] = input[ ( j + i * width ) * channel + 0 ] * 0.3
									+ input[ ( j + i * width ) * channel + 1 ] * 0.59
									+ input[ ( j + i * width ) * channel + 2 ] * 0.11;//转换到灰度
							}
						}
						SAFE_DELETE_ARRAY(input);
						return newData;
					}
				}
			}
		case TextureProperty::PF_R5G6B5 :	//需要数据转换
			{
				ASSERT( 1 != channel );
				ushort* newData = NEW ushort[width*height];
				for ( int i = 0 ; i < height ; i ++  )
				{
					for ( int j = 0 ; j < width ; j ++ )
					{
						ushort data = 0;
						data |= ( input[ ( j + i * width ) * channel + 0 ] >> 3 ) << 11;
						data |= ( input[ ( j + i * width ) * channel + 1 ] >> 2 ) << 5;
						data |= input[ ( j + i * width ) * channel + 2 ] >> 3;
						newData[j + i * width] = data;
					}
				}
				SAFE_DELETE_ARRAY(input);
				return (byte*)newData;
			}
		case TextureProperty::PF_R8G8B8 :
			{
				switch( channel )
				{
				case 3:
					{
						return input;
					}
				case 4://去掉alpha
					{
						byte* newData = NEW byte[width*height*3];
						for ( int i = 0 ; i < height ; i ++  )
						{
							for ( int j = 0 ; j < width ; j ++ )
							{
								int index = ( j + i * width ) * 3;
								int indexSor = ( j + i * width ) * 4;
								newData[ index + 0 ] = input[ indexSor + 0 ];
								newData[ index + 1 ] = input[ indexSor + 1 ];
								newData[ index + 2 ] = input[ indexSor + 2 ];
							}
						}
						SAFE_DELETE_ARRAY(input);
						return newData;
					}
				default:
					{
						ASSERT(false);
					}break;
				}
			}
		case TextureProperty::PF_R4G4B4A4 :
			{
				switch( channel )  //需要数据转换
				{
				case 3: // 这里就直接转换成 RGB565了，应为多alpha通道也是浪费
					{
						ushort* newData = NEW ushort[width*height];
						for ( int i = 0 ; i < height ; i ++  )
						{
							for ( int j = 0 ; j < width ; j ++ )
							{
								ushort data = 0;
								data |= ( input[ ( j + i * width ) * channel + 0 ] >> 3 ) << 11;
								data |= ( input[ ( j + i * width ) * channel + 1 ] >> 2 ) << 6;
								data |= input[ ( j + i * width ) * channel + 2 ] >> 3;
								newData[j + i * width] = data;
							}
						}
						SAFE_DELETE_ARRAY(input);
						return (byte*)newData;
					}
				case 4:
					{
						ushort* newData = NEW ushort[width*height];
						for ( int i = 0 ; i < height ; i ++  )
						{
							for ( int j = 0 ; j < width ; j ++ )
							{
								ushort data = 0;
								data |= ( input[ ( j + i * width ) * channel + 0 ] >> 4 ) << 12;
								data |= ( input[ ( j + i * width ) * channel + 1 ] >> 4 ) << 8;
								data |= ( input[ ( j + i * width ) * channel + 2 ] >> 4 ) << 4;
								data |=   input[ ( j + i * width ) * channel + 3 ] >> 4;
								newData[j + i * width] = data;
							}
						}
						SAFE_DELETE_ARRAY(input);
						return (byte*)newData;
					}
				default:
					{
						ASSERT(false);
					}break;
				}
			}
		case TextureProperty::PF_R8G8B8A8 :
			{
				switch( channel )
				{
				case 3:
					{
						byte* newData = NEW byte[width*height*4];
						for ( int i = 0 ; i < height ; i ++  )
						{
							for ( int j = 0 ; j < width ; j ++ )
							{
								int index = ( j + i * width ) * 4;
								int indexSor = ( j + i * width ) * 3;
								newData[ index + 0 ] = input[ indexSor + 0 ];
								newData[ index + 1 ] = input[ indexSor + 1 ];
								newData[ index + 2 ] = input[ indexSor + 2 ];
								newData[ index + 3 ] = 255;
							}
						}
						SAFE_DELETE_ARRAY(input);
						return newData;
					}
				case 4:
					{
						return input;
					}
				default:
					{
						ASSERT(false);
					}break;
				}
			}
		case TextureProperty::PF_DEPTH16 :
		case TextureProperty::PF_DEPTH24_STENCIL8 :
			{
				return input;
			}
		default:
			{
				ASSERT(false);
			}
		}
		return NULL;
	}
	//------------------------------------------------------------------------------------------------------
	void TextureSource::_DoLoadResource()
	{
		const ISourceMetadataList& metas = GetSourceMetadata();
		bool isFirst = true;
		FOR_EACH_CONST( ISourceMetadataList, it, metas )
		{
			(*it)->ProcessMetadata();
			TextureProperty::TextrueSourceData* data = (*it)->GetMetadata<TextureProperty::TextrueSourceData>();
			if ( isFirst )
			{
				isFirst = false;
				m_isMipMap = data->m_isMipMap;
				SetSize( data->m_Size );
				SetTextureUsage( data->m_Usage );
				SetWarp( data->m_eSWarp, data->m_eTWarp );
				SetFilter( data->m_eMagFilter, data->m_eMinFilter );
				SetPixelFormat( data->m_ePixelFormat );
				SetAnisotropic( data->m_uAnisotropic );
				SetTextureType( data->m_eType );
			}
			data->m_pData = _FitFormat( data->m_Size.m_x, data->m_Size.m_y, data->m_Channel, data->m_pData );
			m_DataVector.push_back( data );
		}
		if ( TextureProperty::TT_AUTO == m_eTextureType )
		{
			switch( metas.size() )
			{
			case 6: m_eTextureType = TextureProperty::TT_TEXTURECUBE;break;
			case 1:
				{
					1 == m_DataVector[0]->m_Size.m_y
						? m_eTextureType = TextureProperty::TT_TEXTURE1D
						: m_eTextureType = TextureProperty::TT_TEXTURE2D;
				}break;
			default:ASSERT(false);
			}
		}
	}
	//------------------------------------------------------------------------------------------------------
	ISharedResource* TextureSource::MakeResource()
	{
		ASSERT(false==m_isLoaded);
		TextureResource* res = static_cast<TextureResource*>( IGraphicSystem::Instance()->CreateResource( IResourceProperty::SRT_TEXTURE ) );
		res->GenerateTexture( m_DataVector, m_eTextureType, m_eFormat );
		res->SetTextureUseage( m_Usage );
		res->SetAnisotropic( m_Anisotropic );
		res->SetFilterType( m_Filter );
		res->SetWarpType( m_Warp );
		if (m_isMipMap)
		{
			res->GenerateMipMap();
		}
		m_isLoaded = true;
		return res;
	}
	//------------------------------------------------------------------------------------------------------
	void TextureSource::_DoReleaseUnnecessaryDataAfterLoaded()
	{
		m_DataVector.clear();
	}

}
