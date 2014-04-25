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



namespace BohgeEngine
{
	namespace TextureStatus
	{
		enum PixelFormat
		{
			PF_A8 = 0,
			PF_L8,
			PF_R8G8B8,//8_8_8
			PF_R5G6B5,//5_6_5
			PF_R8G8B8A8,//8_8_8_8
			PF_R4G4B4A4,//4_4_4_4
			PF_DEPTH16,
			PF_DEPTH24_STENCIL8,
			//PF_LA8
		};
		enum TextureType
		{
			TT_TEXTURE1D = 0,
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
			bool			m_isLoad;
			vector2d		m_Size;
			byte*			m_pData;
			TextrueSourceData()
				:m_isLoad(false)
			{
			}
			TextrueSourceData(const vector2d& size, byte* data)
				:m_isLoad(true),m_Size(size),m_pData(data)
			{
			}
			~TextrueSourceData()
			{
				SAFE_DELETE_ARRAY(m_pData);
			}
		};
		typedef std::vector< TextrueSourceData* >	TextrueSourceDataVector;
	}
}