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
#pragma once
#include "3DMath.h"


#include <EASTL/map.h>

namespace BohgeEngine
{
	namespace TextureProperty
	{
		enum TextureUseage
		{
			TU_STATIC = 0,
			TU_READ,//从显存读取到内存
			TU_WRITE,//从内存写到显存
		};
		enum PixelFormat
		{
			PF_AUTO = -1,//自动根据纹理格式变换
			PF_A8,
			PF_L8,
			//PF_LA8,
			PF_R8G8B8,//8_8_8
			PF_R5G6B5,//5_6_5
			PF_R8G8B8A8,//8_8_8_8
			PF_R4G4B4A4,//4_4_4_4
			PF_DEPTH16,
			PF_DEPTH32,//not on mobile device;
			PF_DEPTH24_STENCIL8,
		};
		enum TextureType
		{
			TT_AUTO = 0,
			TT_TEXTURE1D,
			TT_TEXTURE2D,
			TT_TEXTURE3D,
			TT_TEXTURECUBE,
		};
		enum TextureWarp
		{
			TW_REPEAT = 0,
			TW_CLAMP_TO_EDGE,
			TW_MIRRORED_REPEAT,
		};
		enum TextureFilter
		{
			TF_NEAREST = 0,
			TF_LINEAR,
			TF_NEAREST_MIPMAP_NEAREST,
			TF_LINEAR_MIPMAP_NEAREST,
			TF_NEAREST_MIPMAP_LINEAR,
			TF_LINEAR_MIPMAP_LINEAR,
		};
		struct WarpType
		{
			TextureWarp m_SWarp;
			TextureWarp m_TWarp;
			WarpType()
				:m_SWarp( TW_CLAMP_TO_EDGE ),
				m_TWarp( TW_CLAMP_TO_EDGE )
			{}
		};
		struct FilterType
		{
			TextureFilter m_MagFilter;
			TextureFilter m_MinFilter;
			FilterType()
				:m_MagFilter( TF_NEAREST ),
				m_MinFilter( TF_NEAREST )
			{}
		};
		struct TextrueSourceData
		{
			vector2d							m_Size;
			TextureProperty::TextureUseage		m_Usage;
			TextureProperty::TextureType		m_eType;
			TextureProperty::PixelFormat		m_ePixelFormat;
			TextureProperty::TextureWarp		m_eSWarp;
			TextureProperty::TextureWarp		m_eTWarp;
			TextureProperty::TextureFilter		m_eMagFilter;
			TextureProperty::TextureFilter		m_eMinFilter;
			uint								m_uAnisotropic;
			bool								m_isMipMap;
			byte*								m_pData;
			byte								m_Channel;
			TextrueSourceData()
				:m_Channel(0),
				m_pData(NULL),
				m_Usage( TextureProperty::TU_STATIC ),
				m_eType( TextureProperty::TT_AUTO ),
				m_ePixelFormat( TextureProperty::PF_AUTO ),
				m_eSWarp( TextureProperty::TW_CLAMP_TO_EDGE ),
				m_eTWarp( TextureProperty::TW_CLAMP_TO_EDGE ),
				m_eMagFilter( TextureProperty::TF_NEAREST ),
				m_eMinFilter( TextureProperty::TF_NEAREST ),
				m_uAnisotropic( 1 ),
				m_isMipMap(false)
			{
			}
			//------------------------------------------------------------------------------------------------------
			TextrueSourceData(const vector2d& size, TextureProperty::TextureType type, byte c, byte* data)
				:m_Size(size),
				m_pData(data),
				m_Channel( c ),
				m_eType(type),
				m_Usage( TextureProperty::TU_STATIC ),
				m_ePixelFormat( TextureProperty::PF_AUTO ),
				m_eSWarp( TextureProperty::TW_CLAMP_TO_EDGE ),
				m_eTWarp( TextureProperty::TW_CLAMP_TO_EDGE ),
				m_eMagFilter( TextureProperty::TF_NEAREST ),
				m_eMinFilter( TextureProperty::TF_NEAREST ),
				m_isMipMap(false)
			{
			}
			//------------------------------------------------------------------------------------------------------
			~TextrueSourceData()
			{
				SAFE_DELETE_ARRAY(m_pData);
			}
			//------------------------------------------------------------------------------------------------------
			BOHGE_FORCEINLINE void SetWarp( TextureProperty::TextureWarp s, TextureProperty::TextureWarp t )
			{
				m_eSWarp = s;
				m_eTWarp = t;
			}
			//------------------------------------------------------------------------------------------------------
			BOHGE_FORCEINLINE void SetFilter( TextureProperty::TextureFilter mag, TextureProperty::TextureFilter min )
			{
				m_eMagFilter = mag;
				m_eMinFilter = min;
			}
			//------------------------------------------------------------------------------------------------------
			BOHGE_FORCEINLINE void SetMemoryUseage( TextureProperty::TextureUseage mu )
			{
				m_Usage = mu;
			}
			//------------------------------------------------------------------------------------------------------
			BOHGE_FORCEINLINE void SetPixelFormat( TextureProperty::PixelFormat pf )
			{
				m_ePixelFormat = pf;
			}
			//------------------------------------------------------------------------------------------------------
			BOHGE_FORCEINLINE void SetAnisotropic( uint x )
			{
				m_uAnisotropic = x;
			}
			//------------------------------------------------------------------------------------------------------
			BOHGE_FORCEINLINE void SetMipMap( bool ismip )
			{
				m_isMipMap = ismip;
			}
		};
		typedef eastl::vector< TextrueSourceData* >	TextrueSourceDataVector;
	}
}